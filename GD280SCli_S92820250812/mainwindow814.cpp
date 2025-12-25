#include "math.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <synchapi.h>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QDirModel>
#include <QDateTime>
#include "usb_listener.h"
#include "gd280slensdata.h"

QReadWriteLock hidLock;
//QReadWriteLock ImgLock;
QReadWriteLock netBufLock;

pthread_mutex_t gTrackingMutex;
pthread_cond_t gTrackingTransImg;

#ifdef WIN32
#include <windows.h>
#include <dbghelp.h>
#include <DbgHelp.h>
#include <QDir>
#include <QProcess>
#include <pid3/pid_ex5.h>


#pragma comment(lib,"dbghelp.lib")
#define COREFILE_DIR_PATH "d:\\corefile"
#define WCOREFILE_DIR_PATH L"d:\\corefile"

usb_listener * m_usb_listener = Q_NULLPTR;
#endif


QT_BEGIN_NAMESPACE
class QLabel;

namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   /* LabStatebarInfo = new QLabel("状态");
    LabStatebarInfo->setMinimumWidth(200);
    ui->statusbar->addWidget(LabStatebarInfo);
    LabStatebarInfo->setText("状态：未连接"); */
#if 0
	if(!CheckAppStatus("IPCComm.exe"))
    {
        QProcess process(this);
        QString  str = "D:/GD380/IpCommRelease/IPCComm.exe";
        process.startDetached(str);
        //process.start(str);       //会随process close 而退出
        qDebug()<<"process:"<<str;
        QThread::msleep(15*1000);
    }
#endif
    readCfgFile();            // 读取配置文件
    initGlobalParam();        // 初始化全局参数
    initControlStat();        // 初始化控件状态
    initTimerAndThread();     // 初始化定时器和线程

    // 建立各种通信连接
    if( -1== crtCommuThd()) // 创建指控通信
    {
        ui->te_Output->append("状态：未连接主控!");
    }
    if( false== openSerialPort()) // 打开串口
    {
        ui->te_Output->append("伺服连接失败！");
    }
    else
    {
        ui->te_Output->append("伺服连接OK");
    }

    connectGyroBc();    // 连接陀螺仪
    getRtspStreamA();   // 获取光电A视频流
    getRtspStreamB();   // 获取光电B视频流
    connectPtzAUc();    // 连接光电A
    connectPtzBUc();    // 连接光电B

#if 0
#ifdef SIMUCAMERA
    connectFlag_A = connectFlag_B = true;
#endif

    if(connectFlag_A == true && connectFlag_B == true )
    {
        setCtlObjStatus(true);
        LabStatebarInfo->setText("状态：连接正常");
    }
    else
    {
        setCtlObjStatus(false);
        LabStatebarInfo->setText("状态：连接异常");
    }
#endif
    // 启动跟踪功能
    // trackerThd->bTrackerStart = true;
     trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
    trackerThd->bTrackerStart = false;
//    trackerThd->trackingMethod = TRACKING_METHOD_NONE;
#ifndef SIMUCAMERA
    //on_pbRec_clicked();  // 开始录像
#endif

}

MainWindow::~MainWindow()
{
    //killTimer(timerTheo_pp);
    timerTheo_pp->stop();
    delete(timerTheo_pp);
    trackerThd->bRunflag = false;
    workStateLoopThd->bRunflag = false;
    hid->runflag=false;
    hid->quit();

    //delete(pTheo_PP_DLL);
    delete ui;
}

/*建立指控通讯
 *
 */
int MainWindow::crtCommuThd()
{
    pnetworkComm = new NetworkComm(0);
    pnetworkComm->centerUdpSocket = new QUdpSocket();

    // 连接数据接收槽函数
    QObject::connect(pnetworkComm->centerUdpSocket,&QUdpSocket::readyRead,this,&MainWindow::CenterSocket_Read_Data);

    pnetworkComm->centerUdpSocket->abort();

    // 绑定端口和连接
    pnetworkComm->centerUdpSocket->bind((quint16)mMyPort);
    pnetworkComm->centerUdpSocket->connectToHost(mZhiKongIp,(quint16)mZhiKongPort);

    if(!pnetworkComm->centerUdpSocket->waitForConnected(10000))
    {
        qDebug()<<tr("connect ZhiKong failed, pls try again.");
        pnetworkComm->ctrConnectFlag = false;
        return -1;
    }
    else
    {
        pnetworkComm->ctrConnectFlag = true;
        qDebug()<<tr("ZhiKong connect OK")<<"   mZhiKongIp:"<<mZhiKongIp <<"   Port:"<<mZhiKongPort <<"   mMyIp:"<<mMyIp <<"   Port:"<<mMyPort;
    }
    return 0;
}

int MainWindow::connectGyroBc()
{
    if(connectGyroFlag == true)
    {
        connectGyroFlag = false;
        pGyroComm->centerUdpSocket->close();
        delete pGyroComm;
        qDebug()<<tr("Gyro Disconnected");
    }
    pGyroComm = new NetworkComm(0);
    pGyroComm->centerUdpSocket = new QUdpSocket();
    QObject::connect(pGyroComm->centerUdpSocket,&QUdpSocket::readyRead,this,&MainWindow::GyroSocket_Read_Data);

    pGyroComm->centerUdpSocket->abort();

    pGyroComm->centerUdpSocket->bind((quint16)mListenGyroPort);

    pGyroComm->centerUdpSocket->connectToHost(mGyroIp,(quint16)mGyroPort);

    if(!pGyroComm->centerUdpSocket->waitForConnected(10000))
    {
        qDebug()<<tr("connect Gyro failed, pls try again.");
        connectGyroFlag = false;
        return -1;
    }
    else
    {
        connectGyroFlag = true;
        qDebug()<<tr("Gyro connect OK")<<"   mZhiKongIp:"<<mGyroIp <<"   Port:"<<mGyroPort <<"   mMyIp:"<<mMyIp <<"   Port:"<<mListenGyroPort;
    }

    pGyroComm->msgFromGyro.roll =0;
    pGyroComm->msgFromGyro.pitch =0;
    pGyroComm->msgFromGyro.yaw =0;
    pGyroComm->msgFromGyro.B =0;
    pGyroComm->msgFromGyro.L =0;
    pGyroComm->msgFromGyro.H =0;
    pGyroComm->msgFromGyro.direction =0;

    return 0;
}


void MainWindow::getRtspStreamA()
{
    if(connectRtspFlag_A == true)
    {
         mPlayer->runflag=false;

         connectRtspFlag_A = false;
         mImageCh1GetNew = false;
         ui->te_Output->append("重连光电A视频");
     }

    mPlayer = new vidoeplayer;

#ifndef SIMUCAMERA
    mPlayer->channelUrl.sprintf("rtsp://admin:admin@%s:%d/cam/realmonitor?channel=1&subtype=0",hdCamIp_A,mHdCamPort_A);        //SH相机直出 RTSP流 0:主码流 1:辅码流
#else
    //mPlayer->channelUrl.sprintf( "D:/video/20240119/GD120240119_171948.mp4");
    mPlayer->channelUrl.sprintf( "D:/Users/15941/Desktop/jy/jy_xk/GD280SCli_S92820240315/GD280SCli_S928/detector/moni.mp4");
#endif

    mPlayer->channelNo=1;
    connect(mPlayer, SIGNAL(sig_GetOneFrameCh1(QImage)), this, SLOT(slotGetOneFrameCh1(QImage)));
    mPlayer->startPlay();
    connectRtspFlag_A = true;
}

void MainWindow::getRtspStreamB()
{
    if(connectRtspFlag_B == true)
    {
         mPlayerSub->runflag=false;
        connectRtspFlag_B = false;
        mImageCh2GetNew = false;
         ui->te_Output->append("重连光电B视频");
    }

    mPlayerSub = new vidoeplayer;
#ifndef SIMUCAMERA
    mPlayerSub->channelUrl.sprintf("rtsp://admin:admin@%s:%d/cam/realmonitor?channel=1&subtype=0",hdCamIp_B,mHdCamPort_B);        //SH相机直出 RTSP流 0:主码流 1:辅码流
#else
    //mPlayerSub->channelUrl.sprintf( "D:/video/20240119/GD220240119_171948.mp4");
#endif
    mPlayerSub->channelNo=2;
    connect(mPlayerSub, SIGNAL(sig_GetOneFrameCh2(QImage)), this, SLOT(slotGetOneFrameCh2(QImage)));
    mPlayerSub->startPlay();
    connectRtspFlag_B = true;
}

void MainWindow::connectPtzAUc()
{
    if(connectFlag_A == true)
    {
        connectFlag_A = false;
        mImageCh1GetNew = false;
        udpSocket_A->disconnectFromHost();
        socket_Disconnected_A();
        delete udpSocket_A;
        qDebug()<<tr("GDA Disconnected");
        ui->te_Output->append("重连光电A通讯");
    }

    udpSocket_A = new QUdpSocket();
    QObject::connect(udpSocket_A,&QUdpSocket::readyRead,this,&MainWindow::socket_Read_Data_A);

    udpSocket_A->abort();
    udpSocket_A->connectToHost(ui->leGD_A_IP->text(),(quint16)serverPort_A);

    if(!udpSocket_A->waitForConnected(10000))
    {
        qDebug()<<tr("connect GD_A failed, pls try again.");
        connectFlag_A = false;
#ifdef SIMUCAMERA
    connectFlag_A = true;
#endif
    }
    else
    {
        connectFlag_A = true;
        /*ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));*/
        qDebug()<<tr("GD_A connect OK");
        //QMessageBox::warning(this,tr("Tips"),tr("connect server success"),QMessageBox::Yes,QMessageBox::No);
    }
}

void MainWindow::connectPtzBUc()
{
    if(connectFlag_B == true)
    {
        connectFlag_B = false;
        mImageCh2GetNew = false;
        udpSocket_B->disconnectFromHost();
        socket_Disconnected_B();
        delete udpSocket_B;
        qDebug()<<tr("GDB Disconnected");
        ui->te_Output->append("重连光电B通讯");
    }

    udpSocket_B = new QUdpSocket();
    QObject::connect(udpSocket_B,&QUdpSocket::readyRead,this,&MainWindow::socket_Read_Data_B);

    udpSocket_B->abort();
    udpSocket_B->connectToHost(ui->leGD_B_IP->text(),(quint16)serverPort_B);

    if(!udpSocket_B->waitForConnected(10000))
    {
        qDebug()<<tr("connect GD_B failed, pls try again.");
        connectFlag_B = false;
#ifdef SIMUCAMERA
    connectFlag_B = true;
#endif
    }
    else
    {
        connectFlag_B = true;
        /*ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));*/
        qDebug()<<tr("GD_B connect OK");
        //QMessageBox::warning(this,tr("Tips"),tr("connect server success"),QMessageBox::Yes,QMessageBox::No);
    }
}

int MainWindow::chkCrossBorder()
{
    if(mPtzDirection_A >315 || mPtzDirection_A < 135 )
    {
        pnetworkComm->msgToZhiKong.gd1ZheDangFlag = 0;
    }
    else
    {
        pnetworkComm->msgToZhiKong.gd1ZheDangFlag = 1;
    }
    if(mPtzDirection_B < 315 && mPtzDirection_B > 135 )
    {
        pnetworkComm->msgToZhiKong.gd2ZheDangFlag = 0;
    }
    else
    {
        pnetworkComm->msgToZhiKong.gd2ZheDangFlag = 1;
    }
}


double MainWindow::getGyroDirectionInDegrees()
{
    // 使用direction（调试现在直接就是度）
    double directionInDegrees = pGyroComm->msgFromGyro.direction;

    // 使用yaw（度）
    double yawInDegrees = pGyroComm->msgFromGyro.yaw;

    qDebug() << QString("惯导角度对比 - direction(直接度): %1°, yaw(度): %2°")
               .arg(directionInDegrees, 0, 'f', 3)
               .arg(yawInDegrees, 0, 'f', 3);

    double angleDiff = abs(directionInDegrees - yawInDegrees);
    if (angleDiff > 180.0) {
        angleDiff = 360.0 - angleDiff;
    }

    // 如果差异小，用yaw
    if (angleDiff < 5.0) {
        qDebug() << "使用yaw角度，差异较小: " << angleDiff << "度";
        return yawInDegrees;
    }

    // 如果差异较大，就要看哪个更准（理论上direction（双天线）比yaw（单天线）更准）
    qDebug() << "角度差异较大: " << angleDiff << "度，用direction";
    return directionInDegrees;
}

