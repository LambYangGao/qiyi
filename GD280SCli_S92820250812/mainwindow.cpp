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
QReadWriteLock netBufLock;

#ifdef WIN32
#include <windows.h>
#include <dbghelp.h>
#include <DbgHelp.h>
#include <QDir>
#include <QProcess>
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

#ifndef SIMUCAMERA
    //on_pbRec_clicked();  // 开始录像
#endif

}

MainWindow::~MainWindow()
{
    hid->runflag=false;
    hid->quit();
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
    //mPlayer->channelUrl.sprintf("rtsp://admin:admin@%s:%d/cam/realmonitor?channel=1&subtype=0",hdCamIp_A,mHdCamPort_A);        //SH相机直出 RTSP流 0:主码流 1:辅码流
    mPlayer->channelUrl = "rtsp://127.0.0.1/ch1";
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
    //mPlayerSub->channelUrl.sprintf("rtsp://admin:admin@%s:%d/cam/realmonitor?channel=1&subtype=0",hdCamIp_B,mHdCamPort_B);        //SH相机直出 RTSP流 0:主码流 1:辅码流
    mPlayer->channelUrl = "rtsp://127.0.0.1/ch1";
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
        qDebug()<<tr("GD_A connect OK");
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

//    qDebug() << QString("惯导角度对比 - direction(直接度): %1°, yaw(度): %2°")
//               .arg(directionInDegrees, 0, 'f', 3)
//               .arg(yawInDegrees, 0, 'f', 3);

    double angleDiff = abs(directionInDegrees - yawInDegrees);
    if (angleDiff > 180.0) {
        angleDiff = 360.0 - angleDiff;
    }

    // 如果差异小，用yaw
    if (angleDiff < 5.0) {
        //qDebug() << "使用yaw角度，差异较小: " << angleDiff << "度";
        return yawInDegrees;
    }

    // 如果差异较大，就要看哪个更准（理论上direction（双天线）比yaw（单天线）更准）
    //qDebug() << "角度差异较大: " << angleDiff << "度，用direction";
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

    double Site_B = 0;//站址纬度 度°
    double Site_L = 0;//站址经度 度°
    double Site_H = 0;//站址高程 m
    operateMode = OPMODE_MARKING;  //JoyStick;

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


    mAEJoyCorrect.dblA = 0;//中心引导修正量
    mAEJoyCorrect.dblE = 0;

    mStick_y = 512;
    mStick_x = 512;
    mStick_z = 512;
    mStickBtn = 0;
    mStickBtn2 =0;

    mJoy_x = 80;
    mJoy_y = 80;
    mJoy_btn = 0;

    dd_diffx=0;
    dd_diffy=0;
    mImageCh1GetNew = false;
    mImageCh2GetNew = false;
    mGyroPitch_A = 0;
    mGyroPitch_B = 0;
    currentMainSrc = 1;
}

