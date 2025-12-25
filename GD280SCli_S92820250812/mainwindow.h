#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QUdpSocket>
//#include <QAbstractSocket>
#include <qmessagebox.h>
#include <QMouseEvent>

#include <QImage>
#include <QPaintEvent>
#include <QBasicTimer>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <iostream>
#include <stdint.h>

#include <QDesktopWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDebug>
#include <QFileDialog>
#include <QReadWriteLock>

#include "vidoeplayer.h"
#include "HidThread.h"
#include "fullscreenform.h"
#include "theo_pp_dll.h"
//#include "dlgexposure.h"
#include "networkcomm.h"
#include "dlgAutoHide.h"
#include "CommonDef.h"
#include "CoordTranser.h"
#include "trackerthread.h"
#include "pid.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "pod_cmd.h"
#include "ctlloopthd.h"
#include "qtmaterialautocomplete.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define PKT_HEADER 0x3f
#define PKT_TYPE_POS 1
#define PKT_TYPE_ROISIZE 2
#define PKT_TYPE_ZOOM 3
#define PKT_TYPE_SETMAINSCREEN 4
#define PKT_TYPE_PTZUP 5
#define PKT_TYPE_PTZDOWN 6
#define PKT_TYPE_PTZLEFT 7
#define PKT_TYPE_PTZRIGHT 8
#define PKT_TYPE_PTZSTOP 9
#define PKT_TYPE_PICINPIC 10
#define PKT_TYPE_TOPRESET 	11
#define PKT_TYPE_SCAN 		12
#define PKT_TYPE_TVIRSWITCH 13
#define PKT_TYPE_WIPER 		14
#define PKT_TYPE_DEFOG 		15
#define PKT_TYPE_TOZERO		16
#define PKT_TYPE_FOCUS      17
#define PKT_TYPE_AUTOFOCUS  18
#define PKT_TYPE_MANUSPEED  19
#define PKT_TYPE_AUTOSPEED  20
#define PKT_TYPE_GYROCTL    21
#define PKT_TYPE_INITZERO   22
#define PKT_TYPE_TOANGLE    23
#define PKT_TYPE_RESETMOTOR 24
#define PKT_TYPE_SETIPADDR  25
#define PKT_TYPE_SETIPMASK  26
#define PKT_TYPE_SETIPGW    27
//#define PKT_TYPE_CAMSWITCH  29
#define PKT_TYPE_PALETTE    28
#define PKT_TYPE_PTZPANTILT 29
#define PKT_TYPE_ADJDRIFT   30
#define PKT_TYPE_LOCKAXIAL   31
#define PKT_TYPE_SAVEPARAM   32
#define PKT_TYPE_BALANCE     33
#define PKT_TYPE_ONOFFCROSS  34
#define PKT_TYPE_OPTDEFOG    35
#define PKT_TYPE_ONOFFCOLOR  36
#define PKT_TYPE_EXPOSURE    37
#define PKT_TYPE_PTZPANTILTFOCZOOM 38
#define PKT_TYPE_EXPOLIMIT   39
#define PKT_TYPE_IRPOWER    40
#define PKT_TYPE_CAMCFG     41
#define PKT_TYPE_TRANSCOMBUF    42

#define  PTZ_SPEED_VAL 2000     //  20°/s
#define  PTZ_PELCO_SPD_VAL  32

//#define PI 3.1415927
//#define PI 3.1415926535897922

#define HID_RESOLUTION  0x200


#define  PTZMSG_BUF_LEN     33  //转台返回信息的buffer长度

#define OPMODE_STICK    0
#define OPMODE_TOZERO   1
#define OPMODE_CENTER   2
#define OPMODE_MARKING  3


//#define GD200           1       //转台型号
//#define PUV150          1       //转台型号
//#define S500           1       //转台型号
#define GD280S           1

#define IMAGE_PIXEL_WIDTH   1920
#define IMAGE_PIXEL_HEIGHT   1080

#define IMAGE_PIXEL_WIDTH_IR   800
#define IMAGE_PIXEL_HEIGHT_IR   600