void MainWindow::debugGyroAngles()
{
    double directionRaw = pGyroComm->msgFromGyro.direction;
    double yawDeg = pGyroComm->msgFromGyro.yaw;

    QString debugInfo = QString("惯导调试 - direction(度): %1°, yaw: %2°")
                       .arg(directionRaw, 0, 'f', 3)
                       .arg(yawDeg, 0, 'f', 3);

    qDebug() << debugInfo;
    ui->te_Output->append(debugInfo);

    // 记录到文件
    static QFile debugFile("gyro_angles_debug.txt");
    if (!debugFile.isOpen()) {
        debugFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    QTextStream stream(&debugFile);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
           << " " << debugInfo << "\n";
    stream.flush();
}

bool MainWindow::validateGyroData()
{
    // direction 现在是度：0~360°
    if (pGyroComm->msgFromGyro.direction < 0 || pGyroComm->msgFromGyro.direction >= 360) {
        qDebug() << "警告：direction超出范围：" << pGyroComm->msgFromGyro.direction;
        return false;
    }

    // yaw 0~360°
    if (pGyroComm->msgFromGyro.yaw < 0 || pGyroComm->msgFromGyro.yaw >= 360) {
        qDebug() << "警告：yaw超出范围：" << pGyroComm->msgFromGyro.yaw;
        return false;
    }

    return true;
}

void MainWindow::initGlobalParam()
{
    serverPort_A = 8888;
    serverPort_B = 8888;
    pelco_spd_val = ui->vsld_speed->value();    //设置上下左右按钮速度

    connectFlag_A = false;
    connectFlag_B = false;
    connectRtspFlag_A = false;
    connectRtspFlag_B = false;
    connectGyroFlag = false;

    //ui->gridLayout_Theo->addWidget(pTheo_PP_DLL);
    double Site_B = 0;//站址纬度 度°
    double Site_L = 0;//站址经度 度°
    double Site_H = 0;//站址高程 m
    //Step1 :设置设备站址 及坐标系*1-84坐标系,2-2000坐标系,3-54坐标系,4-靶场坐标系
    //pTheo_PP_DLL->InitialDll(Site_B,Site_L,Site_H,Site_B,Site_L,Site_H,2);


    operateMode = OPMODE_MARKING;  //JoyStick;
    mAETheo.dblA = 0;
    mAETheo.dblE = 0;


    mPtzDirection_A = 0;
    mHdViewAngle_A = 30;
    mPtzPitch_A = 0;
    hdFocusVal =0;
    hdZoomVal =0;
    hdLensVal = 30;

    mPtzDirection_B = 0;
    mHdViewAngle_B = 30;
    mPtzPitch_B = 0;
    hdFocusVal_B =0;
    hdZoomVal_B =0;
    hdLensVal_B = 30;

    speedGear=0;
    mExpo=4.5;
    char str[20];
    sprintf(str,"%.1f",mExpo);
    //ui->le_setStickExpo->setText(str);
    /*
    titleLabel = new QLabel;//定义一个label
    titleLabel->setMargin(2);
    QFont ft("Microsoft YaHei", 10, 75);;

    titleLabel->setFont(ft);
    //titleLabel->setStyleSheet("color:red;");
    titleLabel->setText("启动完成!");//label显示内容
    statusBar()->addWidget(titleLabel);//将label加到状态栏上
    */

    mAEJoyCorrect.dblA = 0;//中心引导修正量
    mAEJoyCorrect.dblE = 0;

    mStick_y = 512;
    mStick_x = 512;
    mStick_z = 512;
    mStickBtn = 0;
    mStickBtn2 =0;

    //PIDInit(&PidX,0,PID_X_SCALE,PID_X_P,PID_X_I,PID_X_D);
    //PIDInit(&PidY,0,PID_Y_SCALE,PID_Y_P,PID_Y_I,PID_Y_D);
    PIDInit2(&PidX,0,mPid_X_Scale,mPid_X_P,mPid_X_I,mPid_X_D,mPidMaxOut);
    PIDInit2(&PidY,0,mPid_Y_Scale,mPid_Y_P,mPid_Y_I,mPid_Y_D,mPidMaxOut);
    //PIDInit2(&PidX,0,0,0.2,0.3,0.2);
    //PIDInit2(&PidY,0,0,0.5,0.5,0.2);

    mJoy_x = 80;
    mJoy_y = 80;
    mJoy_btn = 0;

    dd_diffx=0;
    dd_diffy=0;
    mImageCh1GetNew = false;
    mImageCh2GetNew = false;
    mGyroPitch_A = 0;
    mGyroPitch_B = 0;

    roiBox.x = IMAGE_PIXEL_WIDTH/2 - 40;
    roiBox.y = IMAGE_PIXEL_HEIGHT/2 - 30 ;
    roiBox.width = 80;
    roiBox.height = 30;
    currentMainSrc = 1;
}

void MainWindow::initControlStat()
{
    //ui->lineEdit_IP->setText(rtrim(serverIp));
    //ui->cbx_CfgLocal->setCurrentIndex(localRoleFlag);
    //char str[64];
    //sprintf(str,"%.3f",dd_diffx);
    //ui->le_setDirecionDiff->setText(str);
    //ui->le_setDirecionDiff->setVisible(0);  //不做配置文件方位补偿功能
    //ui->label_21->setVisible(0);
    //ui->pbSSetDiffX->setVisible(0);
    //sprintf(str,"%.3f",dd_diffy);
    //ui->le_setPitchDiff->setText(str);

    //ui->pbConnect->setDisabled(true);
    //ui->pbDisConnect->setDisabled(true);
    //ui->pbConnect_B->setDisabled(true);
    //ui->pbDisConnect_B->setDisabled(false);

    ui->lbMousePosition_x->setVisible(0);   //只在调试时使用，正式版本不打开
    ui->lbMousePosition_y->setVisible(0);
    ui->label_5->setVisible(0);
    ui->label_6->setVisible(0);
    ui->lbDiffX->setVisible(0);   //只在调试时使用，正式版本不打开
    ui->lbDiffY->setVisible(0);   //只在调试时使用，正式版本不打开
    //ui->pbSetZero->setVisible(0);

    //ui->pbOnOffColor->setVisible(1);

    ui->lbl_lenVal->setVisible(1);
    ui->lbl_focalVal->setVisible(1);
    ui->lbl_focalVal_B->setVisible(1);
    ui->lblLens->setVisible(1);
    ui->lbl_lensTitle->setVisible(1);
    ui->pb_OpenSelfTest->setVisible(0);
    ui->pb_CloseSelfTest->setVisible(0);
    ui->pb_OpenSelfTest_B->setVisible(0);
    ui->pb_CloseSelfTest_B->setVisible(0);
    ui->sbx_cbx_backLightVal->setEnabled(false);
    ui->gbx_Expo->setEnabled(true);

    ui->lblPadDirection->installEventFilter(this);//安装事件过滤器
    ui->lblPadDirection_B->installEventFilter(this);//安装事件过滤器
    ui->lbl_SimJoystick->installEventFilter(this);//安装事件过滤器
    //ui->lbl_padAngleGap->installEventFilter(this);//安装事件过滤器
    //ui->lbl_stickEXPO->installEventFilter(this);//安装事件过滤器

    QIntValidator* validv = new QIntValidator;
    validv->setRange(1, 16);//可以改成（-255,255），这样就是负数

    QIntValidator* validv2 = new QIntValidator;
    validv2->setRange(0, 1440);//

    QIntValidator* validv3 = new QIntValidator;
    validv3->setRange(0, 59);//
/*
    QIntValidator* validv4 = new QIntValidator;
    validv4->setRange(0, 360);//
    ui->le_setDirecion ->setValidator(validv4);
    ui->le_setDirecion->setPlaceholderText("0~360");
    ui->le_setPitch ->setValidator(validv4);
    ui->le_setPitch->setPlaceholderText("-180~180");
*/

    //ui->tabWidConfig->removeTab(4);     //关闭debug页

    ui->pbConfigSave->setEnabled(false);
    ui->pbOsdWordSave->setEnabled(false);
    ui->leRecFileDir->setEnabled(false);
    ui->leSetIP->setEnabled(false);
    //ui->leSetIPMask->setEnabled(false);
    //ui->leSetIPGageway->setEnabled(false);
    ui->leSetHdCamIP->setEnabled(false);
    ui->leSetHdCamPort->setEnabled(false);

    //ui->pb_ReplayStart->setEnabled(false);
    //ui->pb_ReplayPause->setEnabled(false);
    //ui->pb_ReplayStop->setEnabled(false);
    /*ui->lblTrackingFps->setVisible(false);
    ui->label_trackfps->setVisible(false);
    ui->gbx_RoiSize->setEnabled(false);*/
    //ui->pbIrConfigSave->setDisabled(true);
    //ui->pbIrConfigSave->setVisible(false);

    ui->cbxCrosser->setChecked(true);

    /*ui->checkBoxTheoGuid->setEnabled(false);
    ui->checkBoxCenterGuid->setEnabled(false);
    ui->checkBoxTheoGuid->setChecked(true);
    ui->checkBoxCenterGuid->setChecked(true);*/
    //ui->pbSyncSystime->setEnabled(false);
    /*ui->cbxMarkline->setChecked(true);
    ui->cbxRoiBox->setChecked(true);*/
    ui->lbl_Version->setText( "版本:"  __DATE__ " " __TIME__);

    QMainWindow::setMouseTracking(true);
    QMainWindow::centralWidget()->setMouseTracking(true);

    ui->lblPad->setMouseTracking(true);
    ui->lbl_SimJoystick->setMouseTracking(true);

    ui->lbDiffX->setText("0°");
    ui->lbDiffY->setText( "0°");
}

void MainWindow::initTimerAndThread()
{

    //pnetworkComm = new NetworkComm(localRoleFlag);
#if 1
    timerTheo_pp = new QTimer(); // 主控制循环
    connect(timerTheo_pp,SIGNAL(timeout()),this,SLOT(Theo_timeOut()));
    timerTheo_pp->start(100);
    qDebug()<<tr("start timer Theo");
#endif

    timerRecFileSeg = new QTimer();
    connect(timerRecFileSeg,SIGNAL(timeout()),this,SLOT(timerRecFileSeg_timeOut()));
    timerRecFileSeg->start(1000*60*3);   //3min


    m_usb_listener = new usb_listener;
    m_usb_listener->registerGUID();
    qApp->installNativeEventFilter(m_usb_listener);
    connect(m_usb_listener, SIGNAL(sig_DeviceChangeCbk(int)), this, SLOT(slotHidChanged(int)));


    hid= new HidProcThd;
    //connect(hid, SIGNAL(sig_UpdateJoystick()), this, SLOT(slotGetUpdateJoystick()));
    hid->start();
    //connect(this,SIGNAL(mouseMove(QMouseEvent *)),this,SLOT(draw(QMouseEvent *)));

    //pTheo_PP_DLL = new Theo_PP_DLL();

    //strcpy(serverIp,"192.168.1.114");
    // 创建跟踪线程
    trackerThd = new TrackerThread;
    connect(trackerThd, SIGNAL(sig_UpdateTrackingBox(int,int,int,int,int)), this, SLOT(slotUpdateTrackingBox(int,int,int,int,int)));
    connect(trackerThd, SIGNAL(sig_UpdateDetectingBox(int,int,int,int,int,int,float)), this, SLOT(slotUpdateDetectingBox(int,int,int,int,int,int,float)));
    trackerThd->start();    // 启动线程，进入run()等待状态
    pthread_mutex_init(&gTrackingMutex,NULL);  // 初始化线程同步机制
    pthread_cond_init(&gTrackingTransImg,NULL);


    timerDelayPid = new QTimer();
    connect(timerDelayPid,SIGNAL(timeout()),this,SLOT(delayPid_timeOut()));
    //timerDelayPid->start(20);

//#ifndef SIMUCAMERA
    timerChkIfOffline = new QTimer();  // 离线检测
    connect(timerChkIfOffline,SIGNAL(timeout()),this,SLOT(chkIfOffline_timeOut()));
    timerChkIfOffline->start(1000 * 5);
//#endif

    timerJoystick= new QTimer();  // 遥杆
    connect(timerJoystick,SIGNAL(timeout()),this,SLOT(stick_timeOut()));
    timerJoystick->start(10);
    qDebug()<<tr("start timer JoyStick");

    timerZhiKongSocket = new QTimer(); // 指控
    connect(timerZhiKongSocket,SIGNAL(timeout()),this,SLOT(zhiKongSocket_timeOut()));
    timerZhiKongSocket->start(50);  //20HZ
    qDebug()<<tr("start timer ZhiKongSocket");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
#if 0
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出程序"),
        QString(tr("警告：程序正在运行中，是否结束操作退出?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes) {
        fullscreen.close();
        event->accept();  //接受退出信号，程序退出
    }
#endif
}

void MainWindow::on_lineEdit_IP_returnPressed()
{
    on_pbConnect_clicked();
}

void MainWindow::on_pbConnect_clicked()
{
    getRtspStreamA();
}

void MainWindow::on_pbDisConnect_clicked()
{
    connectPtzAUc();
}
void MainWindow::on_pbConnect_B_clicked()
{
    getRtspStreamB();
}

void MainWindow::on_pbDisConnect_B_clicked()
{
    connectPtzBUc();
}
void MainWindow::calcScreenPos(int * x_out,int * y_out,int x_in,int y_in,int screen_w,int screen_h)
{
    float rate_w = (float)ui->lblPad->width()/(float)screen_w;
    float rate_h = (float)ui->lblPad->height()/(float)screen_h;
    int pos_x=0,pos_y=0;

    if(rate_w < rate_h)
    {
        rate_h = rate_w;
        pos_y = y_in - (ui->lblPad->height() -screen_h * rate_h)/2;
    }
    else
    {
        pos_y = x_in;
    }
    pos_x = x_in/rate_h;
    pos_y = pos_y / rate_h;
    if(pos_x > 0 && pos_x <=screen_w && pos_y > 0 && pos_y <=screen_h)
    {
        *x_out = pos_x;
        *y_out = pos_y;

    }
    return;
}

void MainWindow::getMouseOnVirtualJoyStk(QMouseEvent *event)
{
    QPoint p_SimuStick = event->pos() - ui->tabWidConfig->pos();
    //QPoint p_SimuStick = event->pos() - ui->groupBox_10->pos();
    if(p_SimuStick.x() > 20 && p_SimuStick.x() < ui->lbl_SimJoystick->width()+20 && p_SimuStick.y() > 0 && p_SimuStick.y() < ui->lbl_SimJoystick->height())
    {
        if (event->button() == Qt::LeftButton)
        {
            btnStatus=1;
        }
        if(ui->tabWidConfig->currentIndex() == 5 )
        {
            if(btnStatus == 1)
            {
                mJoy_x = p_SimuStick.x()-20;
                mJoy_y = p_SimuStick.y()-15;   //qDebug()<<"mJoy_y:"<<mJoy_y  <<"    event->pos().y:"<<event->pos().y() <<"    ui->tabWidConfig->pos().y:"<<ui->tabWidConfig->pos().y();
            }
            else
            {
                mJoy_x = 80;
                mJoy_y = 80;
            }
            mJoy_btn = btnStatus;
            updateVirtualJoyStick(mJoy_x,mJoy_y);
        }
    }
}

int MainWindow::getOptimalFocusValue(int lensVal)
{

    if(lensVal <= 0) return hdCmdFocalToVal[0].focval;
    if(lensVal >= hdCmdFocalToVal[HDCMDFOCALTO-1].hdLensVal)
        return hdCmdFocalToVal[HDCMDFOCALTO-1].focval;

    // 查找焦距
    for(int i = 0; i < HDCMDFOCALTO-1; i++)
    {
        if(lensVal >= hdCmdFocalToVal[i].hdLensVal && lensVal < hdCmdFocalToVal[i+1].hdLensVal)
        {
            // 线性插值计算
            int val1 = hdCmdFocalToVal[i].hdLensVal;
            int val2 = hdCmdFocalToVal[i+1].hdLensVal;
            int focus1 = hdCmdFocalToVal[i].focval;
            int focus2 = hdCmdFocalToVal[i+1].focval;

            // 线性插值公式: focus = focus1 + (focus2-focus1) * (lensVal-val1) / (val2-val1)
            int focusValue = focus1 + (focus2 - focus1) * (lensVal - val1) / (val2 - val1);
            return focusValue;
        }
    }

    // 如果没有找到合适区间，返回最接近的值
    int minDiff = abs(lensVal - hdCmdFocalToVal[0].hdLensVal);
    int bestIndex = 0;

    for(int i = 1; i < HDCMDFOCALTO; i++)
    {
        int diff = abs(lensVal - hdCmdFocalToVal[i].hdLensVal);
        if(diff < minDiff)
        {
            minDiff = diff;
            bestIndex = i;
        }
    }

    return hdCmdFocalToVal[bestIndex].focval;
}


void MainWindow::sendAutoFocusCommand(int focusValue, QUdpSocket* udpSocket)
{
    if(udpSocket == nullptr) return;

    // 对焦值范围 0x1000 ~ 0x8000
    if(focusValue < 0x1000) focusValue = 0x1000;
    if(focusValue > 0x8000) focusValue = 0x8000;

    uint8_t focusCmd[9];
    memcpy(focusCmd, hdCmdFocalTo, sizeof(hdCmdFocalTo));

    focusCmd[4] = (focusValue>>12)&0xf;
    focusCmd[5] = (focusValue>>8)&0xf;
    focusCmd[6] = (focusValue>>4)&0xf;
    focusCmd[7] = (focusValue>>0)&0xf;

    udpSocket->write((char *)focusCmd, sizeof(focusCmd));
    qDebug() << "Auto Focus: lensVal=" << hdLensVal << "mm, focusValue=" << focusValue;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{//移动
    int camWidth,camHeight,pos_x=0,pos_y=0;
    float rate_w=0,rate_h=0;

    QPoint p_re = event->pos() - ui->lblPad->pos();
    //qDebug()<<"mouse posX:"<<posX<<"    posY:"<<posY;
    camWidth = IMAGE_PIXEL_WIDTH;
    camHeight = IMAGE_PIXEL_HEIGHT;
    rate_w = (float)ui->lblPad->width()/(float)camWidth;
    rate_h = (float)ui->lblPad->height()/(float)camHeight;
    if(rate_w < rate_h)
    {
        rate_h = rate_w;
        pos_y = p_re.y()-(ui->lblPad->height() -camHeight * rate_h)/2;
    }
    else
    {
        pos_y = p_re.y();
    }
    pos_x = p_re.x()/rate_w;
    pos_y = pos_y / rate_h;
    if(pos_x > 0 && pos_x <=camWidth && pos_y > 0 && pos_y <=camHeight)
    {
        posX = pos_x;
        posY = pos_y;
    }
    getMouseOnVirtualJoyStk(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{//单击
    //if(event->pos().x() > 20 && (event->pos().y() > 20) && event->pos().x() < 950 && (event->pos().y() < 518))
    if(event->pos().x() > 20 && (event->pos().y() > 20) && event->pos().x() < 620 && (event->pos().y() < 320))
    {
        ui->rb_selGDA->setChecked(true);
        ui->lblPad->setFrameShape(QFrame::WinPanel);
        ui->lblPad_B->setFrameShape(QFrame::NoFrame);
        ui->lblPad->setFrameShadow(QFrame::Raised);
        ui->lblPad_B->setFrameShadow(QFrame::Sunken);
    }
    //else if(event->pos().x() > 700 && (event->pos().y() > 20) && event->pos().x() < 1890 && (event->pos().y() < 518))
    else if(event->pos().x() > 700 && (event->pos().y() > 20) && event->pos().x() < 1890 && (event->pos().y() < 320))
    {
        ui->rb_selGDB->setChecked(true);
        ui->lblPad_B->setFrameShape(QFrame::WinPanel);
        ui->lblPad->setFrameShape(QFrame::NoFrame);
        ui->lblPad_B->setFrameShadow(QFrame::Raised);
        ui->lblPad->setFrameShadow(QFrame::Sunken);
    }

    QPoint p_re = event->pos() - ui->lblPad->pos();

    if(p_re.x() > 0 && p_re.x() < ui->lblPad->width() && p_re.y() > 0 && p_re.y() < ui->lblPad->height())
    {
        // 如果是鼠标左键按下
        if (event->button() == Qt::LeftButton)
        {
            btnStatus=1;
            //qDebug() << "left click";
        }
        // 如果是鼠标右键按下
        else if (event->button() == Qt::RightButton)
        {
            btnStatus=2;
            //qDebug() << "right click";
        }
        else if (event->button() == Qt::MidButton)
        {
            //qDebug() << "mid click";
        }

        if( OPMODE_STICK == operateMode )
        {
            //qDebug()<<"mouse posX:"<<posX<<"    posY:"<<posY;
            //sendCmd();
        }
        else if( OPMODE_MARKING == operateMode)
        {
            if(1 == btnStatus) // 左键点击启动跟踪
            {
                //int r_x=0,r_y=0;
                //calcScreenPos(&r_x,&r_y,p_re.x(),p_re.y(),1920,1080);
                int camWidth,camHeight,pos_x=0,pos_y=0;
                float rate_w=0,rate_h=0;
                //if(currentMainSrc == 1)
                {
                    camWidth = IMAGE_PIXEL_WIDTH;
                    camHeight = IMAGE_PIXEL_HEIGHT;
                }
               /* else
                {
                    camWidth = IMAGE_PIXEL_WIDTH_IR;
                    camHeight = IMAGE_PIXEL_HEIGHT_IR;
                }*/
                // 计算鼠标在图像中的实际坐标 UI坐标 → 图像坐标
                rate_w = (float)ui->lblPad->width()/(float)camWidth;
                rate_h = (float)ui->lblPad->height()/(float)camHeight;
                // 坐标转换
                if(rate_w < rate_h)
                {
                    rate_h = rate_w;
                    pos_y = p_re.y()-(ui->lblPad->height() -camHeight * rate_h)/2;
                }
                else
                {
                    pos_y = p_re.y();
                }
                pos_x = p_re.x()/rate_h;
                pos_y = pos_y / rate_h;
                if(currentMainSrc == 2)
                {
                    //pos_x = pos_x - 128;
                    pos_x = pos_x - 32;
                }
                if(pos_x > 0 && pos_x <=camWidth && pos_y > 0 && pos_y <=camHeight)
                {
                    posX = pos_x;
                    posY = pos_y;
                    //sendCmd();
                }


                // 启动小目标跟踪
                qDebug() << "切换到小目标精确跟踪模式，点击位置: (" << posX << "," << posY << ")";
                ui->rb_SelDetectNone->setChecked(false);  // 取消选择停止
                ui->rb_SelDetectMtd2->setChecked(true);  //由于有时候没有目标框，添加这个
                // 设置跟踪参数
                trackerThd->bTrackerStart = true;
                trackerThd->boxChanged = true;
                trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
                // 设置参数
                trackerThd->irParams.tarLmax = 15;
                trackerThd->irParams.k1 = 3;
                trackerThd->irParams.k2 = 2;
                trackerThd->irParams.scoreThreshold = 2.5f;
                // 设置ROI窗口大小（roiTrackbydetect函数）
                trackerThd->roi_width_ = 320;
                trackerThd->roi_height_ = 256;
                // 设置指定的跟踪中心点
                cv::Point2f center = cv::Point2f(posX, posY);
                trackerThd->setCustomCenter(center);
                // 重置PID控制器
                PidX.enable_I_flag = false;
                PidY.enable_I_flag = false;
                timerDelayPid->start(2000);
                // 更新UI
                ui->te_Output->append("光电A跟踪状态：小目标精确跟踪");
                ui->cbxStartTracking->setChecked(true);


                // 启动KCF跟踪
               // qDebug()<<"Clicked r_x:"<<posX<<"    posY:"<<posY;
               // trackerThd->roi_min_width = posX-50;
               // trackerThd->roi_min_height = posY-40;
               // trackerThd->bTrackerStart = true;
               // trackerThd->boxChanged = true;
               // trackerThd->trackingMethod = TRACKING_METHOD_KCF;
               // trackerThd->roi_width = 100;
               // trackerThd->roi_height = 80 ;
               // PidX.enable_I_flag = false;
               // PidY.enable_I_flag = false;
               // timerDelayPid->start(2000);

                //LabStatebarInfo->setText("跟踪状态：跟踪中");
                /*ui->lblTrackingFps->setEnabled(true);
                ui->lblTrackingFps->setVisible(true);
                ui->label_trackfps->setVisible(true);*/
            }
            else if( 2 == btnStatus)  // 右键点击 - 停止跟踪
            {    
                trackerThd->bTrackerStart = false;
                on_pbStop_pressed();      // 停止云台运动
                ui->cbxStartTracking->setChecked(false);
                //ui->lblTrackingFps->setText("0");
                //LabStatebarInfo->setText("跟踪状态：结束");
                /*ui->lblTrackingFps->setText("0");
                ui->lblTrackingFps->setEnabled(false);
                ui->lblTrackingFps->setVisible(false);
                ui->label_trackfps->setVisible(false);*/
                btnStatus = 0;
                PIDReset2(&PidX);
                PIDReset2(&PidY);
                //PIDReset2(&sysConfig.PidXSpeed);
                //PIDReset2(&sysConfig.PidYSpeed);
                ui->te_Output->append("光电A跟踪状态：停止");
            }
            ui->lblPad->update();
        }
    }
    //光电B的鼠标点击跟踪
    QPoint p_re_B = event->pos() - ui->lblPad_B->pos();
    if(p_re_B.x() > 0 && p_re_B.x() < ui->lblPad_B->width() && p_re_B.y() > 0 && p_re_B.y() < ui->lblPad_B->height())
    {
        if (event->button() == Qt::LeftButton)
        {
            btnStatus=1;
        }
        else if (event->button() == Qt::RightButton)
        {
            btnStatus=2;
        }

        if( OPMODE_MARKING == operateMode)
        {
            if(1 == btnStatus) // 左键点击启动跟踪
            {
                // 确保选择了光电B
                ui->rb_selGDB->setChecked(true);

                // 计算鼠标在图像中的实际坐标
                int camWidth = IMAGE_PIXEL_WIDTH;
                int camHeight = IMAGE_PIXEL_HEIGHT;
                float rate_w = (float)ui->lblPad_B->width()/(float)camWidth;
                float rate_h = (float)ui->lblPad_B->height()/(float)camHeight;

                int pos_x, pos_y;
                if(rate_w < rate_h)
                {
                    rate_h = rate_w;
                    pos_y = p_re_B.y()-(ui->lblPad_B->height() -camHeight * rate_h)/2;
                }
                else
                {
                    pos_y = p_re_B.y();
                }
                pos_x = p_re_B.x()/rate_h;
                pos_y = pos_y / rate_h;

                if(pos_x > 0 && pos_x <=camWidth && pos_y > 0 && pos_y <=camHeight)
                {
                    posX = pos_x;
                    posY = pos_y;

                    // 启动小目标跟踪
                    trackerThd->bTrackerStart = true;
                    trackerThd->boxChanged = true;
                    trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
                    // 设置跟踪参数
                    trackerThd->irParams.tarLmax = 15;
                    trackerThd->irParams.k1 = 3;
                    trackerThd->irParams.k2 = 2;
                    trackerThd->irParams.scoreThreshold = 2.5f;
                    trackerThd->roi_width_ = 320;
                    trackerThd->roi_height_ = 256;
                    cv::Point2f center = cv::Point2f(posX, posY);
                    trackerThd->setCustomCenter(center);

                    // qDebug()<<"Clicked r_x:"<<posX<<"    posY:"<<posY;
                    // trackerThd->roi_min_width = posX-50;
                    // trackerThd->roi_min_height = posY-40;
                    // trackerThd->bTrackerStart = true;
                    // trackerThd->boxChanged = true;
                    // trackerThd->trackingMethod = TRACKING_METHOD_KCF;
                    // trackerThd->roi_width = 100;
                    // trackerThd->roi_height = 80 ;


                    // // 重置PID控制器
                    // PidX.enable_I_flag = false;
                    // PidY.enable_I_flag = false;
                    // timerDelayPid->start(2000);

                    ui->te_Output->append("光电B跟踪状态：小目标精确跟踪");
                    ui->cbxStartTracking->setChecked(true);
                    ui->rb_SelDetectMtd2->setChecked(true);  //由于有时候没有目标框，添加这个
                }
            }
            else if( 2 == btnStatus)  // 右键点击 - 停止跟踪
            {
                trackerThd->bTrackerStart = false;
                on_pbStop_pressed();
                ui->cbxStartTracking->setChecked(false);
                btnStatus = 0;
                PIDReset2(&PidX);
                PIDReset2(&PidY);
                ui->te_Output->append("光电B跟踪状态：停止");
            }
        }
    }

	getMouseOnVirtualJoyStk(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{//释放
    //setMouseState(MouseState::Release, 0);
    btnStatus=0;
    mJoy_x = 80;
    mJoy_y = 80;
    updateVirtualJoyStick(mJoy_x,mJoy_y);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{//双击
    // 如果是鼠标左键按下
    if (event->button() == Qt::LeftButton){
        //qDebug() << "left double click";
        //setMouseState(MouseState::L_DC, 0);
    }
    else if (event->button() == Qt::RightButton){
        //qDebug() << "right double click";
        //setMouseState(MouseState::R_DC, 0);
    }
}

//滚轮
void MainWindow::wheelEvent(QWheelEvent *event)
{
    int wheel_val = event->delta();

    // 当滚轮远离使用者时
    if (wheel_val > 0){
        if(scale<80) scale++;
    }
    else{//当滚轮向使用者方向旋转时
        if(scale>40) scale--;
    }
}


#if 0
void MainWindow::on_pushButton_Send_clicked()
{
    /*
    qint64 writeResult = udpSocket->write(ui->textEdit_Send->toPlainText().toLatin1());
    bool BoolFlush = udpSocket->flush();
    if(writeResult != -1 && BoolFlush==1)
    {
        if(writeResult==0)
        {
            QMessageBox::warning(this,"warning",tr("write data returon 0"),QMessageBox::Yes,QMessageBox::No);
        }
        QMessageBox::warning(this,"warning",tr("write data success "),QMessageBox::Yes,QMessageBox::No);
    }
    */
       QByteArray datagram;
       QHostAddress address;
       address.setAddress(serverIp);//发送者要把数据发送到的ip地址
       udpSocket->writeDatagram(datagram,address,serverPort);//发送者把数据发送的端口号，需要接受者绑定该端口号

}
#endif

QString MainWindow::uncharToQstring(unsigned char * id,int len)
{
    QString temp,msg;
    int j = 0;

    while (j<len)
    {
        temp = QString("%1").arg((int)id[j], 2, 16, QLatin1Char('0'));
        msg.append(temp);
        j++;
    }

    return msg;
}

void MainWindow::sendCmd()
{
    unsigned char cmd[10];

    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    cmd[0]=PKT_HEADER;
    cmd[1]=PKT_TYPE_POS;
    cmd[2]=(posX >> 8) & 0xff;
    cmd[3]=posX & 0xff;
    cmd[4]=(posY >> 8) & 0xff;
    cmd[5]=posY & 0xff;
    cmd[6]=btnStatus;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    cmd[9]=0;
    udpskt->write((char *)cmd,8);
}

void MainWindow::on_SendText_changed()
{
    //ui->pushButton_Send->setEnabled(true);
}

// void MainWindow::CenterSocket_Read_Data()
// {
//     double direction=0;
//     double pitch=0;
//     int zkDist = 0;
//     int camVal=0;
//     int selDetectFunc = 0;
//     int autoZoomVal = 10209;
//     bool chktrkflag = false;

//     if(connectRtspFlag_A == false && connectRtspFlag_B == false)
//     {
//         return;
//     }

//     int retu = pnetworkComm->centerSocket_Read_Data();
//     if(retu != 0 ) return;

//     ui->lbRecvZhiKongPkt->setText(QString::number(pnetworkComm->recvZhiKongPktCnt));

//     if(ui->cbxIgnorCenterCmd->isChecked() == true)  //超越指控命令
//     {
//         return;
//     }

//     // 获取位掩码标志位
//     uint8_t pktTypeFlag = pnetworkComm->msgFromZhiKong.pktType;
//     uint16_t mouseX = pnetworkComm->msgFromZhiKong.shubiaox;
//     uint16_t mouseY = pnetworkComm->msgFromZhiKong.shubiaoy;
//     uint8_t suanfaValue = pnetworkComm->msgFromZhiKong.suanfa;

//     // qDebug() << "收到指控数据 - pktType:" << pktTypeFlag << ", suanfa:" << suanfaValue << ", mouse:(" << mouseX << "," << mouseY << ")";

//     // 按位检查
//     // bit0: cmdId
//     if(pktTypeFlag & 0x01)
//     {
//         handleCmdIdProcessing();
//     }

//     // bit1: startTracking
//     if(pktTypeFlag & 0x02)
//     {
//         handleStartTrackingProcessing();
//     }

//     // bit2: suanfa
//     if(pktTypeFlag & 0x04)
//     {
//         handleAlgorithmProcessing();
//     }

//     // bit3: fangweifuyangjuli
//     if(pktTypeFlag & 0x08)
//     {
//         handleDirectionPitchDistanceProcessing();
//     }

//     // bit4: 变倍对焦
//     if(pktTypeFlag & 0x10)
//     {
//         handleZoomFocusProcessing();
//     }

//     // bit5: shubiao 鼠标点击
//     if(pktTypeFlag & 0x20)
//     {
//         handleMouseProcessing();
//     }

//     // 如果没有任何标志位被设置，则按默认cmdId方式处理
//     // if(pktTypeFlag == 0)
//     // {
//     //     qDebug() << "pktType为0，按默认cmdId方式处理";
//     //     handleCmdIdProcessing();
//     // }
// }

void MainWindow::CenterSocket_Read_Data()
{
    double direction=0;
    double pitch=0;
    int zkDist = 0;
    int camVal=0;
    int selDetectFunc = 0;
    int autoZoomVal = 10209;
    bool chktrkflag = false;

    if(connectRtspFlag_A == false && connectRtspFlag_B == false)
    {
        return;
    }

    int retu = pnetworkComm->centerSocket_Read_Data();
    if(retu != 0 ) return;

    ui->lbRecvZhiKongPkt->setText(QString::number(pnetworkComm->recvZhiKongPktCnt));

    if(ui->cbxIgnorCenterCmd->isChecked() == true)  //超越指控命令
    {
        return;
    }

    // 首先根据pktType进行主要分发处理
    uint8_t pktTypeFlag = pnetworkComm->msgFromZhiKong.pktType;
    uint16_t mouseX = pnetworkComm->msgFromZhiKong.shubiaox;
    uint16_t mouseY = pnetworkComm->msgFromZhiKong.shubiaoy;
    uint8_t suanfaValue = pnetworkComm->msgFromZhiKong.suanfa;

    qDebug() << "收到指控数据 - pktType:" << pktTypeFlag << ", suanfa:" << suanfaValue << ", mouse:(" << mouseX << "," << mouseY << ")";

    switch(pktTypeFlag)
    {
        case 1: // cmdId 相关处理
            handleCmdIdProcessing();
            break;

        case 2: // startTracking 相关处理
            handleStartTrackingProcessing();
            break;

        case 3: // suanfa 算法相关处理
            handleAlgorithmProcessing();
            break;

        case 4: // fangweifuyangjuli 方位俯仰距离相关处理
            handleDirectionPitchDistanceProcessing();
            break;

        case 5: // 变倍对焦相关处理
            handleZoomFocusProcessing();
            break;

        case 6: // shubiao 鼠标相关处理
            handleMouseProcessing();
            break;

        default:
            qDebug() << "未知的pktType标志位:" << pktTypeFlag;
            ui->te_Output->append(QString("警告：未知pktType=%1，按默认方式处理").arg(pktTypeFlag));
            handleCmdIdProcessing(); // 默认按cmdId方式处理
            break;
    }
}

#if 0
void MainWindow::CenterSocket_Read_Data()
{
    double direction=0;
    double pitch=0;
    int zkDist = 0;
    int camVal=0;
    int selDetectFunc = 0;
    int autoZoomVal = 10209;
    bool chktrkflag = false;

    if(connectRtspFlag_A == false && connectRtspFlag_B == false)
    {
        return;
    }

    int retu = pnetworkComm->centerSocket_Read_Data();
    if(retu != 0 ) return;

    ui->lbRecvZhiKongPkt->setText(QString::number(pnetworkComm->recvZhiKongPktCnt));


    camVal = pnetworkComm->msgFromZhiKong.zkBianBeiDuiJiao;

    if(ui->cbxIgnorCenterCmd->isChecked() == true)  //超越指控命令
    {
        return;
    }
    ui->cbxStartTracking->setChecked(bool((pnetworkComm->msgFromZhiKong.startTracking)&0x1));
    selDetectFunc = (pnetworkComm->msgFromZhiKong.startTracking>>1)&0x3 ; //bits 1~2 是算法选择
    switch(pnetworkComm->msgFromZhiKong.cmdId)
    {
        case 0: //待机
            sendSetCmd(PKT_TYPE_PTZSTOP,0);
            ui->rb_trackStick->setChecked(true);
            pnetworkComm->msgToZhiKong.status = 0;
            //pnetworkComm->msgFromZhiKong.zkDistence = 0;
            //zkDist = 0;
/*
            ui->rb_SelDetectNone->setChecked(true);
            ui->cbxStartTracking->setChecked(false);
            trackerThd->bTrackerStart = false;
            trackerThd->trackingMethod = TRACKING_METHOD_NONE;
*/
            if( 0 == selDetectFunc )
            {
                ui->rb_SelDetectNone->setChecked(true);
                trackerThd->bTrackerStart = false;
                trackerThd->trackingMethod = TRACKING_METHOD_NONE;
                operateMode = OPMODE_STICK;
            }
            else if( 1 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            {
                ui->rb_SelDetectMtd1->setChecked(true);
                trackerThd->bTrackerStart = true;
                // trackerThd->trackingMethod = TRACKING_METHOD_DETECT;
                trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
                // trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
                operateMode = OPMODE_MARKING;
            }
            else if( 2 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            {
                ui->rb_SelDetectMtd2->setChecked(true);
                trackerThd->bTrackerStart = true;
                trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
                // trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
                operateMode = OPMODE_MARKING;
            }

            convZhiKongCameraCmd(camVal);
            ui->lbRecvZhiKongDist->setText(QString::number(zkDist) +'m');
            break;

        case 1: //指控引导
            direction = pnetworkComm->msgFromZhiKong.zkFangWei / ANGLENET2DEG;
            pitch=pnetworkComm->msgFromZhiKong.zkFuYang /ANGLENET2DEG;
            zkDist = pnetworkComm->msgFromZhiKong.zkDistence;
            ui->lbRecvZhiKongFangwei->setText(QString::number(direction,'f',2) );
            ui->lbRecvZhiKongFuyang->setText(QString::number(pitch,'f',2));
            ui->lbRecvZhiKongDist->setText(QString::number(zkDist) +'m');
//qDebug()<<"direction:"<<direction <<"   pitch:"<<pitch;

            if(pitch > 270)
            {
                pitch = pitch - 360;
            }
            if(pitch > 90)
            {
                pitch = 90;
            }
            else if(pitch < -20)
                pitch = -20;
            direction = direction - pGyroComm->msgFromGyro.direction + radar_diffx_A;   //pGyroComm->msgFromGyro.yaw; //雷达叠加了陀螺数据，需要减掉才是车体为参照的方位

            if(direction >= 360) direction = direction - 360;
            else if(direction <= -360) direction = direction + 360;

            //ui->cbxStartTracking->setChecked(bool((pnetworkComm->msgFromZhiKong.startTracking)&0x1));


            //selDetectFunc = (pnetworkComm->msgFromZhiKong.startTracking>>1)&0x3 ; //bits 1~2 是算法选择
            if( 0 == selDetectFunc )
            {
                ui->rb_SelDetectNone->setChecked(true);
                trackerThd->bTrackerStart = false;
                trackerThd->trackingMethod = TRACKING_METHOD_NONE;
            }
            else if( 1 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            {
                ui->rb_SelDetectMtd1->setChecked(true);
                trackerThd->bTrackerStart = true;
                //trackerThd->trackingMethod = TRACKING_METHOD_DETECT;
                trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            }
            else if( 2 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            {
                ui->rb_SelDetectMtd2->setChecked(true);
                trackerThd->bTrackerStart = true;
                trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            }

            if( false == ui->cbxStartTracking->isChecked() || pnetworkComm->msgToZhiKong.numOfObj == 0 )
            {
                sendPosition(direction, pitch,true,true);
                /*autoZoomVal = getZoomFromDistance(zkDist);
                qDebug()<<"===>autoZoomVal:"<<autoZoomVal;
                gotoNewLen(autoZoomVal);*/
            }
            else if( ui->cbxStartTracking->isChecked() && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL && ui->rb_SelDetectMtd2->isChecked())
            {
                // trackerThd->bTrackerStart = true;
                // trackerThd->trackingMethod = TRACKING_METHOD_KCF;
                // trackerThd->roi_min_width = roiBox.x;
                // trackerThd->roi_min_height = roiBox.y;
                // trackerThd->roi_width = roiBox.width;
                // trackerThd->roi_height = roiBox.height ;
                // trackerThd->boxChanged = true;
                // PidX.enable_I_flag = false;
                // PidY.enable_I_flag = false;
                // timerDelayPid->start(2000);

                // 切到IR_SMALL
                trackerThd->bTrackerStart = true;
                trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
                trackerThd->irParams.tarLmax = 15;
                trackerThd->irParams.k1 = 3;
                trackerThd->irParams.k2 = 2;
                trackerThd->irParams.scoreThreshold = 2.5f;

                trackerThd->roi_width_ = 320;  // IR算法专用ROI窗口大小
                trackerThd->roi_height_ = 256;
                // 设置跟踪中心点（从TopHat检测结果中获得）
                cv::Point2f center = cv::Point2f(roiBox.x + roiBox.width/2,
                                                roiBox.y + roiBox.height/2);
                trackerThd->setCustomCenter(center);
                trackerThd->boxChanged = true;
                // PID控制器设置
                PidX.enable_I_flag = false;
                PidY.enable_I_flag = false;
                timerDelayPid->start(2000);
            }
            //else
            {
                convZhiKongCameraCmd(camVal);
            }

            if(ui->rb_Center->isChecked() == false)
            {
                ui->rb_Center->setChecked(true);
                operateMode = OPMODE_CENTER;
                pnetworkComm->msgToZhiKong.status = 1;
            }

            break;

        case 2: break;
        case 3: //归零
            /*if(connectFlag_A != false)
            {
                 udpSocket_A->write((char *)S9ToZero ,sizeof(S9ToZero));
            }
            if(connectFlag_B != false)
            {
                 udpSocket_B->write((char *)S9ToZero ,sizeof(S9ToZero));
            } */
            ui->rb_trackStick->setChecked(true);
            operateMode = OPMODE_STICK;
            pnetworkComm->msgToZhiKong.status = 0;
            break;
#if 0
        case 4:     //将光电1的当前指向设置为xx度

            if(connectFlag_A != false)
            {
                 //udpSocket_A->write((char *)S9ResetDirection ,sizeof(S9ResetDirection));
                if(0x1 == pnetworkComm->msgFromZhiKong.pktType) //校标指令
                {
                    uint8_t S9calibDirection[]={0xff, 0x22, 0x00, 0x00, 0x00, 0x00, 0x22};       //置方位0位
                    S9calibDirection[2] = (pnetworkComm->msgFromZhiKong.zkFangWei>>8)& 0xff;
                    S9calibDirection[3] = (pnetworkComm->msgFromZhiKong.zkFangWei)& 0xff;
                    S9calibDirection[6] = 0xff & (S9calibDirection[1]+ S9calibDirection[2]+ S9calibDirection[3]+ S9calibDirection[4]+ S9calibDirection[5]);
                    udpSocket_A->write((char *)S9calibDirection ,sizeof(S9calibDirection));
                }
            }
            break;
        case 5:     //将光电2的当前指向设置为xx度
            if(connectFlag_B != false)
            {
                 //udpSocket_B->write((char *)S9ResetDirection ,sizeof(S9ResetDirection));
                 if(0x1 == pnetworkComm->msgFromZhiKong.pktType) //校标指令
                 {
                     uint8_t S9calibDirection[]={0xff, 0x22, 0x00, 0x00, 0x00, 0x00, 0x22};       //置方位0位
                     S9calibDirection[2] = (pnetworkComm->msgFromZhiKong.zkFangWei>>8)& 0xff;
                     S9calibDirection[3] = (pnetworkComm->msgFromZhiKong.zkFangWei)& 0xff;
                     S9calibDirection[6] = 0xff & (S9calibDirection[1]+ S9calibDirection[2]+ S9calibDirection[3]+ S9calibDirection[4]+ S9calibDirection[5]);
                     udpSocket_B->write((char *)S9calibDirection ,sizeof(S9calibDirection));
                 }
            }
            break;
#endif
        case 6:     //不自动转图像跟踪
            pnetworkComm->msgToZhiKong.switchTrack = 0;
            break;
        case 7:     //自动转入图像跟踪
            pnetworkComm->msgToZhiKong.switchTrack = 1;
            break;
        case 10:     //退出程序并关机
            quitAndShutdown();
            break;
        default:
            break;

    }
}
#endif


void MainWindow::handleCmdIdProcessing()
{
    double direction=0;
    double pitch=0;
    int zkDist = 0;
    int camVal=0;
    int selDetectFunc = 0;

    camVal = pnetworkComm->msgFromZhiKong.zkBianBeiDuiJiao;
    ui->cbxStartTracking->setChecked(bool((pnetworkComm->msgFromZhiKong.startTracking)&0x1));
    selDetectFunc = pnetworkComm->msgFromZhiKong.suanfa;  // 新的suanfa字节！！！！！

    switch(pnetworkComm->msgFromZhiKong.cmdId)
    {
        case 0: //待机
//            sendSetCmd(PKT_TYPE_PTZSTOP,0);
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
        };
            //on_pbStop_pressed();      // 停止云台运动
            ui->rb_trackStick->setChecked(true);  //勾选手动模式
            pnetworkComm->msgToZhiKong.status = 0;

            // if( 0 == selDetectFunc )
            // {
            //     ui->rb_SelDetectNone->setChecked(true);
            //     trackerThd->bTrackerStart = false;
            //     trackerThd->trackingMethod = TRACKING_METHOD_NONE;
            //     operateMode = OPMODE_STICK;
            // }
            // else if( 1 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            // {
            //     ui->rb_SelDetectMtd1->setChecked(true);
            //     trackerThd->bTrackerStart = true;
            //     trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            //     operateMode = OPMODE_MARKING;
            // }
            // else if( 2 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            // {
            //     ui->rb_SelDetectMtd2->setChecked(true);
            //     trackerThd->bTrackerStart = true;
            //     trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            //     operateMode = OPMODE_MARKING;
            // }

            // convZhiKongCameraCmd(camVal);
            // zkDist = pnetworkComm->msgFromZhiKong.zkDistence;
            // ui->lbRecvZhiKongDist->setText(QString::number(zkDist) +'m');
            // break;

        case 1: //指控引导
            // direction = pnetworkComm->msgFromZhiKong.zkFangWei / ANGLENET2DEG;
            // pitch=pnetworkComm->msgFromZhiKong.zkFuYang /ANGLENET2DEG;
            // zkDist = pnetworkComm->msgFromZhiKong.zkDistence;
            // ui->lbRecvZhiKongFangwei->setText(QString::number(direction,'f',2) );
            // ui->lbRecvZhiKongFuyang->setText(QString::number(pitch,'f',2));
            // ui->lbRecvZhiKongDist->setText(QString::number(zkDist) +'m');

            // if(pitch > 270)
            // {
            //     pitch = pitch - 360;
            // }
            // if(pitch > 90)
            // {
            //     pitch = 90;
            // }
            // else if(pitch < -20)
            //     pitch = -20;
            // direction = direction - pGyroComm->msgFromGyro.direction + radar_diffx_A;

            // if(direction >= 360) direction = direction - 360;
            // else if(direction <= -360) direction = direction + 360;

            // if( 0 == selDetectFunc )
            // {
            //     ui->rb_SelDetectNone->setChecked(true);
            //     trackerThd->bTrackerStart = false;
            //     trackerThd->trackingMethod = TRACKING_METHOD_NONE;
            // }
            // else if( 1 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            // {
            //     ui->rb_SelDetectMtd1->setChecked(true);
            //     trackerThd->bTrackerStart = true;
            //     trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            // }
            // else if( 2 == selDetectFunc && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL)
            // {
            //     ui->rb_SelDetectMtd2->setChecked(true);
            //     trackerThd->bTrackerStart = true;
            //     trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            // }

            // if( false == ui->cbxStartTracking->isChecked() || pnetworkComm->msgToZhiKong.numOfObj == 0 )
            // {
            //     sendPosition(direction, pitch,true,true);
            // }
            // else if( ui->cbxStartTracking->isChecked() && trackerThd->trackingMethod != TRACKING_METHOD_IR_SMALL && ui->rb_SelDetectMtd2->isChecked())
            // {
            //     startIRSmallTargetTracking();
            // }

            // convZhiKongCameraCmd(camVal);

            if(ui->rb_Center->isChecked() == false)
            {
                ui->rb_Center->setChecked(true);    //勾选指控引导
                operateMode = OPMODE_CENTER;
                pnetworkComm->msgToZhiKong.status = 1;
            }
            break;

        case 2: break;
        case 3: //归零
            ui->rb_trackStick->setChecked(true);
            operateMode = OPMODE_STICK;
            pnetworkComm->msgToZhiKong.status = 0;
            break;
        case 10: //退出程序并关机
            quitAndShutdown();
            break;
        default:
            break;
    }
}


void MainWindow::handleStartTrackingProcessing()
{
    ui->te_Output->append("处理跟踪启动指令");

    bool trackingEnabled = bool((pnetworkComm->msgFromZhiKong.startTracking)&0x1);
    ui->cbxStartTracking->setChecked(trackingEnabled);

    if(trackingEnabled) {
        // 使用当前算法设置
        // int selDetectFunc = pnetworkComm->msgFromZhiKong.suanfa;
        // applyTrackingAlgorithm(selDetectFunc);
        // ui->te_Output->append(QString("启动跟踪，算法=%1").arg(selDetectFunc));
    } else {
        // 停止跟踪
        trackerThd->bTrackerStart = false;
        trackerThd->trackingMethod = TRACKING_METHOD_NONE;
        ui->rb_SelDetectNone->setChecked(true);
        operateMode = OPMODE_STICK;
        ui->te_Output->append("停止跟踪");
    }
}


void MainWindow::handleAlgorithmProcessing()
{
    int selDetectFunc = pnetworkComm->msgFromZhiKong.suanfa;
    ui->te_Output->append(QString("处理算法选择指令: %1").arg(selDetectFunc));

    applyTrackingAlgorithm(selDetectFunc);
}

void MainWindow::applyTrackingAlgorithm(int algorithmId)
{
    switch(algorithmId) {
        case 0:
            ui->rb_SelDetectNone->setChecked(true);
            trackerThd->bTrackerStart = false;
            trackerThd->trackingMethod = TRACKING_METHOD_NONE;
            operateMode = OPMODE_STICK;
            ui->te_Output->append("算法: 无跟踪");
            break;

        case 1:
            ui->rb_SelDetectMtd1->setChecked(true);
            trackerThd->bTrackerStart = true;
            trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            operateMode = OPMODE_MARKING;
            ui->te_Output->append("算法: TopHat检测");
            break;

        case 2:
            ui->rb_SelDetectMtd2->setChecked(true);
            trackerThd->bTrackerStart = true;
            trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
            operateMode = OPMODE_MARKING;
            ui->te_Output->append("算法: TopHat检测");
            break;

        default:
            qDebug() << "未知算法ID:" << algorithmId;
            ui->te_Output->append(QString("警告: 未知算法ID %1").arg(algorithmId));
            break;
    }
}


void MainWindow::handleDirectionPitchDistanceProcessing()
{
    double direction = pnetworkComm->msgFromZhiKong.zkFangWei / ANGLENET2DEG;
    double pitch = pnetworkComm->msgFromZhiKong.zkFuYang / ANGLENET2DEG;
    int zkDist = pnetworkComm->msgFromZhiKong.zkDistence;

     // 验证惯导数据
     if (!validateGyroData()) {
         ui->te_Output->append("警告：惯导数据异常！！！！！！！！！！");
         return;
     }
     // 调试惯导角度
    debugGyroAngles();

//    ui->te_Output->append(QString("处理方位俯仰距离指令 - 方位:%.2f°, 俯仰:%.2f°, 距离:%1m")
//                         .arg(direction, 0, 'f', 2)
//                         .arg(pitch, 0, 'f', 2)
//                         .arg(zkDist));

    // 俯仰角处理
    if(pitch > 270) pitch = pitch - 360;
    if(pitch > 90) pitch = 90;
    else if(pitch < -20) pitch = -20;

    // 方位角处理
    double gyroDirectionDeg = getGyroDirectionInDegrees();   // 和上报指控一样，惯导统一使用度
    direction = direction - gyroDirectionDeg + radar_diffx_A;
    //direction = direction - pGyroComm->msgFromGyro.direction + radar_diffx_A;

    if(direction >= 360) direction = direction - 360;
    else if(direction <= -360) direction = direction + 360;

//    qDebug() << QString("指控方位转换: 指控下发方位 - 惯导%2° + 补偿%3° = %4°")
//               .arg(gyroDirectionDeg, 0, 'f', 3)
//               .arg(radar_diffx_A, 0, 'f', 3)
//               .arg(direction, 0, 'f', 3);

    // 更新界面显示
    ui->lbRecvZhiKongFangwei->setText(QString::number(direction,'f',2));
    ui->lbRecvZhiKongFuyang->setText(QString::number(pitch,'f',2));
    ui->lbRecvZhiKongDist->setText(QString::number(zkDist) + 'm');

    // 执行位置引导
    sendPosition(direction, pitch, true, true);

    // 设置为中心引导模式
    ui->rb_Center->setChecked(true);
    operateMode = OPMODE_CENTER;
    pnetworkComm->msgToZhiKong.status = 1;
}


void MainWindow::handleZoomFocusProcessing()
{
    int camVal = pnetworkComm->msgFromZhiKong.zkBianBeiDuiJiao;
    ui->te_Output->append(QString("处理变倍对焦指令: %1").arg(camVal));
    convZhiKongCameraCmd(camVal);
}


void MainWindow::handleMouseProcessing()
{
    uint16_t mouseX = pnetworkComm->msgFromZhiKong.shubiaox;
    uint16_t mouseY = pnetworkComm->msgFromZhiKong.shubiaoy;

    ui->te_Output->append(QString("处理鼠标操作指令 - 坐标:(%1,%2)").arg(mouseX).arg(mouseY));

    // 验证坐标有效性
    if(mouseX >= IMAGE_PIXEL_WIDTH || mouseY >= IMAGE_PIXEL_HEIGHT) {
        ui->te_Output->append(QString("错误：鼠标坐标超出范围 (%1,%2)").arg(mouseX).arg(mouseY));
        return;
    }
    if(mouseX == 0 && mouseY == 0) {
        ui->te_Output->append("收到鼠标清零指令");
        return;
    }

    // 启动跟踪
    handleRemoteMouseClick(mouseX, mouseY);
}


void MainWindow::startIRSmallTargetTracking()
{
    trackerThd->bTrackerStart = true;
    trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
    trackerThd->irParams.tarLmax = 15;
    trackerThd->irParams.k1 = 3;
    trackerThd->irParams.k2 = 2;
    trackerThd->irParams.scoreThreshold = 2.5f;
    trackerThd->roi_width_ = 320;
    trackerThd->roi_height_ = 256;

    // 优先使用鼠标坐标，其次使用ROI中心
    uint16_t mouseX = pnetworkComm->msgFromZhiKong.shubiaox;
    uint16_t mouseY = pnetworkComm->msgFromZhiKong.shubiaoy;

    if(mouseX > 0 && mouseY > 0 && mouseX < IMAGE_PIXEL_WIDTH && mouseY < IMAGE_PIXEL_HEIGHT)
    {
        cv::Point2f center = cv::Point2f(mouseX, mouseY);
        trackerThd->setCustomCenter(center);
        ui->te_Output->append(QString("使用鼠标坐标启动IR跟踪: (%1,%2)").arg(mouseX).arg(mouseY));
    }
    else
    {
        cv::Point2f center = cv::Point2f(roiBox.x + roiBox.width/2, roiBox.y + roiBox.height/2);
        trackerThd->setCustomCenter(center);
        ui->te_Output->append("使用TopHat检测结果中心启动IR跟踪");
    }

    trackerThd->boxChanged = true;
    PidX.enable_I_flag = false;
    PidY.enable_I_flag = false;
    timerDelayPid->start(2000);
}


void MainWindow::handleRemoteMouseClick(uint16_t mouseX, uint16_t mouseY)
{
    if(operateMode == OPMODE_MARKING)
    {
        qDebug() << "指控点击位置: (" << mouseX << "," << mouseY << ")";
        ui->rb_SelDetectNone->setChecked(false);
        trackerThd->bTrackerStart = true;
        trackerThd->boxChanged = true;
        trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
        trackerThd->irParams.tarLmax = 15;
        trackerThd->irParams.k1 = 3;
        trackerThd->irParams.k2 = 2;
        trackerThd->irParams.scoreThreshold = 2.5f;
        trackerThd->roi_width_ = 320;
        trackerThd->roi_height_ = 256;
        // 使用指控鼠标坐标设置跟踪中心点
        cv::Point2f center = cv::Point2f(mouseX, mouseY);
        trackerThd->setCustomCenter(center);
        PidX.enable_I_flag = false;
        PidY.enable_I_flag = false;
        timerDelayPid->start(2000);

        // 更新UI
        ui->te_Output->append("跟踪状态：指控鼠标小目标精确跟踪");
        ui->cbxStartTracking->setChecked(true);

        // 触发界面更新
        update();
    }
    else
    {
        // 如果不在标记模式，切换到标记模式然后启动跟踪
        operateMode = OPMODE_MARKING;
        ui->rb_Tracking->setChecked(true);
        handleRemoteMouseClick(mouseX, mouseY); // 递归调用
    }
}


int MainWindow::convZhiKongCameraCmd(int camVal)
{
    static int sendFocFlag = false;
    static int sendZoomFlag = false;

    switch(camVal)
    {
        case 0:
        //qDebug()<<"case 0:" <<sendZoomFlag <<sendFocFlag;
            if(sendFocFlag == true)
            {
                on_pbFocusF_released();
                sendFocFlag = false;
            }
            if(sendZoomFlag == true)
            {
                on_pbZoomOut_released();
                sendZoomFlag = false;
            }
            break;
        case 1 ... 50:
            on_vsld_SetBeiShu_valueChanged(camVal);
            break;
        case 51:   //变倍停
            //on_pbZoomOut_released();
            break;
        case 52:   //变倍+
            on_pbZoomOut_pressed();
            sendZoomFlag = true;
            break;
        case 53:   //变倍-
            on_pbZoomIn_pressed();
            sendZoomFlag = true;
            break;
        case 100:   //自动对焦
            on_pbAutoFoc_clicked();
            break;
        case 101:   //对焦+
            on_pbFocusF_pressed();
            sendFocFlag = true;
            break;
        case 102:   //对焦-
            on_pbFocusN_pressed();
            sendFocFlag = true;
            break;
        default:
            break;
    }
    return 0;
}

void MainWindow::GyroSocket_Read_Data()
{
    if(ui->cbxIgnoreGyroData->isChecked())
    {
        pGyroComm->msgFromGyro.roll =0;
        pGyroComm->msgFromGyro.pitch =0;
        pGyroComm->msgFromGyro.yaw =0;
        pGyroComm->msgFromGyro.B =0;
        pGyroComm->msgFromGyro.L =0;
        pGyroComm->msgFromGyro.H =0;
        pGyroComm->msgFromGyro.direction =0;
        return;
    }
    else
    {
        pGyroComm->gyroSocket_Read_Data();
    }

    mGyroPitch_A = pGyroComm->msgFromGyro.roll * cos(Deg2Rad(90 - mPtzDirection_A)) + pGyroComm->msgFromGyro.pitch * cos(Deg2Rad(mPtzDirection_A));
    mGyroPitch_B = pGyroComm->msgFromGyro.roll * cos(Deg2Rad(90 - mPtzDirection_B)) + pGyroComm->msgFromGyro.pitch * cos(Deg2Rad(mPtzDirection_B));
    ui->lbRecvGyroPkt->setText(QString::number(pGyroComm->recvPacketCnt));
    ui->lbRecvGyroWeiDu->setText(QString::number(pGyroComm->msgFromGyro.B,'f',10 ));
    ui->lbRecvGyroJingdu->setText(QString::number(pGyroComm->msgFromGyro.L,'f',10 ));
    ui->lbRecvGyroGaodu->setText(QString::number(pGyroComm->msgFromGyro.H,'f',3 ));
    ui->lbRecvGyroGpsDirect->setText(QString::number(pGyroComm->msgFromGyro.direction,'f',2 ));
    ui->lbRecvGyroRoll->setText(QString::number(pGyroComm->msgFromGyro.roll,'f',2 ));
    ui->lbRecvGyroPitch->setText(QString::number(pGyroComm->msgFromGyro.pitch,'f',2 ));
    ui->lbRecvGyroYaw->setText(QString::number(pGyroComm->msgFromGyro.yaw,'f',2 ));

    ui->lbGyroPitch_A->setText(QString::number(mGyroPitch_A,'f',2 ));

}

/*void printFrame(QByteArray& ba)
{
    QString str;
    for(int i=0; i<ba.size(); i++){
        str += QString("%1").arg((unsigned char)ba.at(i), 2, 16, QLatin1Char('0') ) + ' ';
    }

    qDebug() << str;
} */

//视角计算公式
//1920      =  2 * arcTan(5.3 /2f)
//640      =  2 * arcTan(10.88 /2f)
void MainWindow::socket_Read_Data_A()
{
    QUdpSocket * udpskt = udpSocket_A;  //getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray buffer;
    //uint8_t cmdbuf[7];
    buffer = udpskt->readAll();
    int p=0;
    int direction = 0;
    int pitch = 0;
    int zoomVal = 0;
    int focalVal =0;
    recvPktNum_A++;
//qDebug()<<"socket_Read_Data_A() recvPktNum_A:"<<recvPktNum_A;
    ui->lbRecvPkt_A->setText(QString::number(recvPktNum_A) );
    if(buffer.length() < 7)
    {
        //qDebug()<<"ptz buffer len="<<buffer.length();
        return;
    }
    //qDebug()<<"bufLen:"<<buffer.length();
    //printFrame(buffer);
    //QString qstr =  buffer.toHex(' ');
    //ui->te_Output->append(qstr);

    int ch0=0;
    int ch1=0;
    int ch2=0;
    int ch3=0;
    int ch4=0;
    int ch5=0;
    int k=0;
    for(int i=0;i<2;i++)
    {

        ch0 = (uint8_t)buffer[0+k] & 0xff;
        ch1 = (uint8_t)buffer[1+k] & 0xff;
        switch (ch0)
        {
            case 0xff: // 标准云台数据
                ch3 = (uint8_t)buffer[3+k] & 0xff;
                //qDebug()<<"aa:"<<aa;
                switch (ch3)
                {
                    case 0x59:  // 方位角数据
                        direction = (((uint8_t)buffer[4+k]<<8)&0xff00)+ (uint8_t)buffer[5+k];
                        //qDebug()<<"direction:" << direction;
                        mRawPtzDirection_A = ((double)direction/(double)100.0);
                        mPtzDirection_A = mRawPtzDirection_A + dd_diffx_default_A;
                        if(mPtzDirection_A > 360) mPtzDirection_A = mPtzDirection_A - 360;

                        ui->lerawdira->setText( QString::number(mRawPtzDirection_A, 'f',3)+"°");
                        break;
                    case 0x5b: // 俯仰角数据
                        pitch = (((uint8_t)buffer[4+k]<<8)&0xff00)+ (uint8_t)buffer[5+k];        //Range:0~9000 and 27000~35999
                        //qDebug()<<"pitch:" << pitch;
                        switch(pitch)
                        {
                            case 0 ... 9000:
                                mPtzPitch_A = -((double)pitch/(double)100.0) + dd_diffy_default_A;
                                break;
                            case 27000 ... 35999:
                                mPtzPitch_A = ((double)(35999-pitch)/(double)100.0) + dd_diffy_default_A;
                                break;
                            default:
                                break;
                        }

                    default: break;
                }
                k=k+7;
                break;
            case 0x90:  //HD feedback高清相机反馈
                switch(ch1)
                {
                    case 0x50:  //HD zoom val 变焦值反馈
                        ch2 = (uint8_t)buffer[2+k] & 0xff;
                        ch3 = (uint8_t)buffer[3+k] & 0xff;
                        ch4 = (uint8_t)buffer[4+k] & 0xff;
                        ch5 = (uint8_t)buffer[5+k] & 0xff;
                        zoomVal = ((ch2&0xf)<<12) + ((ch3&0xf)<<8) + ((ch4&0xf)<<4) + (ch5&0xf);
                        if(0 == mFoc_or_zoom_flag )     // 0 is Focal return
                        {
                            //qDebug()<<"A Focus Val:"<< QString::number(zoomVal)<<"  ch2: "<<ch2<<" "<<ch3<<" "<<ch4<<" "<<ch5;
                            //ZoomValToLensData(zoomVal,&hdBianBei,&hdLensVal, &mHdViewAngle_A);
                            ui->lbl_focalVal->setText(QString::number(zoomVal));  //将聚焦值显示ui界面 估计是200ms一次
                        }
                        else    //Zoom pos return 变焦位置反馈模式
                        {
                            //qDebug()<<"zoomVal:"<< QString::number(zoomVal)<<"  ch2: "<<ch2<<" "<<ch3<<" "<<ch4<<" "<<ch5;
                            //ZoomValToLensData(zoomVal,&hdBianBei,&hdLensVal, &mHdViewAngle_A);
                            // qDebug()<<"zoomVal:"<< QString::number(zoomVal);
                            ZoomValToLensData_Improved(zoomVal, &hdBianBei, &hdLensVal, &mHdViewAngle_A);
                            //logZoomAndFOV(zoomVal, hdLensVal, mHdViewAngle_A, "GDA");
                            ui->lbl_lenVal->setText(QString::number(hdLensVal)+"mm");
                            ui->lblBeishu->setText("X" + QString::number(hdBianBei, 'f',1));
                            ui->lblLens->setText( QString::number(mHdViewAngle_A,'f', 1)+"°");
                            hdZoomVal = zoomVal;

                            //sendAutoFocusCommand(8500, udpskt);//所有视场下对焦位置设置为8500，根据实测得到
                            // 新增对焦位置设置
                            // static int lastLensVal = 0;
                            // if(hdLensVal != lastLensVal)
                            // {
                            //     int optimalFocusValue = getOptimalFocusValue(hdLensVal);
                            //     sendAutoFocusCommand(optimalFocusValue, udpskt);
                            //     lastLensVal = hdLensVal;
                            // }

                        }
                        break;
                }
                break;
        }
    }

    // 修正上报数据计算
    if (!validateGyroData()) {
        // 如果惯导数据无效，只上报原始光电数据
        pnetworkComm->msgToZhiKong.gd1FangWei = (uint16_t)(mPtzDirection_A * ANGLENET2DEG);
        pnetworkComm->msgToZhiKong.gd1FuYang = (uint16_t)(mPtzPitch_A * ANGLENET2DEG);
    } else {
        // 统一惯导  使用度
        double gyroDirectionDeg = getGyroDirectionInDegrees();

        double reportDirection = mPtzDirection_A + gyroDirectionDeg - radar_diffx_A;
        double reportPitch = mPtzPitch_A + mGyroPitch_A;

        while(reportDirection >= 360) reportDirection -= 360;
        while(reportDirection < 0) reportDirection += 360;

        pnetworkComm->msgToZhiKong.gd1FangWei = (uint16_t)(reportDirection * ANGLENET2DEG);
        pnetworkComm->msgToZhiKong.gd1FuYang = (uint16_t)(reportPitch * ANGLENET2DEG);

//        qDebug() << QString("光电A上报: 本地%1° + 惯导%2° - 补偿%3° = %4°")
//                   .arg(mPtzDirection_A, 0, 'f', 3)
//                   .arg(gyroDirectionDeg, 0, 'f', 3)
//                   .arg(radar_diffx_A, 0, 'f', 3)
//                   .arg(reportDirection, 0, 'f', 3);
    }

    pnetworkComm->msgToZhiKong.gd1BianBei = (uint16_t)hdLensVal;
#if 0
    //qDebug()<<"direction:"<<ptzDirection<<"    pitch:"<<ptzPitch;
    ui->lblDirection->setText( QString::number(

    mPtzDirection_A, 'f',2)+"°");
    //706测试-伺服上报数据
    // QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    // qDebug() <<"收到伺服上报方位值时间"<< timeStr
    //          << "方位值:" << mPtzDirection_A << "°";
    // //打开文件并写入数据（追加模式）
    // QFile file("pid_receive.txt");
    // if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    //     QTextStream out(&file);
    //     out << timeStr << " " <<mPtzDirection_A<< "\n";
    //     file.close();
    // }
    //ui->lblPitch->setText( QString::number(ptzPitch-dd_diffy_default, 'f',2)+"°");
    ui->lblPitch->setText( QString::number(mPtzPitch_A, 'f',2)+"°");

    pnetworkComm->msgToZhiKong.gd1FangWei = (uint16_t)((mPtzDirection_A + pGyroComm->msgFromGyro.yaw - radar_diffx_A) * ANGLENET2DEG );
    //pnetworkComm->msgToZhiKong.gd1FuYang = (uint16_t)(-(((double)pitch/(double)100.0) + GyroPitch) * ANGLENET2DEG);     //累加上惯导数据
    pnetworkComm->msgToZhiKong.gd1FuYang = (uint16_t)((mPtzPitch_A + mGyroPitch_A) * ANGLENET2DEG);     //累加上惯导数据
    pnetworkComm->msgToZhiKong.gd1BianBei = (uint16_t)hdLensVal;
#endif
}

void MainWindow::socket_Read_Data_B()
{
    QUdpSocket * udpskt = udpSocket_B;  //getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray buffer;
    //uint8_t cmdbuf[7];
    buffer = udpskt->readAll();
    int p=0;
    int direction = 0;
    int pitch = 0;
    int zoomVal = 0;
    int focalVal =0;
    recvPktNum_B++;
    //qDebug()<<"socket_Read_Data_B() recvPktNum_B:"<<recvPktNum_B;
    ui->lbRecvPkt_B->setText(QString::number(recvPktNum_B) );
    if(buffer.length() < 7)
    {
        //qDebug()<<"ptz buffer len="<<buffer.length();
        return;
    }
    //qDebug()<<"bufLen:"<<buffer.length();
    //printFrame(buffer);
    //QString qstr =  buffer.toHex(' ');
    //ui->te_Output->append(qstr);

    int ch0=0;
    int ch1=0;
    int ch2=0;
    int ch3=0;
    int ch4=0;
    int ch5=0;
    int k=0;
    for(int i=0;i<2;i++)
    {

        ch0 = (uint8_t)buffer[0+k] & 0xff;
        ch1 = (uint8_t)buffer[1+k] & 0xff;
        switch (ch0)
        {
            case 0xff:
                ch3 = (uint8_t)buffer[3+k] & 0xff;
                //qDebug()<<"aa:"<<aa;
                switch (ch3)
                {
                    case 0x59:
                        direction = (((uint8_t)buffer[4+k]<<8)&0xff00)+ (uint8_t)buffer[5+k];
                        mRawPtzDirection_B = ((double)direction/(double)100.0);
                        mPtzDirection_B = mRawPtzDirection_B + dd_diffx_default_B;
                        ui->lerawdirb->setText( QString::number(mRawPtzDirection_B, 'f',3)+"°");
                        if(mPtzDirection_B > 360) mPtzDirection_B = mPtzDirection_B - 360;
                        break;
                    case 0x5b:
                        pitch = (((uint8_t)buffer[4+k]<<8)&0xff00)+ (uint8_t)buffer[5+k];        //Range:0~9000 and 27000~35999
                        //qDebug()<<"pitch:" << pitch;
                        switch(pitch)
                        {
                            case 0 ... 9000:
                                mPtzPitch_B = -((double)pitch/(double)100.0) + dd_diffy_default_B;
                                break;
                            case 27000 ... 35999:
                                mPtzPitch_B = ((double)(35999-pitch)/(double)100.0) + dd_diffy_default_B;
                                break;
                            default:
                                break;
                        }

                    default: break;
                }
                k=k+7;
                break;
            case 0x90:  //HD feedback
                switch(ch1)
                {
                    case 0x50:  //HD zoom val
                        ch2 = (uint8_t)buffer[2+k] & 0xff;
                        ch3 = (uint8_t)buffer[3+k] & 0xff;
                        ch4 = (uint8_t)buffer[4+k] & 0xff;
                        ch5 = (uint8_t)buffer[5+k] & 0xff;
                        zoomVal = ((ch2&0xf)<<12) + ((ch3&0xf)<<8) + ((ch4&0xf)<<4) + (ch5&0xf);
                        if(0 == mFoc_or_zoom_flag )     // 0 is Focal return
                        {
                            //qDebug()<<"B Focus Val:"<< QString::number(zoomVal)<<"  ch2: "<<ch2<<" "<<ch3<<" "<<ch4<<" "<<ch5;
                            ui->lbl_focalVal_B->setText(QString::number(zoomVal));
                        }
                        else    //Zoom pos return
                        {
                            //qDebug()<<"B zoomVal:"<< QString::number(zoomVal)<<"  ch2: "<<ch2<<" "<<ch3<<" "<<ch4<<" "<<ch5;
                            //ZoomValToLensData(zoomVal,&hdBianBei_B,&hdLensVal_B, &mHdViewAngle_B);
                            ZoomValToLensData_Improved(zoomVal, &hdBianBei_B, &hdLensVal_B, &mHdViewAngle_B);
                            ui->lbl_lenVal_B->setText(QString::number(hdLensVal_B)+"mm");
                            ui->lblBeishu_B->setText("X" + QString::number(hdBianBei_B));
                            ui->lblLens_B->setText( QString::number(mHdViewAngle_B,'f', 1)+"°");
                            hdZoomVal_B = zoomVal;
                        }
                        break;
                }
                break;
        }
    }

#if 1
    if (!validateGyroData()) {
        pnetworkComm->msgToZhiKong.gd2FangWei = (uint16_t)(mPtzDirection_B * ANGLENET2DEG);
        pnetworkComm->msgToZhiKong.gd2FuYang = (uint16_t)(mPtzPitch_B * ANGLENET2DEG);
    } else {
        double gyroDirectionDeg = getGyroDirectionInDegrees();
        double reportDirection = mPtzDirection_B + gyroDirectionDeg - radar_diffx_B;
        double reportPitch = mPtzPitch_B + mGyroPitch_B;

        while(reportDirection >= 360) reportDirection -= 360;
        while(reportDirection < 0) reportDirection += 360;

        pnetworkComm->msgToZhiKong.gd2FangWei = (uint16_t)(reportDirection * ANGLENET2DEG);
        pnetworkComm->msgToZhiKong.gd2FuYang = (uint16_t)(reportPitch * ANGLENET2DEG);

//        qDebug() << QString("光电B上报: 本地%1° + 惯导%2° - 补偿%3° = %4°")
//                   .arg(mPtzDirection_B, 0, 'f', 3)
//                   .arg(gyroDirectionDeg, 0, 'f', 3)
//                   .arg(radar_diffx_B, 0, 'f', 3)
//                   .arg(reportDirection, 0, 'f', 3);
    }

    pnetworkComm->msgToZhiKong.gd2BianBei = (uint16_t)hdLensVal_B;
#endif

#if 0
    double gyroDirectionDeg = getGyroDirectionInDegrees();
    double reportDirection = mPtzDirection_B + gyroDirectionDeg - radar_diffx_B;
    double reportPitch = mPtzPitch_B + mGyroPitch_B;

    while(reportDirection >= 360) reportDirection -= 360;
    while(reportDirection < 0) reportDirection += 360;

    pnetworkComm->msgToZhiKong.gd2FangWei = (uint16_t)(reportDirection * ANGLENET2DEG);
    pnetworkComm->msgToZhiKong.gd2FuYang = (uint16_t)(reportPitch * ANGLENET2DEG);

    qDebug() << QString("光电B上报: 本地%1° + 惯导%2° - 补偿%3° = %4°")
               .arg(mPtzDirection_B, 0, 'f', 3)
               .arg(gyroDirectionDeg, 0, 'f', 3)
               .arg(radar_diffx_B, 0, 'f', 3)
               .arg(reportDirection, 0, 'f', 3);

    pnetworkComm->msgToZhiKong.gd2BianBei = (uint16_t)hdLensVal_B;
#endif

#if 0
    //qDebug()<<"direction:"<<ptzDirection<<"    pitch:"<<ptzPitch;
    ui->lblDirection_B->setText( QString::number(mPtzDirection_B, 'f',2)+"°");
    //ui->lblPitch->setText( QString::number(ptzPitch-dd_diffy_default, 'f',2)+"°");
    ui->lblPitch_B->setText( QString::number(mPtzPitch_B, 'f',2)+"°");

    pnetworkComm->msgToZhiKong.gd2FangWei = (uint16_t)((mPtzDirection_B + pGyroComm->msgFromGyro.yaw - radar_diffx_B) * ANGLENET2DEG);
    //pnetworkComm->msgToZhiKong.gd2FuYang = (uint16_t)(-(((double)pitch/(double)100.0) + GyroPitch) * ANGLENET2DEG);     //累加上惯导数据
    pnetworkComm->msgToZhiKong.gd2FuYang = (uint16_t)((mPtzPitch_B + mGyroPitch_B) * ANGLENET2DEG);     //累加上惯导数据
    pnetworkComm->msgToZhiKong.gd2BianBei = (uint16_t)hdLensVal_B;
#endif
}

//int MainWindow::ZoomValToLensData(int in_zoomVal,float * BeiShu,float * Lens,double * hfov)
//{
//    int n = 0;
//    if(in_zoomVal> 16385) in_zoomVal = 16384;
//    for(int i=0;i<LENSDATAQNUM_SCZ2090NM;i++)
//    {
//        if(in_zoomVal <= SCZ2090NMlensDataQ[i].Val)
//        {
//            n=i;
//            break;
//        }
//    }
//    *Lens = SCZ2090NMlensDataQ[n].JiaoJu;
//    *BeiShu = SCZ2090NMlensDataQ[n].BeiShu;
//    *hfov = SCZ2090NMlensDataQ[n].HFov;
//    return 0;
//}

int MainWindow::ZoomValToLensData_Improved(int in_zoomVal, int* BeiShu, int* Lens, double* hfov)
{
    if(in_zoomVal > 16385) in_zoomVal = 16384;

    // 找插值区间
    for(int i = 0; i < LENSDATAQNUM_SCZ2090NM-1; i++) {
        if(in_zoomVal >= SCZ2090NMlensDataQ[i].Val &&
           in_zoomVal < SCZ2090NMlensDataQ[i+1].Val) {

            // 线性插值
            double ratio = (double)(in_zoomVal - SCZ2090NMlensDataQ[i].Val) /
                          (SCZ2090NMlensDataQ[i+1].Val - SCZ2090NMlensDataQ[i].Val);

            *Lens = SCZ2090NMlensDataQ[i].JiaoJu +
                   (int)((SCZ2090NMlensDataQ[i+1].JiaoJu - SCZ2090NMlensDataQ[i].JiaoJu) * ratio);

            *BeiShu = SCZ2090NMlensDataQ[i].BeiShu +
                     (int)((SCZ2090NMlensDataQ[i+1].BeiShu - SCZ2090NMlensDataQ[i].BeiShu) * ratio);

            *hfov = SCZ2090NMlensDataQ[i].HFov +
                   (SCZ2090NMlensDataQ[i+1].HFov - SCZ2090NMlensDataQ[i].HFov) * ratio;

            return 0;
        }
    }

    return -1;
}

void MainWindow::logZoomAndFOV(int zoomVal, int lensVal, double fov, const QString& camera) {
    static QFile logFile("zoom_fov_log.txt");
    static QTextStream logStream;

    if(!logFile.isOpen()) {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);
        logStream.setDevice(&logFile);
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    logStream << timestamp << "," << camera << "," << zoomVal << ","
              << lensVal << "," << QString::number(fov, 'f', 6) << "\n";
    logStream.flush();
}

void MainWindow::socket_Disconnected_A()
{
    QUdpSocket * udpskt = udpSocket_A;
    if( udpskt == NULL) return;

    udpskt->close();
}

void MainWindow::socket_Disconnected_B()
{
    QUdpSocket * udpskt = udpSocket_B;
    if( udpskt == NULL) return;

    udpskt->close();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
 //   if(connectFlag == false) return;
    try
    {
        QImage img;
        QPixmap fitPixmap;
        if(true==mImageCh1GetNew)
        {
            if(mImageCh1.isNull())
            {
                return;
            }
            //将图像按比例缩放成和小窗口一样大小

//            auto start = std::chrono::high_resolution_clock::now();

            img = mImageCh1.scaled(ui->lblPad->size(), Qt::KeepAspectRatio);
            //QImage image = mImage.mirrored(false, true);
            fitPixmap=QPixmap::fromImage(img);
            if(fitPixmap.isNull())
            {
                qDebug()<<"fitPixmap is null";
                return;
            }

            if(true== ui->cbxCrosser->isChecked() ) //是否画十字线
            {
                drawCross(ui->lblPad,&fitPixmap);
            }
            
            if(true== ui->cbxRoiBox->isChecked() ) //是否画跟踪框
            {
                if( false == ui->rb_SelDetectNone->isChecked() && ui->rb_selGDA->isChecked())
                {
                    if(trackerThd->bTrackerStart == true && trackerThd->roi_num>0)
                    {
                        drawROIBox(ui->lblPad,&fitPixmap, roiBox );
                    }
                }
            }
#if 0
        if(true== ui->cbxAzimuthScale->isChecked() ) //是否画刻度线
        {
            int lblw;
            int lblh;

            if(currentMainSrc==1)
            {
                lblw=ui->lblPad->size().width()/2;
                lblh=lblw* 0.5625;     //16:9 为：0.5625;

            }
            else
            {
                lblh=ui->lblPad->size().height()/2;
                lblw=lblh * 1.33;     //4:3 为 1.33;

            }

            drawAzimuthScale(&fitPixmap,lblw*2,lblh*2);

        }
        else if(true== ui->cbxMarkline->isChecked() ) //是否画刻度线
            {
                int lblw;
                int lblh;

                if(currentMainSrc==1)
                {
                    lblw=ui->lblPad->size().width()/2;
                    lblh=lblw*0.5625;

                }
                else
                {
                    lblh=ui->lblPad->size().height()/2;
                    lblw=lblh*1.33;

                }

                drawViewAngle(&fitPixmap,lblw*2,lblh*2);

            }
#endif
            /*if(currentMainSrc==1)
            {
                DrawGuideOSD(&fitPixmap,hdViewAngle);
            }
            else
            {
                DrawGuideOSD(&fitPixmap,irViewAngle);
            }*/
            ui->lblPad->setPixmap(fitPixmap);
            mImageCh1GetNew = false;

//            auto end = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double, std::milli> elapsed = end - start;
//            qDebug() << "osd耗时:" << elapsed.count() << "ms" ;
        }
        if(true==mImageCh2GetNew)
        {
            if(mImageCh2.isNull())
            {
                return;
            }
            //将图像按比例缩放成和小窗口一样大小
            img = mImageCh2.scaled(ui->lblPad_B->size(), Qt::KeepAspectRatio);
            fitPixmap=QPixmap::fromImage(img);
            if(fitPixmap.isNull())
            {
                qDebug()<<"fitPixmap is null";
                return;
            }
            if(true== ui->cbxCrosser->isChecked() ) //是否画十字线
            {
                drawCross(ui->lblPad_B,&fitPixmap);
            }

            if(true== ui->cbxRoiBox->isChecked() ) //是否画跟踪框
            {
                if( false == ui->rb_SelDetectNone->isChecked() && ui->rb_selGDB->isChecked())
                {
                    if(trackerThd->bTrackerStart == true && trackerThd->roi_num>0)
                    {
                        drawROIBox(ui->lblPad_B,&fitPixmap, roiBox );
                    }
                }
            }
            ui->lblPad_B->setPixmap(fitPixmap);
            mImageCh2GetNew = false;
        }
    }
    catch (EXCEPTION_RECORD)
    {
        QMessageBox::about(NULL, "提示", "paintEvent 出错");
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    
    //if(connectFlag == false) return false;

    if(watched==ui->lblPadDirection && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        drawAngleMeter(this->ui->lblPadDirection,mPtzDirection_A,mHdViewAngle_A,mPtzPitch_A); //绘制
    }
    if(watched==ui->lblPadDirection_B && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        drawAngleMeter(this->ui->lblPadDirection_B,mPtzDirection_B,mHdViewAngle_B,mPtzPitch_B); //绘制
    }

    if(watched==ui->lbl_SimJoystick && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        drawVirtualJoyStick(mJoy_x,mJoy_y,mJoy_btn);

    }
    /*if(watched==ui->lbl_padAngleGap && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        //if(operateMode == 1)
        {
            lblPaintAngleGap(ptzDirection,ptzPitch,dd_diffx,dd_diffy-dd_diffy_default); //绘制
        }
    }*/
    /*if(watched==ui->lbl_stickEXPO && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        drawStickExpo(mExpo);
    }*/
    return QWidget::eventFilter(watched,event);
    

}

void MainWindow::drawCross(QLabel * uipad, QPixmap * fitPixmap)
{
    QPainter painter;
    painter.begin(fitPixmap);

    if(ui->rb_OSDColorGreen->isChecked())
    {
        painter.setPen(Qt::green);
    }
    else if(ui->rb_OSDColorRed->isChecked())
    {
        painter.setPen(Qt::red);	//红色
    }
    else if(ui->rb_0SDColorBlk->isChecked())
    {
        painter.setPen(Qt::black); //黑
    }
    else
    {
        painter.setPen(Qt::white); //白
    }

    int lblw;
    int lblh;
    if(currentMainSrc==1)
    {
        lblw=uipad->size().width()/2;
        lblh=lblw * 0.5625;     //16:9 为：0.5625;
    }
    else
    {
        lblh=uipad->size().height()/2;
        //lblw=lblh * 1.33;     //4:3 为 1.33;
        lblh=uipad->size().height()/2;
        lblw=uipad->size().width()/2;
        int tmpdif = (uipad->size().width() - fitPixmap->width())/2;
        lblw = lblw -tmpdif;
    }

    /*int nX=(mPan_outVal)+lblw;
    int nY=(mTilt_outVal)+lblh;

    if (nX<lblw-50) nX=lblw-50;
    if (nX>lblw+50) nX=lblw+50;
    if (nY<lblh-50) nY=lblh-50;
    if (nY>lblh+50) nY=lblh+50; */

    painter.drawLine(lblw-10,lblh ,lblw-50,lblh);
    painter.drawLine(lblw+10,lblh ,lblw+50,lblh);
    painter.drawLine(lblw,lblh-10,lblw,lblh-50);
    painter.drawLine(lblw,lblh+10,lblw,lblh+50);
    //painter.drawRect(nX-1, nY-1,2,2);
    painter.end();
}

/*在pad上画方位角度示意
direction: 方位， angle： 视场角*/
void MainWindow::drawAngleMeter(QLabel * uipad,int direction,int angle, int pitch)
{
    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(100,100,0,70));
    brush.setStyle(Qt::SolidPattern);
    QPainter p(uipad);

    p.setBrush(brush);
    p.setPen(pen);
    p.drawEllipse(34,17,80,80);
    pen.setWidth(1);//画笔粗细
    pen.setColor(QColor(255,0,0,255));//画笔颜色
    p.setPen(pen);

    QRectF rectangle(34,17,80,80);
    int startAngle = ((90-direction)-angle/2) * 16 ;//起始角度,单位为1/16度 30 * 16就是30度
    int spanAngle = angle * 16;//夹角
    p.drawPie(rectangle,startAngle,spanAngle);//画扇形

    //俯仰
    pen.setColor(QColor(0,0,0,40));//画笔颜色
    p.setPen(pen);
    p.drawLine(250,60,300,60);
    p.drawLine(250,10,250,105);
    pen.setColor(QColor(255,0,0,255));//画笔颜色
    p.setPen(pen);

    QRectF rectangle2(205,15,90,90);
    startAngle = (pitch-angle/2) * 16 ;//起始角度,单位为1/16度 30 * 16就是30度
    spanAngle = angle * 16;//夹角
    p.drawPie(rectangle2,startAngle,spanAngle);//画扇形
}

//在屏幕上画刻度线
//视角计算公式
//1920      =  2 * arcTan(5.3 /2f)      449~950  150~1500
//640      =  2 * arcTan(10.88 /2f)
void MainWindow::drawViewAngle(QPixmap * fitPixmap,int scrWidth,int scrHeight)
{
#if 0
    int miao;
    float n;
    //int lens;
    double viewAngle;
    if(currentMainSrc == 1)
    {
        viewAngle = hdViewAngle;

    }
    else
    {
        viewAngle = irViewAngle;
    }
    miao = viewAngle * 60*60;
    n= scrWidth/(float)miao*10 ;
    //qDebug()<<"  viewAngle"<<viewAngle <<"   miao"<<miao<<"   n"<<n;

    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(100,100,0,70));
    brush.setStyle(Qt::SolidPattern);
    QPainter p;
    p.begin(fitPixmap);
    pen.setColor(QColor(0,255,0,150));//画笔颜色
    p.setPen(pen);

    //设置显示字体的大小
    QFont font;
    font.setPixelSize(12);
    //p.setPen(pen);
    p.setFont(font);

    //p.drawText(800-28,800,"10\"" );

    int oldPos=scrWidth/2;
    int oldMiaoPos=oldPos;
    p.drawLine(200,scrHeight-100,scrWidth-200,scrHeight-100);
    p.drawLine(oldPos,scrHeight-100-50, oldPos,scrHeight-100);
    for(int i=0;i<4000;i++)
    {
        int curPos= scrWidth/2 + i*n;

        if(curPos>scrWidth-200)
        {
            break;
        }
        if(i%6==0)
        {
            if((curPos-oldMiaoPos) <10)
            {
                continue;
            }
            oldMiaoPos=curPos;
            p.drawLine(curPos,scrHeight-100-3, curPos,scrHeight-100);
            p.drawLine(scrWidth - curPos,scrHeight-100-3, scrWidth - curPos,scrHeight-100);

            if(i%10==0)
            {
                if((curPos-oldPos) <60)
                {
                    continue;
                }
                oldPos = curPos;
                QString ParamText = QString::number(i/6, 10);
                p.drawText(curPos,scrHeight-100-10-5, ParamText);
                p.drawText(scrWidth - curPos,scrHeight-100-10-5, "-"+ParamText);
                p.drawLine(curPos,scrHeight-100-10, curPos,scrHeight-100);
                p.drawLine(scrWidth - curPos,scrHeight-100-10, scrWidth - curPos,scrHeight-100);

            }
        }
        else
        {
            //p.drawLine(curPos,797, curPos,800);
        }

    }

    p.drawLine(150,200,150,scrHeight-200);
    oldPos=scrHeight/2;
    oldMiaoPos=oldPos;
    p.drawLine(150,oldPos, 200,oldPos);
    for(int i=0;i<1800;i++)
    {
        int curPos= scrHeight/2 + i*n;
        if(curPos>scrHeight-200)
        {
            break;
        }

        if(i%6==0)
        {
            if((curPos-oldMiaoPos) <10)
            {
                continue;
            }
            oldMiaoPos=curPos;
            p.drawLine(150,curPos, 150+3,curPos);
            p.drawLine(150,scrHeight-curPos, 150+3,scrHeight-curPos);

            if(i%10==0)
            {
                if((curPos - oldPos) <60)
                {
                    continue;
                }
                oldPos = curPos;
                QString ParamText = QString::number(i/6, 10);
                p.drawText(150+15,curPos, "-"+ParamText);
                p.drawText(150+15,scrHeight-curPos, ParamText);
                p.drawLine(150,curPos, 150+10,curPos);
                p.drawLine(150,scrHeight-curPos, 150+10,scrHeight-curPos);
            }
        }
        else
        {
            //p.drawLine(200,curPos, 200+3,curPos);
        }

    }
#endif
}

#define AZMARGIN 30
#define AZMARGINY 50
#define AZCHARARGIN 12
#define AZCHARARGINY 32

//在屏幕上画方位及俯仰刻度
//视角计算公式
//1920      =  2 * arcTan(5.3 /2f)      449~950  150~1500
//640      =  2 * arcTan(10.88 /2f)
void MainWindow::drawAzimuthScale(QPixmap * fitPixmap,int scrWidth,int scrHeight)
{
#if 0
    double viewAngle;

    if(currentMainSrc == 1)
    {
        viewAngle = hdViewAngle;
    }
    else
    {
        viewAngle = irViewAngle;
    }

    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(3);//画笔粗细
    brush.setColor(QColor(100,100,0,70));
    brush.setStyle(Qt::SolidPattern);
    QPainter p;
    p.begin(fitPixmap);


    if(ui->rb_OSDColorGreen->isChecked())
    {
        p.setPen(Qt::green);
    }
    else if(ui->rb_OSDColorRed->isChecked())
    {
        p.setPen(Qt::red);	//红色
    }
    else if(ui->rb_0SDColorBlk->isChecked())
    {
        p.setPen(Qt::black); //黑
    }
    else
    {
        p.setPen(Qt::white); //白
    }

    //pen.setColor(QColor(0,255,0,150));//画笔颜色
    //p.setPen(pen);

    //设置显示字体的大小
    QFont font;
    font.setPixelSize(24);
    font.setBold(true);
    //p.setPen(pen);
    p.setFont(font);


    int curPix= 0;
    int curPix2= 0;

    curPix=scrWidth/2;
    QString ParamText;

    //p.drawLine(curPix,AZMARGIN, curPix,AZMARGIN+50);


    double leftStartDegPos = ptzDirection - viewAngle / scrWidth * (scrWidth - 200)/2 ;

    double DotDegPerPix = viewAngle*100 / scrWidth;

    //qDebug()<<"===> leftStartDegPos:"<<leftStartDegPos<<" DotDegPerPix:"<<DotDegPerPix<<" viewAngle:"<<viewAngle;

    double pos = leftStartDegPos;
    double dotOnePos = pos;
    int lastPix = 100;
    int cur10Pix = scrWidth / viewAngle / 10 ;  //0.1

    for(int i=0; i<scrWidth-200; i++)
    {
        curPix = 100 + i;
        if( curPix>scrWidth-100 )   //画到线的最右端停止
        {
            break;
        }

        switch((int)viewAngle)
        {
        case 0 ... 2:
            //qDebug()<<"===> pos:"<<pos<<"  (int)pos:"<<(int)pos<<"  curPix:"<<curPix<<"  lastPix:"<<lastPix;
            //double p = qRound(ptzDirection*100)/100 - int(ptzDirection);
            pos = (double)qRound(ptzDirection);
            dotOnePos = (double)qRound(ptzDirection);

            curPix = scrWidth/2 + (dotOnePos - pos)*100*cur10Pix/10;
            curPix2 = curPix - cur10Pix;
            for(int j=0;j<20;j++)
            {
                if( curPix + j*cur10Pix > scrWidth-100 )   //画到线的最右端停止
                {
                    break;
                }
                ParamText = QString::number(pos + (dotOnePos - pos) +j*0.1 ,'f', 1);
                p.drawText(curPix + j*cur10Pix-7,AZMARGIN-AZCHARARGIN, ParamText);
                p.drawLine(curPix + j*cur10Pix,AZMARGIN, curPix + j*cur10Pix,AZMARGIN+3);

                if( curPix2- j*cur10Pix < 100 )   //画到线的最左端停止
                {
                    break;
                }
                ParamText = QString::number(pos - (0.1 - dotOnePos + pos) -j*0.1 ,'f', 1);
                p.drawText(curPix2 - j*cur10Pix-7,AZMARGIN-AZCHARARGIN, ParamText);
                p.drawLine(curPix2 - j*cur10Pix,AZMARGIN, curPix2 - j*cur10Pix,AZMARGIN+3);
                //qDebug()<<"  j:"<<j<<"  curPix2:"<<curPix2 <<"  --:"<<curPix2 - j*cur10Pix <<"  pos:" << pos - (0.1 - dotOnePos + pos) -j*0.1 <<"   (dotOnePos - pos)"<<(0.1 - dotOnePos + pos);
            }
            goto LINE_AZY;
            break;
        case 3 ... 5:
            if((abs(pos) - abs((int)pos)) < 0.02 && curPix - lastPix >50)
            {
                p.drawLine(curPix,AZMARGIN, curPix,AZMARGIN+5);
                double tempD = pos;
                if(tempD>180) tempD = tempD-360;

                ParamText = QString::number(tempD,'f', 0);
                p.drawText(curPix-5,AZMARGIN-AZCHARARGIN, ParamText);

                for(int j=1;j<10;j++)
                {
                    if( curPix+ j * cur10Pix > scrWidth-100 )   //画到线的最右端停止
                    {
                        break;
                    }
                    p.drawLine(curPix+ j * cur10Pix,AZMARGIN, curPix + j * cur10Pix,AZMARGIN+3);
                    if(lastPix == 100)
                    {
                        if( curPix - j * cur10Pix > 100 )   //画到线的最左端停止
                        {
                            p.drawLine(curPix - j * cur10Pix,AZMARGIN, curPix - j * cur10Pix,AZMARGIN+3);
                        }
                    }
                }
                lastPix = curPix;
            }
            break;
        case 6 ... 180:
            //qDebug()<<"pos:"<<QString::number(pos,'f',4) <<"_ " << QString::number(pos-(int)pos,'f',1).toDouble()<<" curPix:"<<curPix<<" lastPix:"<<lastPix;
            //if((abs(pos) - abs((int)pos)) < 0.02 && curPix - lastPix >10)
            if(QString::number(pos-(int)pos,'f',1).toDouble() == 0.0 && curPix - lastPix >40)
            {
                p.drawLine(curPix,AZMARGIN, curPix,AZMARGIN+5);
                double tempD = pos;
                if(tempD>180) tempD = tempD-360;

                ParamText = QString::number(tempD,'f', 0);
                p.drawText(curPix-5,AZMARGIN-AZCHARARGIN, ParamText);
                lastPix = curPix;
            }
            break;
        default:
            break;
        }
        pos =(double)(pos + DotDegPerPix/100);
    }
LINE_AZY:
    curPix=scrHeight/2;
    //p.drawLine(AZMARGINY,curPix, AZMARGINY + 50,curPix);


    double upStartDegPos = ptzPitch + viewAngle / scrWidth * (scrHeight - 200)/2 ;
    pos = upStartDegPos;
    dotOnePos = pos;
    lastPix = 100;
    cur10Pix = scrWidth / viewAngle / 10 ;  //0.1

    for(int i=0; i<scrHeight-200; i++)
    {
        curPix = 100 + i;
        if( curPix>scrHeight-100 )   //画到线的最下端停止
        {
            break;
        }

        switch((int)viewAngle)
        {
        case 0 ... 2:
            pos = (double)qRound(ptzPitch);
            dotOnePos = (double)qRound(ptzPitch);
            curPix = scrHeight/2 - (double)(dotOnePos - pos)*10*cur10Pix;
            curPix2 = curPix - cur10Pix;
            for(int j=0;j<20;j++)
            {
                if( curPix + j*cur10Pix > scrHeight-100 )   //画到Y线的最下端停止
                {
                    break;
                }
                ParamText = QString::number(pos - (dotOnePos - pos) -j*0.1 ,'f', 1);
                p.drawText(AZMARGINY-AZCHARARGINY,curPix + j*cur10Pix+5, ParamText);
                p.drawLine(AZMARGINY,curPix + j*cur10Pix,AZMARGINY+3, curPix + j*cur10Pix);

                if( curPix2- j*cur10Pix < 100 )   //画到Y线的最上端停止
                {
                    break;
                }
                ParamText = QString::number(pos + (0.1 - dotOnePos + pos) +j*0.1 ,'f', 1);
                p.drawText(AZMARGINY-AZCHARARGINY, curPix2 - j*cur10Pix+5,ParamText);
                p.drawLine(AZMARGINY, curPix2 - j*cur10Pix,AZMARGINY+3,curPix2 - j*cur10Pix);
            }
            return;
            break;
       case 3 ... 5:
            if((abs(pos) - abs((int)pos)) < 0.02 && curPix - lastPix >50)
            {
                p.drawLine(AZMARGINY,curPix,AZMARGINY+5, curPix);
                ParamText = QString::number(pos,'f', 0);
                p.drawText(AZMARGINY-AZCHARARGINY,curPix+5, ParamText);

                for(int j=1;j<10;j++)
                {
                    if( curPix+ j * cur10Pix > scrHeight-100 )   //画到线的最下端停止
                    {
                        break;
                    }
                    p.drawLine(AZMARGINY,curPix+ j * cur10Pix,AZMARGINY+3, curPix + j * cur10Pix);
                    if(lastPix == 100)
                    {
                        if( curPix - j * cur10Pix > 100 )   //画到线的最上端停止
                        {
                            p.drawLine(AZMARGINY,curPix - j * cur10Pix,AZMARGINY+3, curPix - j * cur10Pix);
                        }
                    }
                }
                lastPix = curPix;
            }
            break;
        case 6 ... 180:
            //if((abs(pos) - abs((int)pos)) < 0.02 && curPix - lastPix >20)
            if(QString::number(pos-(int)pos,'f',1).toDouble() == 0.0 && curPix - lastPix >40)
            {
                p.drawLine(AZMARGINY,curPix,AZMARGINY+5, curPix);
                ParamText = QString::number(pos,'f', 0);
                p.drawText(AZMARGINY-AZCHARARGINY,curPix+5, ParamText);
                lastPix = curPix;
            }
            break;
        default:
            break;
        }
        pos =(double)(pos - DotDegPerPix/100);
    }
#endif
}
void MainWindow::DrawGuideOSD(QPixmap * pixTmp, double dblCamField)
{
    QPainter painter;
    if (painter.begin(pixTmp)==false)
    {
        painter.end();
        return;
    }
    int nLblW=pixTmp->width();
    int nLblH=pixTmp->height();

    if (mPlayer->recflag == true | mPlayerSub->recflag == true)
    {
        QFont font;
        font.setPixelSize(30);//设置显示字体的大小
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(QColor(255, 0, 0));
        painter.drawText(ui->lblPad->width()/2-10,40, "REC");
    }
    painter.end();
}

void MainWindow::drawStickExpo(double expo)
{
 /*   QPen pen;//画笔对象
    QBrush brush;

    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(255,255,255,100));
    //brush.setStyle(Qt::SolidPattern);
    //QPainter p(this->ui->lbl_stickEXPO);

    pen.setColor(QColor(0,0,0,255));//画笔颜色
    p.setPen(pen);
    p.drawLine(0,50,100,50);
    p.drawLine(50,0,50,100);

   // pen.setWidth(4);//画笔粗细
    p.setPen(pen);
    //qDebug()<<"=========================================";
    for(int x=50;x<100;x++)
    {
        double a =(x-50)*0.02;
        double b=a*pow(expo,a)*(50/expo);
        double a2 = (x-50+1)*0.02;
        double b2=a2*pow(expo,a2)*(50/expo);

        p.drawLine(x,50-b,x+1,50-b2);
        p.drawLine(100-x,b+50,100-x-1,b2+50);
    } */
}

/*
 * 改变鼠标箭头，并画出鼠标点击处距离中心的xy偏移量
 * x_in y_in 为逻辑坐标
 */
void MainWindow::drawMouseHit(QPixmap * fitPixmap,int x_in,int y_in)
{
    int w = 0;
    int h = 0;
    float rato_w,rato_h,scr_du;
    if(ui->rb_selGDA->isChecked())
        scr_du= IMAGE_PIXEL_WIDTH/(float)mHdViewAngle_A;
    else
        scr_du= IMAGE_PIXEL_WIDTH/(float)mHdViewAngle_B;
    rato_w = (float)ui->lblPad->width()/(float)IMAGE_PIXEL_WIDTH;
    rato_h = (float)ui->lblPad->height() /(float)IMAGE_PIXEL_HEIGHT;

    w = rato_w * x_in;
    h = rato_h * y_in;
    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(100,100,0,255));
    brush.setStyle(Qt::SolidPattern);
    QPainter p;
    p.begin(fitPixmap);
    pen.setColor(QColor(255,255,255,255));//画笔颜色
    p.setPen(pen);
    p.drawLine(w-20,h,w-10,h);
    p.drawLine(w+20,h,w+10,h);
    p.drawLine(w,h-20,w,h-10);
    p.drawLine(w,h+20,w,h+10);

    QFont font;
    //设置显示字体的大小
    font.setPixelSize(20);

    //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //QPen pen = p.pen();
    pen.setColor( QColor(255, 255, 255,255));
    p.setPen(pen);
    p.setFont(font);
    //painter.drawText(fitPixmap.rect(), Qt::AlignCenter, imageText);

    int diffx = (x_in - IMAGE_PIXEL_WIDTH/2)/scr_du * 60;
    int diffy = (IMAGE_PIXEL_HEIGHT/2 - y_in)/scr_du * 60;

    QString ParamText = QString::number(diffx, 10) +"," + QString::number(diffy, 10);
    int a=20,b=20;
    if(x_in>1730)
    {
        a=-130;
    }
    if(y_in<100)
    {
        b=-30;
    }
    p.drawText(w+a,h-b, ParamText);
}

void MainWindow::drawROIBox(QLabel * uipad,QPixmap * fitPixmap,cv::Rect roi )
{
    float rate_w=0,rate_h=0;
    rate_w = (float)uipad->width() / (float)IMAGE_PIXEL_WIDTH;
    rate_h = rate_w;       //(float)ui->uipad->height() / (float)IMAGE_PIXEL_HEIGHT;
    {
        //rate_h = (float)uipad->height() / (float)IMAGE_PIXEL_HEIGHT_IR;
        //rate_w = rate_h;
        //dy = (ui->lblPad->width() - ui->lblPad->height()*1.33)/2;
        //qDebug()<<"dy:"<<dy;
    }
    //qDebug()<<"rate_w:"<<rate_w<<"  rate_h:"<<rate_h;

    QPen pen;//画笔对象
    QBrush brush;

    pen.setWidth(1);//画笔粗细
    //brush.setColor(QColor(100,100,0,70));
    brush.setStyle(Qt::SolidPattern);
    QPainter p;

    p.begin(fitPixmap);
    pen.setColor(QColor(0,255,0,200));//画笔颜色
    p.setPen(pen);

    //设置显示字体的大小
    QFont font;
    font.setPixelSize(28);
    p.setFont(font);

    int x,y;
    int roi_width = roi.width * rate_w;      //trackerThd->roi_width * rate_w * 1;
    int roi_height = roi.height * rate_h;    //trackerThd->roi_height * rate_h * 1;
    x = (roi.x) *  rate_w * 1;
    y = (roi.y) *  rate_h * 1;
    //qDebug()<<"x:"<<x<<"  y:"<<y <<"    roi_width:"<<roi_width<<"   roi_height:"<<roi_height;

    // 显示尺寸限制
    if(roi_width < 16) {
        int center_x = x + roi_width/2;
        roi_width = 16;
        x = center_x - roi_width/2;
    }
    if(roi_height < 16) {
        int center_y = y + roi_height/2;
        roi_height = 16;
        y = center_y - roi_height/2;
    }
    x = std::max(0, std::min(fitPixmap->width() - roi_width, x));
    y = std::max(0, std::min(fitPixmap->height() - roi_height, y));

    p.drawRect(x, y,roi_width,roi_height);
}

void MainWindow::drawMouseBox(QPixmap * fitPixmap,int x_in,int y_in)
{
    int x=0,y=0, dx=0;
    float rate_w=0,rate_h=0;
    int roi_width,roi_height;

    rate_w = (float)ui->lblPad->width() / (float)IMAGE_PIXEL_WIDTH;
    //rate_h =  (float)ui->lblPad->height() / (float)IMAGE_PIXEL_HEIGHT;
    if(currentMainSrc==2)
    {
        rate_h = (float)ui->lblPad->height() / (float)IMAGE_PIXEL_HEIGHT_IR;
        roi_width = trackerThd->roi_width * rate_h;
        roi_height = trackerThd->roi_height * rate_h;
        dx = (ui->lblPad->width() - ui->lblPad->height()*1.33)/2;
    }
    else
    {
        roi_width = trackerThd->roi_width * rate_w;
        roi_height = trackerThd->roi_height * rate_w;
    }

    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(2);//画笔粗细
    brush.setColor(QColor(100,100,0,70));
    brush.setStyle(Qt::SolidPattern);
    QPainter p;
    p.begin(fitPixmap);
    pen.setColor(QColor(200,200,200,255));//画笔颜色
    p.setPen(pen);

    //设置显示字体的大小
    QFont font;
    font.setPixelSize(12);
    p.setFont(font);

    x= x_in * rate_w - roi_width/2 - dx;
    if(currentMainSrc == 2)
    {
        y= y_in * rate_w - roi_height/2 + 100;
    }
    else
    {
        y= y_in * rate_w - roi_height/2;
    }
    p.drawRect(x, y,roi_width,roi_height);
}

//在label上画虚拟摇杆
void MainWindow::drawVirtualJoyStick(int x,int y, int btn)
{
    QPen pen;//画笔对象
    QBrush brush;
    pen.setColor(QColor(0,0,0,0));//画笔颜色
    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(0,0,0,70));
    brush.setStyle(Qt::SolidPattern );
    QPainter p(this->ui->lbl_SimJoystick);

    p.setBrush(brush);
    p.setPen(pen);
    p.drawEllipse(0,0,160,160);


    //pen.setWidth(1);//画笔粗细
    pen.setColor(QColor(0,0,0,255));//画笔颜色
    p.setPen(pen);
    //p.drawEllipse(80,80,2,2);
    p.drawLine(0,80,160,80);
    p.drawLine(80,0,80,160);

    brush.setColor(QColor(0,0,255,230));
    p.setBrush(brush);
    p.drawEllipse(QPoint(x, y),10,10);

    return;
}

#if 0
void MainWindow::lblPaintAngleGap(double d1,double p1,double d2,double p2)
{
    QPen pen;//画笔对象
    QBrush brush;

    pen.setWidth(1);//画笔粗细
    brush.setColor(QColor(255,255,255,100));
    //brush.setStyle(Qt::SolidPattern);
    QPainter p(this->ui->lbl_padAngleGap);

    pen.setColor(QColor(0,0,0,255));//画笔颜色
    p.setPen(pen);
    p.drawLine(10,55,96,55);
    p.drawLine(52,5,52,105);

    pen.setWidth(4);//画笔粗细
    p.setPen(pen);
    int gapX=int(-d2*10);

    int gapY=int(p2*10);

    if(gapX>40) gapX=40;
    if(gapX<-40) gapX=-40;
    if(gapY>40) gapY=40;
    if(gapY<-40) gapY=-40;
    p.drawLine(52+gapX,55+gapY,54+gapX,55+gapY);
}
#endif

void MainWindow::updateJoyStick()
{
//    QUdpSocket * udpskt = getCurMainPTZ();
//    if( udpskt == NULL)
//    {
//        //qDebug()<<"updateJoyStick() getcurPTZ fail";
//        return;
//    }
    // 检查连接状态
    if(connectFlag_A == false && connectFlag_B == false) {
        return;
    }

    int pan=0,tilt=0,axial=0,btn=0;
    static short pan_outVal=0,tilt_outVal=0;
    static int pan_old=0,tilt_old=0,axial_old=0,btn_old=0,focusVal_old,zoomVal_old;
    int ptzsended=0;
    static int focusVal=0,zoomVal=0,lensVal=0;
    static short oldll4 = 0;
    static short oldll5 = 0;

    if(ui->rb_selGDA->isChecked())
    {
        focusVal =  hdFocusVal;
        zoomVal =  hdZoomVal;
        lensVal = hdLensVal;
    }
    else
    {
        focusVal =  hdFocusVal_B;
        zoomVal =  hdZoomVal_B;
        lensVal = hdLensVal_B;
    }
    hidLock.lockForRead();
        pan=hid->panVal;
        tilt = hid->tiltVal;
        axial = hid->axialVal;
        btn=hid->btnVal2;
    hidLock.unlock();

    //qDebug()<<"pan:"<<pan<<"  tilt:"<<tilt;

    if(pan_old != pan || tilt_old != tilt)   //方向 俯仰
    {
        double kh=25000;
        //static short stopSended = 0;

        switch(lensVal)
        {
            case 400 ... 540:
                kh=2000; break;
            case 300 ... 399:
                kh=3000; break;
            case 200 ... 299:
                kh=5000; break;
            case 100 ... 199:
                kh=8000; break;
            case 50 ... 99:
                kh=12000; break;
            case 12 ... 49:
                kh=25000; break;
            default:
                kh=30000;             //3°*100=300
            break;
        }

        pan_outVal=updateJoysticktoExpo(pan, mExpo,HID_RESOLUTION,kh);
        tilt_outVal=updateJoysticktoExpo(tilt, mExpo,HID_RESOLUTION,kh);


        // 生成控制命令
        uint8_t * cmd = mPodCmd.crtSteplessCmd((pan_outVal ), -(tilt_outVal));

        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }

        pan_old=pan;
        tilt_old=tilt;

        /* if(ui->rb_flipPicOn->isChecked())
        {
            uint8_t * cmd = mPodCmd.crtSteplessCmd(pan_outVal* ANGLENET2DEG, tilt_outVal* ANGLENET2DEG);
            udpskt->write((char *)cmd,7);
        }
        else    //倒装吊舱 */
//        {
//            //uint8_t * cmd = mPodCmd.crtSteplessCmd((pan_outVal* ANGLENET2DEG), -(tilt_outVal* ANGLENET2DEG));
//            /*if(pan_outVal>32768) pan_outVal = 32768;
//            else if(pan_outVal<-32768) pan_outVal = -32768;

//            if(tilt_outVal>32768) tilt_outVal = 32768;
//            else if(tilt_outVal<-32768) tilt_outVal = -32768; */
//            //qDebug()<<"pan_outVal:"<<pan_outVal<<"  tilt:"<<-tilt_outVal;
//            uint8_t * cmd = mPodCmd.crtSteplessCmd((pan_outVal ), -(tilt_outVal));
//            udpskt->write((char *)cmd,7);
//        }

//        pan_old=pan;
//        tilt_old=tilt;
    }
    if(axial_old != axial)
    {
        QThread::msleep(15);
        if(axial > 514)
        {
            switch (axial) {
                case 514 ... 600:
                    hdCmdZoomOutWithSpd[4]=0x20;
                    break;
                case 601 ... 700:
                    hdCmdZoomOutWithSpd[4]=0x22;
                    break;
                case 701 ... 1050:
                    hdCmdZoomOutWithSpd[4]=0x27;
                    break;
                default:
                    hdCmdZoomOutWithSpd[4]=0x20;

            }

            on_pbZoomOut_pressed();
        }
        else if(axial < 510)
        {
            switch (axial) {
                case 400 ... 510:
                    hdCmdZoomInWithSpd[4]=0x30;
                    break;
                case 300 ... 399:
                    hdCmdZoomInWithSpd[4]=0x32;
                    break;
                case 0 ... 299:
                    hdCmdZoomInWithSpd[4]=0x37;
                    break;
                default:
                    hdCmdZoomInWithSpd[4]=0x30;

            }

            on_pbZoomIn_pressed();
        }
        else
            on_pbZoomIn_released();
        //ui->te_Output->append(QString::number(axial));
        axial_old = axial;
    }
}

/* 响应虚拟摇杆动作
 */
void MainWindow::updateVirtualJoyStick(int x,int y)
{
    int pan=0,tilt=0;
    static short pan_outVal=0,tilt_outVal=0;
    static int pan_old=0,tilt_old=0;
    static QDateTime pre_date_time = QDateTime::currentDateTime();

    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
/*
    QDateTime current_date_time =QDateTime::currentDateTime();
    int diff = pre_date_time.msecsTo(current_date_time);
    pre_date_time = current_date_time;
    qDebug()<<" diff:"<<diff;
    if(diff< 30) return; */

    pan = (x-80)*2 + 512;
    tilt = (y-80)*2 + 512;

    double kh=1200;

    if(pan_old != pan || tilt_old != tilt)
    {
        //qDebug()<<"pan:"<<pan <<" tilt:"<<tilt;
        pan_outVal=updateJoysticktoExpo(pan, mExpo,HID_RESOLUTION,kh);
        tilt_outVal=updateJoysticktoExpo(tilt, mExpo,HID_RESOLUTION,kh);
        //qDebug()<<"pan_outVal:"<<pan_outVal <<" tilt_outVal:"<<tilt_outVal;
        uint8_t * cmd = mPodCmd.crtSteplessCmd(pan_outVal* ANGLENET2DEG, -(tilt_outVal* ANGLENET2DEG));
        udpskt->write((char *)cmd,7);

        pan_old=pan;
        tilt_old=tilt;
    }
    return;
}

/*
 * inVal 0 ~ 1024
 */
int MainWindow::updateJoysticktoExpo(int inVal,double expo,int maxVal,int tho)
{
    int val;
    int sbol=1;
    if(inVal>maxVal)
    {
       val= inVal-maxVal;
    }
    else if(inVal<maxVal)
    {
        val= maxVal-inVal;
        sbol=-1;
    }
    else
    {
        return 0;
    }

    double a = (val) * (double)1/(double)maxVal;
    double outVal = a*pow(expo,a) * (tho/expo)*sbol;
    //qDebug()<<"inVal"<<inVal<<" expo"<<expo<<"  maxVal"<<maxVal<<"  outVal"<<outVal <<" a"<<a;

    if(outVal>0 && outVal<1) outVal=1;
    else if(outVal>-1 && outVal<0) outVal=-1;
    return (int)outVal;
}

void MainWindow::slotGetOneFrameCh1(QImage img)
{

    //QUdpSocket * udpskt = getCurMainPTZ();
    // 检查连接状态
    if( connectFlag_A == false)
    {
        qDebug()<<"slotGetOneFrameCh1 udpskt is null" <<"   connectFlag_A:"<<connectFlag_A <<"   connectFlag_B:"<<connectFlag_B;
        return;
    }

    if( connectRtspFlag_A == false)
    {
        qDebug()<<"slotGetOneFrameCh1 udpskt is null" <<"   connectRtspFlag_A:"<<connectRtspFlag_A <<"   connectFlag_B:"<<connectFlag_B;
        return;
    }

    // 保存当前图像并标记更新
    mImageCh1 = img.copy();
    mImageCh1GetNew = true;

    // 关键：向跟踪线程传递图像
    if(trackerThd->bTrackerStart == true  && ui->rb_selGDA->isChecked())
    {

        trackerThd->qImg = img;                     // 传递图像
        pthread_cond_signal(&gTrackingTransImg);    // 唤醒跟踪线程

    }

    update();  // 会触发paintEvent重绘

    ui->lbRecvFrameA->setText(QString::number(mPlayer->num_frame));

}

void MainWindow::slotGetOneFrameCh2(QImage img)
{
    //QUdpSocket * udpskt = getCurMainPTZ();
    if( connectFlag_B == false)
    {
        qDebug()<<"slotGetOneFrameCh2 udpskt is null" <<"   connectFlag_A:"<<connectFlag_A <<"   connectFlag_B:"<<connectFlag_B;
        return;
    }
    if( connectRtspFlag_B == false)
    {
        qDebug()<<"slotGetOneFrameCh2 udpskt is null" <<"   connectRtspFlag_A:"<<connectRtspFlag_A <<"   connectRtspFlag_B:"<<connectRtspFlag_B;
        return;
    }

    mImageCh2 = img.copy();
    mImageCh2GetNew = true;

    if(trackerThd->bTrackerStart == true && ui->rb_selGDB->isChecked())
    {
        trackerThd->qImg = img;
        pthread_cond_signal(&gTrackingTransImg);
    }

    update(); //调用update将执行 paintEvent函数
    ui->lbRecvFrameB->setText(QString::number(mPlayerSub->num_frame));
}

void MainWindow::sendSetCmd(int type,int value)
{
    unsigned char cmd[10];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    cmd[0]=PKT_HEADER;
    cmd[1]=type;
    cmd[2]=value;
    cmd[3]=0;
    cmd[4]=0;
    cmd[5]=0;
    cmd[6]=0;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    udpskt->write((char *)cmd,8);
}

void MainWindow::sendSetCmd(int type,int val1,int val2)
{
    unsigned char cmd[10];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    cmd[0]=PKT_HEADER;
    cmd[1]=type;
    cmd[2]= val1;
    cmd[3]= val2;
    //qDebug()<<"Tilt:"<<y;
    cmd[4]=0;
    cmd[5]=0;
    cmd[6]=0;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    udpskt->write((char *)cmd,8);
}

void MainWindow::sendSetCmd(int type,int v1,int v2,int v3,int v4)
{
    unsigned char cmd[10];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    cmd[0]=PKT_HEADER;
    cmd[1]=type;
    cmd[2]= v1;
    cmd[3]= v2;
    cmd[4]= v3;
    cmd[5]= v4;
    cmd[6]=0;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    udpskt->write((char *)cmd,8);
    //qDebug()<<("type=")<<cmd[1]<<" "<<cmd[2]<<" "<<cmd[3]<<" "<<cmd[4]<<" "<<cmd[5];
}

void MainWindow::sendSetCmd(int type,int panValH,int panValL,int tiltValH,int tiltValL,int hdFocal,int hdZoom,int irFocal,int irZoom)
{
    unsigned char cmd[10];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    cmd[0]=PKT_HEADER;
    cmd[1]=type;
    cmd[2]= panValH;
    cmd[3]= panValL;
    cmd[4]= tiltValH;
    cmd[5]= tiltValL;
    cmd[6]= (irFocal<<6) |(irZoom<<4) |(hdFocal<<2) | hdZoom;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    udpskt->write((char *)cmd,8);
    //qDebug()<<("type=")<<cmd[1]<<" "<<cmd[2]<<" "<<cmd[3]<<" "<<cmd[4]<<" "<<cmd[5]<<" "<<cmd[6];
}


void MainWindow::sendSetIP(int type)
{
    unsigned char cmd[10];

    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    cmd[0]=PKT_HEADER;
    cmd[1]=type;
    cmd[2]=(cfgIpAddr>>24) & 0xff;
    cmd[3]=(cfgIpAddr>>16) & 0xff;
    cmd[4]=(cfgIpAddr>> 8) & 0xff;
    cmd[5]=(cfgIpAddr>> 0) & 0xff;
    cmd[6]=0;
    cmd[7]=0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    cmd[8]=0;
    cmd[9]=0;
    udpskt->write((char *)cmd,8);
}

void MainWindow::on_pushButton_swap_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if(currentMainSrc==2)
    {
        sendSetCmd(PKT_TYPE_SETMAINSCREEN,1);   //切换1高清到主窗口
        currentMainSrc=1;
    }
    else if(currentMainSrc==1)
    {
        sendSetCmd(PKT_TYPE_SETMAINSCREEN,2);   //切换2红外到主窗口
        currentMainSrc=2;
    }

}

void MainWindow::on_pbStop_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    //sendSetCmd(PKT_TYPE_PTZSTOP,0);
    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    // udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop))
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    };
}

void MainWindow::on_pbPicInPicOnOff_clicked()
{
/*    if(picInPicOn == true)
    {
        ui->pbPicInPicOnOff->setText("打开画中画");
        picInPicOn = false;
    }
    else
    {
        ui->pbPicInPicOnOff->setText("关闭画中画");
        picInPicOn = true;
    }
    sendSetCmd(PKT_TYPE_PICINPIC,(int)picInPicOn);
    //qDebug()<<("picInPicOn=")<<(int)picInPicOn;
*/
}

void MainWindow::on_pbUp_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    // udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
}

void MainWindow::on_pbDown_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    // udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
}

void MainWindow::on_pbLeft_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    // udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    //qDebug()<<"on_pbLeft_released";
}

void MainWindow::on_pbRight_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    // udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    // 同时发送给A和B两个光电
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    //qDebug()<<"on_pbRight_released";
}