void MainWindow::initControlStat()
{
    ui->lbMousePosition_x->setVisible(0);   //只在调试时使用，正式版本不打开
    ui->lbMousePosition_y->setVisible(0);
    ui->label_5->setVisible(0);
    ui->label_6->setVisible(0);
    ui->lbDiffX->setVisible(0);   //只在调试时使用，正式版本不打开
    ui->lbDiffY->setVisible(0);   //只在调试时使用，正式版本不打开
    ui->lbl_lenVal->setVisible(1);
    ui->lbl_focalVal->setVisible(1);
    ui->lbl_focalVal_B->setVisible(1);
    ui->lblLens->setVisible(1);
    ui->lbl_lensTitle->setVisible(1);
    ui->pb_OpenSelfTest->setVisible(0);
    ui->pb_CloseSelfTest->setVisible(0);
    ui->pb_OpenSelfTest_B->setVisible(0);
    ui->pb_CloseSelfTest_B->setVisible(0);
    ui->gbx_Expo->setEnabled(true);
    ui->lbl_SimJoystick->installEventFilter(this);//安装事件过滤器

    QIntValidator* validv = new QIntValidator;
    validv->setRange(1, 16);//可以改成（-255,255），这样就是负数

    QIntValidator* validv2 = new QIntValidator;
    validv2->setRange(0, 1440);//

    QIntValidator* validv3 = new QIntValidator;
    validv3->setRange(0, 59);//

    ui->pbConfigSave->setEnabled(false);
    ui->leRecFileDir->setEnabled(false);
    ui->leSetIP->setEnabled(false);
    ui->leSetHdCamIP->setEnabled(false);
    ui->leSetHdCamPort->setEnabled(false);

    ui->cbxCrosser->setChecked(true);
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
    timerRecFileSeg = new QTimer();
    connect(timerRecFileSeg,SIGNAL(timeout()),this,SLOT(timerRecFileSeg_timeOut()));
    timerRecFileSeg->start(1000*60*3);   //3min


    m_usb_listener = new usb_listener;
    m_usb_listener->registerGUID();
    qApp->installNativeEventFilter(m_usb_listener);
    connect(m_usb_listener, SIGNAL(sig_DeviceChangeCbk(int)), this, SLOT(slotHidChanged(int)));


    hid= new HidProcThd;
    hid->start();

    timerChkIfOffline = new QTimer();  // 离线检测
    connect(timerChkIfOffline,SIGNAL(timeout()),this,SLOT(chkIfOffline_timeOut()));
    timerChkIfOffline->start(1000 * 5);

    timerJoystick= new QTimer();  // 遥杆
    connect(timerJoystick,SIGNAL(timeout()),this,SLOT(stick_timeOut()));
    timerJoystick->start(10);
    qDebug()<<tr("start timer JoyStick");

    timerZhiKongSocket = new QTimer(); // 指控
    connect(timerZhiKongSocket,SIGNAL(timeout()),this,SLOT(zhiKongSocket_timeOut()));
    timerZhiKongSocket->start(50);  //20HZ
    qDebug()<<tr("start timer ZhiKongSocket");
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
                mJoy_y = p_SimuStick.y()-15;   
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
                int camWidth,camHeight,pos_x=0,pos_y=0;
                float rate_w=0,rate_h=0;
                {
                    camWidth = IMAGE_PIXEL_WIDTH;
                    camHeight = IMAGE_PIXEL_HEIGHT;
                }
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
                    pos_x = pos_x - 32;
                }
                if(pos_x > 0 && pos_x <=camWidth && pos_y > 0 && pos_y <=camHeight)
                {
                    posX = pos_x;
                    posY = pos_y;
                }
                // TODO: 发送“点击跟踪”指令给新光电，坐标(posX, posY)

            }
            else if( 2 == btnStatus)  // 右键点击 - 停止跟踪
            {    

            }
            //ui->lblPad->update();
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

                }
            }
            else if( 2 == btnStatus)  // 右键点击 - 停止跟踪
            {

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

    }
    else if (event->button() == Qt::RightButton){

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
            //TODO: 转发指控跟踪/算法指令给嵌入式
            break;

        case 3: // suanfa 算法相关处理
            //TODO: 转发指控跟踪/算法指令给嵌入式
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
        case 7: //选择控制为光电B
            ui->rb_selGDB->setChecked(true);
            break;
        case 8: //选择控制为光电A
            ui->rb_selGDA->setChecked(true);
            break;
        default:
            qDebug() << "未知的pktType标志位:" << pktTypeFlag;
            ui->te_Output->append(QString("警告：未知pktType=%1，按默认方式处理").arg(pktTypeFlag));
            handleCmdIdProcessing(); // 默认按cmdId方式处理
            break;
    }
}

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
            on_pbStop_pressed();      // 停止云台运动
            ui->rb_trackStick->setChecked(true);  //勾选手动模式
            pnetworkComm->msgToZhiKong.status = 0;

        case 1: //指控引导

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

    } else {
        // 停止跟踪

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
    // debugGyroAngles();

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
}