struct lensData_s
{
    double BeiShu;  //倍数
    unsigned int JiaoJu;  //焦距
    unsigned int Val;
    double HFov;  //水平视场角
};

/*typedef struct ZhiKongGuideData_s
{
    double direction;
    double pitch;
    int distance;
    int timestamp;
}ZhiKongGuideData_s; */

using namespace std;
//using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    struct GuideData//引导数据
    {
        QTime tTime;//ms
        double dblR;//m，[0, 16777214], 16777215表示大于16777214m, 分辨率1m
        double dblA;//°，[0, 360), 分辨率0.1"
        double dblE;//°，[-90, 190], 分辨率0.1"
    };
    struct AE
    {
        double dblA;//°，[0, 360), 分辨率0.1"
        double dblE;//°，[-90, 190], 分辨率0.1"
    };
    //QString msCommCenter;
    QByteArray mzRecvBuf;//收交互引导buf
    int mnBufIdx;//滑动窗口指针
    char * mpSendBuf;//发交互buf
    bool mbCenterAsk;//是否有数据请求
    char mcSID;
    char mServoCom[16];
    //中心引导
typedef struct gTargetPos_s
    {
        double lontude;     //目标经度
        double latitude;    //目标纬度
        double    laserDist1;   //测距机1目标距离
        double    laserDist2;   //测距机2目标距离
        double    distance;   //测距机2目标距离
        int    height;      //目标高度
        double angle;       //目标偏角，相对于船尾方向
        int laserOnFlag;
        int currentIrLens;
        int currentHdLens;
        int x;
        int y;
    } gTargetPos_s;
	gTargetPos_s gTargetPos;
	
    struct IRDetectionConfig {
        int tarLmax;
        int k1;
        int k2;
        float scoreThreshold;
    };
    IRDetectionConfig irConfig;
	
    uchar mcDevID;//设备ID，仅小光学局域使用
    GuideData mCenterGuide;
    GuideData mAEJoyCorrect;
    AE mAETheo;//理论弹道引导AE
    int mnRelativeT;
    bool ParseCommData(int nIdx);//滑动窗口
    //void CommSendToCenterRAE(uchar cDID, const QTime & tTime, bool bCaught, double dblR, double dblA, double dblE);
    int sendServoComm(double dblGd1A, double dblGd1E,double dblGd2A, double dblGd2E,int coverflag);
    QLabel *titleLabel;

    //站址
    QVector3D mvStationBlh;
    QString msStationName;
    CoordTranser mCoordTrsr;
    QString msStationGdcType;

public slots:
    void Theo_timeOut();
    void stick_timeOut();
    /*void focus_timeOut();
    void zoom_timeOut(); */
    void sendStepless_timeOut();

    void chkIfOffline_timeOut();
    void zhiKongSocket_timeOut();
    void delayPid_timeOut();