void MainWindow::on_pbUp_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if(ui->rb_flipPicOn->isChecked())
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,-(pelco_spd_val* ANGLENET2DEG));
        udpskt->write((char *)cmd,7);
    }
    else
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,(pelco_spd_val* ANGLENET2DEG));
//        udpskt->write((char *)cmd,7);
        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }
        //qDebug()<<"-(pelco_spd_val* ANGLENET2DEG):"<<-(pelco_spd_val* ANGLENET2DEG) << "-pelco_spd_val* ANGLENET2DEG"<<-pelco_spd_val* ANGLENET2DEG;
        //qDebug()<<"-(pelco_spd_val*91):"<<-(pelco_spd_val*93) << "-pelco_spd_val*91"<<-pelco_spd_val*93;
    }

}

void MainWindow::on_pbDown_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if(ui->rb_flipPicOn->isChecked())
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,(pelco_spd_val* ANGLENET2DEG));
        udpskt->write((char *)cmd,7);
    }
    else
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,-(pelco_spd_val* ANGLENET2DEG));
//        udpskt->write((char *)cmd,7);
        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }
    }
}

void MainWindow::on_pbLeft_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if(ui->rb_flipPicOn->isChecked())
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(pelco_spd_val* ANGLENET2DEG,0);
        udpskt->write((char *)cmd,7);
    }
    else
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(-(pelco_spd_val* ANGLENET2DEG),0);
//         udpskt->write((char *)cmd,7);

        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }
    }



    /*QByteArray mBuf;
    mBuf.resize(7);
    memcpy(mBuf.data(),cmd, 7);
    qDebug()<<"left:"<<mBuf.toHex(' ');
    */
}