int MainWindow::convZhiKongCameraCmd(int camVal)
{
    static int sendFocFlag = false;
    static int sendZoomFlag = false;

    switch(camVal)
    {
        case 0:
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

//视角计算公式
//1920      =  2 * arcTan(5.3 /2f)
//640      =  2 * arcTan(10.88 /2f)
void MainWindow::socket_Read_Data_A()
{
    QUdpSocket * udpskt = udpSocket_A;  //getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray buffer;
    buffer = udpskt->readAll();
    int p=0;
    int direction = 0;
    int pitch = 0;
    int zoomVal = 0;
    int focalVal =0;
    recvPktNum_A++;
    ui->lbRecvPkt_A->setText(QString::number(recvPktNum_A) );
    if(buffer.length() < 7)
    {
        return;
    }

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
                            ui->lbl_focalVal->setText(QString::number(zoomVal));  //将聚焦值显示ui界面 估计是200ms一次
                        }
                        else    //Zoom pos return 变焦位置反馈模式
                        {
                            ZoomValToLensData_Improved(zoomVal, &hdBianBei, &hdLensVal, &mHdViewAngle_A);
                            ui->lbl_lenVal->setText(QString::number(hdLensVal)+"mm");
                            ui->lblBeishu->setText("X" + QString::number(hdBianBei, 'f',1));
                            ui->lblLens->setText( QString::number(mHdViewAngle_A,'f', 1)+"°");
                            hdZoomVal = zoomVal;

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

    }

    pnetworkComm->msgToZhiKong.gd1BianBei = (uint16_t)hdLensVal;
    ui->lblDirection->setText( QString::number(

    mPtzDirection_A, 'f',2)+"°");
    ui->lblPitch->setText( QString::number(mPtzPitch_A, 'f',2)+"°");
}

void MainWindow::socket_Read_Data_B()
{
    QUdpSocket * udpskt = udpSocket_B;  //getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray buffer;
    buffer = udpskt->readAll();
    int p=0;
    int direction = 0;
    int pitch = 0;
    int zoomVal = 0;
    int focalVal =0;
    recvPktNum_B++;
    ui->lbRecvPkt_B->setText(QString::number(recvPktNum_B) );
    if(buffer.length() < 7)
    {
        return;
    }


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
                            ui->lbl_focalVal_B->setText(QString::number(zoomVal));
                        }
                        else    //Zoom pos return
                        {
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
    }

    pnetworkComm->msgToZhiKong.gd2BianBei = (uint16_t)hdLensVal_B;
#endif


    ui->lblDirection_B->setText( QString::number(mPtzDirection_B, 'f',2)+"°");
    ui->lblPitch_B->setText( QString::number(mPtzPitch_B, 'f',2)+"°");

}


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
            img = mImageCh1.scaled(ui->lblPad->size(), Qt::KeepAspectRatio);
            fitPixmap=QPixmap::fromImage(img);
            if(fitPixmap.isNull())
            {
                qDebug()<<"fitPixmap is null";
                return;
            }

            ui->lblPad->setPixmap(fitPixmap);
            mImageCh1GetNew = false;
        }
        if(true==mImageCh2GetNew)
        {
            if(mImageCh2.isNull())
            {
                return;
            }
            img = mImageCh2.scaled(ui->lblPad_B->size(), Qt::KeepAspectRatio);
            fitPixmap=QPixmap::fromImage(img);
            if(fitPixmap.isNull())
            {
                qDebug()<<"fitPixmap is null";
                return;
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

    if(watched==ui->lbl_SimJoystick && event->type()==QEvent::Paint)//判断是否是lblImage控件，是否是绘制事件
    {
        drawVirtualJoyStick(mJoy_x,mJoy_y,mJoy_btn);

    }

    return QWidget::eventFilter(watched,event);
    
}

//在屏幕上画刻度线
//视角计算公式
//1920      =  2 * arcTan(5.3 /2f)      449~950  150~1500
//640      =  2 * arcTan(10.88 /2f)
void MainWindow::drawViewAngle(QPixmap * fitPixmap,int scrWidth,int scrHeight)
{

}

#define AZMARGIN 30
#define AZMARGINY 50
#define AZCHARARGIN 12
#define AZCHARARGINY 32

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
    pen.setColor(QColor(0,0,0,255));//画笔颜色
    p.setPen(pen);
    p.drawLine(0,80,160,80);
    p.drawLine(80,0,80,160);

    brush.setColor(QColor(0,0,255,230));
    p.setBrush(brush);
    p.drawEllipse(QPoint(x, y),10,10);

    return;
}

void MainWindow::updateJoyStick()
{
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
    if(pan_old != pan || tilt_old != tilt)   //方向 俯仰
    {
        double kh=25000;
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

        qDebug()<<"pan:"<<pan_outVal<<"  tilt:"<<tilt_outVal;
        // 生成控制命令
        uint8_t * cmd = mPodCmd.crtSteplessCmd((pan_outVal ), -(tilt_outVal));

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
    pan = (x-80)*2 + 512;
    tilt = (y-80)*2 + 512;

    double kh=1200;

    if(pan_old != pan || tilt_old != tilt)
    {
        pan_outVal=updateJoysticktoExpo(pan, mExpo,HID_RESOLUTION,kh);
        tilt_outVal=updateJoysticktoExpo(tilt, mExpo,HID_RESOLUTION,kh);
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
    if(outVal>0 && outVal<1) outVal=1;
    else if(outVal>-1 && outVal<0) outVal=-1;
    return (int)outVal;
}

void MainWindow::slotGetOneFrameCh1(QImage img)
{
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
    mImageCh1 = img.copy();
    mImageCh1GetNew = true;

    update();  // 会触发paintEvent重绘
    ui->lbRecvFrameA->setText(QString::number(mPlayer->num_frame));

}

void MainWindow::slotGetOneFrameCh2(QImage img)
{
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
    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    };
}

void MainWindow::on_pbUp_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
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
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
}