private:
    Ui::MainWindow *ui;

    char serverIp_A[16];
    unsigned int serverPort_A;
    //char serverIpMask_A[16];
    //char serverIpGW[16];
    char serverIp_B[16];
    unsigned int serverPort_B;
    char mZhiKongIp[16];
    int mZhiKongPort;
    char mGyroIp[16];
    int mGyroPort,mListenGyroPort;
    char mMyIp[16];
    int mMyPort;

    char hdCamIp_A[16];
    int mHdCamPort_A;
    char hdCamIp_B[16];
    int mHdCamPort_B;

    QTcpSocket * socket;
    QUdpSocket * udpSocket_A;           // 云台A控制
    QUdpSocket * udpSocket_B;           // 云台B控制
    int posX,posY,btnStatus,scale;
    double mGyroPitch_A;
    double mGyroPitch_B;

    bool connectGyroFlag;
    bool connectFlag_A;
    bool connectFlag_B;
    bool connectRtspFlag_A;
    bool connectRtspFlag_B;
    //bool scan_enabled;  //巡航开始标记
    //QBasicTimer timer;
    //QBasicTimer timerJoyStick;
    //int timerScan;
    QTimer * timerJoystick;
    /*QTimer * timerFocusLoop;
    QTimer * timerZoomLoop; */
    QTimer * timerTheo_pp;
    QTimer * timerRecFileSeg;

    QTimer * timerTestStepless;
    int mLeftBtn = 0;
    int mRightBtn = 0;

    QTimer * timerChkIfOffline;
    QTimer * timerZhiKongSocket;
    QTimer * timerDelayPid;

    bool toAngleflag;

    double mExpo;

    //void timerEvent(QTimerEvent *event) override;

    QSerialPortInfo * SerialPortInfo;
    QSerialPort SerialPort;

    int currentPresetPos;
    int minP,maxP;
    //QThread mThdSendUcPkt;
    //QThread mThdRecvUcPkt;
    //UdpRecvThread * recvThd;
    //UdpSendThread * sendThd;

    int operateMode;
    double dd_diffx;
    double dd_diffy;
    double dd_diffx_default_A;
    double dd_diffx_default_B;
    double dd_diffy_default_A;
    double dd_diffy_default_B;

    double radar_diffx_A;
    double radar_diffx_B;
    double radar_diffy_A;
    double radar_diffy_B;

    FullScreenForm fullscreen;
    TipsDlg dlg;

    vidoeplayer * mPlayer;             //播放线程 RTSP视频流A
    QImage mImageCh1;                  //记录当前的图像
    bool mImageCh1GetNew;              //通道1图像更新标记

    vidoeplayer * mPlayerSub;          //播放线程 RTSP视频流B
    QImage mImageCh2;                  //记录当前的图像
    bool mImageCh2GetNew;              //通道2更新标记
    int currentMainSrc;                 //当前主屏源
    bool scanflag;                      //通道切换
    bool picInPicOn;                    //画中画开关
    int presetCurrentType;
    int presetCurrentIdx;
    uint32_t cfgIpAddr;
    char * rtrim(char *str);

    int hdFocusVal;
    int hdZoomVal;
    int hdLensVal;
    int hdBianBei;

    int hdFocusVal_B;
    int hdZoomVal_B;
    int hdLensVal_B;
    int hdBianBei_B;
    cv::Rect roiBox;


    int speedGear;

    bool leRecDirChanged;
    bool leSetIpChanged;
    bool leSetIPMaskChanged;
    bool leSetIPGWChanged;
    bool leAutoSpdChanged;
    bool leSetHdCamIpChanged;
    bool leCSidChanged;
    bool leSetCenterComChanged;
    long recvPktNum_A;
    long recvPktNum_B;

    HidProcThd * hid;   // Create empty HidDevice object
    Theo_PP_DLL * pTheo_PP_DLL;
    NetworkComm * pnetworkComm;        // 指控通信
    NetworkComm * pGyroComm;           // 陀螺仪通信

    TrackerThread * trackerThd;
    ctlLoopThd * workStateLoopThd;

    QTime CurTime;
    pod_cmd mPodCmd;

    double getGyroDirectionInDegrees();  // 新增：获取统一的惯导方向角（度！！！）
    void debugGyroAngles();              // 调试惯导角度
    bool validateGyroData();             // 验证惯导数据有效性

    void initGlobalParam();
    void initControlStat();
    void initTimerAndThread();
    void sendSetCmd(int type,int value);
    void sendSetCmd(int type,int val1, int val2);
    void sendSetCmd(int type,int v1,int v2,int v3,int v4);
    void sendSetCmd(int type,int panValH,int panValL,int tiltValH,int tiltValL,int hdFocal,int hdZoom,int irFocal,int irZoom);
    int  sendPTZStepless(short direction,short pitch,int hdFocal,int hdZoom,int irFocal,int irZoom);
    void sendSetIP(int type);
    int  sendPosition(double direction,double pitch,bool sendA_flag,bool sendB_flag);
    int  sendPosition3(double direction,double pitch,double ptzdirect,double ptzpitch,int interval,QUdpSocket * udpskt);
    void sendTransComData(int comid,int len,char * buf);

    void setCtlObjStatus(bool enFlag);
    bool checkip(QString ip);
    uint32_t IPV4StringToInteger(const QString& ip);
    int  readCfgFile();
    //void readOsdWordFile();
    void writeCfgFile();
    void writeCfgFile(char * connectIp);
    void updateJoyStick();  //响应摇杆操作
    int  updateJoysticktoExpo(int inVal,double expo,int maxVal,int tho);
    bool openSerialPort();
    void closeSerialPort();
    void calcScreenPos(int * x_out,int * y_out,int x_in,int y_in,int screen_w,int screen_h);

    int crtCommuThd();
    int connectGyroBc();
    void getRtspStreamA();
    void getRtspStreamB();
    void connectPtzAUc();
    void connectPtzBUc();

    int chkCrossBorder();

    int convZhiKongCameraCmd(int camVal);
    void quitAndShutdown();
    int convToRadarAE(double * outRadarPan,double * outRadarTilt,
                      double inDistance, double panVal,double tiltVal,
                      double AnZhuangX,double AnZhuangY,double AnZhuangZ);
    int convToRadarAE2(double * outRadarPan,double * outRadarTilt,
                      double inDistance, double panVal,double tiltVal,
                      double AnZhuangX,double AnZhuangY,double AnZhuangZ);

    int mStick_x;
    int mStick_y;
    int mStick_z;
    int mStickBtn;
    int mStickBtn2;
    QLabel * LabStatebarInfo;   //状态显示标签
    //PID
    struct PID2 PidX,PidY;
    float mPid_X_Scale;
    float mPid_X_P;
    float mPid_X_I;
    float mPid_X_D;
    float mPid_Y_Scale;
    float mPid_Y_P;
    float mPid_Y_I;
    float mPid_Y_D;
    int mPidMaxOut;
    int mPidMinOut;
    int mPtzDefSpeed;

    unsigned int pelco_spd_val;

    double mPtzDirection_A,mPtzPitch_A,mHdViewAngle_A;
    double mPtzDirection_B,mPtzPitch_B,mHdViewAngle_B;
    double mRawPtzDirection_A =0, mRawPtzDirection_B =0;

    int mJoy_x,mJoy_y,mJoy_btn;

    double mGD1MountPosX,mGD1MountPosY,mGD1MountPosZ;
    double mGD2MountPosX,mGD2MountPosY,mGD2MountPosZ;
    double mBorder180Begin;
    double mBorder180End;

    int mFoc_or_zoom_flag = 0;

    /*ZhiKongGuideData_s gZhiKongGuideData_s[3];
    int predict_Radar_Target(double latest_direction,double latest_pitch,int latest_dist);     //雷达引导时预测目标位置 */

    int getOptimalFocusValue(int lensVal);  // 根据焦距值获取最佳对焦位置
    void sendAutoFocusCommand(int focusValue, QUdpSocket* udpSocket);  // 发送自动对焦命令

    // 新增：基于pktType的主要处理函数
    void handleCmdIdProcessing();                    // 处理cmdId相关
    void handleStartTrackingProcessing();            // 处理startTracking相关
    void handleAlgorithmProcessing();                // 处理算法选择相关
    void handleDirectionPitchDistanceProcessing();   // 处理方位俯仰距离相关
    void handleZoomFocusProcessing();               // 处理变倍对焦相关
    void handleMouseProcessing();                   // 处理鼠标相关
    void applyTrackingAlgorithm(int algorithmId);   // 应用跟踪算法的通用函数
    void startIRSmallTargetTracking();              // 启动IR小目标跟踪
    void handleRemoteMouseClick(uint16_t mouseX, uint16_t mouseY);  // 处理远程鼠标点击