void MainWindow::on_pbRight_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if(ui->rb_flipPicOn->isChecked())
    {

        uint8_t * cmd = mPodCmd.crtSteplessCmd(-(pelco_spd_val* ANGLENET2DEG),0);
        udpskt->write((char *)cmd,7);
    }
    else
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(pelco_spd_val* ANGLENET2DEG,0);
//         udpskt->write((char *)cmd,7);
        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }
    }
    /*QByteArray mBuf;
    mBuf.resize(7);
    memcpy(mBuf.data(),cmd, 7);
    qDebug()<<"right:"<<mBuf.toHex(' ');*/
}


#if 0
void MainWindow::on_pbScan_clicked()
{
    sendSetCmd(PKT_TYPE_SCAN,0);
    scanflag=!scanflag;
    if(scanflag)
        sendSetCmd(PKT_TYPE_SCAN,1);   //开始扫描
    else
        sendSetCmd(PKT_TYPE_SCAN,2);   //停止扫描
}
#endif
void MainWindow::on_pbSetZero_clicked()
{
    sendSetCmd(PKT_TYPE_TOZERO,0);
}

void MainWindow::on_pbWiper_clicked()
{
    sendSetCmd(PKT_TYPE_WIPER,2);
}

void MainWindow::on_pbDefog_pressed()
{
    sendSetCmd(PKT_TYPE_DEFOG,0);
}