void MainWindow::on_pbRight_released()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    ptzCmdStop[6]=0xff & ( ptzCmdStop[1]+ ptzCmdStop[2]+ ptzCmdStop[3]+ ptzCmdStop[4]+ ptzCmdStop[5]);
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
}

void MainWindow::on_pbUp_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,(pelco_spd_val* ANGLENET2DEG));
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

void MainWindow::on_pbDown_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(0,-(pelco_spd_val* ANGLENET2DEG));
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

    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(-(pelco_spd_val* ANGLENET2DEG),0);
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

void MainWindow::on_pbRight_pressed()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    {
        uint8_t * cmd = mPodCmd.crtSteplessCmd(pelco_spd_val* ANGLENET2DEG,0);
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

int MainWindow::adjFocusPosByDist(QUdpSocket * udpSocket,int dist)
{
    int t =dist;
    static int postdist =0;
    if(ui->cbxSetDbgDistance->isChecked())  //调试或者校标用
    {
        t = ui->leSetDbgDistance->text().toInt();  //在调试界面开启了设置调试距离，则获取距离值为t
    }

    if(t < 1  ) t = 1000;
    int focval = ui->leHdaDefFoc->text().toInt();   //4400;//getFocValByDist(t);   //配置中相机A对焦值
    hdCmdFocalTo[4] = (focval>>12)&0xf;
    hdCmdFocalTo[5] = (focval>>8)&0xf;
    hdCmdFocalTo[6] = (focval>>4)&0xf;
    hdCmdFocalTo[7] = (focval>>0)&0xf;
    qDebug()<<"adjFocusPosByDist" <<focval;
    udpSocket->write((char *)hdCmdFocalTo ,sizeof(hdCmdFocalTo));
    postdist = t;
}

void MainWindow::on_cbxPresetNoEdit_currentIndexChanged(int index)
{
    presetCurrentIdx = index+1;
}

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
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdFocalStop ,sizeof(hdCmdFocalStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdFocalStop ,sizeof(hdCmdFocalStop));
    }
}

void MainWindow::on_pbFocusN_released()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdFocalStop ,sizeof(hdCmdFocalStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdFocalStop ,sizeof(hdCmdFocalStop));
    }
}

void MainWindow::on_pbFocusF_pressed()
{
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdFocalFar ,sizeof(hdCmdFocalFar));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdFocalFar ,sizeof(hdCmdFocalFar));
    }
}

void MainWindow::on_pbFocusN_pressed()
{

    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)hdCmdFocalNear ,sizeof(hdCmdFocalNear));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)hdCmdFocalNear ,sizeof(hdCmdFocalNear));
    }
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
    writeCfgFile();
    ui->cbxEditLock_sys->setCheckState(Qt::Unchecked);
}