protected:	//mouse
    void mouseMoveEvent(QMouseEvent *event);         //移动
    void mousePressEvent(QMouseEvent *event);        //单击
    void mouseReleaseEvent(QMouseEvent *event);      //释放
    void mouseDoubleClickEvent(QMouseEvent *event);  //双击
    void wheelEvent(QWheelEvent *event);             //滑轮
    void sendCmd();
    QString uncharToQstring(unsigned char * id,int len);
    //void setMouseState(MouseState ms, int wheelVal);
    //void setMouseUIdefault();

    void paintEvent(QPaintEvent *event);

    //double dd_Direction,dd_Pitch;
    bool eventFilter(QObject *watched, QEvent *event);  ////事件过滤
    void drawAngleMeter(QLabel * uipad,int direction,int angle,int pitch);  //控件绘制函数
    void drawCross(QLabel * uipad, QPixmap * fitPixmap);
    void drawMouseHit(QPixmap * fitPixmap,int x,int y);
    void lblPaintAngleGap(double d1,double p1,double d2,double p2);
    void drawViewAngle(QPixmap *fitPixmap,int scrWidth,int scrHeight);
    void drawStickExpo(double expo);
    void drawROIBox(QLabel * uipad,QPixmap * fitPixmap,cv::Rect box );
    void drawMouseBox(QPixmap * fitPixmap,int x,int y);
    void drawAzimuthScale(QPixmap * fitPixmap,int scrWidth,int scrHeight);

    void drawVirtualJoyStick(int x,int y, int btn);
    void getMouseOnVirtualJoyStk(QMouseEvent *event);
    void updateVirtualJoyStick(int x,int y);
    //int ZoomValToLensData(int zoomVal,int * BeiShu,int * Lens,double * hfov);
    //int ZoomValToLensData(int zoomVal,float * BeiShu,float * Lens,double * hfov);
    int ZoomValToLensData_Improved(int in_zoomVal, int* BeiShu, int* Lens, double* hfov);
    void logZoomAndFOV(int zoomVal, int lensVal, double fov, const QString& camera);

	void autoFilmCalcTargetPosition(double * outMuBiaoWeiZhiX,double * outMuBiaoWeiZhiY, double * outMuBiaoGaoDu, double * outMuBiaoJiaoDu,int MuBiaoJuli,double panVal,double tiltVal,double AnZhuangX,double AnZhuangY,double AnZhuangZ);
    void autoAdjZoomFocal(int roi_width);
    int  gotoNewLen(int autoZoomVal);
    int getFocalFromLens(int ch,int inlensVal,int distance);
    int getZoomFromDistance(int distance);
    bool CheckAppStatus(const QString &appName);

    int adjFocusPosByDist(QUdpSocket * udpSocket,int dist);
	
    QUdpSocket * getCurMainPTZ();