void MainWindow::on_cbxPresetNo_currentIndexChanged(int index)
{
    presetCurrentIdx = index+1;
    qDebug()<<("presetCurrentIdx=")<<(int)presetCurrentIdx;
}

void MainWindow::stick_timeOut()
{
    updateJoyStick();
}

void MainWindow::Theo_timeOut()
{
    //if(connectFlag_A == false || connectFlag_B == false)
    if(connectRtspFlag_A == false || connectRtspFlag_B == false)
        return;

    //*********************************************************************************仅作测试用

    CurTime = QTime::currentTime(); //仅作测试用  系统的当前时间 相对0：0：0.000 的ms数
    /*ui->lbl_BjTime->setText(QString("%1").arg(CurTime.hour(),2,10,QLatin1Char('0'))+":"+ \
                            QString("%1").arg(CurTime.minute(),2,10,QLatin1Char('0'))+":"+ \
                            QString("%1").arg(CurTime.second(),2,10,QLatin1Char('0'))+"."+ \
                            QString("%1").arg(CurTime.msec(),3,10,QLatin1Char('0'))); */
    //*********************************************************************************仅作测试用
#if 0
    SRC_DATA input;
    RET_DATA output;

    //input.T0 = QTime(0,0,0);//中心会下发，按实际赋值
    input.time = CurTime;   //取本机系统时间
    input.T0_Valid = false;//中心停止发送T0后，置为fasle  正在发送置为true  中心会连续发送10s
    input.iCurMcLeadMode = 0;//默认填0
    output.bT0_Valid = false;
    output.A = 0;
    output.E = 0;
    mAETheo.dblA= output.A;
    mAETheo.dblE= output.E;

    fullscreen.OsdTime =CurTime;
    fullscreen.OsdA =mPtzDirection_A;
    fullscreen.OsdE =mPtzPitch_A-dd_diffy;
#endif
    mPlayer->OsdTime =CurTime;
    mPlayer->OsdA =mPtzDirection_A;
    mPlayer->OsdE =mPtzPitch_A;
    mPlayer->OsdR =pnetworkComm->msgFromZhiKong.zkDistence ;
    mPlayer->OsdLens = hdLensVal;
    mPlayer->OsdViewAngle = mHdViewAngle_A;
    mPlayer->OsdUpdate =true;

    mPlayerSub->OsdTime =CurTime;
    mPlayerSub->OsdA =mPtzDirection_B;
    mPlayerSub->OsdE =mPtzPitch_B;
    mPlayerSub->OsdR = pnetworkComm->msgFromZhiKong.zkDistence ;
    mPlayerSub->OsdLens = hdLensVal_B;
    mPlayerSub->OsdViewAngle = mHdViewAngle_B;
    mPlayerSub->OsdUpdate =true;


    //fullscreen.RelativeTime = 0;
    mPlayer->RelativeTime = 0;
    mPlayerSub->RelativeTime = 0;
    //calcAngleSpeed();   //计算角速度并显示
    //chkCrossBorder();



    if(0 == mFoc_or_zoom_flag ) //zoom
    {
        mFoc_or_zoom_flag = 1;
        udpSocket_A->write((char *)cmd_ZoomPosInq ,sizeof(cmd_ZoomPosInq));
        udpSocket_B->write((char *)cmd_ZoomPosInq ,sizeof(cmd_ZoomPosInq));
    }
    else
    {
        mFoc_or_zoom_flag = 0;
        udpSocket_A->write((char *)cmd_FocusPosInq ,sizeof(cmd_FocusPosInq));
        udpSocket_B->write((char *)cmd_FocusPosInq ,sizeof(cmd_FocusPosInq));

        static int postHdZoomVal_B;
        static int postHdZoomVal_A;
        //qDebug()<<"hdZoomVal_B:"<<hdZoomVal_B <<" postHdZoomVal_B:"<<postHdZoomVal_B ;

        if(hdZoomVal != postHdZoomVal_A )
        {
            if(ui->cbxUseDefFocA->isChecked())
            {
                //adjFocusPosByDist(udpSocket_A,pnetworkComm->msgFromZhiKong.zkDistence);
                adjFocusPosByDist(udpSocket_A,pnetworkComm->msgFromZhiKong.zkDistence);
                postHdZoomVal_A = hdZoomVal;
            }
        }

        if(hdZoomVal_B != postHdZoomVal_B )
        {
            if(ui->cbxUseDefFocB->isChecked())
            {
                //adjFocusPosByDist(udpSocket_A,pnetworkComm->msgFromZhiKong.zkDistence);
                adjFocusPosByDist(udpSocket_B,pnetworkComm->msgFromZhiKong.zkDistence);
                postHdZoomVal_B = hdZoomVal_B;
            }
        }

    }


//    // 惯导数据监控
//    static int gyroDebugCounter = 0;
//    gyroDebugCounter++;

//    // 每5秒输出一次惯导调试信息
//    if (gyroDebugCounter % 50 == 0) {  // 100ms * 50 = 5秒
//        debugGyroAngles();
//    }


    return;
}


int MainWindow::adjFocusPosByDist(QUdpSocket * udpSocket,int dist)
{
    int t =dist;
    static int postdist =0;
    if(ui->cbxSetDbgDistance->isChecked())  //调试或者校标用
    {
        t = ui->leSetDbgDistance->text().toInt();  //在调试界面开启了设置调试距离，则获取距离值为t
    }

    if(t < 1  ) t = 1000;
    //if(postdist == t ) return 100;
    //84 01 04 48 0p 0q 0r 0s FF 设置聚焦位置pgrs: 0x1000 ~ 0x8000


    int focval = ui->leHdaDefFoc->text().toInt();   //4400;//getFocValByDist(t);   //配置中相机A对焦值

    hdCmdFocalTo[4] = (focval>>12)&0xf;
    hdCmdFocalTo[5] = (focval>>8)&0xf;
    hdCmdFocalTo[6] = (focval>>4)&0xf;
    hdCmdFocalTo[7] = (focval>>0)&0xf;
    qDebug()<<"adjFocusPosByDist" <<focval;
    udpSocket->write((char *)hdCmdFocalTo ,sizeof(hdCmdFocalTo));
    // QByteArray cmdData((const char *)hdCmdFocalTo, sizeof(hdCmdFocalTo));
    // qDebug() << "=============UDP Focal Command:" << cmdData.toHex(' ');  // 打印这里发送的数据
    postdist = t;
}

void MainWindow::calcAngleSpeed()
{
#if 0
    static QTime preTime;
    CurTime = QTime::currentTime();
    int timeDiff = CurTime.msec() - preTime.msec();
    if(timeDiff < 0)  timeDiff = timeDiff + 1000;
    preTime = CurTime;

    static int calcCnt = 0;
    static double oldPtzDir = 0;
    static double oldPtzPitch = 0;
    static double angle_diff_x;
    static double angle_diff_y;
    double angle_speed_x;
    double angle_speed_y;
    static int timeDiffSeg;

    if(calcCnt >4)
    {
        angle_speed_x = angle_diff_x * 1000/timeDiffSeg;       //20=1000/50    定时器50ms，以每秒计算角速度
        angle_speed_y = angle_diff_y * 1000/timeDiffSeg;
        if(angle_speed_x > 100) angle_speed_x = 63;
        if(angle_speed_x > 100) angle_speed_y = 65;
        //ui->lblAngleSpeedX->setText( QString::number(angle_speed_x,'f', 1)+"°");
        //ui->lblAngleSpeedY->setText( QString::number(angle_speed_y,'f', 1)+"°");
        //qDebug()<<"time diff:"<<timeDiff<<"   angle_speedx:"<<angle_speed_x <<"    angle_speedy:"<<angle_speed_y;

        calcCnt = 0;
        timeDiffSeg = 0;
        angle_diff_x = 0;
        angle_diff_y = 0;
    }
    else
    {
        angle_diff_x = angle_diff_x + abs(ptzDirection - oldPtzDir);
        angle_diff_y = angle_diff_y + abs(ptzPitch - oldPtzPitch);
        timeDiffSeg = timeDiffSeg + timeDiff;
    }
    calcCnt++;
    oldPtzDir = ptzDirection;
    oldPtzPitch = ptzPitch;
#endif
}

void MainWindow::on_cbxPresetNoEdit_currentIndexChanged(int index)
{
    presetCurrentIdx = index+1;
    //qDebug()<<("presetCurrentIdx=")<<(int)presetCurrentIdx;
}
/*
void MainWindow::on_pbAutoFocus_clicked()
{
    sendSetCmd(PKT_TYPE_AUTOFOCUS,0);
}*/

void MainWindow::on_pbAutoFoc_clicked()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdOneAutoFocal ,sizeof(hdOneAutoFocal));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdOneAutoFocal ,sizeof(hdOneAutoFocal));
    }
}

void MainWindow::on_pbZoomOut_pressed()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdZoomOutWithSpd ,sizeof(hdCmdZoomOutWithSpd));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdZoomOutWithSpd ,sizeof(hdCmdZoomOutWithSpd));
    }
}

void MainWindow::on_pbZoomOut_released()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdZoomStop ,sizeof(hdCmdZoomStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdZoomStop ,sizeof(hdCmdZoomStop));
    }
}

void MainWindow::on_pbZoomIn_pressed()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdZoomInWithSpd ,sizeof(hdCmdZoomInWithSpd));

    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdZoomInWithSpd ,sizeof(hdCmdZoomInWithSpd));
    }
}

void MainWindow::on_pbZoomIn_released()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdZoomStop ,sizeof(hdCmdZoomStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdZoomStop ,sizeof(hdCmdZoomStop));
    }
}

void MainWindow::on_pbFocusF_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdFocalStop,sizeof(hdCmdFocalStop));
}

void MainWindow::on_pbFocusN_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdFocalStop,sizeof(hdCmdFocalStop));
}

void MainWindow::on_pbFocusF_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdFocalFar,sizeof(hdCmdFocalFar));
}

void MainWindow::on_pbFocusN_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdFocalNear,sizeof(hdCmdFocalNear));
}

//调整方位零位（寻北）type 34
void MainWindow::on_pbSetInitZero_clicked()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("调整零位"),
        QString(tr("警告：零位将会修改到当前方位，是否继续?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes) {
        sendSetCmd(PKT_TYPE_INITZERO,0);
    }
}

/*
void MainWindow::on_pbResetMotor_clicked()
{
    sendSetCmd(PKT_TYPE_RESETMOTOR,0);
}
*/

void MainWindow::on_pbRec_clicked()
{
    QString filePath1,path1;
    QString filePath2,path2;
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd_hhmmss");
    if(mPlayer->recflag == false)
    {
        mPlayer->recDir = ui->leRecFileDir->text();
        mPlayer->recflag  = true;
        mPlayer->mFileSegTimeOut  = true;
        mPlayerSub->recDir = ui->leRecFileDir->text();
        mPlayerSub->recflag  = true;
        mPlayerSub->mFileSegTimeOut  = true;
        ui->pbRec->setText("停止录像");
    }
    else
    {
        mPlayer->recflag  = false;
        mPlayerSub->recflag  = false;
        ui->pbRec->setText("开始录像");
        QThread::msleep(300);
/*
        filePath1 = ui->leRecFileDir->text() + "/" + "ch1.srt";
        path1 = ui->leRecFileDir->text() + "/"+ current_date+"ch1.srt";
        QFile::rename(filePath1,path1);

        filePath1 = ui->leRecFileDir->text() + "/" + "ch1.h264";
        path1 = ui->leRecFileDir->text() + "/"+ current_date+"ch1.h264";
        QFile::rename(filePath1,path1);



		filePath2 = ui->leRecFileDir->text() + "/" + "ch2.srt";
        path2 = ui->leRecFileDir->text() + "/"+ current_date+"ch2.srt";
        QFile::rename(filePath2,path2);
		
        filePath2 = ui->leRecFileDir->text() + "/" + "ch2.h264";
        path2 = ui->leRecFileDir->text() + "/"+ current_date+"ch2.h264";
        //qDebug()<<tr(" ")<<filePath2<<" _ "<<path2;
        QFile::rename(filePath2,path2);
*/
        //QMessageBox::about(NULL, "提示", "录像已保存\n" + path1);
        QString msg="录像已保存至 "+ mPlayer->recDir;
        dlg.initFrame(msg);
        dlg.setAttribute(Qt::WA_ShowModal, true);
        dlg.startTimer();
        dlg.show();
    }

}

void MainWindow::on_pbConfigSave_clicked()
{
    char fullPath[120];
    strncpy(fullPath, ui->leRecFileDir->text().toLatin1(),120);
    QDir dir(fullPath);
    if(dir.exists() == false || ui->leRecFileDir->text()=="")
    {
        QMessageBox::about(NULL, "提示", "输入的目录不存在");
        return;
    }

    if(checkip(ui->leSetIP->text()) == false)
    {
        QMessageBox::about(NULL, "提示", "输入的IP地址无效");
        return;
    }
    /*if(checkip(ui->leSetIPMask->text()) == false)
    {
        QMessageBox::about(NULL, "提示", "输入的IP掩码无效");
        return;
    }
    if(checkip(ui->leSetIPGageway->text()) == false)
    {
        QMessageBox::about(NULL, "提示", "输入的网关地址无效");
        return;
    }*/

    if(checkip(ui->leSetHdCamIP->text()) == false)
    {
        QMessageBox::about(NULL, "提示", "输入的相机地址无效");
        return;
    }

    ui->pbConfigSave->setEnabled(false);

    if(leRecDirChanged == true)
    {
        QMessageBox::about(NULL, "提示", "录像保存目录已设为\n" + ui->leRecFileDir->text().toLatin1());
    }

    if(leSetIpChanged == true)
    {
        cfgIpAddr = IPV4StringToInteger(ui->leSetIP->text());
        sendSetIP(PKT_TYPE_SETIPADDR);
        QMessageBox::about(NULL, "提示", "转台IP地址重新上电后将设置为新的IP地址\n" + ui->leSetIP->text().toLatin1());
        QThread::msleep(300);
    }
    /*if(leSetIPMaskChanged == true)
    {
        cfgIpAddr = IPV4StringToInteger(ui->leSetIPMask->text());
        sendSetIP(PKT_TYPE_SETIPMASK);
        QMessageBox::about(NULL, "提示", "转台IP地址重新上电后将设置为新的IP掩码地址\n" + ui->leSetIPMask->text().toLatin1());
        QThread::msleep(300);
    }
    if(leSetIPGWChanged == true)
    {
        cfgIpAddr = IPV4StringToInteger(ui->leSetIPGageway->text());
        sendSetIP(PKT_TYPE_SETIPGW);
        QMessageBox::about(NULL, "提示", "转台IP地址重新上电后将设置为新的网关地址\n" + ui->leSetIPGageway->text().toLatin1());
    }
    if(leSetHdCamIpChanged == true)
    {
        strncpy(hdCamIp,ui->leSetHdCamIP->text().toLatin1(),16);
        QMessageBox::about(NULL, "提示", "RTSP相机IP地址已修改\n" + ui->leSetHdCamIP->text().toLatin1());
    }*/
    writeCfgFile();
    ui->cbxEditLock_sys->setCheckState(Qt::Unchecked);
}

void MainWindow::setCtlObjStatus(bool enFlag)
{
   // ui->rbSmallTracking->setEnabled(enFlag);
   // ui->rbDetection->setEnabled(enFlag);
    ui->tabWidConfig->setEnabled(enFlag);
    ui->pbOsdWordSave->setEnabled(enFlag);
    ui->cbxEditLock_sys->setEnabled(enFlag);

    ui->pbUp->setEnabled(enFlag);
    ui->pbDown->setEnabled(enFlag);
    ui->pbLeft->setEnabled(enFlag);
    ui->pbRight->setEnabled(enFlag);
    ui->pbStop->setEnabled(enFlag);
    ui->pbRec->setEnabled(enFlag);
    ui->pbFocusN->setEnabled(enFlag);
    ui->pbFocusF->setEnabled(enFlag);
    ui->pbZoomIn->setEnabled(enFlag);
    ui->pbZoomOut->setEnabled(enFlag);
    ui->pbAutoFoc->setEnabled(enFlag);
    ui->pbApUp->setEnabled(enFlag);
    ui->pbApDown->setEnabled(enFlag);
    ui->pbToZero->setEnabled(enFlag);

//    ui->cbxBalance->setEnabled(enFlag);
    ui->lblPadDirection->setEnabled(enFlag);
    ui->lblPadDirection_B->setEnabled(enFlag);
    //ui->gridLayout_Theo->setEnabled(enFlag);
    ui->rb_trackStick->setEnabled(enFlag);
    //ui->rb_trackTable->setEnabled(enFlag);
    ui->rb_Center->setEnabled(enFlag);
    //ui->rb_toZero->setEnabled(enFlag);
    ui->rb_Tracking->setEnabled(enFlag);


    ui->rb_trackRoi10->setEnabled(enFlag);
    ui->rb_trackRoi20->setEnabled(enFlag);
    ui->rb_trackRoi40->setEnabled(enFlag);
    /*ui->rb_StickSpeedH->setEnabled(enFlag);
    ui->rb_StickSpeedM->setEnabled(enFlag);
    ui->rb_StickSpeedL->setEnabled(enFlag);
    ui->checkBoxCaught->setEnabled(enFlag);
    ui->checkBoxTheoGuid->setEnabled(enFlag);
    ui->checkBoxCenterGuid->setEnabled(enFlag);
    ui->pbSyncSystime->setEnabled(enFlag);*/
    ui->pb_OpenSelfTest->setEnabled(enFlag);
    ui->pb_CloseSelfTest->setEnabled(enFlag);
    ui->cbxStartTracking->setEnabled(enFlag);

}

void MainWindow::on_cbxEditLock_sys_stateChanged(int arg1)
{
    if(ui->cbxEditLock_sys->isChecked() == true)    //开始编辑
    {
        ui->leRecFileDir->setEnabled(true);
        ui->leSetIP->setEnabled(true);
        //ui->leSetIPMask->setEnabled(true);
        //ui->leSetIPGageway->setEnabled(true);
        leRecDirChanged = false;
        leSetIpChanged = false;
        leSetIPMaskChanged = false;
        leSetIPGWChanged = false;
        ui->leSetHdCamIP->setEnabled(true);
        leSetHdCamIpChanged = false;
        ui->leSetHdCamPort->setEnabled(true);
        leCSidChanged = false;
        leSetCenterComChanged = false;

        ui->pbConfigSave->setEnabled(true);
    }
    else    //完成编辑
    {
        ui->leRecFileDir->setEnabled(false);
        ui->leSetIP->setEnabled(false);
        //ui->leSetIPMask->setEnabled(false);
        //ui->leSetIPGageway->setEnabled(false);
        ui->pbConfigSave->setEnabled(false);
        ui->leSetHdCamIP->setEnabled(false);
        ui->leSetHdCamPort->setEnabled(false);
    }
}

void MainWindow::on_leRecFileDir_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leRecDirChanged = true;
}

void MainWindow::on_leSetIP_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leSetIpChanged = true;
}


void MainWindow::on_leSetIPMask_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leSetIPMaskChanged = true;
}

void MainWindow::on_leSetIPGageway_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leSetIPGWChanged = true;
}

void MainWindow::on_leAutoSpeed_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leAutoSpdChanged = true;
}

void MainWindow::on_leSetHdCamIP_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leSetHdCamIpChanged = true;
}