void MainWindow::setCtlObjStatus(bool enFlag)
{
    ui->tabWidConfig->setEnabled(enFlag);
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
    ui->pbToZero->setEnabled(enFlag);
    ui->rb_trackStick->setEnabled(enFlag);
    ui->rb_Center->setEnabled(enFlag);
    ui->rb_Tracking->setEnabled(enFlag);
    ui->rb_trackRoi10->setEnabled(enFlag);
    ui->rb_trackRoi20->setEnabled(enFlag);
    ui->rb_trackRoi40->setEnabled(enFlag);
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


void MainWindow::slotGetSigBackToMainWin()
{
    {
        mPlayer->runflag=false;
        fullscreen.close();
    }
}

void MainWindow::slotGetUpdateJoystick()
{
    updateJoyStick();
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

    // 球坐标转直角坐标
    double horDistance = (inDistance) * cos(radTiltVal);    //求到光电的水平距离
    double vertDistance = (inDistance) * sin(radTiltVal);    //求垂直距离
    double MuBiaoWeiZhiX = 0;   //以基座为原点的xyz
    double MuBiaoWeiZhiY = 0;
    double MuBiaoWeiZhiZ = 0;
    double MuBiaoWeiZhiHL = 0;  //到雷达的水平斜距离

    // 安装偏差，转到天线基座
    MuBiaoWeiZhiX = horDistance* sin(radPanVal)+AnZhuangX;
    MuBiaoWeiZhiY = horDistance* cos(radPanVal)+AnZhuangY;
    MuBiaoWeiZhiZ = vertDistance + AnZhuangZ;
    MuBiaoWeiZhiHL = qSqrt(MuBiaoWeiZhiX * MuBiaoWeiZhiX + MuBiaoWeiZhiY * MuBiaoWeiZhiY);

    //qDebug()<<"MuBiaoWeiZhiX:"<<MuBiaoWeiZhiX <<"   Y:"<<MuBiaoWeiZhiY <<"   Z:"<<MuBiaoWeiZhiZ <<" MuBiaoWeiZhiZL"<<MuBiaoWeiZhiHL;

    // 计算天线坐标系下的方位俯仰
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
    pnetworkComm->msgToZhiKong.status = 0;
    operateMode = OPMODE_STICK;
    if(connectFlag_A != false)
    {
        udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    }
    if(connectFlag_B != false)
    {
        udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
    };

    ui->lblPad->setCursor(Qt::ArrowCursor);

    on_rb_HGyroUnLock_clicked();
    ui->rb_HGyroUnLock->setChecked(true);
}

void MainWindow::on_rb_toZero_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    udpskt->write((char *)S9ToZero ,sizeof(S9ToZero));

    operateMode=OPMODE_TOZERO;
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

bool MainWindow::openSerialPort()
{
    QString commServo="COM3";
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

int MainWindow::recvComData()
{
#if 1
    if( connectFlag_A == false || connectFlag_B == false) return 0;

    mzRecvBuf.append(SerialPort.readAll());
    int nLen=mzRecvBuf.length();
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

int MainWindow::sendServoComm(double dblGd1A, double dblGd1E,double dblGd2A, double dblGd2E,int coverflag)
{
    if(pnetworkComm->servo422ConnectFlag == false)  return 100;
    pnetworkComm->msgToServo422.loopNo++;
    pnetworkComm->msgToServo422.gd1FangWei.fData = dblGd1A;
    pnetworkComm->msgToServo422.gd1FuYang.fData = dblGd1E;
    pnetworkComm->msgToServo422.gd2FangWei.fData = dblGd2A;
    pnetworkComm->msgToServo422.gd2FuYang.fData = dblGd2E;
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

void MainWindow::timerRecFileSeg_timeOut()
{
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
    double pitch;


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

void MainWindow::on_pClearbDbgOut_clicked()
{
    ui->te_Output->clear();
}

void MainWindow::on_pbDbgSend_clicked()
{
    uint8_t blkCmd[100];
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;

    QByteArray array = QByteArray::fromHex(ui->le_dbgSend->text().toLatin1());
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

void MainWindow::on_pbSetDirection_clicked()
{
    QUdpSocket * udpskt = getCurMainPTZ();
    if( udpskt == NULL) return;
    udpskt->write((char *)S9ResetDirection ,sizeof(S9ResetDirection));
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
    mPtzDirection_A =  QString(ui->leTestPan->text()).toDouble();
    mPtzDirection_B =  QString(ui->leTestTilt->text()).toDouble();
    chkCrossBorder();
}

void MainWindow::on_rb_SelDetectNone_clicked()
{
    ui->cbxStartTracking->setChecked(false);
}

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
    double dirct = ui->leGD1Blh2A->text().trimmed().toDouble();
    dd_diffx_default_A = dirct - mRawPtzDirection_A;
    if(dd_diffx_default_A >= 360)   dd_diffx_default_A = dd_diffx_default_A - 360;
    ui->leSetDiffX_default_A->setText(QString::number(dd_diffx_default_A,'f',4));
}

void MainWindow::on_pbSetGD2Direction_clicked()
{
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
    if(ui->rb_SelDetectMtd2->isChecked()||ui->rb_SelDetectMtd1->isChecked())
    {
        if(ui->cbxStartTracking->isChecked() == true)
        {

        }
        else
        {
            if(connectFlag_A != false)
            {
                udpSocket_A->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
            }
            if(connectFlag_B != false)
            {
                udpSocket_B->write((char *)ptzCmdStop,sizeof(ptzCmdStop));
            };
        }
    }
}

void MainWindow::quitAndShutdown()
{
    sendPosition(225.0, -20.0,true,false);
    sendPosition(45.0, -20.0,false,true);
    QThread::msleep(500);
    QProcess process(this);
    QString  str = "C:/WINDOWS/system32/shutdown.exe -f -p";
    process.startDetached(str);
    qDebug()<<"process:"<<str;
    this->close();
}

void MainWindow::on_pbSysClose_clicked()
{
    quitAndShutdown();
}