private slots:
    void on_pbConnect_clicked();
    void on_pbDisConnect_clicked();
    //void on_pushButton_Send_clicked();
    void on_SendText_changed();
    void socket_Read_Data_A();
    void socket_Read_Data_B();
    void socket_Disconnected_A();
    void socket_Disconnected_B();

    void CenterSocket_Read_Data();
    void GyroSocket_Read_Data();

    void slotGetOneFrameCh1(QImage img);
    void slotGetOneFrameCh2(QImage img);
    //void slotGrabCamlinkFrame(void * raw);
    void slotGetSigBackToMainWin();       //全屏相应
    void slotGetUpdateJoystick();
    void slotUpdateTrackingBox(int num,int x,int y,int width,int height);
    void slotUpdateTrackingBoxDeg(int num,int x,int y,int width,int height);
    void slotUpdateDetectingBox(int num,int id,int x,int y,int width,int height,float prob);
    void on_pushButton_swap_clicked();

    void on_pbPicInPicOnOff_clicked();

    void on_pbUp_released();
    void on_pbDown_released();
    void on_pbLeft_released();
    void on_pbRight_released();

    void on_pbUp_pressed();
    void on_pbDown_pressed();
    void on_pbLeft_pressed();
    void on_pbRight_pressed();

    void on_pbZoomOut_pressed();
    void on_pbZoomOut_released();
    void on_pbZoomIn_pressed();
    void on_pbZoomIn_released();

    //void on_pbScan_clicked();

    void on_pbSetZero_clicked();

    void on_pbWiper_clicked();

    //void on_pbPreset_clicked();

    //void on_cbxPresetAct_currentIndexChanged(int index);

    void on_cbxPresetNo_currentIndexChanged(int index);

    //void on_cbxPresetNoEdit_currentIndexChanged(const QString &arg1);

    void on_cbxPresetNoEdit_currentIndexChanged(int index);

    //void on_pbAutoFocus_clicked();

    void on_pbFocusF_released();

    void on_pbFocusN_released();

    void on_pbFocusF_pressed();

    void on_pbFocusN_pressed();