bool MainWindow::checkip(QString ip)
{
    QRegExp rx2("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
    if( !rx2.exactMatch(ip) )
    {
        return false;
    }
    return true;
}

uint32_t MainWindow::IPV4StringToInteger(const QString& ip)
{
    QStringList ips = ip.split(".");
    if(ips.size() == 4){
        return ips.at(3).toInt()
                | ips.at(2).toInt() << 8
                | ips.at(1).toInt() << 16
                | ips.at(0).toInt() << 24;
    }
    return 0;
}

int MainWindow::readCfgFile()
{
    QString Path = QCoreApplication::applicationDirPath();
    QString endPath = Path + "/ptzcfgfile.ini";
    QFileInfo fileinfo(endPath);
    if (!fileinfo.isFile()) {
        QMessageBox::about(NULL, "提示", "ptzcfgfile.ini文件不存在，程序无法运行");
        return 0;
        /*qDebug() << "ptzcfgfile.ini文件不存在,创建默认配置文件";
        //创建配置文件，写入默认值
        QSettings setting(endPath, QSettings::IniFormat);
        setting.beginGroup("NETWORK");
        setting.setValue("PtzIp", "192.168.1.202");         // 云台A的IP地址
        setting.setValue("SetHdCamIP", "192.168.1.108");    // 高清相机A的IP
        setting.setValue("SetHdCamPort", 8555);             // 相机A端口
        setting.setValue("PtzIp_B", "192.168.1.203");       // 云台B的IP地址
        setting.setValue("SetHdCamIP_B", "192.168.1.109");  // 高清相机B的IP
        setting.setValue("SetHdCamPort_B", 8555);           // 相机B端口
        setting.setValue("DeviceID", 10);                   // 设备ID
        setting.setValue("CenterCom", "COM6");              // 中心通信串口
        setting.setValue("ZhiKongIP", "192.168.1.10");      // 指控系统IP
        setting.setValue("ZhiKongPort", 10050);             // 指控系统端口
        setting.setValue("GyroIP", "192.168.1.30");         // 陀螺仪IP
        setting.setValue("GyroPort", 10070);                // 陀螺仪端口
        setting.setValue("ListenGyroPort", 10080);          // 监听陀螺仪端口
        setting.setValue("MyIP", "192.168.1.20");           // 本机IP
        setting.setValue("MyPort", 10060);                  // 本机端口
        setting.endGroup();

        setting.beginGroup("STATIONINFO");
        setting.setValue("StationName", "10号站");          // 站点名称
        setting.setValue("StationGdcType", "WGS84");       // 坐标系类型
        setting.setValue("StationB", "119.7907");          // 站址纬度(度)
        setting.setValue("StationL", "35.3571");           // 站址经度(度)
        setting.setValue("StationH", "60");                // 站址高程(米)
        setting.endGroup();

        setting.beginGroup("PARAM");
        setting.setValue("RecordDir", "d:");              // 录像保存目录
        setting.setValue("Diff_X_A", 0);                  // 光电A方位补偿
        setting.setValue("Diff_X_B", 0);                  // 光电B方位补偿
        setting.setValue("Diff_Y_A", 0);                  // 光电A俯仰补偿
        setting.setValue("Diff_Y_B", 0);                  // 光电B俯仰补偿
        setting.setValue("CentroidWhMax", 245);           // 质心算法白色最大值
        setting.setValue("CentroidBlkMin", 80);           // 质心算法黑色最小值
        setting.endGroup();
#if 0
        setting.beginGroup("OSD");
        setting.setValue("OSDDeviceName", "10号站点");
        setting.setValue("OSDFont", "黑体");
        setting.setValue("OSDFontSize", 32);
        setting.setValue("OSDLocationY", 950);
        setting.endGroup();
#endif

        setting.beginGroup("MOUNTPOS");
        setting.setValue("GD1MOUNTX", 1.34);           // 光电1安装X坐标(米)
        setting.setValue("GD1MOUNTY", 0.73);           // 光电1安装Y坐标(米)
        setting.setValue("GD1MOUNTZ", -0.17);          // 光电1安装Z坐标(米)
        setting.setValue("GD2MOUNTX", -3.21);          // 光电2安装X坐标(米)
        setting.setValue("GD2MOUNTY", -0.73);          // 光电2安装Y坐标(米)
        setting.setValue("GD2MOUNTZ", -0.17);          // 光电2安装Z坐标(米)
        setting.setValue("CROSSBORDERBEG",315.0);      // 越界开始角度
        setting.setValue("CROSSBORDEREND",45.0);       // 越界结束角度
        setting.endGroup();

        setting.beginGroup("PID");
        setting.setValue("Pid_X_Scale", 3);            // X轴PID缩放系数
        setting.setValue("Pid_X_P", 1);                // X轴比例系数
        setting.setValue("Pid_X_I", 0.5);              // X轴积分系数
        setting.setValue("Pid_X_D", 1);                // X轴微分系数
        setting.setValue("Pid_Y_Scale", 3);            // Y轴PID缩放系数
        setting.setValue("Pid_Y_P", 0.6);              // Y轴比例系数
        setting.setValue("Pid_Y_I", 0.5);              // Y轴积分系数
        setting.setValue("Pid_Y_D", 1);                // Y轴微分系数
        setting.setValue("Pid_MaxOut", 255);           // PID最大输出
        setting.setValue("Pid_MinOut", 1);             // PID最小输出
        setting.setValue("Ptz_DefSpeed", 10);          // 云台默认速度
        setting.endGroup();

        //dd_diffx =0;
        dd_diffx_default_A=0;
        dd_diffx_default_B=0;
        dd_diffy_default_A=0;
        dd_diffy_default_B=0;
        mcDevID=10;
        mPid_X_Scale = 3;
        mPid_X_P = 1;
        mPid_X_I = 0.5;
        mPid_X_D = 1;
        mPid_Y_Scale = 3;
        mPid_Y_P = 0.6;
        mPid_Y_I = 0.5;
        mPid_Y_D = 1;
        strcpy(mServoCom,"COM6");
        strcpy(mZhiKongIp,"192.168.1.10");
        mZhiKongPort = 10050;
        strcpy(mMyIp,"192.168.1.20");
        mMyPort = 10060;*/

        //return -1;
    }

    QSettings readSetting(endPath, QSettings::IniFormat);
    readSetting.beginGroup("NETWORK");
    QString sPtzIp = readSetting.value("PtzIp").toString();
    QString sSetHdCamIP = readSetting.value("SetHdCamIP").toString();
    mHdCamPort_A = readSetting.value("SetHdCamPort").toInt();
    QString sPtzIp_B = readSetting.value("PtzIp_B").toString();
    QString sSetHdCamIP_B = readSetting.value("SetHdCamIP_B").toString();
    mHdCamPort_B = readSetting.value("SetHdCamPort_B").toInt();

    mcDevID = readSetting.value("DeviceID").toInt();
    QString sCenterCom = readSetting.value("CenterCom").toString();

    QString sZhiKongIP = readSetting.value("ZhiKongIP").toString();
    mZhiKongPort = readSetting.value("ZhiKongPort").toInt();

    QString sGyroIP = readSetting.value("GyroIP").toString();
    mGyroPort = readSetting.value("GyroPort").toInt();
    mListenGyroPort = readSetting.value("ListenGyroPort").toInt();

    QString sMyIP = readSetting.value("MyIP").toString();
    mMyPort = readSetting.value("MyPort").toInt();
    readSetting.endGroup();

    ui->leSetIP->setText(sPtzIp);
    ui->leGD_A_IP->setText(sPtzIp);
    ui->leSetHdCamIP->setText(sSetHdCamIP);
    ui->leSetHdCamPort->setText(QString::number(mHdCamPort_A));
    strncpy(serverIp_A,ui->leSetIP->text().toLatin1(),16);
    strncpy(hdCamIp_A,ui->leSetHdCamIP->text().toLatin1(),16);

    ui->leSetIP_B->setText(sPtzIp_B);
    ui->leGD_B_IP->setText(sPtzIp_B);
    ui->leSetHdCamIP_B->setText(sSetHdCamIP_B);
    ui->leSetHdCamPort_B->setText(QString::number(mHdCamPort_B));
    strncpy(serverIp_B,ui->leSetIP_B->text().toLatin1(),16);
    strncpy(hdCamIp_B,ui->leSetHdCamIP_B->text().toLatin1(),16);

    ui->leSetZhiKongIP->setText(sZhiKongIP);
    strncpy(mZhiKongIp,ui->leSetZhiKongIP->text().toLatin1(),16);

    ui->leSetGyroIP->setText(sGyroIP);
    strncpy(mGyroIp,ui->leSetGyroIP->text().toLatin1(),16);
    ui->leSetMyIP->setText(sMyIP);
    strncpy(mMyIp,ui->leSetMyIP->text().toLatin1(),16);
    ui->leSetZhiKongPort->setText(QString::number(mZhiKongPort));
    ui->leSetGyroPort->setText(QString::number(mGyroPort));
    ui->leSetListenGyroPort->setText(QString::number(mListenGyroPort));
    ui->leSetMyPort->setText(QString::number(mMyPort));

    readSetting.beginGroup("STATIONINFO");
    msStationName = readSetting.value("StationName").toString();
    msStationGdcType = readSetting.value("StationGdcType").toString();
    float fStationB = readSetting.value("StationB").toFloat();
    float fStationL = readSetting.value("StationL").toFloat();
    float StationH = readSetting.value("StationH").toFloat();
    readSetting.endGroup();

    mvStationBlh.setX(fStationB);
    mvStationBlh.setY(fStationL);
    mvStationBlh.setZ(StationH);
    mCoordTrsr.Init(msStationGdcType, mvStationBlh);


    readSetting.beginGroup("PARAM");
    QString sRecordDir = readSetting.value("RecordDir").toString();
    dd_diffx_default_A = readSetting.value("Diff_X_A").toDouble();
    dd_diffx_default_B = readSetting.value("Diff_X_B").toDouble();
    dd_diffy_default_A = readSetting.value("Diff_Y_A").toDouble();
    dd_diffy_default_B = readSetting.value("Diff_Y_B").toDouble();

    radar_diffx_A = readSetting.value("RadarDiff_X_A").toDouble();
    radar_diffx_B = readSetting.value("RadarDiff_X_B").toDouble();
    radar_diffy_A = readSetting.value("RadarDiff_Y_A").toDouble();
    radar_diffy_B = readSetting.value("RadarDiff_Y_B").toDouble();

    //trackerThd->mCentroidWhMax = readSetting.value("CentroidWhMax").toInt();
    //trackerThd->mCentroidBlkMin = readSetting.value("CentroidBlkMin").toInt();
    readSetting.endGroup();

    ui->leRecFileDir->setText(sRecordDir);
    ui->leSetDiffX_default_A->setText(QString::number(dd_diffx_default_A,'f',4));
    ui->leSetDiffX_default_B->setText(QString::number(dd_diffx_default_B,'f',4));
    ui->leSetDiffY_default_A->setText(QString::number(dd_diffy_default_A,'f',4));
    ui->leSetDiffY_default_B->setText(QString::number(dd_diffy_default_B,'f',4));

    ui->leSetRadarDiffX_A->setText(QString::number(radar_diffx_A,'f',4));
    ui->leSetRadarDiffX_B->setText(QString::number(radar_diffx_B,'f',4));
    ui->leSetRadarDiffY_A->setText(QString::number(radar_diffy_A,'f',4));
    ui->leSetRadarDiffY_B->setText(QString::number(radar_diffy_B,'f',4));
/*
    readSetting.beginGroup("OSD");
    QString sOSDDeviceName = readSetting.value("OSDDeviceName").toString();
    QString sOSDFont = readSetting.value("OSDFont").toString();
    int iFontSize = readSetting.value("OSDFontSize").toInt();
    int iOSDLocationY = readSetting.value("OSDLocationY").toInt();
    readSetting.endGroup();

    ui->leOsdWord->setText(sOSDDeviceName);
    ui->cbxOSDFont->setCurrentText(sOSDFont);
    ui->cbxOSDFontSize->setCurrentText(QString::number(iFontSize,10));
    ui->leOSDLocationY->setText(QString::number(iOSDLocationY,10));
    fullscreen.OSDText=ui->leOsdWord->text();
    fullscreen.OSDFont=ui->cbxOSDFont->currentText();
    fullscreen.OSDFontHeight=ui->cbxOSDFontSize->currentText().toInt() ;
    fullscreen.OSDLocationY=ui->leOSDLocationY->text().toInt();
*/

    readSetting.beginGroup("MOUNTPOS");
    mGD1MountPosX = readSetting.value("GD1MOUNTX").toDouble();
    mGD1MountPosY = readSetting.value("GD1MOUNTY").toDouble();
    mGD1MountPosZ = readSetting.value("GD1MOUNTZ").toDouble();
    mGD2MountPosX = readSetting.value("GD2MOUNTX").toDouble();
    mGD2MountPosY = readSetting.value("GD2MOUNTY").toDouble();
    mGD2MountPosZ = readSetting.value("GD2MOUNTZ").toDouble();
    mBorder180Begin = readSetting.value("CROSSBORDERBEG").toDouble();
    mBorder180End = readSetting.value("CROSSBORDEREND").toDouble();
    readSetting.endGroup();

    ui->leGd1InstallX->setText(QString::number(mGD1MountPosX, 'f',3));
    ui->leGd1InstallY->setText(QString::number(mGD1MountPosY, 'f',3));
    ui->leGd1InstallZ->setText(QString::number(mGD1MountPosZ, 'f',3));
    ui->leGd2InstallX->setText(QString::number(mGD2MountPosX, 'f',3));
    ui->leGd2InstallY->setText(QString::number(mGD2MountPosY, 'f',3));
    ui->leGd2InstallZ->setText(QString::number(mGD2MountPosZ, 'f',3));
    ui->leSetBorderBeg->setText(QString::number(mBorder180Begin, 'f',3));
    ui->leSetBorderEnd->setText(QString::number(mBorder180End, 'f',3));

    readSetting.beginGroup("PID");
    mPid_X_Scale = readSetting.value("Pid_X_Scale").toFloat();
    mPid_X_P = readSetting.value("Pid_X_P").toFloat();
    mPid_X_I = readSetting.value("Pid_X_I").toFloat();
    mPid_X_D = readSetting.value("Pid_X_D").toFloat();
    mPid_Y_Scale = readSetting.value("Pid_Y_Scale").toFloat();
    mPid_Y_P = readSetting.value("Pid_Y_P").toFloat();
    mPid_Y_I = readSetting.value("Pid_Y_I").toFloat();
    mPid_Y_D = readSetting.value("Pid_Y_D").toFloat();
    mPidMaxOut = readSetting.value("Pid_MaxOut").toInt();
    mPidMinOut = readSetting.value("Pid_MinOut").toInt();
    mPtzDefSpeed = readSetting.value("Ptz_DefSpeed").toInt();
    readSetting.endGroup();

    ui->lePid_X_Scale->setText(QString::number(mPid_X_Scale, 'f',2));
    ui->lePid_X_P->setText(QString::number(mPid_X_P, 'f',2));
    ui->lePid_X_I->setText(QString::number(mPid_X_I, 'f',2));
    ui->lePid_X_D->setText(QString::number(mPid_X_D, 'f',2));
    ui->lePid_Y_Scale->setText(QString::number(mPid_Y_Scale, 'f',2));
    ui->lePid_Y_P->setText(QString::number(mPid_Y_P, 'f',2));
    ui->lePid_Y_I->setText(QString::number(mPid_Y_I, 'f',2));
    ui->lePid_Y_D->setText(QString::number(mPid_Y_D, 'f',2));
    ui->lePidMaxOut->setText(QString::number(mPidMaxOut));
    ui->lePidMinOut->setText(QString::number(mPidMinOut));
    ui->lePtzDefSpeed->setText(QString::number(mPtzDefSpeed));

//    readSetting.beginGroup("IR_DETECTION");
//    irConfig.tarLmax = readSetting.value("TarLmax", 9).toInt();
//    irConfig.k1 = readSetting.value("K1", 2).toInt();
//    irConfig.k2 = readSetting.value("K2", 1).toInt();
//    irConfig.scoreThreshold = readSetting.value("ScoreThreshold", 3.0).toFloat();
//    readSetting.endGroup();


    return 0;
}

void MainWindow::writeCfgFile()
{
    QString Path = QCoreApplication::applicationDirPath();
    QString endPath = Path + "/ptzcfgfile.ini";
    QSettings setting(endPath, QSettings::IniFormat);

    setting.beginGroup("NETWORK");
    setting.setValue("PtzIp", ui->leSetIP->text().toLatin1().data());
    setting.setValue("SetHdCamIP", ui->leSetHdCamIP->text().toLatin1());
    setting.setValue("SetHdCamPort", ui->leSetHdCamPort->text().toLatin1());

    setting.setValue("PtzIp_B", ui->leSetIP_B->text().toLatin1().data());
    setting.setValue("SetHdCamIP_B", ui->leSetHdCamIP_B->text().toLatin1());
    setting.setValue("SetHdCamPort_B", ui->leSetHdCamPort_B->text().toLatin1());

    setting.setValue("ZhiKongIP", ui->leSetZhiKongIP->text().toLatin1());
    setting.setValue("ZhiKongPort", ui->leSetZhiKongPort->text().toLatin1());
    setting.setValue("GyroIP", ui->leSetGyroIP->text().toLatin1());
    setting.setValue("GyroPort", ui->leSetGyroPort->text().toLatin1());
    setting.setValue("ListenGyroPort", ui->leSetListenGyroPort->text().toLatin1());
    setting.setValue("MyIP", ui->leSetMyIP->text().toLatin1());
    setting.setValue("MyPort", ui->leSetMyPort->text().toLatin1());
    setting.endGroup();

    setting.beginGroup("PARAM");
    setting.setValue("RecordDir", ui->leRecFileDir->text().toLatin1());
    setting.setValue("Diff_X_A", ui->leSetDiffX_default_A->text().toLatin1());
    setting.setValue("Diff_X_B", ui->leSetDiffX_default_B->text().toLatin1());
    setting.setValue("Diff_Y_A", ui->leSetDiffY_default_A->text().toLatin1());
    setting.setValue("Diff_Y_B", ui->leSetDiffY_default_B->text().toLatin1());

    setting.setValue("RadarDiff_X_A", ui->leSetRadarDiffX_A->text().toLatin1());
    setting.setValue("RadarDiff_X_B", ui->leSetRadarDiffX_B->text().toLatin1());
    setting.setValue("RadarDiff_Y_A", ui->leSetRadarDiffY_A->text().toLatin1());
    setting.setValue("RadarDiff_Y_B", ui->leSetRadarDiffY_B->text().toLatin1());

    setting.setValue("CentroidWhMax",0);
    setting.setValue("CentroidBlkMin",0);
    setting.endGroup();
#if 0
    setting.beginGroup("OSD");
    setting.setValue("OSDDeviceName", ui->leOsdWord->text());
    setting.setValue("OSDFont", ui->cbxOSDFont->currentText());
    setting.setValue("OSDFontSize", ui->cbxOSDFontSize->currentText());
    setting.setValue("OSDLocationY", ui->leOSDLocationY->text());
    setting.endGroup();
#endif

    setting.beginGroup("MOUNTPOS");
    setting.setValue("GD1MOUNTX", ui->leGd1InstallX->text().toLatin1());
    setting.setValue("GD1MOUNTY", ui->leGd1InstallY->text().toLatin1());
    setting.setValue("GD1MOUNTZ", ui->leGd1InstallZ->text().toLatin1());
    setting.setValue("GD2MOUNTX", ui->leGd2InstallX->text().toLatin1());
    setting.setValue("GD2MOUNTY", ui->leGd2InstallY->text().toLatin1());
    setting.setValue("GD2MOUNTZ", ui->leGd2InstallZ->text().toLatin1());
    setting.setValue("CROSSBORDERBEG",ui->leSetBorderBeg->text().toLatin1());
    setting.setValue("CROSSBORDEREND",ui->leSetBorderEnd->text().toLatin1());
    setting.endGroup();

    mGD1MountPosX = QString(ui->leGd1InstallX->text()).toDouble();
    mGD1MountPosY = QString(ui->leGd1InstallY->text()).toDouble();
    mGD1MountPosZ = QString(ui->leGd1InstallZ->text()).toDouble();
    mGD2MountPosX = QString(ui->leGd2InstallX->text()).toDouble();
    mGD2MountPosY = QString(ui->leGd2InstallY->text()).toDouble();
    mGD2MountPosZ = QString(ui->leGd2InstallZ->text()).toDouble();
    mBorder180Begin = QString(ui->leSetBorderBeg->text()).toDouble();
    mBorder180End = QString(ui->leSetBorderEnd->text()).toDouble();

    setting.beginGroup("PID");
    setting.setValue("Pid_X_Scale", ui->lePid_X_Scale->text());
    setting.setValue("Pid_X_P", ui->lePid_X_P->text());
    setting.setValue("Pid_X_I", ui->lePid_X_I->text());
    setting.setValue("Pid_X_D", ui->lePid_X_D->text());
    setting.setValue("Pid_Y_Scale", ui->lePid_Y_Scale->text());
    setting.setValue("Pid_Y_P", ui->lePid_Y_P->text());
    setting.setValue("Pid_Y_I", ui->lePid_Y_I->text());
    setting.setValue("Pid_Y_D", ui->lePid_Y_D->text());
    setting.setValue("Pid_MaxOut", ui->lePidMaxOut->text());
    setting.setValue("Pid_MinOut", ui->lePidMinOut->text());
    setting.setValue("Ptz_DefSpeed", ui->lePtzDefSpeed->text());
    setting.endGroup();

    mPid_X_Scale = QString(ui->lePid_X_Scale->text()).toFloat();
    mPid_X_P = QString(ui->lePid_X_P->text()).toFloat();
    mPid_X_I = QString(ui->lePid_X_I->text()).toFloat();
    mPid_X_D = QString(ui->lePid_X_D->text()).toFloat();
    mPid_Y_Scale = QString(ui->lePid_Y_Scale->text()).toFloat();
    mPid_Y_P = QString(ui->lePid_Y_P->text()).toFloat();
    mPid_Y_I = QString(ui->lePid_Y_I->text()).toFloat();
    mPid_Y_D = QString(ui->lePid_Y_D->text()).toFloat();
    mPidMaxOut = QString(ui->lePidMaxOut->text()).toInt();
    mPidMinOut = QString(ui->lePidMinOut->text()).toInt();
    mPtzDefSpeed = QString(ui->lePtzDefSpeed->text()).toInt();

    PIDInit2(&PidX,0,mPid_X_Scale,mPid_X_P,mPid_X_I,mPid_X_D,mPidMaxOut);
    PIDInit2(&PidY,0,mPid_Y_Scale,mPid_Y_P,mPid_Y_I,mPid_Y_D,mPidMaxOut);

//    setting.beginGroup("IR_DETECTION");
//    setting.setValue("TarLmax", irConfig.tarLmax);
//    setting.setValue("K1", irConfig.k1);
//    setting.setValue("K2", irConfig.k2);
//    setting.setValue("ScoreThreshold", irConfig.scoreThreshold);
//    setting.endGroup();
}

char * MainWindow::rtrim(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }

    int len = strlen(str);
    char *p = str + len - 1;
    while (p >= str  && isspace(*p))
    {
        *p = '\0';
        --p;
    }

    return str;
}

void MainWindow::on_cbxPalette_currentIndexChanged(int index)
{
    int val = index;
    sendSetCmd(PKT_TYPE_PALETTE,val);
    qDebug()<<("CurrentIdx=")<<(int)val;
}

void MainWindow::on_pbLockAxial_clicked()
{
    sendSetCmd(PKT_TYPE_LOCKAXIAL,0);
}

void MainWindow::on_pbAdjDriftXAdd_pressed()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,1,0);
}

void MainWindow::on_pbAdjDriftXAdd_released()

{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,0);
}

void MainWindow::on_pbAdjDriftXDec_pressed()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0xff,0);
}

void MainWindow::on_pbAdjDriftXDec_released()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,0);
}

void MainWindow::on_pbAdjDriftYAdd_pressed()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,1);
}

void MainWindow::on_pbAdjDriftYAdd_released()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,0);
}

void MainWindow::on_pbAdjDriftYDec_pressed()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,0xff);
}

void MainWindow::on_pbAdjDriftYDec_released()
{
    sendSetCmd(PKT_TYPE_ADJDRIFT,0,0);
}

void MainWindow::on_pbLockAxial_2_clicked()
{
    sendSetCmd(PKT_TYPE_LOCKAXIAL,0);
}


void MainWindow::on_cbxBalance_currentIndexChanged(int index)
{
    int val = index;
    sendSetCmd(PKT_TYPE_BALANCE,val);
    qDebug()<<("CurrentIdx=")<<(int)val;
}


void MainWindow::on_pbOnOffColor_clicked()
{
    int val = 0;
    static bool onoff = false;

    if(onoff == true )
    {
        val =0;
        onoff = false;
    }
    else
    {
        val = 1;
        onoff = true;
    }
    sendSetCmd(PKT_TYPE_ONOFFCOLOR,val);
}

void MainWindow::on_le_setDirecion_returnPressed()
{
    on_pbSendDirection_clicked();
}

void MainWindow::on_le_setPitch_returnPressed()
{
    on_pbSendPitch_clicked();
}

/*
void MainWindow::on_pbFullScreen_clicked()
{

    //fullscreen.showFullScreen();//全屏
    fullscreen.show();
    fullscreen.grabKeyboard();
    this->hide();
}
*/


void MainWindow::slotGetSigBackToMainWin()
{
   // if(connectFlag == true)
    {
        mPlayer->runflag=false;
        fullscreen.close();
    }
    //delete(mPlayer);
}

void MainWindow::slotGetUpdateJoystick()
{
    updateJoyStick();
}

void MainWindow::slotUpdateTrackingBox(int num,int x,int y,int width,int height)
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    auto start = std::chrono::high_resolution_clock::now();
    // 跟踪丢失处理
    if( num == 0 )
    {
        pnetworkComm->msgToZhiKong.numOfObj = 0;
        // 设置默认ROI位置
        pnetworkComm->msgToZhiKong.obj1PosX = roiBox.x = 1920/2-30;
        pnetworkComm->msgToZhiKong.obj1PosY = roiBox.y = 1080/2-20;
        pnetworkComm->msgToZhiKong.roiWidth = roiBox.width = 60;
        pnetworkComm->msgToZhiKong.roiHeight = roiBox.height = 40;
        //trackerThd->bTrackerStart = false;
        pnetworkComm->msgToZhiKong.status = 4;

        // 切换回TopHat算法继续搜索
        if(trackerThd->trackingMethod == TRACKING_METHOD_IR_SMALL)
        {
            trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
             // PIDReset2(&PidX);
             // PIDReset2(&PidY);
        }
        // ui->cbxStartTracking->setChecked(false);
        on_pbStop_pressed();
        ui->lblTrackingFps->setText("0");
        // PIDReset2(&PidX);
        // PIDReset2(&PidY);
        ui->te_Output->append("跟踪状态：丢失");

        return;
    }

    // 跟踪成功 - 计算控制量
    pnetworkComm->msgToZhiKong.status = 3; // 跟踪状态
    //LabStatebarInfo->setText("跟踪状态：正在跟踪");
    // 平滑滤波
    static int xOld,yOld,wOld,hOld;
    int xx,yy,ww,hh;
    float k = 0.2;
    xx = x*k+xOld*(1-k);
    yy = y*k+yOld*(1-k);
    ww = width*k+wOld*(1-k);
    hh = height*k+hOld*(1-k);
    xOld = x;
    yOld = y;
    wOld = width;
    hOld = height;

    pnetworkComm->msgToZhiKong.numOfObj = 1;
    // 更新ROI框
    pnetworkComm->msgToZhiKong.obj1PosX = roiBox.x = xx ;
    pnetworkComm->msgToZhiKong.obj1PosY = roiBox.y = yy ;
    roiBox.width = ww;
    roiBox.height = hh;

    pnetworkComm->msgToZhiKong.roiWidth  = ww/2;
    pnetworkComm->msgToZhiKong.roiHeight  = hh/2;

    // 计算目标偏差（像素）
    int diff_x = xx + roiBox.width/2 - IMAGE_PIXEL_WIDTH/2 ;
    int diff_y = yy + roiBox.height/2 - IMAGE_PIXEL_HEIGHT/2 ;
    //qDebug()<<"Recv tracking Rect diff_x:" << diff_x<< " diff_y:" << diff_y <<" PidX.SumError:"<<PidX.SumError;
    //伺服测试
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    // qDebug() << "报脱靶量时间:" << timestamp<<"目标偏差x："<<diff_x<<"目标偏差y："<<diff_y;
    //打开文件并写入数据（追加模式）
    // QFile file("pid_x.txt");
    // if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    //     QTextStream out(&file);
    //     out << timestamp << " " <<diff_x<< "\n";
    //     file.close();
    // }

    // PID控制计算
    int x_out,y_out;
    x_out=-(signed short)PIDCalc3(&PidX,diff_x);
    y_out=(signed short)PIDCalc3(&PidY,diff_y);
    //qDebug()<<"1 x_out:" << x_out<< " y_out:" << y_out;
    //ui->lbSendUdpDataA->setText( QString::number(PidX.SumError,'f',0));
    //ui->lbSendUdpDataE->setText( QString::number(diff_y));

    double kh=0;
    int lensVal = 0;
    if(ui->rb_selGDA->isChecked())
    {
        lensVal = hdLensVal;
    }
    else
    {
        lensVal = hdLensVal_B;
    }
    //打开文件并写入数据（追加模式）
     QFile file("lensVal.txt");
     if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
         QTextStream out(&file);
         out << lensVal << "\n";
         file.close();}
    // 根据镜头焦距计算控制增益
    switch(lensVal)
    {
        case 480 ... 540 :
            kh=0.6; break;       //0.5°*100=50
        case 400 ... 479:
            kh=1; break;
        case 350 ... 399:
            kh=1.2; break;
        case 300 ... 349:
            kh=1.7; break;
        case 250 ... 299:
            kh=1.9; break;
        case 200 ... 249:
            kh=2.3; break;
        case 150 ... 199:
            kh=2.7; break;
        case 100 ... 149:
            kh=3.5; break;
        case 50 ... 99:
            kh=7; break;
        case 30 ... 49:
            kh=13; break;
        case 12 ... 29:
            kh=20; break;
        default:
            kh=30;             //3°*100=300
        break;
    }
    //double am = kh/(double)0x200;
    // 根据镜头焦距调整控制增益

    x_out = x_out * kh;
    y_out = y_out * kh;
    //qDebug()<<"2 x_out:" << x_out<< " y_out:" << y_out <<"  kh:" << kh;
    //qDebug()<<"kh:" << kh;
    // 死区处理

    // if(diff_x>3 && x_out == 0)
    // {
    //     x_out=1;
    // }
    // else if(diff_x<-3 && x_out == 0)
    // {
    //     x_out=-1;
    // }
    // if(diff_y>5 && y_out == 0)
    // {
    //     y_out=-1;
    // }
    // else if(diff_y<-5 && y_out == 0)
    // {
    //     y_out=1;
    // }



    // 发送控制命令
    if( true == ui->cbxStartTracking->isChecked() )
    {
        // 限幅处理
        if(x_out>32768) x_out = 32768;
        else if(x_out<-32768) x_out = -32768;

        if(y_out>32768) y_out = 32768;
        else if(y_out<-32768) y_out = -32768;

        //706测试-伺服正弦信号
        // 限幅处理前先赋值测试值
        // static double angle = 0.0;  // 静态变量保持角度值
        // const double amplitude = 10000.0;  // 正弦波幅度
        // const double y_fixed = 0;    // y固定值

        // 生成正弦值 (x_out)
        // angle += 0.1;  // 每次增加的角度（控制频率）
        // if(angle > 2*M_PI) angle -= 2*M_PI;  // 保持在0-2π范围内

        // x_out = amplitude * sin(angle);  // 正弦波输出
        // y_out = y_fixed;                // y固定值

        // QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        // qDebug() << "报伺服指令时间:" << timestamp<<"x_out："<<x_out<<"y_out："<<y_out;
        // //打开文件并写入数据（追加模式）
        // QFile file("pid_send.txt");
        // if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        //     QTextStream out(&file);
        //     out << timestamp << " " <<x_out<< "\n";
        //     file.close();
        // }

        uint8_t * cmd = mPodCmd.crtSteplessCmd(x_out, y_out);//给伺服发速度指令
        // udpskt->write((char *)cmd,7);
        // 同时发送给A和B两个光电
        if(connectFlag_A != false)
        {
            udpSocket_A->write((char *)cmd,7);
        }
        if(connectFlag_B != false)
        {
            udpSocket_B->write((char *)cmd,7);
        }


        //706测试正弦发送位置指令
        // static double angle = 0.0;
        // const double centerDir = 30.0;  // 中心角度30°
        // const double amplitude = 10.0;  // 幅度±10° → 范围20°~40°
        // const float fixedPitch = 10;

        // 更新正弦波角度
        // angle += 0.2;  // 调整步长改变速度
        // if (angle > 2 * M_PI) angle -= 2 * M_PI;
        // 计算正弦波方位角（20°~40°）
        // float sinDirection = centerDir + amplitude * sin(angle);
        // sendPosition(sinDirection, fixedPitch,true,true);//给伺服发位置指令
        // QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        // qDebug() << "报伺服位置指令时间:" << timestamp<<"方位："<<sinDirection<<"俯仰："<<fixedPitch;
        //打开文件并写入数据（追加模式）
        // QFile file("pid_send.txt");
        // if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        //     QTextStream out(&file);
        //     out << timestamp << " " <<sinDirection<< "\n";
        //     file.close();
        // }



        //uint8_t * cmd = mPodCmd.crtSteplessCmd(x_out*10, y_out*10);

        // QByteArray cmdData(reinterpret_cast<const char*>(cmd), 7); // 假设长度是7字节
        // qDebug() << "给伺服发送指令:" << cmdData.toHex(' '); // 用空格分隔每个字节
        // QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        // qDebug() << "给伺服发送时间:" << timestamp<<"目标偏差x："<<diff_x<<"目标偏差y："<<diff_y;




        //测试延时
        // QString timeStr2 = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        // //打开文件并写入数据（追加模式）
        // QFile file("pid_log.txt");
        // if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        //     QTextStream out(&file);
        //     out << timeStr2 << " " << "\n";
        //     file.close();
        // }



       /* if( ui->rb_selGDA->isChecked())
        {
            QString qstr ="光电A 目标 方位: " + QString::number(mPtzDirection_A,'f',2) + " 俯仰: "+ QString::number(mPtzPitch_A,'f',2);
            ui->te_Output->append(qstr);
        }
        else
        {
            QString qstr ="光电B 目标 方位: " + QString::number(mPtzDirection_B,'f',2) + " 俯仰: "+ QString::number(mPtzPitch_B,'f',2);
            ui->te_Output->append(qstr);
        } */

    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    //qDebug() << "pid计算耗时:" << elapsed.count() << "ms";


    ui->lblTrackingFps->setText(QString::number(trackerThd->fps));
}

void MainWindow::slotUpdateTrackingBoxDeg(int num,int x,int y,int width,int height)
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    //qDebug()<<"slotUpdateTrackingBoxDeg() num:"<< num<<"   x:"<< x<<"   y:"<< y <<"   width:"<< width <<"   height:"<< height;
    if(num == 0)
    {
        trackerThd->bTrackerStart = false;
        pnetworkComm->msgToZhiKong.status = 4;
        on_pbStop_pressed();
        ui->cbxStartTracking->setChecked(false);
        ui->lblTrackingFps->setText("0");
        // PIDReset2(&PidX);
        // PIDReset2(&PidY);
        ui->te_Output->append("跟踪状态：丢失");
        return;
    }
    pnetworkComm->msgToZhiKong.status = 3;
    //LabStatebarInfo->setText("跟踪状态：正在跟踪");

    float dtx = 0 ;
    float dty = 0 ;
    float scr_du = 0;
    int x_out,y_out;

    if(ui->rb_selGDA->isChecked())
        scr_du= IMAGE_PIXEL_WIDTH/(float)mHdViewAngle_A;
    else
        scr_du= IMAGE_PIXEL_WIDTH/(float)mHdViewAngle_B;

    roiBox.x = x ;
    roiBox.y = y ;
    roiBox.width = width;
    roiBox.height = height;

    if( OPMODE_MARKING == operateMode && true == ui->cbxStartTracking->isChecked() )
    {
        dtx = (roiBox.x + trackerThd->roi_width/2 - IMAGE_PIXEL_WIDTH/2)/scr_du;
        if(dtx>360)
        {
            dtx= dtx-360;
        }
        dty = (IMAGE_PIXEL_HEIGHT/2 - (roiBox.y + trackerThd->roi_height/2))/scr_du;


        dtx=-PIDCalc2(&PidX,dtx);
        dty=-PIDCalc2(&PidY,dty);

        x_out = dtx/0.002;
        y_out = dty/0.002;


        /*if(ui->rb_flipPicOn->isChecked())
        {
            uint8_t * cmd = mPodCmd.crtOffsetCmd(-(x_out), -(y_out));
            udpskt->write((char *)cmd,7);
        }
        else */
        {
            uint8_t * cmd = mPodCmd.crtOffsetCmd(x_out, y_out);
            udpskt->write((char *)cmd,7);
        }

        //ui->lbSendUdpDataA->setText( QString::number(dtx, 'f',2)+"°" );
        //ui->lbSendUdpDataE->setText( QString::number(dty, 'f',2)+"°" );
    }
}

void MainWindow::slotUpdateDetectingBox(int num,int id,int x,int y,int width,int height,float prob)
{
    //qDebug()<<"slotUpdateDetectingBox() num:"<< num<<"  id"<<id<<"   x:"<< x<<"   y:"<< y<<"   prob:"<< prob ;

    if(num == 0 )
    {
        //qDebug()<<"slotUpdateDetectingBox() num:"<< num<<"  id"<<id<<"   x:"<< x<<"   y:"<< y<<"   prob:"<< prob <<" autoStat:"<<autofilmThd->autoStat;
        //autofilmThd->mDetect.prob = prob;
        return ;
    }
    if(ui->rb_SelDetectMtd1->isChecked()) {
        slotUpdateTrackingBox(num, x*4, y*4, width*4, height*4);
    } /*else if(ui->rb_SelIRSmallTarget->isChecked()) {
        // 小目标检测处理
        //slotUpdateTrackingBox(num, x, y, width, height);
        update();
    }*/
}

/*
 * outMuBiaoJiaoDu: 输出计算结果，目标相对落点水平方向偏离角度,单位度
 * outMuBiaoGaoDu: 输出计算结果,目标相对甲板高度,单位米
 *
 * targetDistance: 输入参数,光电转台安装位置到目标测距机结果，单位分米
 * panVal: 输入参数, 光电转台相对海平面的真水平角度,单位度
 * tiltVal: 输入参数, 光电转台相对海平面的真俯仰角度
 * AnZhuangX: 输入参数,光电转台安装位置相对落点水平X方向距离,单位米
 * AnZhuangY: 输入参数,光电转台安装位置相对落点水平Y方向距离,单位米
 * AnZhuangZ: 输入参数,光电转台安装位置相对落点垂直Z方向距离,单位米
 */
void MainWindow::autoFilmCalcTargetPosition(double * outMuBiaoWeiZhiX,double * outMuBiaoWeiZhiY, double * outMuBiaoGaoDu, double * outMuBiaoJiaoDu,int targetDistance,
                                            double panVal,double tiltVal,double AnZhuangX,double AnZhuangY,double AnZhuangZ)
{
    if(targetDistance > 4000 || targetDistance < 1)
        return;
    double radPanVal = Deg2Rad(panVal);
    double radTiltVal = Deg2Rad(tiltVal);
    double horDistance = (targetDistance) * cos(radTiltVal);

    double MuBiaoWeiZhiX = 0;
    double MuBiaoWeiZhiY = 0;

    MuBiaoWeiZhiX = horDistance* sin(radPanVal)+AnZhuangX;
    MuBiaoWeiZhiY = horDistance* cos(radPanVal)+AnZhuangY;

    *outMuBiaoJiaoDu = Rad2Deg(atan(MuBiaoWeiZhiX / MuBiaoWeiZhiY));
    *outMuBiaoGaoDu = targetDistance* sin(radTiltVal)+AnZhuangZ;

    gTargetPos.angle = *outMuBiaoJiaoDu;
    gTargetPos.height = *outMuBiaoGaoDu;

#if 0
    double tailDirection = gImuData.direction + 180;
    if(tailDirection > 360)
        tailDirection = tailDirection-360;

    gTargetPos.latitude = gImuData.latitude + ((horDistance * cos(Deg2Rad(tailDirection + gTargetPos.angle)))/0.1111) * 0.000001;   //纬度
    gTargetPos.lontude = gImuData.lontude + ((horDistance * sin(Deg2Rad(tailDirection + gTargetPos.angle)))/(0.1113 * cos(Deg2Rad(gTargetPos.latitude)))) * 0.000001;   //经度

    //qDebug()<<"X:"<<MuBiaoWeiZhiX<<"    Y:"<<MuBiaoWeiZhiY <<"Z:"<<*outMuBiaoGaoDu <<"  jiaodu:"<<gTargetPos.angle<<" horDistance:"<<horDistance <<" tailDirection:"<<tailDirection <<" WeiDu:"<<QString::number(gTargetPos.latitude,'f',6) <<" JingDu:"<<QString::number(gTargetPos.lontude,'f',6);
#endif
    *outMuBiaoWeiZhiX = MuBiaoWeiZhiX;
    *outMuBiaoWeiZhiY = MuBiaoWeiZhiY;
    gTargetPos.x= MuBiaoWeiZhiX;
    gTargetPos.y= MuBiaoWeiZhiY;

    return;
}

/*
 * 根据目标大小变换变倍值
 * */
void MainWindow::autoAdjZoomFocal(int roi_width)
{
    int new_lens = 0;
    if(trackerThd->trackingMethod != TRACKING_METHOD_KCF )
    {
        return;
    }
    if(roi_width < 30)
    {
        new_lens = hdLensVal + hdLensVal*0.1;
        gotoNewLen(new_lens);
    }
    else if(roi_width > 150)
    {
        new_lens = hdLensVal - hdLensVal*0.1;
        if(new_lens < 5727) return;
        gotoNewLen(new_lens);
    }

}

int MainWindow::gotoNewLen(int autoZoomVal)
{
    static int post_val = 0;

    if(post_val == autoZoomVal)
        return 0;
    else
        post_val = autoZoomVal;

    if(autoZoomVal < 0) autoZoomVal = 0;
    else if(autoZoomVal > 16384) autoZoomVal = 16384;

    hdCmdsetDirctZoom[4] = (autoZoomVal>>12)& 0xf;
    hdCmdsetDirctZoom[5] = (autoZoomVal>>8)& 0xf;
    hdCmdsetDirctZoom[6] = (autoZoomVal>>4)& 0xf;
    hdCmdsetDirctZoom[7] = (autoZoomVal)& 0xf;
    udpSocket_A->write((char *)hdCmdsetDirctZoom ,sizeof(hdCmdsetDirctZoom));
    udpSocket_B->write((char *)hdCmdsetDirctZoom ,sizeof(hdCmdsetDirctZoom));
    return 0;
}

int MainWindow::getFocalFromLens(int ch,int inlensVal,int distance)
{

}

int MainWindow::getZoomFromDistance(int distance)
{
    int retu = 0;
    int n = 3;  //defaut to 15X
    if(distance> 3000) distance = 3000;
    for(int i=0;i<DIST2ZOOMVAL;i++)
    {
        if(distance <= distToZoomValDataQ[i].distance)
        {
            n=i;
            break;
        }
    }
    retu = distToZoomValDataQ[n].zoomVal;

    return retu;
}

/*
 * outRadarPan:输出水平方位
 * outRadarTilt:输出俯仰
 * inDistance: 为目标斜距距离
 * panVal: 输入光电A/B的水平方位
 * tiltVal:输入光电A/B的俯仰
 * AnZhuangX/Y/Z:输入光电A/B的相对于天线基座的位置
 * */
int MainWindow::convToRadarAE(double * outRadarPan,double * outRadarTilt, double inDistance, double panVal,double tiltVal, double AnZhuangX,double AnZhuangY,double AnZhuangZ)
{
    if(inDistance > 5000 || inDistance < 1)
    {
        *outRadarPan = panVal;
        *outRadarTilt = tiltVal;
        return 100;
    }
    double radPanVal = Deg2Rad(panVal);
    double radTiltVal = Deg2Rad(tiltVal);

    // 1. 球坐标转直角坐标
    double horDistance = (inDistance) * cos(radTiltVal);    //求到光电的水平距离
    double vertDistance = (inDistance) * sin(radTiltVal);    //求垂直距离
    //qDebug()<<"horDistance:"<<horDistance <<"vertDistance:"<<vertDistance;

    double MuBiaoWeiZhiX = 0;   //以基座为原点的xyz
    double MuBiaoWeiZhiY = 0;
    double MuBiaoWeiZhiZ = 0;
    double MuBiaoWeiZhiHL = 0;  //到雷达的水平斜距离

    // 2. 考虑安装偏差，转换到天线基座坐标系
    MuBiaoWeiZhiX = horDistance* sin(radPanVal)+AnZhuangX;
    MuBiaoWeiZhiY = horDistance* cos(radPanVal)+AnZhuangY;
    MuBiaoWeiZhiZ = vertDistance + AnZhuangZ;
    MuBiaoWeiZhiHL = qSqrt(MuBiaoWeiZhiX * MuBiaoWeiZhiX + MuBiaoWeiZhiY * MuBiaoWeiZhiY);

    //qDebug()<<"MuBiaoWeiZhiX:"<<MuBiaoWeiZhiX <<"   Y:"<<MuBiaoWeiZhiY <<"   Z:"<<MuBiaoWeiZhiZ <<" MuBiaoWeiZhiZL"<<MuBiaoWeiZhiHL;

    // 3. 重新计算天线坐标系下的方位角和俯仰角------ 处理四象限方位角计算 计算俯仰角
    if(MuBiaoWeiZhiX>=0 && MuBiaoWeiZhiY>=0)
    {
        *outRadarPan = Rad2Deg(atan(MuBiaoWeiZhiX / MuBiaoWeiZhiY));
    }
    else if(MuBiaoWeiZhiX>=0 && MuBiaoWeiZhiY<0 )
    {
        *outRadarPan = 180 + Rad2Deg(atan(MuBiaoWeiZhiX / MuBiaoWeiZhiY)) ;
    }
    else if(MuBiaoWeiZhiX<0 && MuBiaoWeiZhiY<0 )
    {
        *outRadarPan = Rad2Deg(atan(-MuBiaoWeiZhiX / -MuBiaoWeiZhiY)) + 180;
    }
    else if(MuBiaoWeiZhiX<0 && MuBiaoWeiZhiY>=0 )
    {
        *outRadarPan = Rad2Deg(atan(MuBiaoWeiZhiX / MuBiaoWeiZhiY)) + 360;
    }

    *outRadarTilt = Rad2Deg(atan(MuBiaoWeiZhiZ / MuBiaoWeiZhiHL));

    return 0;
}

/*
 * outRadarPan:输出水平方位
 * outRadarTilt:输出俯仰
 * inDistance: 为目标斜距距离
 * panVal: 输入光电A/B的水平方位
 * tiltVal:输入光电A/B的俯仰
 * AnZhuangX/Y/Z:输入光电A/B的相对于天线基座的位置
 *
 * long2 = long1 + d*sinα/[ARC*cos(lat1)*2π/360]
 * lat2 = lat1 +d*cosα/ (ARC *2π/360)
 * */
//#define     JINGDU_UNIT_MI     0.1113
//#define     WEIDU_UNIT_MI     0.1111
#define     EARTH_RA_MI         6378137.0
int MainWindow::convToRadarAE2(double * outRadarPan,double * outRadarTilt, double inDistance, double panVal,double tiltVal, double AnZhuangX,double AnZhuangY,double AnZhuangZ)
{
    double antna_B = pGyroComm->msgFromGyro.B;
    double antna_L = pGyroComm->msgFromGyro.L;
    double antna_H = pGyroComm->msgFromGyro.H;

    double gd_B = 0;
    double gd_L = 0;
    double gd_H = 0;

    double rr = sqrt(AnZhuangX * AnZhuangX + AnZhuangY*AnZhuangY);
    double rad_jiaodu_cheti = atan(AnZhuangX/AnZhuangY);
    double rad_jiaodu_dadi =rad_jiaodu_cheti + Deg2Rad(pGyroComm->msgFromGyro.direction);
    if( rad_jiaodu_dadi> PI)
    {
        rad_jiaodu_dadi = rad_jiaodu_dadi - PI;
    }

    double bb = rr * cos(rad_jiaodu_dadi);  //纬度方向相差距离,单位 米
    double ll = rr * sin(rad_jiaodu_dadi);  //经度方向

    gd_B = antna_B + Rad2Deg(bb /(EARTH_RA_MI * (2* PI/360)));
    gd_L = antna_L + Rad2Deg(ll /(EARTH_RA_MI * (2* PI/360)) * cos(Deg2Rad(antna_B)));
    gd_H = antna_H + AnZhuangZ;


    QVector3D antnaBlh;
    CoordTranser coordTrsr;

    antnaBlh.setX(gd_B);
    antnaBlh.setY(gd_L);
    antnaBlh.setZ(gd_H);
    coordTrsr.Init("WGS84", antnaBlh);

    QVector3D vBlh;

    vBlh = QVector3D((float)antna_B, (float)antna_L, (float)antna_H);
    double x_mb=0,y_mb=0,z_mb=0;
    GuideData out;

    coordTrsr.dd2dx(vBlh.x(),vBlh.y(),vBlh.z(), &x_mb,&y_mb,&z_mb);
    coordTrsr.XYZToAE(x_mb,y_mb,z_mb, coordTrsr.mvBlh.x(),coordTrsr.mvBlh.y(),coordTrsr.mvBlh.z(), out.dblA,out.dblE,out.dblR );

    *outRadarPan = out.dblA;
    *outRadarTilt = out.dblE;
}

void MainWindow::slotHidChanged(int id)
{
    if(1 == id)
    {
        hid= new HidProcThd;
        connect(hid, SIGNAL(sig_UpdateJoystick()), this, SLOT(slotGetUpdateJoystick()));
        hid->start();
        QString msg="摇杆已插入 ";
        dlg.initFrame(msg);
        dlg.setAttribute(Qt::WA_ShowModal, true);
        dlg.startTimer();
        dlg.show();
    }
    else
    {
        hid->quit();
        QString msg="摇杆已拔出 ";
        dlg.initFrame(msg);
        dlg.setAttribute(Qt::WA_ShowModal, true);
        dlg.startTimer();
        dlg.show();
    }
}

void MainWindow::on_pbSendDirection_clicked()
{
    double direction=ui->le_setDirecion->text().toDouble();
    double pitch=ui->le_setPitch->text().toDouble();
    int retu = sendPosition(direction, pitch,true,true);
    if(-1 == retu)
    {
        QMessageBox::about(NULL, "提示", "输入0~360");
    }
    else if(-2 == retu)
    {
        QMessageBox::about(NULL, "提示", "输入-90~90");
    }
}

void MainWindow::on_pbSendPitch_clicked()
{
    double direction=ui->le_setDirecion->text().toDouble();
    double pitch=ui->le_setPitch->text().toDouble();
    int retu = sendPosition(direction, pitch,true,true);
    if(-1 == retu)
    {
        QMessageBox::about(NULL, "提示", "输入0~360");
    }
    else if(-2 == retu)
    {
        QMessageBox::about(NULL, "提示", "输入-90~90");
    }
}


int MainWindow::sendPosition(double direction,double pitch,bool sendA_flag,bool sendB_flag)
{
    static double post_pitch =0, post_direction =0;
    static bool post_sendA_flag, post_sendB_flag;

    if(     post_pitch == pitch &&
            post_direction == direction &&
            post_sendA_flag == sendA_flag &&
            post_sendB_flag == sendB_flag)      //避免重复下发相同命令
    {
        return 0;
    }
    else
    {
        post_pitch = pitch ;
        post_direction = direction ;
        post_sendA_flag = sendA_flag ;
        post_sendB_flag = sendB_flag;
    }
on_pbStop_pressed();
    int v1=0;
    int v2=0;
    double direct_A=direction - dd_diffx_default_A;
    double direct_B=direction - dd_diffx_default_B;
    double pitch_A=pitch - dd_diffy_default_A - mGyroPitch_A;   //叠加上陀螺的俯仰数据
    double pitch_B=pitch - dd_diffy_default_B - mGyroPitch_B;

    if(connectFlag_A != false && sendA_flag)
    {
        if(direct_A <0)
        {
            direct_A = 360 + direct_A;
        }

        v1=(direct_A * ANGLENET2DEG);

        if(v1>=32768)
        {
            v1=-(65535-v1);
        }

        if(pitch_A>=-90 && pitch_A <=90)
        {
            pitch_A=pitch_A+90 ;
            v2=(pitch_A* ANGLENET2DEG);
        }
        else
        {
            return -2;
        }
        v2 =(v2 - 16384);

        S9ToAngle[0]=0xff;
        S9ToAngle[1]=0x6b;
        S9ToAngle[2]= (v1>>8)&0xff;
        S9ToAngle[3]= v1&0xff;
        S9ToAngle[4]= (v2>>8)&0xff;
        S9ToAngle[5]= v2&0xff;
        S9ToAngle[6]=0xff & ( S9ToAngle[1]+ S9ToAngle[2]+ S9ToAngle[3]+ S9ToAngle[4]+ S9ToAngle[5]);
         udpSocket_A->write((char *)S9ToAngle ,sizeof(S9ToAngle));
    }
    if(connectFlag_B != false && sendB_flag)
    {
        if(direct_B <0)
        {
            direct_B = 360 + direct_B;
        }

        v1=(direct_B * ANGLENET2DEG);

        if(v1>=32768)
        {
            v1=-(65535-v1);
        }

        if(pitch_B>=-90 && pitch_B <=90)
        {
            pitch_B=pitch_B+90;
            v2=(pitch_B* ANGLENET2DEG);
        }
        else
        {
            return -2;
        }
        v2 =(v2 - 16384);

        S9ToAngle[0]=0xff;
        S9ToAngle[1]=0x6b;
        S9ToAngle[2]= (v1>>8)&0xff;
        S9ToAngle[3]= v1&0xff;
        S9ToAngle[4]= (v2>>8)&0xff;
        S9ToAngle[5]= v2&0xff;
        S9ToAngle[6]=0xff & ( S9ToAngle[1]+ S9ToAngle[2]+ S9ToAngle[3]+ S9ToAngle[4]+ S9ToAngle[5]);
         udpSocket_B->write((char *)S9ToAngle ,sizeof(S9ToAngle));
    }
    //qDebug()<<("goto angle=")<<direction<<" "<<pitch;
    return 0;
}

/* 指控引导时使用,把偏移转换成角速度
 *
 */
int MainWindow::sendPosition3(double direction,double pitch,double ptzdirect,double ptzpitch,int interval, QUdpSocket * udpskt)
{
    if(1 != pnetworkComm->msgFromZhiKong.cmdId) return 0;

    double panGap=0;
    double tiltGap = 0;
    short pan_outVal=0;
    short tilt_outVal=0;
    panGap = direction - ptzdirect;

    if(panGap>180)
    {
        panGap = panGap-360;
    }
    else if(panGap<-180)
    {
        panGap = 360 + panGap;
    }

    tiltGap = pitch - ptzpitch;

    if(panGap > 10 || panGap< -10 || tiltGap >10 || tiltGap < -10)  //大于一定角度直接调用转到角度接口
    {
        sendPosition(direction,pitch,true,true);
    }
    else
    {

        pan_outVal = (panGap / 0.02 ) * interval;    //1LSB=0.002度/秒
        tilt_outVal = (tiltGap / 0.02 ) * interval;

        if(pan_outVal>32768) pan_outVal = 32768;
        else if(pan_outVal<-32768) pan_outVal = -32768;

        if(tilt_outVal>32768) tilt_outVal = 32768;
        else if(tilt_outVal<-32768) tilt_outVal = -32768;

        uint8_t * cmd = mPodCmd.crtSteplessCmd(pan_outVal, tilt_outVal);
        udpskt->write((char *)cmd,7);
    }

    return 0;
}


int MainWindow::sendPTZStepless(short direction,short pitch,int hdFocal,int hdZoom,int irFocal,int irZoom)
{
    char ch1H=0;
    char ch1L=0;
    char ch2H=0;
    char ch2L=0;

    ch1H = (direction>>8) & 0xff;
    ch1L = direction & 0xff;
    ch2H = (pitch>>8) & 0xff;
    ch2L = pitch & 0xff;
    sendSetCmd(PKT_TYPE_PTZPANTILTFOCZOOM,ch1H,ch1L,ch2H,ch2L,hdFocal,hdZoom,irFocal,irZoom);
    return 0;
}

void MainWindow::on_rb_trackStick_clicked()
{
    //trackerThd->trackingMethod = TRACKING_METHOD_NONE;
    pnetworkComm->msgToZhiKong.status = 0;
    operateMode = OPMODE_STICK;
//    sendSetCmd(PKT_TYPE_PTZSTOP,0);
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    };

    //pTheo_PP_DLL->stopLead()
    //ui->gbx_RoiSize->setEnabled(false);
    ui->lblPad->setCursor(Qt::ArrowCursor);
/*
    if(1 == btnStatus)
    {
        btnStatus = 2;
        sendCmd();
    }*/
    on_rb_HGyroUnLock_clicked();
    ui->rb_HGyroUnLock->setChecked(true);
}

void MainWindow::on_rb_trackTable_clicked()
{/*
    operateMode=OPMODE_THEO;
    dd_diffx=0;
    dd_diffy=0;*/
    //pTheo_PP_DLL->startLead();
    //ui->checkBoxTheoGuid->setEnabled(true);
    //ui->checkBoxCenterGuid->setEnabled(false);
    //ui->checkBoxTheoGuid->setChecked(false);
    //ui->checkBoxCenterGuid->setChecked(false);
}

void MainWindow::on_rb_toZero_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9ToZero ,sizeof(S9ToZero));

    operateMode=OPMODE_TOZERO;

    //on_rb_HGyroUnLock_clicked();
    //ui->rb_HGyroUnLock->setChecked(true);
}

void MainWindow::on_rb_Tracking_clicked()
{
    operateMode = OPMODE_MARKING;
    ui->lblPad->setCursor(Qt::ArrowCursor);
    on_rb_HGyroUnLock_clicked();
    ui->rb_HGyroUnLock->setChecked(true);
}

void MainWindow::on_rb_Center_clicked()
{
    pnetworkComm->msgToZhiKong.status = 1;
    operateMode = OPMODE_CENTER;
}

void MainWindow::on_rb_KcfMarking_clicked()
{qDebug()<<"on_rb_KcfMarking_clicked";
    operateMode = OPMODE_MARKING;
    //trackerThd->trackingMethod = TRACKING_METHOD_KCF;
    //ui->gbx_RoiSize->setEnabled(true);
    ui->lblPad->setCursor(Qt::ArrowCursor);
/*    if(1 == btnStatus)
    {
        btnStatus = 2;
        sendCmd();
    }*/
}

void MainWindow::on_rb_TopHatMarking_clicked()
{
    //trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
    operateMode = OPMODE_MARKING;
    //ui->gbx_RoiSize->setEnabled(false);
    ui->lblPad->setCursor(Qt::ArrowCursor);
    btnStatus = 0;
    //trackerThd->bTrackerStart = true;
}

void MainWindow::on_rb_WhCentroidMarking_clicked()
{
    //trackerThd->trackingMethod = TRACKING_METHOD_CENTROIDWH;
    operateMode = OPMODE_MARKING;
    //ui->gbx_RoiSize->setEnabled(false);
    ui->lblPad->setCursor(Qt::ArrowCursor);
    btnStatus = 0;
    //trackerThd->bTrackerStart = true;
}

void MainWindow::on_rb_BlkCentroidMarking_clicked()
{
    //trackerThd->trackingMethod = TRACKING_METHOD_CENTROIDBLK;
    operateMode = OPMODE_MARKING;
    //ui->gbx_RoiSize->setEnabled(false);
    ui->lblPad->setCursor(Qt::ArrowCursor);
    btnStatus = 0;
    //trackerThd->bTrackerStart = true;
}

void MainWindow::on_leOsdWord_textChanged(const QString &arg1)
{
    ui->pbOsdWordSave->setEnabled(true);
}

void MainWindow::on_pbOsdWordSave_clicked()
{
#if 0
    writeCfgFile();
    fullscreen.OSDText=ui->leOsdWord->text();
    fullscreen.OSDFont=ui->cbxOSDFont->currentText();
    fullscreen.OSDFontHeight=ui->cbxOSDFontSize->currentText().toInt() ;
    fullscreen.OSDLocationY=ui->leOSDLocationY->text().toInt();
#endif
}

bool MainWindow::openSerialPort()
{
    //QString commServo=mServoCom;
    QString commServo="COM3";
/*
    //打开串口
    QList<QSerialPortInfo> listComms;//获取COM口列表
    listComms=QSerialPortInfo::availablePorts();
    if(0==listComms.size())
    {
        titleLabel->setText("未发现串口！不能接收数据交互！");
        qDebug()<<("未发现串口！不能接收数据交互 ");
        return false;
    }
    else
    {
        //ui->txtAppMsg->appendPlainText("串口配置：");
        int nCentrComIdx=-1;
        for(int i=0; i<listComms.size(); i++)
        {
            ui->comboCenterCom->addItem(listComms.at(i).portName());
            if (commServo==listComms.at(i).portName())
                nCentrComIdx=i;
        }
        if (nCentrComIdx==-1)//没找到，则用现有的第一个串口
        {
            if (listComms.size()>0)
            {
                commServo=listComms.at(0).portName();
                ui->comboCenterCom->setCurrentIndex(0);
            }
            else
            {
                titleLabel->setText("未找到可用串口");
                return false;
            }
        }
        else
        {
            ui->comboCenterCom->setCurrentIndex(nCentrComIdx);
        }
    } */

    if (!commServo.isEmpty())
    {
        SerialPort.setPortName((const QString &)commServo);
        SerialPort.setBaudRate(QSerialPort::Baud115200);//设置串口波特率（115200）
        SerialPort.setDataBits(QSerialPort::Data8);//设置数据位（8）
        SerialPort.setParity(QSerialPort::NoParity); //设置奇偶校检（无）
        SerialPort.setStopBits(QSerialPort::OneStop);//设置停止位(一位)
        SerialPort.setFlowControl(QSerialPort::NoFlowControl);//设置流控制（无）
        mnBufIdx=0;
        mbCenterAsk=false;

        //connect(&this->SerialPort,SIGNAL(readyRead()),this,SLOT(recvComData()));
        if(SerialPort.open(QIODevice::WriteOnly))   //打开串口，并设置串口为只读模式
        {
            mpSendBuf=new char[sizeof(CommFrame)];
            ui->te_Output->append(QString("  %1 => 伺服 √").arg(commServo));
            pnetworkComm->servo422ConnectFlag = true;
        }
        else
        {
            ui->te_Output->append(QString("  %1 => 伺服 ×").arg(commServo));
            pnetworkComm->servo422ConnectFlag = false;
            return false;
        }
    }

    return true;
}

void MainWindow::closeSerialPort()
{
    SerialPort.close();
}


int BCDToInt(unsigned char value)
{
    int temp = 0;
    temp = (value>>4)*10;
    temp += value&0x0f;
    return temp;
}

#if 0
int MainWindow::recvComData()
{
   int len=0;
    QByteArray ba;
    //SerialPort.readBufferSize();
    ba = SerialPort.readAll();

    len = ba.length();
    //qDebug()<<"len:"<<len<<"    "<<ba;

/*    printf("%02x %02x %02x %02x	 %02x %02x %02x %02x %02x \
                 %02x %02x %02x  %02x %02x %02x %02x\n", (ba.at(0)) & 0xff, (ba.at(1)) & 0xff, (ba.at(2)) & 0xff, (ba.at(3)) & 0xff,
           (ba.at(4)) & 0xff, (ba.at(5)) & 0xff, (ba.at(6)) & 0xff, (ba.at(7)) & 0xff,(ba.at(8)) & 0xff,
           (ba.at(9)) & 0xff, (ba.at(10)) & 0xff, (ba.at(11)) & 0xff, (ba.at(12)) & 0xff,(ba.at(13)) & 0xff,
           (ba.at(14)) & 0xff, (ba.at(15)) & 0xff);
    std::cout << std::endl;
*/
    if(len == 16 && ((ba.at(0)&0xff) == 0x7e && ((ba.at(15)&0xff) == 0xe7)))
    {

        int h=BCDToInt((ba.at(4)) & 0xff);
        int m=BCDToInt((ba.at(5)) & 0xff);
        int s=BCDToInt((ba.at(6)) & 0xff);
        int ms=((ba.at(7)<<8)+(ba.at(8) & 0xff))*2;
        //qDebug()<<"time "<<h<<":"<<m<<":"<<s<<":"<<ms;
        ui->lbl_BjTime->setText(QString::number(h,10)+":"+QString::number(m,10)+":"+QString::number(s,10)+"."+QString::number(ms,10));
        CurTime.setHMS(h,m,s,ms);
    }

    return len;
}
#endif

int MainWindow::recvComData()
{
#if 1
    if( connectFlag_A == false || connectFlag_B == false) return 0;

    mzRecvBuf.append(SerialPort.readAll());
    int nLen=mzRecvBuf.length();

//	QString sTmp;
//	for (int i=0; i<nLen; ++i)
//		sTmp.append(QString(" %1").arg(uchar(mzRecvBuf.at(i)), 2, 16, QChar('0')));
//	ui->lblInfo->setText(QString::number(nLen) +":" + sTmp);//Debug

    if (nLen<(int)sizeof(CommFrame))
        return -1;
    bool bSucEver=false;//在下面的解析循环中是否成功过
    while (mnBufIdx<nLen)
    {
        if ((mzRecvBuf.at(mnBufIdx)&0X00FF)!=DATA_HEAD)//找Head
        {
            ++mnBufIdx;
            continue;
        }
        if (nLen-mnBufIdx<(int)sizeof(CommFrame))//剩余数据长度不够，则等下一次进入再处理
            break;
        bool bSucOnce=ParseCommData(mnBufIdx);
        bSucEver|=bSucOnce;
        if (bSucOnce)//解析成功了，则继续往下解析，以防有数据堆积
            mnBufIdx+=sizeof(CommFrame);
        else
            ++mnBufIdx;
    }
    mzRecvBuf.remove(0, mnBufIdx);//解析过的数据，不管是否成功，都没用了
    mnBufIdx=0;

    if (!bSucEver)//只要有一次成功，则mGuideData就是有效数据；如果多次成功，则mGuideData是最后一次数据，也就是最新数据
        return 0;
    /*if(operateMode == OPMODE_CENTER )
    {
        double angleA, angleE;
        if (ui->rb_StickSpeedH->isChecked())
        {
            angleA = mCenterGuide.dblA + mAEJoyCorrect.dblA*4;//中心引导修正8°
            angleE = mCenterGuide.dblE + mAEJoyCorrect.dblE*4;
        }
        else if (ui->rb_StickSpeedM->isChecked())
        {
            angleA = mCenterGuide.dblA + mAEJoyCorrect.dblA*2;//中心引导修正4°
            angleE = mCenterGuide.dblE + mAEJoyCorrect.dblE*2;
        }
        else
        {
            angleA = mCenterGuide.dblA;//中心引导
            angleE = mCenterGuide.dblE;
        }

        if(angleA >=360 ) angleA-=360;
        else if(angleA <0) angleA+=360;
        //mPtzCam.SetAEPos(angleA, angleE);//发到经纬仪
        sendPosition2(angleA, angleE);//发到经纬仪
    }

    QTime localSystime = QTime::currentTime();
    if (mbCenterAsk)
    {
        CommSendToCenterRAE(mcSID, localSystime, ui->checkBoxCaught->isChecked(), mCenterGuide.dblR, ptzDirection, ptzPitch);
        mbCenterAsk=false;
    }

    //以下为显示中心数据、实际-中心、中心-理论
    ui->lblCenterT->setText(mCenterGuide.tTime.toString("hh:mm:ss.zzz"));
    ui->lblCenterR->setText(QString::number(mCenterGuide.dblR/1000.0, 'f', 3)+"km");
    ui->lblCenterA->setText(QString::number(mCenterGuide.dblA, 'f', 3));
    ui->lblCenterE->setText(QString::number(mCenterGuide.dblE, 'f', 3));
    ui->lblRealCenterA->setText(QString::number(ptzDirection-mCenterGuide.dblA, 'f', 3));
    ui->lblRealCenterE->setText(QString::number(ptzPitch-mCenterGuide.dblE, 'f', 3));*/
#if 0
    if(pTheo_PP_DLL->m_bLoadFile_Valid)//解决打开加载理论弹道对话框时显示超大数值问题
    {
        SRC_DATA dataSrc;
        RET_DATA dataRet;
        dataSrc.T0 = QTime(0,0,0);//中心会下发，按实际赋值
        //dataSrc.time = localSystime;//mCenterGuide.tTime;	//改为取系统时间
        dataSrc.T0_Valid = false;//中心停止发送T0后，置为fasle  正在发送置为true  中心会连续发送10s
        dataSrc.iCurMcLeadMode = 0;//默认填0
        pTheo_PP_DLL->GetResult(dataSrc, dataRet);
        //ui->lblCenterTheoA->setText(QString::number(mCenterGuide.dblA-dataRet.A, 'f', 3));
        //ui->lblCenterTheoE->setText(QString::number(mCenterGuide.dblE-dataRet.E, 'f', 3));
    }
#endif
    //ui->lblCenterDelay->setText(QString::number(mCenterGuide.tTime.msecsTo(localSystime))+"ms");//中心时延
}

bool MainWindow::ParseCommData(int nIdx)
{
    if (nIdx+(int)sizeof(CommFrame)>mzRecvBuf.length())
        return false;
    if ((mzRecvBuf.at(nIdx+0)&0XFF)!=DATA_HEAD)
        return false;
    CommFrame * pFrame=(CommFrame *)mzRecvBuf.data();
    ushort nCrc=pFrame->nCRC;
    pFrame->nCRC=0;
    for (int i=0; i<(int)sizeof(CommFrame); i++)
        nCrc-=(uchar)mzRecvBuf.at(nIdx+i);
    if (nCrc!=0)
        return false;

    if (pFrame->cType==DATATYPE_T0)
    {
        mCenterGuide.tTime=QTime::fromMSecsSinceStartOfDay(pFrame->nTime);//T, ms
#if 0
        pTheo_PP_DLL->setT0_Center(QTime::fromMSecsSinceStartOfDay(pFrame->data.dataT0.nT0));
#endif
    }
    else if (pFrame->cType==DATATYPE_RAE)
    {
        if (pFrame->cDID!=mcDevID)//RAE只能发给指定设备
            return false;
        mCenterGuide.tTime=QTime::fromMSecsSinceStartOfDay(pFrame->nTime);//T, ms
        mCenterGuide.dblR=(double)pFrame->data.dataRAE.nR/10.0;//R, m
        mCenterGuide.dblA=(double)pFrame->data.dataRAE.nA/36000.0;//A, 0.1"
        mCenterGuide.dblE=(double)pFrame->data.dataRAE.nE/36000.0;
    }
    else if (pFrame->cType==DATATYPE_XYZ)
    {
        mCenterGuide.tTime=QTime::fromMSecsSinceStartOfDay(pFrame->nTime);//T, ms
        QVector3D vXyz = QVector3D((double)pFrame->data.dataXYZ.nX/10.0, //0.1m
                                 (double)pFrame->data.dataXYZ.nY/10.0,
                                 (double)pFrame->data.dataXYZ.nZ/10.0);
        QVector3D vRAE = mCoordTrsr.XyzEcef2Rae(vXyz);
        mCenterGuide.dblR=vRAE.x();
        mCenterGuide.dblA=vRAE.y();
        mCenterGuide.dblE=vRAE.z();
    }
    else if (pFrame->cType==DATATYPE_ASK)//状态请求时没有数据
    {
        if (pFrame->cLoopNo==mcDevID)
        {
            mCenterGuide.tTime=QTime::fromMSecsSinceStartOfDay(pFrame->nTime);//T, ms
            mbCenterAsk=true;
            mcSID=pFrame->cSID;
            return true;
        }
        else
            return false;
    }
    else
        return false;
    if (pFrame->cLoopNo==mcDevID)
    {
        mbCenterAsk=true;
        mcSID=pFrame->cSID;
    }
#endif
    if (mCenterGuide.dblA>=360)
        mCenterGuide.dblA-=360;
    if (mCenterGuide.dblE>=190)
        mCenterGuide.dblE=180-mCenterGuide.dblE;
    return true;
}

#if 0
//dblR m
//dblA °
//dblE °
void MainWindow::CommSendToCenterRAE(uchar cDID, const QTime &tTime, bool bCaught, double dblR, double dblA, double dblE)
{
    if (mpSendBuf==nullptr)
        return;
    CommFrame * pFrame=(CommFrame*)mpSendBuf;
    pFrame->cHead=DATA_HEAD;
    pFrame->cSID=mcDevID;
    pFrame->cDID=cDID;
    pFrame->cType=DATATYPE_RAE;
    pFrame->cValid=bCaught;
    ++pFrame->cLoopNo;
    pFrame->nCRC=0;
    pFrame->nTime=(uint32_t)tTime.msecsSinceStartOfDay();

    pFrame->data.dataRAE.nR=(uint32_t)(dblR*10+0.5);//*10为m->0.1m，+0.5为四舍五入

    if (dblA>=360)
        dblA-=360;
    else if (dblA<0)
        dblA+=360;
    pFrame->data.dataRAE.nA=(uint32_t)(dblA*36000.0+0.5);

    if (dblE>=190)
        dblE=180-dblE;
    if (dblE>=0)
        pFrame->data.dataRAE.nE=(int32_t)(dblE*36000.0+0.5);
    else
        pFrame->data.dataRAE.nE=(int32_t)(dblE*36000.0-0.5);

    ushort nCrc=0;
    for (uint i=0; i<sizeof(CommFrame); i++)
        nCrc+=(uchar)mpSendBuf[i];
    pFrame->nCRC=nCrc;

    SerialPort.write(mpSendBuf, sizeof(CommFrame));
    //ui->lblInfo->setText(QString::number(uint(pFrame->cLoopNo)));
    //titleLabel->setText(QString::number(uint(pFrame->cLoopNo)));
    //ui->lblToCenterComLoop->setText(QString::number(uint(pFrame->cLoopNo)));
}
#endif

int MainWindow::sendServoComm(double dblGd1A, double dblGd1E,double dblGd2A, double dblGd2E,int coverflag)
{
    if(pnetworkComm->servo422ConnectFlag == false)  return 100;

    //unsigned int loopno = pnetworkComm->msgToServo422.loopNo;

    pnetworkComm->msgToServo422.loopNo++;
    pnetworkComm->msgToServo422.gd1FangWei.fData = dblGd1A;
    pnetworkComm->msgToServo422.gd1FuYang.fData = dblGd1E;
    pnetworkComm->msgToServo422.gd2FangWei.fData = dblGd1A;
    pnetworkComm->msgToServo422.gd2FuYang.fData = dblGd1E;


    /*pnetworkComm->msgToServo422.gd1FangWei.fData = 123.12; //0x01020304;
    pnetworkComm->msgToServo422.gd1FuYang.fData = 23.23;    //0x0a0b0c0d;
    pnetworkComm->msgToServo422.gd2FangWei.fData = 123.12; //0x01020304;
    pnetworkComm->msgToServo422.gd2FuYang.fData = 56.56;  //0x0a0b0c0d;*/
    pnetworkComm->convComm422PktToByte();
    pnetworkComm->sendServoPktCnt++;
    SerialPort.write((char *)pnetworkComm->pktToServo422, sizeof(pnetworkComm->pktToServo422));
    return 0;
}

void MainWindow::on_rb_StickSpeedH_clicked()
{
    speedGear=2;
}

void MainWindow::on_rb_StickSpeedM_clicked()
{
    speedGear=1;
}

void MainWindow::on_rb_StickSpeedL_clicked()
{
    speedGear=0;
}

#if 0
void MainWindow::on_pbSSetDiffY_clicked()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("调整补偿"),
        QString(tr("警告：俯仰补偿将会修改，是否继续?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes) {
        //dd_diffy_default=ui->le_setPitchDiff->text().toDouble();
        dd_diffy = dd_diffy_default_A;
        writeCfgFile();
    }
    else
    {
        char str[64];
        sprintf(str,"%.3f",dd_diffy);
        //ui->le_setPitchDiff->setText(str);
    }
}
#endif

#if 0
void MainWindow::on_pbResetDiffY_clicked()
{

    dd_diffy = dd_diffy_default;
    char str[64];
    sprintf(str,"%.3f",dd_diffy);
    //ui->le_setPitchDiff->setText(str);

    dd_diffx=0;

}
#endif

void MainWindow::timerRecFileSeg_timeOut()
{
    // 每3分钟分段录像
    if( connectFlag_A == false ||connectFlag_B == false ) return;

    if(mPlayer->recflag == true)
    {

            mPlayer->mFileSegTimeOut  = true;
            mPlayerSub->mFileSegTimeOut  = true;
            qDebug()<<"==============================>HD create new file";
            qDebug()<<"==============================>IR create new file";
    }
}