/*
    void on_pbLoopScan_clicked();   //自动扫描


    void on_pbHGyroCtl_clicked();
    void on_pbVGyroCtl_clicked();*/
    void on_pbSetInitZero_clicked();

    //void on_pbResetMotor_clicked();

    void on_pbRec_clicked();

    void on_cbxEditLock_sys_stateChanged(int arg1);
    void on_pbConfigSave_clicked();
    void on_leRecFileDir_textChanged(const QString &arg1);
    void on_leSetIP_textChanged(const QString &arg1);
    void on_leSetIPMask_textChanged(const QString &arg1);
    void on_leSetIPGageway_textChanged(const QString &arg1);
    //void on_leManuSpeed_textChanged(const QString &arg1);
    void on_leAutoSpeed_textChanged(const QString &arg1);

    //void on_pbWrite485_clicked();

    //void on_pbCamPWR_clicked();

    void on_cbxPalette_currentIndexChanged(int index);

    void on_lineEdit_IP_returnPressed();

    void on_pbLockAxial_clicked();

    void on_pbAdjDriftXAdd_pressed();

    void on_pbAdjDriftXAdd_released();

    void on_pbAdjDriftXDec_pressed();

    void on_pbAdjDriftXDec_released();

    void on_pbAdjDriftYAdd_pressed();

    void on_pbAdjDriftYAdd_released();

    void on_pbAdjDriftYDec_pressed();

    void on_pbAdjDriftYDec_released();

    void on_pbLockAxial_2_clicked();

    void on_cbxBalance_currentIndexChanged(int index);

    void on_pbOnOffColor_clicked();

    void on_le_setDirecion_returnPressed();

    void on_le_setPitch_returnPressed();

    void on_pbSendDirection_clicked();

    void on_rb_trackStick_clicked();

    void on_rb_trackTable_clicked();

    void on_rb_Center_clicked();

    void on_leOsdWord_textChanged(const QString &arg1);

    void on_pbOsdWordSave_clicked();

    void on_rb_StickSpeedH_clicked();

    void on_rb_StickSpeedL_clicked();

    void on_pbSendPitch_clicked();

    int recvComData();

    void on_pbStop_pressed();

    void on_pbDefog_pressed();

    //void on_cbx_CfgLocal_currentIndexChanged(int index);

    //void on_pbSSetDiffY_clicked();

    //void on_pbResetDiffY_clicked();

    void timerRecFileSeg_timeOut();

    void configCAM(int camid,int cmdid,int val1,int val2);

    void slotHidChanged(int id);
    void on_leSetHdCamIP_textChanged(const QString &arg1);

    //void on_le_setStickExpo_returnPressed();

    void on_rb_StickSpeedM_clicked();
    void DrawGuideOSD(QPixmap * pixTmp, double dblCamField);
    void on_lecSID_textChanged(const QString &arg1);

    void on_leCenterCom_textChanged(const QString &arg1);

    void on_sldVidEnhance_valueChanged(int value);

    void on_sldVidBright_valueChanged(int value);

    void on_cbxDispOSDTime_clicked(bool checked);

    bool EnSetSysTimePrivilege();

    void on_cbxOSDFont_currentIndexChanged(const QString &arg1);

    void on_cbxOSDFontHeight_currentIndexChanged(const QString &arg1);

    void on_leOSDLocationY_textChanged(const QString &arg1);
	void on_rb_trackRoi40_clicked();
    void on_rb_trackRoi20_clicked();
    void on_rb_trackRoi10_clicked();
     void on_rb_KcfMarking_clicked();
    void on_rb_TopHatMarking_clicked();
    void on_rb_WhCentroidMarking_clicked();
    void on_rb_BlkCentroidMarking_clicked();
    void calcAngleSpeed();

    void on_cbxFullScreen_clicked();

    void on_pb_OpenSelfTest_clicked();

    void on_pb_CloseSelfTest_clicked();

    void on_vsld_speed_valueChanged(int value);

    void on_pbRight_Long_pressed();

    void on_pbRight_Long_released();

    void on_pbLeft_Long_pressed();

    void on_pbLeft_Long_released();

    void on_pClearbDbgOut_clicked();

    void on_pbApUp_clicked();

    void on_pbApDown_clicked();

    void on_rb_toZero_clicked();

    void on_rb_Tracking_clicked();


    void on_pbAutoFoc_clicked();

    void on_pbDbgSend_clicked();

    void on_rb_ExpoAuto_clicked();

    void on_rb_ExpoManual_clicked();

    void on_rb_ExpoShutter_clicked();

    void on_rb_ExpoIris_clicked();

    void on_pbExpUp_clicked();

    void on_pbExpDown_clicked();

    void on_pbExpReset_clicked();

    void on_rb_flipPicOff_clicked();

    void on_rb_flipPicOn_clicked();

    void on_rb_HGyroLock_clicked();

    void on_rb_HGyroUnLock_clicked();

    void on_rb_VGyroLock_clicked();

    void on_rb_VGyroUnLock_clicked();

    void on_cbx_WbMode_currentIndexChanged(int index);

    void on_pbBackLightSend_clicked();

    void on_cbx_backLightMode_currentIndexChanged(int index);

    void on_pbApReset_clicked();

    void on_pbSharpenUp_clicked();

    void on_pbSharpenDown_clicked();

    void on_pbSharpenReset_clicked();

    void on_pbShutterUp_clicked();

    void on_pbApShutterDown_clicked();

    void on_pbShutterReset_clicked();

    void on_spb_Gamma_valueChanged(int arg1);

    void on_pbSetDirection_clicked();

    void on_pbSetPitch_clicked();

    void on_pbSetMotor_clicked();

    void on_pbClearDbgCnt_clicked();

    void on_pbDbgSendZhiKong_clicked();

    void on_pbToZero_clicked();

    void on_vsld_SetBeiShu_valueChanged(int value);

    void on_pbDbgSendBorderChk_clicked();

    void on_rb_SelDetectNone_clicked();
    
    void on_rb_SelDetectMtd1_clicked();
    
    void on_rb_SelDetectMtd2_clicked();

//    void on_rb_SelIRSmallTarget_clicked();
    
    void on_pbDbgSendconvAE_clicked();

    void on_pbcalcBlh2Rae_clicked();

    void on_pbSetGD1Direction_clicked();

    void on_pbSetGD2Direction_clicked();

    void on_pbcalcRaeClear_clicked();

    void on_cbxStartTracking_stateChanged(int arg1);

    void on_pbConnect_B_clicked();

    void on_pbDisConnect_B_clicked();

    void on_rb_DefogOff_clicked();

    void on_rb_DefogOn_clicked();

    void on_pbSetMotor_B_clicked();

    void on_pbSysClose_clicked();

    void on_pbAdjDriftUp_pressed();
    void on_pbAdjDriftDown_pressed();
    void on_pbAdjDriftLeft_pressed();
    void on_pbAdjDriftRight_pressed();
    void on_pbAdjDriftStop_clicked();

    void on_pbAAdjDriftSave_clicked();

    void on_pbAdjDriftUp_released();
    void on_pbAdjDriftDown_released();
    void on_pbAdjDriftLeft_released();
    void on_pbAdjDriftRight_released();

signals:
    void mouseMove(QMouseEvent *event);//自定义一个信号
};



#endif // MAINWINDOW_H