void MainWindow::configCAM(int camid,int cmdid,int val1,int val2)
{
    unsigned char cmd[8];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    cmd[0]=PKT_HEADER;
    cmd[1]=PKT_TYPE_CAMCFG;
    cmd[2]= ((camid & 0x1) << 6) + cmdid;
    cmd[3]= val1;
    cmd[4]= val2;
    cmd[5]= 0;
    cmd[6]= 0;
    cmd[7]= 0xff & (cmd[0] + cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]+ cmd[6]);
    udpskt->write((char *)cmd,8);
}
void MainWindow::on_lecSID_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leCSidChanged = true;
}

void MainWindow::on_leCenterCom_textChanged(const QString &arg1)
{
    ui->pbConfigSave->setEnabled(true);
    leSetCenterComChanged= true;
}

void MainWindow::on_sldVidEnhance_valueChanged(int value)
{
    //ui->lbVidEnhanceVal->setText(QString::number(value));
}

void MainWindow::on_sldVidBright_valueChanged(int value)
{
    //ui->lbVidBrightVal->setText(QString::number(value));
}

void MainWindow::on_cbxDispOSDTime_clicked(bool checked)
{
    fullscreen.mdispOSDTime = checked;
    qDebug()<<"mdispOSDTime"<<checked;
}

bool MainWindow::EnSetSysTimePrivilege()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkPriv;
    ZeroMemory(&tkPriv, sizeof(TOKEN_PRIVILEGES));
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        CloseHandle(hToken);
        return false;
    }
    if (!LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tkPriv.Privileges[0].Luid))
    {
        CloseHandle(hToken);
        return false;
    }
    tkPriv.PrivilegeCount=1;
    tkPriv.Privileges[0].Attributes|=SE_PRIVILEGE_ENABLED;
    bool bSuc=AdjustTokenPrivileges(hToken, FALSE, &tkPriv, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    CloseHandle(hToken);
    return bool(bSuc);
}

void MainWindow::on_cbxOSDFont_currentIndexChanged(const QString &arg1)
{
    ui->pbOsdWordSave->setEnabled(true);
}

void MainWindow::on_cbxOSDFontHeight_currentIndexChanged(const QString &arg1)
{
    ui->pbOsdWordSave->setEnabled(true);
}

void MainWindow::on_leOSDLocationY_textChanged(const QString &arg1)
{
    ui->pbOsdWordSave->setEnabled(true);
}

void MainWindow::on_rb_trackRoi40_clicked()
{
    trackerThd->roi_width = 80;
    trackerThd->roi_height = 80;
}

void MainWindow::on_rb_trackRoi20_clicked()
{
    trackerThd->roi_width = 40;
    trackerThd->roi_height = 40;
}

void MainWindow::on_rb_trackRoi10_clicked()
{
    trackerThd->roi_width = 20;
    trackerThd->roi_height = 20;
}



void MainWindow::on_cbxFullScreen_clicked()
{
    if(ui->cbxFullScreen->isChecked() == true)
    {
        fullscreen.show();
    }
    else
        fullscreen.close();
}
void MainWindow::on_pb_OpenSelfTest_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)ptzCmdTiltSelfTestEnable,sizeof(ptzCmdTiltSelfTestEnable));
    QThread::msleep(30);
}

void MainWindow::on_pb_CloseSelfTest_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)ptzCmdTiltSelfTestDisable,sizeof(ptzCmdTiltSelfTestDisable));
    QThread::msleep(30);
}

void MainWindow::on_vsld_speed_valueChanged(int value)
{
    pelco_spd_val = value;
}


void MainWindow::on_pbRight_Long_pressed()
{
    mRightBtn = 1;
}

void MainWindow::on_pbRight_Long_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    mRightBtn = 0;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    qDebug()<<"on_pbRight_released";

}

void MainWindow::on_pbLeft_Long_pressed()
{
    mLeftBtn = 1;
}

void MainWindow::on_pbLeft_Long_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    mLeftBtn = 0;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    udpskt->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
}

void MainWindow::sendStepless_timeOut()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    if( 1 == mLeftBtn)
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(-(pelco_spd_val* ANGLENET2DEG),0);
        udpskt->write((char *)cmd,7);

        QByteArray mBuf;
        mBuf.resize(7);
        memcpy(mBuf.data(),cmd, 7);
        qDebug()<<"left:"<<mBuf.toHex(' ');
    }
    else if( 1 == mRightBtn)
    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd((pelco_spd_val* ANGLENET2DEG),0);
        udpskt->write((char *)cmd,7);

        QByteArray mBuf;
        mBuf.resize(7);
        memcpy(mBuf.data(),cmd, 7);
        qDebug()<<"right:"<<mBuf.toHex(' ');
    }

}

void MainWindow::chkIfOffline_timeOut()
{
    static int recvPktNum_last_A= -1;
    static int recvPktNum_last_B= -1;
    static int recvRtspNum_last_A= -1;
    static int recvRtspNum_last_B= -1;
    static int recvGyroPktNum_last= -1;

    if(mPlayer->num_frame == recvRtspNum_last_A )
    {
        ui->te_Output->append(" 光电A视频中断");
        getRtspStreamA();
    }
    recvRtspNum_last_A = mPlayer->num_frame;

    if(recvPktNum_A == recvPktNum_last_A )
    {
        ui->te_Output->append(" 光电A断开");
        connectPtzAUc();
    }
    recvPktNum_last_A = recvPktNum_A;


    if(mPlayerSub->num_frame == recvRtspNum_last_B )
    {
        ui->te_Output->append(" 光电B视频中断");
        getRtspStreamB();
    }
    recvRtspNum_last_B = mPlayerSub->num_frame;

    if(recvPktNum_B == recvPktNum_last_B )
    {
        ui->te_Output->append(" 光电B断开");
        connectPtzBUc();
    }
    recvPktNum_last_B = recvPktNum_B;

    if(pGyroComm->recvPacketCnt == recvGyroPktNum_last )
    {
        ui->te_Output->append(" 惯导断开");
        connectGyroBc();
    }
    recvGyroPktNum_last = pGyroComm->recvPacketCnt;

    //autoAdjZoomFocal(roiBox.width); //在跟踪时自动变焦调整视场角
    return;
}

void MainWindow::zhiKongSocket_timeOut()
{
    chkCrossBorder();
    pnetworkComm->sendMsg();
    ui->lbSendZhiKongPkt->setText(QString::number(pnetworkComm->sendZhiKongPktCnt));

    double gd1ToRadarA=0;
    double gd1ToRadarE=0;
    double gd2ToRadarA=0;
    double gd2ToRadarE=0;
    double distance = 0;
    double direction;
    double pitch ;


    distance = pnetworkComm->msgFromZhiKong.zkDistence;
    if(ui->cbxSetDbgDistance->isChecked())  //调试或者校标用
    {
        distance = ui->leSetDbgDistance->text().toDouble();
        qDebug()<<"distance:"<<distance<<"  mGD1MountPosX:"<<mGD1MountPosX <<"  Y:"<<mGD1MountPosY;
    }
    if(distance < 5000 && distance > 1) // && pnetworkComm->msgFromZhiKong.cmdId == 1)   //有引导距离数据时,计算夹角
    {
        direction =  mPtzDirection_A;//pnetworkComm->msgToZhiKong.gd1FangWei;
        pitch =  mPtzPitch_A;//pnetworkComm->msgToZhiKong.gd1FuYang;
        convToRadarAE(&gd1ToRadarA,&gd1ToRadarE,distance,direction,pitch, mGD1MountPosX,mGD1MountPosY,mGD1MountPosZ);

        direction =  mPtzDirection_B;   //pnetworkComm->msgToZhiKong.gd2FangWei;
        pitch =  mPtzPitch_B;   //pnetworkComm->msgToZhiKong.gd2FuYang;
        convToRadarAE(&gd2ToRadarA,&gd2ToRadarE,distance,direction,pitch, mGD2MountPosX,mGD2MountPosY,mGD2MountPosZ);
    }
    else    //距离参数无效，直接用光电AE值
    {
        gd1ToRadarA = mPtzDirection_A;
        gd1ToRadarE = mPtzPitch_A;
        gd2ToRadarA = mPtzDirection_B;
        gd2ToRadarE = mPtzPitch_B;
    }
//gd1ToRadarE = gd1ToRadarE -1;
    // 通过串口发送给天线伺服系统
    sendServoComm(gd1ToRadarA,gd1ToRadarE, gd2ToRadarA,gd2ToRadarE,0);

    ui->lbGDASendServoA->setText( QString::number(gd1ToRadarA, 'f',7)+"°" );
    ui->lbGDASendServoE->setText( QString::number(gd1ToRadarE, 'f',7)+"°" );
    ui->lbGDBSendServoA->setText( QString::number(gd2ToRadarA, 'f',7)+"°" );
    ui->lbGDBSendServoE->setText( QString::number(gd2ToRadarE, 'f',7)+"°" );
    ui->lbSendServoPkt->setText(QString::number(pnetworkComm->sendServoPktCnt));

    if(operateMode == OPMODE_STICK)
    {
        if(ui->cbxLianDongFlag->isChecked())
        {
            double dtX = mPtzDirection_A - mPtzDirection_B;
            double dtY = mPtzPitch_A - mPtzPitch_B;
            if(abs(dtX) > 0.5 || abs(dtY) > 0.5) //大于0.5°才两台光电球开始联动
            {
                if(ui->rb_selGDA->isChecked())
                {
                    sendPosition(mPtzDirection_A, mPtzPitch_A,false,true);
                }
                else
                {
                    sendPosition(mPtzDirection_B, mPtzPitch_B,true,false);

                }
            }
        }
    }

    return;
}


void MainWindow::delayPid_timeOut()
{qDebug()<<"delayPid_timeOut enable_I_flag=true";
    timerDelayPid->stop();
    PidX.enable_I_flag = true;
    PidY.enable_I_flag = true;
}

void MainWindow::on_pClearbDbgOut_clicked()
{
    ui->te_Output->clear();
}

void MainWindow::on_pbApUp_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)hdCmdApUp,sizeof(hdCmdApUp));
}

void MainWindow::on_pbApDown_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdApDown,sizeof(hdCmdApDown));
}

void MainWindow::on_pbApReset_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdApReset,sizeof(hdCmdApReset));
}

void MainWindow::on_pbShutterUp_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdShutterUp,sizeof(hdCmdShutterUp));
}

void MainWindow::on_pbApShutterDown_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdShutterDown,sizeof(hdCmdShutterDown));
}

void MainWindow::on_pbShutterReset_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdShutterReset,sizeof(hdCmdShutterReset));
}

void MainWindow::on_spb_Gamma_valueChanged(int arg1)
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    hdGamma[4] = ui->spb_Gamma->value() & 0xff;
    udpskt->write((char *)hdGamma,sizeof(hdGamma));
}

void MainWindow::on_pbSharpenUp_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdSharpenUp,sizeof(hdCmdSharpenUp));
}

void MainWindow::on_pbSharpenDown_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdSharpenDown,sizeof(hdCmdSharpenDown));
}

void MainWindow::on_pbSharpenReset_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdCmdSharpenReset,sizeof(hdCmdSharpenReset));
}

void MainWindow::on_pbDbgSend_clicked()
{
    uint8_t blkCmd[100];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray array = QByteArray::fromHex(ui->le_dbgSend->text().toLatin1());

    //qDebug()<<" cmd:"<<"  barr:"<<array.toHex(' ')<<"   lenth:"<<array.length();
    for(int i=0;i<array.length();i++)
    {
        blkCmd[i] = array.data()[i];
    }
    udpskt->write((char *)blkCmd,array.length());
}

void MainWindow::on_rb_ExpoAuto_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdAEAuto ,sizeof(hdAEAuto));
    ui->gbx_Expo->setEnabled(true);
}

void MainWindow::on_rb_ExpoManual_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdAEManual ,sizeof(hdAEManual));
    ui->gbx_Expo->setEnabled(false);
}

void MainWindow::on_rb_ExpoShutter_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdAEShutter ,sizeof(hdAEShutter));
    ui->gbx_Expo->setEnabled(true);
}

void MainWindow::on_rb_ExpoIris_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdAEIris ,sizeof(hdAEIris));
    ui->gbx_Expo->setEnabled(true);
}

void MainWindow::on_pbExpUp_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdExpoCompAdd ,sizeof(hdExpoCompAdd));
    //udpSocket->write((char *)hdGainAdd ,sizeof(hdGainAdd));
}

void MainWindow::on_pbExpDown_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdExpoCompDec ,sizeof(hdExpoCompDec));
    //udpSocket->write((char *)hdGainDec ,sizeof(hdGainDec));
}

void MainWindow::on_pbExpReset_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdExpoCompReset ,sizeof(hdExpoCompReset));
    //udpSocket->write((char *)hdGainReset ,sizeof(hdGainReset));
}

void MainWindow::on_rb_flipPicOff_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdPicFlipOff ,sizeof(hdPicFlipOff));
}

void MainWindow::on_rb_flipPicOn_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)hdPicFlipOn ,sizeof(hdPicFlipOn));
}

void MainWindow::on_rb_HGyroLock_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)HGyroUnLock ,sizeof(HGyroUnLock));
}

void MainWindow::on_rb_HGyroUnLock_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)HGyroLock ,sizeof(HGyroLock));
}

void MainWindow::on_rb_VGyroLock_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)VGyroLock ,sizeof(VGyroLock));
}

void MainWindow::on_rb_VGyroUnLock_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)VGyroUnLock ,sizeof(VGyroUnLock));
}

void MainWindow::on_cbx_WbMode_currentIndexChanged(int index)
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    int idx = ui->cbx_WbMode->currentIndex();

    switch(idx)
    {
        case 0:
                udpskt->write((char *)hdWBOutdoor ,sizeof(hdWBOutdoor));
                break;
        case 1:
                udpskt->write((char *)hdWBIndoor ,sizeof(hdWBIndoor));
            break;
        case 2:
                udpskt->write((char *)hdWBATW ,sizeof(hdWBATW));
                break;
        case 3:
                udpskt->write((char *)hdWBManual ,sizeof(hdWBManual));
                break;
        case 4:
                udpskt->write((char *)hdWBOutAuto ,sizeof(hdWBOutAuto));
                break;
        case 5:
                udpskt->write((char *)hdWBSodiumAuto ,sizeof(hdWBSodiumAuto));
                break;
        case 6:
                udpskt->write((char *)hdWBSodium ,sizeof(hdWBSodium));
                break;

        default:
                break;

    }
    return;
}

void MainWindow::on_pbBackLightSend_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    int idx = ui->cbx_backLightMode->currentIndex();
    int val = ui->sbx_cbx_backLightVal->value();

    qDebug()<<"on_pbBackLightSend_clicked() idx:"<<idx;

    switch(idx)
    {
        case 0:
        case 1:
            hdBackLight[4] = 0;
            hdBackLight[5] = 0;
            break;
        case 2:
        case 3:
            hdBackLight[4] = (uint8_t)idx & 0xff;
            hdBackLight[5] = (uint8_t)val & 0xff;
            break;
    }
    udpskt->write((char *)hdBackLight ,sizeof(hdBackLight));
}

void MainWindow::on_cbx_backLightMode_currentIndexChanged(int index)
{
    int idx = ui->cbx_backLightMode->currentIndex();
    int val = ui->sbx_cbx_backLightVal->value();

    qDebug()<<"on_cbx_backLightMode_currentIndexChanged() idx:"<<idx;

    switch(idx)
    {
        case 0:
        case 1:
            ui->sbx_cbx_backLightVal->setEnabled(false);
            break;
        case 2:
        case 3:
            ui->sbx_cbx_backLightVal->setEnabled(true);
            break;
    }
}

void MainWindow::on_pbSetDirection_clicked()
{
// #if 0
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)S9ResetDirection ,sizeof(S9ResetDirection));
// #endif
}

void MainWindow::on_pbSetPitch_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)S9ResetPitch ,sizeof(S9ResetPitch));
    qDebug()<<"***已写入置零指令******";
}

void MainWindow::on_pbSetMotor_clicked()
{
    if(connectFlag_A == false) return;

    S9MotorCtl[2] = ui->cbxTiltMotor->isChecked();
    S9MotorCtl[3] = ui->cbxPanMotor->isChecked();
    S9MotorCtl[6] = 0xff & (S9MotorCtl[1]+ S9MotorCtl[2]+ S9MotorCtl[3]+ S9MotorCtl[4]+ S9MotorCtl[5]);
    udpSocket_A->write((char *)S9MotorCtl ,sizeof(S9MotorCtl));
}

void MainWindow::on_pbSetMotor_B_clicked()
{
    if(connectFlag_B == false) return;

    S9MotorCtl[2] = ui->cbxTiltMotor_B->isChecked();
    S9MotorCtl[3] = ui->cbxPanMotor_B->isChecked();
    S9MotorCtl[6] = 0xff & (S9MotorCtl[1]+ S9MotorCtl[2]+ S9MotorCtl[3]+ S9MotorCtl[4]+ S9MotorCtl[5]);
    udpSocket_B->write((char *)S9MotorCtl ,sizeof(S9MotorCtl));
}


QUdpSocket * MainWindow::getCurMainPTZ()
{
   if(ui->rb_selGDA->isChecked())
   {
       if(connectFlag_A == false)
           return NULL;
       else
           return udpSocket_A;
   }
   else
   {
       if(connectFlag_B == false)
            return NULL;
       else
            return udpSocket_B;
   }
}

void MainWindow::on_pbClearDbgCnt_clicked()
{
    pnetworkComm->sendZhiKongPktCnt =0;
    pnetworkComm->recvZhiKongPktCnt =0;
    pGyroComm->recvPacketCnt =0;

    pnetworkComm->sendServoPktCnt =0;

    recvPktNum_A = 0;
    recvPktNum_B = 0;
    mPlayer->num_frame = 0;
    mPlayerSub->num_frame = 0;
    ui->lbRecvZhiKongPkt->setText(QString::number(pnetworkComm->recvZhiKongPktCnt));
    ui->lbRecvZhiKongDist->setText(QString::number(pnetworkComm->msgFromZhiKong.zkDistence) +'m');
    ui->lbRecvGyroPkt->setText(QString::number(pGyroComm->recvZhiKongPktCnt));
}

void MainWindow::on_pbDbgSendZhiKong_clicked()
{
    CenterSocket_Read_Data();
}

void MainWindow::on_pbToZero_clicked()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)S9ToZero ,sizeof(S9ToZero));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)S9ToZero ,sizeof(S9ToZero));
    }
    //operateMode=OPMODE_STICK;
}

void MainWindow::on_vsld_SetBeiShu_valueChanged(int value)
{
    if(value >45) value=45;
    int val = value*10*2;
    hdCmdsetBeiShu[6] = (val>>7) & 0x7f;
    hdCmdsetBeiShu[7] = val & 0x7f;
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdsetBeiShu ,sizeof(hdCmdsetBeiShu));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdsetBeiShu ,sizeof(hdCmdsetBeiShu));
    }
}

void MainWindow::on_pbDbgSendBorderChk_clicked()
{
    //ui->rb_selGDB->setChecked(true);
    //qDebug()<<"A:"<<ui->rb_selGDA->isChecked()<<"   B:"<<ui->rb_selGDB->isChecked();

    mPtzDirection_A =  QString(ui->leTestPan->text()).toDouble();
    mPtzDirection_B =  QString(ui->leTestTilt->text()).toDouble();
    chkCrossBorder();
}

void MainWindow::on_rb_SelDetectNone_clicked()
{
    trackerThd->bTrackerStart = false;
    trackerThd->trackingMethod = TRACKING_METHOD_NONE;
    ui->cbxStartTracking->setChecked(false);
}

void MainWindow::on_rb_SelDetectMtd1_clicked()
{
    trackerThd->bTrackerStart = true;
    trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;    //TRACKING_METHOD_DETECT;
}

void MainWindow::on_rb_SelDetectMtd2_clicked()
{
    trackerThd->bTrackerStart = true;
    trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
}

//void MainWindow::on_rb_SelIRSmallTarget_clicked() {
//    trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
//    operateMode = OPMODE_MARKING;
//    trackerThd->bTrackerStart = true;

//    // 设置检测参数
//}

//原文链接：https://blog.csdn.net/Y_Bingo/article/details/90345601
bool MainWindow::CheckAppStatus(const QString &appName)
{
#ifdef _WIN32      //表示如果在windows下
    QProcess process;
    process.start("tasklist" ,QStringList()<<"/FI"<<"imagename eq " +appName);   //执行tasklist程序
    process.waitForFinished(5000);    //阻塞5秒等待tasklist程序执行完成，超过五秒则直接返回
    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput()); //把tasklist程序读取到的进程信息输出到字符串中
    if(outputStr.contains(appName))
    {
        process.close(); //用完记得把process关闭了，否则如果重新调用这个函数可以会失败
        return true;
    }
    else
    {
        process.close();
        return false;
    }
#endif
}

void MainWindow::on_pbDbgSendconvAE_clicked()
{
    double radA=0;
    double radE=0;
    double distance = 1000;
    double direction;
    double pitch ;

    direction =  QString(ui->leTestPan->text()).toDouble();
    pitch =  QString(ui->leTestTilt->text()).toDouble();

    convToRadarAE(&radA,&radE,distance,direction,pitch, mGD1MountPosX,mGD1MountPosY,mGD1MountPosZ);

    qDebug()<<"radA:"<<radA <<" radE:"<<radE;
}

void MainWindow::on_pbcalcBlh2Rae_clicked()
{
    float GD1WeiDu = ui->leGd1SiteB->text().trimmed().toFloat();
    float GD1JingDu= ui->leGd1SiteL->text().trimmed().toFloat();
    float GD1GaoDu = ui->leGd1SiteH->text().trimmed().toFloat();

    float GD2WeiDu = ui->leGd2SiteB->text().trimmed().toFloat();
    float GD2JingDu = ui->leGd2SiteL->text().trimmed().toFloat();
    float GD2GaoDu = ui->leGd2SiteH->text().trimmed().toFloat();

    float poleWeiDu = ui->leStdPoleSiteB->text().trimmed().toFloat();
    float poleJingDu = ui->leStdPoleSiteL->text().trimmed().toFloat();
    float poleSiteGaoDu = ui->leStdPoleSiteH->text().trimmed().toFloat();

    mvStationBlh.setX(GD1WeiDu);
    mvStationBlh.setY(GD1JingDu);
    mvStationBlh.setZ(GD1GaoDu);
    msStationGdcType = "WGS84";
    mCoordTrsr.Init(msStationGdcType, mvStationBlh);

    QVector3D vBlh;

    vBlh = QVector3D((float)poleWeiDu, (float)poleJingDu, (float)poleSiteGaoDu);
    //QVector3D vRAE = mCoordTrsr.XyzEcef2Rae(vXyz);
    /*QVector3D vRAE = mCoordTrsr.Blh_Blh_2Rae(vBlh);

    mCenterGuide.dblR=vRAE.x();
    mCenterGuide.dblA=vRAE.y();
    mCenterGuide.dblE=vRAE.z(); */
    double x_mb=0,y_mb=0,z_mb=0;

    //大地BLH转地心系
    mCoordTrsr.dd2dx(vBlh.x(),vBlh.y(),vBlh.z(), &x_mb,&y_mb,&z_mb);
    mCoordTrsr.XYZToAE(x_mb,y_mb,z_mb, mCoordTrsr.mvBlh.x(),mCoordTrsr.mvBlh.y(),mCoordTrsr.mvBlh.z(), mCenterGuide.dblA,mCenterGuide.dblE,mCenterGuide.dblR );
    if(mCenterGuide.dblE>180)
    {
        mCenterGuide.dblE = mCenterGuide.dblE - 360;
    }

    ui->leGD1Blh2A->setText(QString::number(mCenterGuide.dblA,'f',4));
    ui->leGD1Blh2E->setText(QString::number(mCenterGuide.dblE,'f',4));
    ui->leGD1Blh2R->setText(QString::number(mCenterGuide.dblR,'f',3));

    double radA=0;
    double radE=0;

    convToRadarAE(&radA,&radE,mCenterGuide.dblR,mCenterGuide.dblA,mCenterGuide.dblE, mGD1MountPosX,mGD1MountPosY,mGD1MountPosZ);
    ui->leGD1AntennaA->setText(QString::number(radA,'f',4));
    ui->leGD1AntennaE->setText(QString::number(radE,'f',4));

    //---------------GD2--------------------------------------------------------
    mvStationBlh.setX(GD2WeiDu);
    mvStationBlh.setY(GD2JingDu);
    mvStationBlh.setZ(GD2GaoDu);
    msStationGdcType = "WGS84";
    mCoordTrsr.Init(msStationGdcType, mvStationBlh);

    vBlh = QVector3D((float)poleWeiDu, (float)poleJingDu, (float)poleSiteGaoDu);
    x_mb=0;y_mb=0;z_mb=0;

    mCoordTrsr.dd2dx(vBlh.x(),vBlh.y(),vBlh.z(), &x_mb,&y_mb,&z_mb);
    mCoordTrsr.XYZToAE(x_mb,y_mb,z_mb, mCoordTrsr.mvBlh.x(),mCoordTrsr.mvBlh.y(),mCoordTrsr.mvBlh.z(), mCenterGuide.dblA,mCenterGuide.dblE,mCenterGuide.dblR );
    if(mCenterGuide.dblE>180)
    {
        mCenterGuide.dblE = mCenterGuide.dblE - 360;
    }

    ui->leGD2Blh2A->setText(QString::number(mCenterGuide.dblA,'f',4));
    ui->leGD2Blh2E->setText(QString::number(mCenterGuide.dblE,'f',4));
    ui->leGD2Blh2R->setText(QString::number(mCenterGuide.dblR,'f',3));

    radA=0;
    radE=0;

    convToRadarAE(&radA,&radE,mCenterGuide.dblR,mCenterGuide.dblA,mCenterGuide.dblE, mGD2MountPosX,mGD2MountPosY,mGD2MountPosZ);
    ui->leGD2AntennaA->setText(QString::number(radA,'f',4));
    ui->leGD2AntennaE->setText(QString::number(radE,'f',4));
}

void MainWindow::on_pbSetGD1Direction_clicked()
{
#if 0
    double dirct = ui->leGD1Blh2A->text().trimmed().toDouble();
    uint16_t direction = (uint16_t)(dirct * ANGLENET2DEG );
    uint8_t cmd[]={0xff, 0x22, 0x00, 0x00, 0x00, 0x00, 0x22};       //置方位
    cmd[2] = (direction>>8)& 0xff;
    cmd[3] = (direction)& 0xff;
    cmd[6] = 0xff & (cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]);
    udpSocket_A->write((char *)cmd ,sizeof(cmd));
#endif
    double dirct = ui->leGD1Blh2A->text().trimmed().toDouble();
    dd_diffx_default_A = dirct - mRawPtzDirection_A;
    if(dd_diffx_default_A >= 360)   dd_diffx_default_A = dd_diffx_default_A - 360;
    ui->leSetDiffX_default_A->setText(QString::number(dd_diffx_default_A,'f',4));
}

void MainWindow::on_pbSetGD2Direction_clicked()
{
#if 0
    double dirct = ui->leGD2Blh2A->text().trimmed().toDouble();
    uint16_t direction = (uint16_t)(dirct * ANGLENET2DEG );
    uint8_t cmd[]={0xff, 0x22, 0x00, 0x00, 0x00, 0x00, 0x22};       //置方位
    cmd[2] = (direction>>8)& 0xff;
    cmd[3] = (direction)& 0xff;
    cmd[6] = 0xff & (cmd[1]+ cmd[2]+ cmd[3]+ cmd[4]+ cmd[5]);
    udpSocket_B->write((char *)cmd ,sizeof(cmd));
#endif
    double dirct = ui->leGD2Blh2A->text().trimmed().toDouble();
    dd_diffx_default_B = dirct - mRawPtzDirection_B;
    if(dd_diffx_default_B >= 360)   dd_diffx_default_B = dd_diffx_default_B - 360;
    ui->leSetDiffX_default_B->setText(QString::number(dd_diffx_default_B,'f',4));
}

void MainWindow::on_pbcalcRaeClear_clicked()
{
    ui->leGD1Blh2A->clear();
    ui->leGD1Blh2E->clear();
    ui->leGD1Blh2R->clear();

    ui->leGD2Blh2A->clear();
    ui->leGD2Blh2E->clear();
    ui->leGD2Blh2R->clear();
}

void MainWindow::on_cbxStartTracking_stateChanged(int arg1)
{
    PIDReset2(&PidX);
    PIDReset2(&PidY);
    if(ui->rb_SelDetectMtd2->isChecked())
    {
        if(ui->cbxStartTracking->isChecked() == true)
        {
            //切换到KCF精确跟踪
            // trackerThd->bTrackerStart = true;
            // trackerThd->trackingMethod = TRACKING_METHOD_KCF;
            // trackerThd->roi_min_width = roiBox.x;
            // trackerThd->roi_min_height = roiBox.y;
            // trackerThd->roi_width = roiBox.width;
            // trackerThd->roi_height = roiBox.height ;
            // trackerThd->boxChanged = true;
            // PidX.enable_I_flag = false;
            // PidY.enable_I_flag = false;
            // timerDelayPid->start(2000);

            // 切到IR_SMALL
            trackerThd->bTrackerStart = true;
            trackerThd->trackingMethod = TRACKING_METHOD_IR_SMALL;
            trackerThd->irParams.tarLmax = 15;
            trackerThd->irParams.k1 = 3;
            trackerThd->irParams.k2 = 2;
            trackerThd->irParams.scoreThreshold = 2.5f;

            trackerThd->roi_width_ = 320;  // IR算法ROI窗口
            trackerThd->roi_height_ = 256;
            // 设置跟踪中心点（从TopHat检测结果中获得）
            cv::Point2f center = cv::Point2f(roiBox.x + roiBox.width/2,
                                            roiBox.y + roiBox.height/2);
            trackerThd->setCustomCenter(center);
            trackerThd->boxChanged = true;
            // PID控制器设置
            PidX.enable_I_flag = false;
            PidY.enable_I_flag = false;
            timerDelayPid->start(2000);
        }
        else
        {
//            sendSetCmd(PKT_TYPE_PTZSTOP,0);
            if(connectFlag_A != false)
            {
                udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
            }
            if(connectFlag_B != false)
            {
                udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
            };
            // 切换回TopHat搜索模式
            trackerThd->bTrackerStart = true;
            trackerThd->trackingMethod = TRACKING_METHOD_TOPHAT;
        }
    }
}

void MainWindow::on_rb_DefogOff_clicked()
{
    udpSocket_A->write((char *)hdCmdDefogOff ,sizeof(hdCmdDefogOff));
    udpSocket_B->write((char *)hdCmdDefogOff ,sizeof(hdCmdDefogOff));
}

void MainWindow::on_rb_DefogOn_clicked()
{
    udpSocket_A->write((char *)hdCmdDefogOn ,sizeof(hdCmdDefogOn));
    udpSocket_B->write((char *)hdCmdDefogOn ,sizeof(hdCmdDefogOn));
}

void MainWindow::quitAndShutdown()
{
    //system("shutdown -f -p");
    sendPosition(225.0, -20.0,true,false);
    sendPosition(45.0, -20.0,false,true);
    QThread::msleep(500);
    QProcess process(this);
    QString  str = "C:/WINDOWS/system32/shutdown.exe -f -p";
    process.startDetached(str);
    //process.start(str);       //会随process close 而退出
    qDebug()<<"process:"<<str;
    //QThread::msleep(1*1000);
    this->close();
}

void MainWindow::on_pbSysClose_clicked()
{
    quitAndShutdown();
}


void MainWindow::on_pbAdjDriftUp_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9AdjDriftUp ,sizeof(S9AdjDriftUp));

}

void MainWindow::on_pbAdjDriftDown_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9AdjDriftDown ,sizeof(S9AdjDriftDown));
}

void MainWindow::on_pbAdjDriftLeft_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9AdjDriftLeft ,sizeof(S9AdjDriftLeft));
}

void MainWindow::on_pbAdjDriftRight_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9AdjDriftRight ,sizeof(S9AdjDriftRight));
}

void MainWindow::on_pbAdjDriftUp_released()
{
    on_pbAdjDriftStop_clicked();
}
void MainWindow::on_pbAdjDriftDown_released()
{
    on_pbAdjDriftStop_clicked();
}

void MainWindow::on_pbAdjDriftLeft_released()
{
    on_pbAdjDriftStop_clicked();
}

void MainWindow::on_pbAdjDriftRight_released()
{
    on_pbAdjDriftStop_clicked();
}
void MainWindow::on_pbAdjDriftStop_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9AdjDriftStop ,sizeof(S9AdjDriftStop));
}

void MainWindow::on_pbAAdjDriftSave_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9SaveParam ,sizeof(S9SaveParam));
}

#if 0
int MainWindow::predict_Radar_Target(double latest_direction,double latest_pitch,int latest_dist)
{
    gZhiKongGuideData_s[1].direction = gZhiKongGuideData_s[0].direction;
    gZhiKongGuideData_s[1].pitch = gZhiKongGuideData_s[0].pitch;
    gZhiKongGuideData_s[1].distance = gZhiKongGuideData_s[0].distance;
    gZhiKongGuideData_s[1].timestamp = gZhiKongGuideData_s[0].timestamp;

    gZhiKongGuideData_s[0].direction = latest_direction;
    gZhiKongGuideData_s[0].pitch = latest_pitch;
    gZhiKongGuideData_s[0].distance = latest_dist;
    gZhiKongGuideData_s[0].timestamp = time(NULL);

    if(gZhiKongGuideData_s[0].timestamp - gZhiKongGuideData_s[1].timestamp < 1 )    //连续引导中,不需要预测
    {
        return 0;
    }



    return 0;
}
#endif
