/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblPad_B;
    QLabel *lblPad;
    QTabWidget *tabWidConfig;
    QWidget *tab_7;
    QGroupBox *groupBox_11;
    QLabel *lbSendZhiKongPkt;
    QLabel *label_28;
    QLabel *lbSendServoPkt;
    QLabel *label_29;
    QLabel *lbRecvGyroPkt;
    QLabel *label_30;
    QPushButton *pbClearDbgCnt;
    QLabel *label_24;
    QLabel *lbRecvPkt_A;
    QLabel *label_23;
    QLabel *lbRecvFrameA;
    QLabel *label_26;
    QLabel *lbRecvPkt_B;
    QLabel *label_25;
    QLabel *lbRecvZhiKongPkt;
    QLabel *lbRecvFrameB;
    QLabel *label_27;
    QPushButton *pClearbDbgOut;
    QGroupBox *groupBox_13;
    QLabel *lbldataA_2;
    QLabel *lbGDASendServoA;
    QLabel *lbldataE_2;
    QLabel *lbGDASendServoE;
    QLabel *lbGDBSendServoA;
    QLabel *lbldataE_3;
    QLabel *lbGDBSendServoE;
    QLabel *lbldataA_3;
    QGroupBox *gbx_PicFlip_5;
    QLabel *lbRecvGyroWeiDu;
    QLabel *lbldataE_4;
    QLabel *lbRecvGyroJingdu;
    QLabel *lbldataA_4;
    QLabel *lbRecvGyroGpsDirect;
    QLabel *lbldataA_5;
    QLabel *lbRecvGyroGaodu;
    QLabel *lbldataE_5;
    QLabel *lbRecvGyroPitch;
    QLabel *lbldataA_6;
    QLabel *lbRecvGyroRoll;
    QLabel *lbRecvGyroYaw;
    QLabel *lbldataE_6;
    QLabel *lbldataA_7;
    QCheckBox *cbxIgnoreGyroData;
    QTextEdit *te_Output;
    QGroupBox *groupBox_18;
    QLabel *lbldataA_8;
    QLabel *lbRecvZhiKongFangwei;
    QLabel *lbRecvRadarFuyang;
    QLabel *lbRecvZhiKongFuyang;
    QLabel *lbRecvZhiKongDist;
    QLabel *lbRecvRadarDist;
    QGroupBox *gbx_PicFlip_4;
    QRadioButton *rb_selGDB;
    QRadioButton *rb_selGDA;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_2;
    QRadioButton *rb_SelDetectMtd1;
    QCheckBox *cbxStartTracking;
    QRadioButton *rb_SelDetectNone;
    QRadioButton *rb_SelDetectMtd2;
    QGroupBox *groupBox_5;
    QRadioButton *rb_trackStick;
    QRadioButton *rb_Center;
    QCheckBox *cbxLianDongFlag;
    QCheckBox *cbxIgnorCenterCmd;
    QLabel *label_15;
    QSlider *vsld_speed;
    QLabel *label_3;
    QSlider *vsld_SetBeiShu;
    QGroupBox *groupBox_2;
    QPushButton *pbSetMotor;
    QCheckBox *cbxPanMotor;
    QCheckBox *cbxTiltMotor;
    QGroupBox *groupBox_7;
    QPushButton *pbSetMotor_B;
    QCheckBox *cbxPanMotor_B;
    QCheckBox *cbxTiltMotor_B;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pbRec;
    QPushButton *pbToZero;
    QPushButton *pbAutoFoc;
    QPushButton *pbFocusN;
    QPushButton *pbZoomOut;
    QPushButton *pbFocusF;
    QPushButton *pbZoomIn;
    QPushButton *pbUp;
    QPushButton *pbDown;
    QPushButton *pbLeft;
    QPushButton *pbRight;
    QPushButton *pbStop;
    QGroupBox *groupBox_19;
    QLabel *lbl_lensTitle_6;
    QLabel *lbl_lenVal;
    QLabel *lblBeishu;
    QLabel *lbl_lensTitle;
    QLabel *lbl_lensTitle_5;
    QLabel *lblLens;
    QLabel *lblDirection;
    QLabel *lblPitch;
    QLabel *lbl_lensTitle_7;
    QLabel *lbl_lensTitle_8;
    QLabel *lbl_focalVal;
    QLabel *lbl_lensTitle_11;
    QGroupBox *groupBox_20;
    QLabel *lblBeishu_B;
    QLabel *lbl_lensTitle_2;
    QLabel *lbl_lenVal_B;
    QLabel *lblLens_B;
    QLabel *lbl_lensTitle_3;
    QLabel *lbl_lensTitle_4;
    QLabel *lblDirection_B;
    QLabel *lblPitch_B;
    QLabel *lbl_lensTitle_9;
    QLabel *lbl_lensTitle_10;
    QLabel *lbl_focalVal_B;
    QLabel *lbl_lensTitle_12;
    QWidget *tab_4;
    QGroupBox *groupBox;
    QRadioButton *rb_ExpoIris;
    QRadioButton *rb_ExpoManual;
    QRadioButton *rb_ExpoAuto;
    QRadioButton *rb_ExpoShutter;
    QGroupBox *gbx_WB;
    QComboBox *cbx_WbMode;
    QGroupBox *gbx_Sharpen;
    QPushButton *pbSharpenReset;
    QPushButton *pbSharpenDown;
    QPushButton *pbSharpenUp;
    QGroupBox *gbx_Gamma;
    QSpinBox *spb_Gamma;
    QGroupBox *gbx_Expo;
    QPushButton *pbExpDown;
    QPushButton *pbExpUp;
    QPushButton *pbExpReset;
    QGroupBox *gbx_backLight;
    QPushButton *pbBackLightSend;
    QComboBox *cbx_backLightMode;
    QSpinBox *sbx_cbx_backLightVal;
    QGroupBox *gbx_Expo_2;
    QPushButton *pbApReset;
    QPushButton *pbApDown;
    QPushButton *pbApUp;
    QGroupBox *gbx_Shutter;
    QPushButton *pbShutterReset;
    QPushButton *pbApShutterDown;
    QPushButton *pbShutterUp;
    QGroupBox *gbx_PicFlip;
    QRadioButton *rb_flipPicOff;
    QRadioButton *rb_flipPicOn;
    QGroupBox *gbx_PicFlip_2;
    QRadioButton *rb_DefogOff;
    QRadioButton *rb_DefogOn;
    QWidget *tab;
    QPushButton *pbOsdWordSave;
    QGroupBox *gbxPalSelect_8;
    QComboBox *cbxOSDFontSize;
    QComboBox *cbxOSDFont;
    QLabel *lblCurrentPos_22;
    QLabel *lblCurrentPos_23;
    QGroupBox *groupBox_9;
    QCheckBox *cbxCrosser;
    QCheckBox *cbxFullScreen;
    QCheckBox *cbxRoiBox;
    QCheckBox *cbxMarkline;
    QCheckBox *cbxAzimuthScale;
    QGroupBox *gbx_RoiSize_2;
    QRadioButton *rb_OSDColorGreen;
    QRadioButton *rb_0SDColorBlk;
    QRadioButton *rb_OSDColorRed;
    QRadioButton *rb_0SDColorWhite;
    QWidget *tab_2;
    QPushButton *pbSetDirection;
    QPushButton *pbSetPitch;
    QGroupBox *groupBox_14;
    QLineEdit *leGD1Blh2A;
    QLabel *lblCurrentPos_47;
    QLabel *lblCurrentPos_48;
    QLineEdit *leGD1Blh2E;
    QLabel *lblCurrentPos_49;
    QLineEdit *leGD1Blh2R;
    QPushButton *pbSetGD1Direction;
    QLabel *lblCurrentPos_59;
    QLabel *lblCurrentPos_60;
    QLabel *lblCurrentPos_61;
    QLineEdit *leGD1AntennaA;
    QLineEdit *leGD1AntennaE;
    QLabel *lblCurrentPos_68;
    QLabel *lblCurrentPos_69;
    QLabel *lblCurrentPos_56;
    QLabel *lblCurrentPos_57;
    QGroupBox *groupBox_15;
    QLineEdit *leGd1SiteB;
    QLineEdit *leGd1SiteL;
    QLabel *lblCurrentPos_50;
    QLabel *lblCurrentPos_51;
    QLineEdit *leGd2SiteL;
    QLineEdit *leGd2SiteB;
    QLineEdit *leStdPoleSiteL;
    QLineEdit *leStdPoleSiteB;
    QLabel *lblCurrentPos_52;
    QLabel *lblCurrentPos_53;
    QLabel *lblCurrentPos_54;
    QLabel *lblCurrentPos_55;
    QLineEdit *leGd2SiteH;
    QLineEdit *leGd1SiteH;
    QLineEdit *leStdPoleSiteH;
    QPushButton *pbcalcBlh2Rae;
    QGroupBox *groupBox_16;
    QLineEdit *leGD2Blh2A;
    QLabel *lblCurrentPos_62;
    QLabel *lblCurrentPos_63;
    QLineEdit *leGD2Blh2E;
    QLabel *lblCurrentPos_64;
    QLineEdit *leGD2Blh2R;
    QPushButton *pbSetGD2Direction;
    QLabel *lblCurrentPos_65;
    QLabel *lblCurrentPos_66;
    QLabel *lblCurrentPos_67;
    QLabel *lblCurrentPos_70;
    QLabel *lblCurrentPos_71;
    QLineEdit *leGD2AntennaA;
    QLabel *lblCurrentPos_58;
    QLineEdit *leGD2AntennaE;
    QLabel *lblCurrentPos_72;
    QPushButton *pbcalcRaeClear;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_9;
    QLineEdit *le_setDirecion;
    QLabel *label_10;
    QLineEdit *le_setPitch;
    QPushButton *pbSendDirection;
    QGroupBox *groupBox_3;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *lblPadDirection;
    QGroupBox *groupBox_8;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *lblPadDirection_B;
    QGroupBox *groupBox_21;
    QPushButton *pbAdjDriftDown;
    QPushButton *pbAdjDriftUp;
    QPushButton *pbAAdjDriftSave;
    QPushButton *pbAdjDriftLeft;
    QPushButton *pbAdjDriftRight;
    QPushButton *pbAdjDriftStop;
    QWidget *tab_8;
    QGroupBox *groupBox_4;
    QLabel *lblCurrentPos_15;
    QLineEdit *leSetHdCamPort;
    QLineEdit *leSetHdCamIP;
    QPushButton *pb_OpenSelfTest;
    QLineEdit *leSetIP;
    QLabel *lblCurrentPos_12;
    QLabel *lblCurrentPos_9;
    QPushButton *pb_CloseSelfTest;
    QLineEdit *leSetDiffY_default_A;
    QLabel *lbl_DiffY_default;
    QLabel *lbl_DiffX_default;
    QLineEdit *leSetDiffX_default_A;
    QLineEdit *leSetRadarDiffX_A;
    QLabel *lbl_DiffX_default_3;
    QLabel *lbl_DiffX_default_5;
    QLineEdit *leSetRadarDiffY_A;
    QLabel *lblCurrentPos_18;
    QLineEdit *leHdaDefFoc;
    QCheckBox *cbxUseDefFocA;
    QGroupBox *groupBox_6;
    QLabel *lblCurrentPos_16;
    QLineEdit *leSetHdCamPort_B;
    QLineEdit *leSetHdCamIP_B;
    QPushButton *pb_OpenSelfTest_B;
    QLineEdit *leSetIP_B;
    QLabel *lblCurrentPos_13;
    QLabel *lblCurrentPos_11;
    QPushButton *pb_CloseSelfTest_B;
    QLineEdit *leSetDiffY_default_B;
    QLineEdit *leSetDiffX_default_B;
    QLabel *lbl_DiffY_default_2;
    QLabel *lbl_DiffX_default_2;
    QLabel *lbl_DiffX_default_4;
    QLineEdit *leSetRadarDiffX_B;
    QLineEdit *leSetRadarDiffY_B;
    QLabel *lbl_DiffX_default_6;
    QLineEdit *leHdbDefFoc_2;
    QLabel *lblCurrentPos_37;
    QCheckBox *cbxUseDefFocB;
    QPushButton *pbConfigSave;
    QCheckBox *cbxEditLock_sys;
    QGroupBox *groupBox_17;
    QLabel *lblCurrentPos_28;
    QLabel *lblCurrentPos_29;
    QLabel *lblCurrentPos_30;
    QLabel *lblCurrentPos_31;
    QLabel *lblCurrentPos_32;
    QLabel *lblCurrentPos_33;
    QSplitter *splitter;
    QLineEdit *lePid_X_Scale;
    QLineEdit *lePid_X_P;
    QLineEdit *lePid_X_I;
    QLineEdit *lePid_X_D;
    QSplitter *splitter_2;
    QLineEdit *lePid_Y_Scale;
    QLineEdit *lePid_Y_P;
    QLineEdit *lePid_Y_I;
    QLineEdit *lePid_Y_D;
    QLineEdit *lePidMaxOut;
    QLabel *lblMaxOut;
    QLabel *lblMinOut;
    QLineEdit *lePidMinOut;
    QLabel *lblDefaultSpeed;
    QLineEdit *lePtzDefSpeed;
    QGroupBox *groupBox_10;
    QLineEdit *leGd1InstallX;
    QLineEdit *leGd1InstallY;
    QLabel *lblCurrentPos_20;
    QLabel *lblCurrentPos_21;
    QLineEdit *leGd2InstallY;
    QLineEdit *leGd2InstallX;
    QLineEdit *leSetBorderEnd;
    QLineEdit *leSetBorderBeg;
    QLabel *lblCurrentPos_19;
    QLabel *lblCurrentPos_24;
    QLabel *lblCurrentPos_25;
    QLabel *lblCurrentPos_26;
    QLineEdit *leGd2InstallZ;
    QLineEdit *leGd1InstallZ;
    QLineEdit *leSetListenGyroPort;
    QLineEdit *leSetGyroPort;
    QLineEdit *leRecFileDir;
    QLineEdit *leSetMyIP;
    QLabel *lblCurrentPos_17;
    QLineEdit *leSetMyPort;
    QLabel *lblCurrentPos_8;
    QLabel *lblCurrentPos_34;
    QLineEdit *leSetZhiKongIP;
    QLabel *lblCurrentPos_10;
    QLabel *lblCurrentPos_27;
    QLabel *lblCurrentPos_14;
    QLineEdit *leSetZhiKongPort;
    QLineEdit *leSetGyroIP;
    QLabel *lblCurrentPos_35;
    QLabel *lblCurrentPos_36;
    QWidget *tab_3;
    QLabel *lbDiffY;
    QLabel *label_20;
    QLabel *label_19;
    QLabel *lbDiffX;
    QLabel *lbl_SimJoystick;
    QLabel *lbSendUdpDataA;
    QLabel *lbldataE;
    QLabel *lbldataA;
    QLabel *lbSendUdpDataE;
    QGroupBox *gbx_PicFlip_3;
    QRadioButton *rb_HGyroUnLock;
    QRadioButton *rb_HGyroLock;
    QPushButton *pbDbgSendZhiKong;
    QPushButton *pbDbgSend;
    QLineEdit *le_dbgSend;
    QPushButton *pbDbgSendBorderChk;
    QLineEdit *leTestPan;
    QLineEdit *leTestTilt;
    QGroupBox *gbx_RoiSize;
    QRadioButton *rb_trackRoi40;
    QRadioButton *rb_trackRoi10;
    QRadioButton *rb_trackRoi20;
    QLabel *lblTrackingFps;
    QLabel *label_22;
    QRadioButton *rb_Tracking;
    QPushButton *pbDbgSendconvAE;
    QLineEdit *leGD_B_IP;
    QPushButton *pbDisConnect;
    QPushButton *pbDisConnect_B;
    QLabel *label_21;
    QLabel *label_18;
    QLineEdit *leGD_A_IP;
    QPushButton *pbConnect;
    QPushButton *pbConnect_B;
    QLabel *lbl_Version;
    QPushButton *pbSysClose;
    QCheckBox *cbxSetDbgDistance;
    QLineEdit *leSetDbgDistance;
    QLabel *label_6;
    QLabel *lbMousePosition_x;
    QLabel *label_5;
    QLabel *lbMousePosition_y;
    QLabel *label_31;
    QLabel *lbGyroPitch_A;
    QLabel *lerawdira;
    QLabel *lerawdirb;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1380, 880);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1380, 880));
        MainWindow->setMaximumSize(QSize(1380, 880));
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(136, 138, 133);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        centralwidget->setMinimumSize(QSize(1158, 634));
        centralwidget->setMaximumSize(QSize(19200, 10800));
        lblPad_B = new QLabel(centralwidget);
        lblPad_B->setObjectName(QStringLiteral("lblPad_B"));
        lblPad_B->setGeometry(QRect(690, 0, 690, 388));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblPad_B->sizePolicy().hasHeightForWidth());
        lblPad_B->setSizePolicy(sizePolicy1);
        lblPad_B->setMinimumSize(QSize(690, 388));
        lblPad_B->setMaximumSize(QSize(500, 282));
        lblPad_B->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 255);"));
        lblPad_B->setFrameShape(QFrame::NoFrame);
        lblPad_B->setFrameShadow(QFrame::Raised);
        lblPad_B->setLineWidth(5);
        lblPad_B->setAlignment(Qt::AlignCenter);
        lblPad = new QLabel(centralwidget);
        lblPad->setObjectName(QStringLiteral("lblPad"));
        lblPad->setGeometry(QRect(0, 0, 690, 388));
        sizePolicy1.setHeightForWidth(lblPad->sizePolicy().hasHeightForWidth());
        lblPad->setSizePolicy(sizePolicy1);
        lblPad->setMinimumSize(QSize(690, 388));
        lblPad->setMaximumSize(QSize(500, 282));
        lblPad->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 255);\n"
""));
        lblPad->setFrameShape(QFrame::NoFrame);
        lblPad->setFrameShadow(QFrame::Raised);
        lblPad->setLineWidth(5);
        lblPad->setAlignment(Qt::AlignCenter);
        lblPad->setMargin(0);
        tabWidConfig = new QTabWidget(centralwidget);
        tabWidConfig->setObjectName(QStringLiteral("tabWidConfig"));
        tabWidConfig->setEnabled(true);
        tabWidConfig->setGeometry(QRect(50, 397, 1261, 441));
        tabWidConfig->setMinimumSize(QSize(430, 400));
        tabWidConfig->setMaximumSize(QSize(2560, 500));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        tabWidConfig->setFont(font);
        tabWidConfig->setAutoFillBackground(false);
        tabWidConfig->setStyleSheet(QString::fromUtf8("QTabWidget::pane{\n"
"	border:1px groove white;\n"
"}\n"
"QTabWidget::tab-bar { \n"
"	alignment: left; \n"
"} \n"
"\n"
"QTabBar::tab{\n"
"	font: 75 12pt \"Arial\";		#\350\256\276\347\275\256\345\255\227\344\275\223\n"
"	width:84px;					#\350\256\276\347\275\256\345\256\275\345\272\246\n"
"	height:30; 					#\350\256\276\347\275\256\351\253\230\345\272\246\n"
"	margin-top:5px; 			#\350\256\276\347\275\256\350\276\271\350\267\235\n"
"	margin-right:1px;\n"
"	margin-left:1px;\n"
"	margin-bottom:0px;\n"
"}\n"
"\n"
"\n"
"\n"
"#\346\234\200\345\267\246\350\276\271\346\240\207\347\255\276\346\234\252\351\200\211\344\270\255\346\227\266\346\230\276\347\244\272\357\274\210\345\233\240\344\270\272\351\234\200\350\246\201\345\267\246\350\276\271\346\211\223\345\234\206\350\247\222\357\274\214\346\211\200\344\273\245\345\222\214\345\205\266\344\273\226\350\203\214\346\231\257\345\233\276\347\211\207\344\270\215\344\270\200\346\240\267\357\274\214\346\263\250\346\204\217\345\255\227\344\275\223\351\242\234\350\211\262\357\274"
                        "\211\n"
"QTabBar::tab:first:!selected {\n"
"	color:#000000;\n"
"	border-image: url(:/common/images/common/\345\267\246_normal.png);\n"
"}\n"
"#\346\234\200\345\267\246\350\276\271\346\240\207\347\255\276\350\242\253\351\200\211\344\270\255\n"
"QTabBar::tab:first:selected {\n"
"	color:#FFFFFF;\n"
"	border-image: url(:/common/images/common/\345\267\246_pressed.png);\n"
"}\n"
"#\346\234\200\345\217\263\350\276\271\346\240\207\347\255\276\346\234\252\351\200\211\344\270\255\346\227\266\346\230\276\347\244\272\357\274\210\345\233\240\344\270\272\351\234\200\350\246\201\345\217\263\350\276\271\346\211\223\345\234\206\350\247\222\357\274\214\346\211\200\344\273\245\345\222\214\345\205\266\344\273\226\350\203\214\346\231\257\345\233\276\347\211\207\344\270\215\344\270\200\346\240\267\357\274\211\n"
"QTabBar::tab:last:!selected {\n"
"	color:#000000;\n"
"	border-image: url(:/common/images/common/\345\217\263_normal.png);\n"
"}\n"
"#\346\234\200\345\217\263\350\276\271\346\240\207\347\255\276\350\242\253\351\200\211\344\270"
                        "\255\n"
"QTabBar::tab:last:selected {\n"
"	color:#FFFFFF;\n"
"	border-image: url(:/common/images/common/\345\217\263_pressed.png);\n"
"}\n"
"#\344\270\255\351\227\264\347\232\204\346\240\207\347\255\276\346\234\252\350\242\253\351\200\211\346\213\251\347\232\204\346\230\276\347\244\272\n"
"QTabBar::tab:!selected {\n"
"	color:#000000;\n"
"	border-image: url(:/common/images/common/\344\270\255_normal.png);\n"
"}\n"
"#\344\270\255\351\227\264\346\240\207\347\255\276\351\200\211\344\270\255\346\230\276\347\244\272\347\232\204\345\233\276\347\211\207\n"
"QTabBar::tab:selected {\n"
"	color:#FFFFFF;\n"
"	border-image: url(:/common/images/common/\344\270\255_pressed.png);\n"
"}\n"
"\n"
""));
        tabWidConfig->setTabPosition(QTabWidget::North);
        tabWidConfig->setTabShape(QTabWidget::Triangular);
        tabWidConfig->setDocumentMode(false);
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        groupBox_11 = new QGroupBox(tab_7);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(230, 237, 381, 171));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        groupBox_11->setFont(font1);
        lbSendZhiKongPkt = new QLabel(groupBox_11);
        lbSendZhiKongPkt->setObjectName(QStringLiteral("lbSendZhiKongPkt"));
        lbSendZhiKongPkt->setEnabled(true);
        lbSendZhiKongPkt->setGeometry(QRect(286, 54, 80, 21));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lbSendZhiKongPkt->sizePolicy().hasHeightForWidth());
        lbSendZhiKongPkt->setSizePolicy(sizePolicy2);
        lbSendZhiKongPkt->setMinimumSize(QSize(80, 21));
        lbSendZhiKongPkt->setMaximumSize(QSize(80, 21));
        lbSendZhiKongPkt->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_28 = new QLabel(groupBox_11);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setEnabled(true);
        label_28->setGeometry(QRect(206, 56, 70, 16));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_28->sizePolicy().hasHeightForWidth());
        label_28->setSizePolicy(sizePolicy3);
        label_28->setMinimumSize(QSize(70, 0));
        label_28->setMaximumSize(QSize(70, 16777215));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        label_28->setFont(font2);
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbSendServoPkt = new QLabel(groupBox_11);
        lbSendServoPkt->setObjectName(QStringLiteral("lbSendServoPkt"));
        lbSendServoPkt->setEnabled(true);
        lbSendServoPkt->setGeometry(QRect(286, 81, 80, 21));
        sizePolicy2.setHeightForWidth(lbSendServoPkt->sizePolicy().hasHeightForWidth());
        lbSendServoPkt->setSizePolicy(sizePolicy2);
        lbSendServoPkt->setMinimumSize(QSize(80, 21));
        lbSendServoPkt->setMaximumSize(QSize(80, 21));
        lbSendServoPkt->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_29 = new QLabel(groupBox_11);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setEnabled(true);
        label_29->setGeometry(QRect(206, 83, 70, 16));
        sizePolicy3.setHeightForWidth(label_29->sizePolicy().hasHeightForWidth());
        label_29->setSizePolicy(sizePolicy3);
        label_29->setMinimumSize(QSize(70, 0));
        label_29->setMaximumSize(QSize(70, 16777215));
        label_29->setFont(font2);
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroPkt = new QLabel(groupBox_11);
        lbRecvGyroPkt->setObjectName(QStringLiteral("lbRecvGyroPkt"));
        lbRecvGyroPkt->setEnabled(true);
        lbRecvGyroPkt->setGeometry(QRect(286, 108, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroPkt->sizePolicy().hasHeightForWidth());
        lbRecvGyroPkt->setSizePolicy(sizePolicy2);
        lbRecvGyroPkt->setMinimumSize(QSize(80, 21));
        lbRecvGyroPkt->setMaximumSize(QSize(80, 21));
        lbRecvGyroPkt->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_30 = new QLabel(groupBox_11);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setEnabled(true);
        label_30->setGeometry(QRect(206, 110, 70, 16));
        sizePolicy3.setHeightForWidth(label_30->sizePolicy().hasHeightForWidth());
        label_30->setSizePolicy(sizePolicy3);
        label_30->setMinimumSize(QSize(70, 0));
        label_30->setMaximumSize(QSize(70, 16777215));
        label_30->setFont(font2);
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbClearDbgCnt = new QPushButton(groupBox_11);
        pbClearDbgCnt->setObjectName(QStringLiteral("pbClearDbgCnt"));
        pbClearDbgCnt->setGeometry(QRect(301, 140, 50, 21));
        pbClearDbgCnt->setMinimumSize(QSize(50, 21));
        pbClearDbgCnt->setMaximumSize(QSize(50, 21));
        pbClearDbgCnt->setFont(font2);
        pbClearDbgCnt->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        label_24 = new QLabel(groupBox_11);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setEnabled(true);
        label_24->setGeometry(QRect(9, 56, 100, 16));
        sizePolicy3.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy3);
        label_24->setMinimumSize(QSize(100, 0));
        label_24->setMaximumSize(QSize(100, 16777215));
        label_24->setFont(font2);
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvPkt_A = new QLabel(groupBox_11);
        lbRecvPkt_A->setObjectName(QStringLiteral("lbRecvPkt_A"));
        lbRecvPkt_A->setEnabled(true);
        lbRecvPkt_A->setGeometry(QRect(115, 27, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvPkt_A->sizePolicy().hasHeightForWidth());
        lbRecvPkt_A->setSizePolicy(sizePolicy2);
        lbRecvPkt_A->setMinimumSize(QSize(80, 21));
        lbRecvPkt_A->setMaximumSize(QSize(80, 21));
        lbRecvPkt_A->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_23 = new QLabel(groupBox_11);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setEnabled(true);
        label_23->setGeometry(QRect(9, 29, 100, 16));
        sizePolicy3.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy3);
        label_23->setMinimumSize(QSize(100, 0));
        label_23->setMaximumSize(QSize(100, 16777215));
        label_23->setFont(font2);
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvFrameA = new QLabel(groupBox_11);
        lbRecvFrameA->setObjectName(QStringLiteral("lbRecvFrameA"));
        lbRecvFrameA->setEnabled(true);
        lbRecvFrameA->setGeometry(QRect(115, 54, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvFrameA->sizePolicy().hasHeightForWidth());
        lbRecvFrameA->setSizePolicy(sizePolicy2);
        lbRecvFrameA->setMinimumSize(QSize(80, 21));
        lbRecvFrameA->setMaximumSize(QSize(80, 21));
        lbRecvFrameA->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_26 = new QLabel(groupBox_11);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setEnabled(true);
        label_26->setGeometry(QRect(9, 83, 100, 16));
        sizePolicy3.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy3);
        label_26->setMinimumSize(QSize(100, 0));
        label_26->setMaximumSize(QSize(100, 16777215));
        label_26->setFont(font2);
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvPkt_B = new QLabel(groupBox_11);
        lbRecvPkt_B->setObjectName(QStringLiteral("lbRecvPkt_B"));
        lbRecvPkt_B->setEnabled(true);
        lbRecvPkt_B->setGeometry(QRect(115, 81, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvPkt_B->sizePolicy().hasHeightForWidth());
        lbRecvPkt_B->setSizePolicy(sizePolicy2);
        lbRecvPkt_B->setMinimumSize(QSize(80, 21));
        lbRecvPkt_B->setMaximumSize(QSize(80, 21));
        lbRecvPkt_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_25 = new QLabel(groupBox_11);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setEnabled(true);
        label_25->setGeometry(QRect(9, 110, 100, 16));
        sizePolicy3.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy3);
        label_25->setMinimumSize(QSize(100, 0));
        label_25->setMaximumSize(QSize(100, 16777215));
        label_25->setFont(font2);
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvZhiKongPkt = new QLabel(groupBox_11);
        lbRecvZhiKongPkt->setObjectName(QStringLiteral("lbRecvZhiKongPkt"));
        lbRecvZhiKongPkt->setEnabled(true);
        lbRecvZhiKongPkt->setGeometry(QRect(287, 27, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvZhiKongPkt->sizePolicy().hasHeightForWidth());
        lbRecvZhiKongPkt->setSizePolicy(sizePolicy2);
        lbRecvZhiKongPkt->setMinimumSize(QSize(80, 21));
        lbRecvZhiKongPkt->setMaximumSize(QSize(80, 21));
        lbRecvZhiKongPkt->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbRecvFrameB = new QLabel(groupBox_11);
        lbRecvFrameB->setObjectName(QStringLiteral("lbRecvFrameB"));
        lbRecvFrameB->setEnabled(true);
        lbRecvFrameB->setGeometry(QRect(115, 108, 80, 21));
        sizePolicy2.setHeightForWidth(lbRecvFrameB->sizePolicy().hasHeightForWidth());
        lbRecvFrameB->setSizePolicy(sizePolicy2);
        lbRecvFrameB->setMinimumSize(QSize(80, 21));
        lbRecvFrameB->setMaximumSize(QSize(80, 21));
        lbRecvFrameB->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_27 = new QLabel(groupBox_11);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setEnabled(true);
        label_27->setGeometry(QRect(206, 29, 70, 16));
        sizePolicy3.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy3);
        label_27->setMinimumSize(QSize(70, 0));
        label_27->setMaximumSize(QSize(70, 16777215));
        label_27->setFont(font2);
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pClearbDbgOut = new QPushButton(tab_7);
        pClearbDbgOut->setObjectName(QStringLiteral("pClearbDbgOut"));
        pClearbDbgOut->setGeometry(QRect(160, 380, 50, 21));
        sizePolicy3.setHeightForWidth(pClearbDbgOut->sizePolicy().hasHeightForWidth());
        pClearbDbgOut->setSizePolicy(sizePolicy3);
        pClearbDbgOut->setFont(font2);
        pClearbDbgOut->setLayoutDirection(Qt::LeftToRight);
        pClearbDbgOut->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pClearbDbgOut->setAutoRepeatDelay(10);
        pClearbDbgOut->setAutoRepeatInterval(10);
        groupBox_13 = new QGroupBox(tab_7);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setGeometry(QRect(230, 10, 381, 91));
        groupBox_13->setFont(font1);
        lbldataA_2 = new QLabel(groupBox_13);
        lbldataA_2->setObjectName(QStringLiteral("lbldataA_2"));
        lbldataA_2->setEnabled(true);
        lbldataA_2->setGeometry(QRect(10, 38, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_2->sizePolicy().hasHeightForWidth());
        lbldataA_2->setSizePolicy(sizePolicy3);
        lbldataA_2->setMinimumSize(QSize(30, 0));
        lbldataA_2->setMaximumSize(QSize(100, 16777215));
        lbldataA_2->setFont(font2);
        lbldataA_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbGDASendServoA = new QLabel(groupBox_13);
        lbGDASendServoA->setObjectName(QStringLiteral("lbGDASendServoA"));
        lbGDASendServoA->setEnabled(true);
        lbGDASendServoA->setGeometry(QRect(77, 37, 110, 21));
        sizePolicy2.setHeightForWidth(lbGDASendServoA->sizePolicy().hasHeightForWidth());
        lbGDASendServoA->setSizePolicy(sizePolicy2);
        lbGDASendServoA->setMinimumSize(QSize(110, 21));
        lbGDASendServoA->setMaximumSize(QSize(110, 21));
        QFont font3;
        font3.setPointSize(12);
        lbGDASendServoA->setFont(font3);
        lbGDASendServoA->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE_2 = new QLabel(groupBox_13);
        lbldataE_2->setObjectName(QStringLiteral("lbldataE_2"));
        lbldataE_2->setEnabled(true);
        lbldataE_2->setGeometry(QRect(10, 59, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataE_2->sizePolicy().hasHeightForWidth());
        lbldataE_2->setSizePolicy(sizePolicy3);
        lbldataE_2->setMinimumSize(QSize(30, 0));
        lbldataE_2->setMaximumSize(QSize(100, 16777215));
        lbldataE_2->setFont(font2);
        lbldataE_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbGDASendServoE = new QLabel(groupBox_13);
        lbGDASendServoE->setObjectName(QStringLiteral("lbGDASendServoE"));
        lbGDASendServoE->setEnabled(true);
        lbGDASendServoE->setGeometry(QRect(77, 59, 110, 21));
        sizePolicy2.setHeightForWidth(lbGDASendServoE->sizePolicy().hasHeightForWidth());
        lbGDASendServoE->setSizePolicy(sizePolicy2);
        lbGDASendServoE->setMinimumSize(QSize(110, 21));
        lbGDASendServoE->setMaximumSize(QSize(110, 21));
        lbGDASendServoE->setFont(font3);
        lbGDASendServoE->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbGDBSendServoA = new QLabel(groupBox_13);
        lbGDBSendServoA->setObjectName(QStringLiteral("lbGDBSendServoA"));
        lbGDBSendServoA->setEnabled(true);
        lbGDBSendServoA->setGeometry(QRect(266, 37, 110, 21));
        sizePolicy2.setHeightForWidth(lbGDBSendServoA->sizePolicy().hasHeightForWidth());
        lbGDBSendServoA->setSizePolicy(sizePolicy2);
        lbGDBSendServoA->setMinimumSize(QSize(110, 21));
        lbGDBSendServoA->setMaximumSize(QSize(110, 21));
        lbGDBSendServoA->setFont(font3);
        lbGDBSendServoA->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE_3 = new QLabel(groupBox_13);
        lbldataE_3->setObjectName(QStringLiteral("lbldataE_3"));
        lbldataE_3->setEnabled(true);
        lbldataE_3->setGeometry(QRect(200, 59, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataE_3->sizePolicy().hasHeightForWidth());
        lbldataE_3->setSizePolicy(sizePolicy3);
        lbldataE_3->setMinimumSize(QSize(30, 0));
        lbldataE_3->setMaximumSize(QSize(100, 16777215));
        lbldataE_3->setFont(font2);
        lbldataE_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbGDBSendServoE = new QLabel(groupBox_13);
        lbGDBSendServoE->setObjectName(QStringLiteral("lbGDBSendServoE"));
        lbGDBSendServoE->setEnabled(true);
        lbGDBSendServoE->setGeometry(QRect(266, 59, 110, 21));
        sizePolicy2.setHeightForWidth(lbGDBSendServoE->sizePolicy().hasHeightForWidth());
        lbGDBSendServoE->setSizePolicy(sizePolicy2);
        lbGDBSendServoE->setMinimumSize(QSize(110, 21));
        lbGDBSendServoE->setMaximumSize(QSize(110, 21));
        lbGDBSendServoE->setFont(font3);
        lbGDBSendServoE->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataA_3 = new QLabel(groupBox_13);
        lbldataA_3->setObjectName(QStringLiteral("lbldataA_3"));
        lbldataA_3->setEnabled(true);
        lbldataA_3->setGeometry(QRect(200, 38, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_3->sizePolicy().hasHeightForWidth());
        lbldataA_3->setSizePolicy(sizePolicy3);
        lbldataA_3->setMinimumSize(QSize(30, 0));
        lbldataA_3->setMaximumSize(QSize(100, 16777215));
        lbldataA_3->setFont(font2);
        lbldataA_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gbx_PicFlip_5 = new QGroupBox(tab_7);
        gbx_PicFlip_5->setObjectName(QStringLiteral("gbx_PicFlip_5"));
        gbx_PicFlip_5->setGeometry(QRect(230, 105, 381, 131));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(gbx_PicFlip_5->sizePolicy().hasHeightForWidth());
        gbx_PicFlip_5->setSizePolicy(sizePolicy4);
        gbx_PicFlip_5->setMinimumSize(QSize(100, 70));
        gbx_PicFlip_5->setMaximumSize(QSize(400, 200));
        gbx_PicFlip_5->setFont(font1);
        lbRecvGyroWeiDu = new QLabel(gbx_PicFlip_5);
        lbRecvGyroWeiDu->setObjectName(QStringLiteral("lbRecvGyroWeiDu"));
        lbRecvGyroWeiDu->setEnabled(true);
        lbRecvGyroWeiDu->setGeometry(QRect(77, 29, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroWeiDu->sizePolicy().hasHeightForWidth());
        lbRecvGyroWeiDu->setSizePolicy(sizePolicy2);
        lbRecvGyroWeiDu->setMinimumSize(QSize(110, 21));
        lbRecvGyroWeiDu->setMaximumSize(QSize(110, 21));
        lbRecvGyroWeiDu->setFont(font3);
        lbRecvGyroWeiDu->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE_4 = new QLabel(gbx_PicFlip_5);
        lbldataE_4->setObjectName(QStringLiteral("lbldataE_4"));
        lbldataE_4->setEnabled(true);
        lbldataE_4->setGeometry(QRect(10, 51, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataE_4->sizePolicy().hasHeightForWidth());
        lbldataE_4->setSizePolicy(sizePolicy3);
        lbldataE_4->setMinimumSize(QSize(30, 0));
        lbldataE_4->setMaximumSize(QSize(100, 16777215));
        lbldataE_4->setFont(font2);
        lbldataE_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroJingdu = new QLabel(gbx_PicFlip_5);
        lbRecvGyroJingdu->setObjectName(QStringLiteral("lbRecvGyroJingdu"));
        lbRecvGyroJingdu->setEnabled(true);
        lbRecvGyroJingdu->setGeometry(QRect(77, 51, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroJingdu->sizePolicy().hasHeightForWidth());
        lbRecvGyroJingdu->setSizePolicy(sizePolicy2);
        lbRecvGyroJingdu->setMinimumSize(QSize(110, 21));
        lbRecvGyroJingdu->setMaximumSize(QSize(110, 21));
        lbRecvGyroJingdu->setFont(font3);
        lbRecvGyroJingdu->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataA_4 = new QLabel(gbx_PicFlip_5);
        lbldataA_4->setObjectName(QStringLiteral("lbldataA_4"));
        lbldataA_4->setEnabled(true);
        lbldataA_4->setGeometry(QRect(10, 30, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_4->sizePolicy().hasHeightForWidth());
        lbldataA_4->setSizePolicy(sizePolicy3);
        lbldataA_4->setMinimumSize(QSize(30, 0));
        lbldataA_4->setMaximumSize(QSize(100, 16777215));
        lbldataA_4->setFont(font2);
        lbldataA_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroGpsDirect = new QLabel(gbx_PicFlip_5);
        lbRecvGyroGpsDirect->setObjectName(QStringLiteral("lbRecvGyroGpsDirect"));
        lbRecvGyroGpsDirect->setEnabled(true);
        lbRecvGyroGpsDirect->setGeometry(QRect(77, 95, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroGpsDirect->sizePolicy().hasHeightForWidth());
        lbRecvGyroGpsDirect->setSizePolicy(sizePolicy2);
        lbRecvGyroGpsDirect->setMinimumSize(QSize(110, 21));
        lbRecvGyroGpsDirect->setMaximumSize(QSize(110, 21));
        lbRecvGyroGpsDirect->setFont(font3);
        lbRecvGyroGpsDirect->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataA_5 = new QLabel(gbx_PicFlip_5);
        lbldataA_5->setObjectName(QStringLiteral("lbldataA_5"));
        lbldataA_5->setEnabled(true);
        lbldataA_5->setGeometry(QRect(10, 74, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_5->sizePolicy().hasHeightForWidth());
        lbldataA_5->setSizePolicy(sizePolicy3);
        lbldataA_5->setMinimumSize(QSize(30, 0));
        lbldataA_5->setMaximumSize(QSize(100, 16777215));
        lbldataA_5->setFont(font2);
        lbldataA_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroGaodu = new QLabel(gbx_PicFlip_5);
        lbRecvGyroGaodu->setObjectName(QStringLiteral("lbRecvGyroGaodu"));
        lbRecvGyroGaodu->setEnabled(true);
        lbRecvGyroGaodu->setGeometry(QRect(77, 73, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroGaodu->sizePolicy().hasHeightForWidth());
        lbRecvGyroGaodu->setSizePolicy(sizePolicy2);
        lbRecvGyroGaodu->setMinimumSize(QSize(110, 21));
        lbRecvGyroGaodu->setMaximumSize(QSize(110, 21));
        lbRecvGyroGaodu->setFont(font3);
        lbRecvGyroGaodu->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE_5 = new QLabel(gbx_PicFlip_5);
        lbldataE_5->setObjectName(QStringLiteral("lbldataE_5"));
        lbldataE_5->setEnabled(true);
        lbldataE_5->setGeometry(QRect(10, 95, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataE_5->sizePolicy().hasHeightForWidth());
        lbldataE_5->setSizePolicy(sizePolicy3);
        lbldataE_5->setMinimumSize(QSize(30, 0));
        lbldataE_5->setMaximumSize(QSize(100, 16777215));
        lbldataE_5->setFont(font2);
        lbldataE_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroPitch = new QLabel(gbx_PicFlip_5);
        lbRecvGyroPitch->setObjectName(QStringLiteral("lbRecvGyroPitch"));
        lbRecvGyroPitch->setEnabled(true);
        lbRecvGyroPitch->setGeometry(QRect(266, 51, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroPitch->sizePolicy().hasHeightForWidth());
        lbRecvGyroPitch->setSizePolicy(sizePolicy2);
        lbRecvGyroPitch->setMinimumSize(QSize(110, 21));
        lbRecvGyroPitch->setMaximumSize(QSize(110, 21));
        lbRecvGyroPitch->setFont(font3);
        lbRecvGyroPitch->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataA_6 = new QLabel(gbx_PicFlip_5);
        lbldataA_6->setObjectName(QStringLiteral("lbldataA_6"));
        lbldataA_6->setEnabled(true);
        lbldataA_6->setGeometry(QRect(199, 30, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_6->sizePolicy().hasHeightForWidth());
        lbldataA_6->setSizePolicy(sizePolicy3);
        lbldataA_6->setMinimumSize(QSize(30, 0));
        lbldataA_6->setMaximumSize(QSize(100, 16777215));
        lbldataA_6->setFont(font2);
        lbldataA_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvGyroRoll = new QLabel(gbx_PicFlip_5);
        lbRecvGyroRoll->setObjectName(QStringLiteral("lbRecvGyroRoll"));
        lbRecvGyroRoll->setEnabled(true);
        lbRecvGyroRoll->setGeometry(QRect(266, 29, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroRoll->sizePolicy().hasHeightForWidth());
        lbRecvGyroRoll->setSizePolicy(sizePolicy2);
        lbRecvGyroRoll->setMinimumSize(QSize(110, 21));
        lbRecvGyroRoll->setMaximumSize(QSize(110, 21));
        lbRecvGyroRoll->setFont(font3);
        lbRecvGyroRoll->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbRecvGyroYaw = new QLabel(gbx_PicFlip_5);
        lbRecvGyroYaw->setObjectName(QStringLiteral("lbRecvGyroYaw"));
        lbRecvGyroYaw->setEnabled(true);
        lbRecvGyroYaw->setGeometry(QRect(266, 73, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvGyroYaw->sizePolicy().hasHeightForWidth());
        lbRecvGyroYaw->setSizePolicy(sizePolicy2);
        lbRecvGyroYaw->setMinimumSize(QSize(110, 21));
        lbRecvGyroYaw->setMaximumSize(QSize(110, 21));
        lbRecvGyroYaw->setFont(font3);
        lbRecvGyroYaw->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE_6 = new QLabel(gbx_PicFlip_5);
        lbldataE_6->setObjectName(QStringLiteral("lbldataE_6"));
        lbldataE_6->setEnabled(true);
        lbldataE_6->setGeometry(QRect(199, 51, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataE_6->sizePolicy().hasHeightForWidth());
        lbldataE_6->setSizePolicy(sizePolicy3);
        lbldataE_6->setMinimumSize(QSize(30, 0));
        lbldataE_6->setMaximumSize(QSize(100, 16777215));
        lbldataE_6->setFont(font2);
        lbldataE_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbldataA_7 = new QLabel(gbx_PicFlip_5);
        lbldataA_7->setObjectName(QStringLiteral("lbldataA_7"));
        lbldataA_7->setEnabled(true);
        lbldataA_7->setGeometry(QRect(199, 74, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_7->sizePolicy().hasHeightForWidth());
        lbldataA_7->setSizePolicy(sizePolicy3);
        lbldataA_7->setMinimumSize(QSize(30, 0));
        lbldataA_7->setMaximumSize(QSize(100, 16777215));
        lbldataA_7->setFont(font2);
        lbldataA_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cbxIgnoreGyroData = new QCheckBox(gbx_PicFlip_5);
        cbxIgnoreGyroData->setObjectName(QStringLiteral("cbxIgnoreGyroData"));
        cbxIgnoreGyroData->setGeometry(QRect(232, 105, 141, 16));
        cbxIgnoreGyroData->setFont(font1);
        cbxIgnoreGyroData->setChecked(false);
        te_Output = new QTextEdit(tab_7);
        te_Output->setObjectName(QStringLiteral("te_Output"));
        te_Output->setGeometry(QRect(10, 136, 211, 271));
        te_Output->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        groupBox_18 = new QGroupBox(tab_7);
        groupBox_18->setObjectName(QStringLiteral("groupBox_18"));
        groupBox_18->setGeometry(QRect(10, 10, 211, 111));
        groupBox_18->setFont(font1);
        lbldataA_8 = new QLabel(groupBox_18);
        lbldataA_8->setObjectName(QStringLiteral("lbldataA_8"));
        lbldataA_8->setEnabled(true);
        lbldataA_8->setGeometry(QRect(10, 38, 61, 20));
        sizePolicy3.setHeightForWidth(lbldataA_8->sizePolicy().hasHeightForWidth());
        lbldataA_8->setSizePolicy(sizePolicy3);
        lbldataA_8->setMinimumSize(QSize(30, 0));
        lbldataA_8->setMaximumSize(QSize(100, 16777215));
        lbldataA_8->setFont(font2);
        lbldataA_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvZhiKongFangwei = new QLabel(groupBox_18);
        lbRecvZhiKongFangwei->setObjectName(QStringLiteral("lbRecvZhiKongFangwei"));
        lbRecvZhiKongFangwei->setEnabled(true);
        lbRecvZhiKongFangwei->setGeometry(QRect(77, 37, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvZhiKongFangwei->sizePolicy().hasHeightForWidth());
        lbRecvZhiKongFangwei->setSizePolicy(sizePolicy2);
        lbRecvZhiKongFangwei->setMinimumSize(QSize(110, 21));
        lbRecvZhiKongFangwei->setMaximumSize(QSize(110, 21));
        lbRecvZhiKongFangwei->setFont(font3);
        lbRecvZhiKongFangwei->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbRecvRadarFuyang = new QLabel(groupBox_18);
        lbRecvRadarFuyang->setObjectName(QStringLiteral("lbRecvRadarFuyang"));
        lbRecvRadarFuyang->setEnabled(true);
        lbRecvRadarFuyang->setGeometry(QRect(10, 59, 61, 20));
        sizePolicy3.setHeightForWidth(lbRecvRadarFuyang->sizePolicy().hasHeightForWidth());
        lbRecvRadarFuyang->setSizePolicy(sizePolicy3);
        lbRecvRadarFuyang->setMinimumSize(QSize(30, 0));
        lbRecvRadarFuyang->setMaximumSize(QSize(100, 16777215));
        lbRecvRadarFuyang->setFont(font2);
        lbRecvRadarFuyang->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbRecvZhiKongFuyang = new QLabel(groupBox_18);
        lbRecvZhiKongFuyang->setObjectName(QStringLiteral("lbRecvZhiKongFuyang"));
        lbRecvZhiKongFuyang->setEnabled(true);
        lbRecvZhiKongFuyang->setGeometry(QRect(77, 59, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvZhiKongFuyang->sizePolicy().hasHeightForWidth());
        lbRecvZhiKongFuyang->setSizePolicy(sizePolicy2);
        lbRecvZhiKongFuyang->setMinimumSize(QSize(110, 21));
        lbRecvZhiKongFuyang->setMaximumSize(QSize(110, 21));
        lbRecvZhiKongFuyang->setFont(font3);
        lbRecvZhiKongFuyang->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbRecvZhiKongDist = new QLabel(groupBox_18);
        lbRecvZhiKongDist->setObjectName(QStringLiteral("lbRecvZhiKongDist"));
        lbRecvZhiKongDist->setEnabled(true);
        lbRecvZhiKongDist->setGeometry(QRect(77, 81, 110, 21));
        sizePolicy2.setHeightForWidth(lbRecvZhiKongDist->sizePolicy().hasHeightForWidth());
        lbRecvZhiKongDist->setSizePolicy(sizePolicy2);
        lbRecvZhiKongDist->setMinimumSize(QSize(110, 21));
        lbRecvZhiKongDist->setMaximumSize(QSize(110, 21));
        lbRecvZhiKongDist->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbRecvRadarDist = new QLabel(groupBox_18);
        lbRecvRadarDist->setObjectName(QStringLiteral("lbRecvRadarDist"));
        lbRecvRadarDist->setEnabled(true);
        lbRecvRadarDist->setGeometry(QRect(11, 83, 61, 16));
        sizePolicy3.setHeightForWidth(lbRecvRadarDist->sizePolicy().hasHeightForWidth());
        lbRecvRadarDist->setSizePolicy(sizePolicy3);
        lbRecvRadarDist->setMinimumSize(QSize(61, 0));
        lbRecvRadarDist->setMaximumSize(QSize(61, 16777215));
        lbRecvRadarDist->setFont(font2);
        lbRecvRadarDist->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gbx_PicFlip_4 = new QGroupBox(tab_7);
        gbx_PicFlip_4->setObjectName(QStringLiteral("gbx_PicFlip_4"));
        gbx_PicFlip_4->setGeometry(QRect(940, 309, 121, 100));
        sizePolicy4.setHeightForWidth(gbx_PicFlip_4->sizePolicy().hasHeightForWidth());
        gbx_PicFlip_4->setSizePolicy(sizePolicy4);
        gbx_PicFlip_4->setMinimumSize(QSize(100, 70));
        gbx_PicFlip_4->setMaximumSize(QSize(150, 100));
        gbx_PicFlip_4->setFont(font1);
        rb_selGDB = new QRadioButton(gbx_PicFlip_4);
        rb_selGDB->setObjectName(QStringLiteral("rb_selGDB"));
        rb_selGDB->setGeometry(QRect(9, 57, 70, 21));
        sizePolicy3.setHeightForWidth(rb_selGDB->sizePolicy().hasHeightForWidth());
        rb_selGDB->setSizePolicy(sizePolicy3);
        rb_selGDB->setMinimumSize(QSize(70, 0));
        rb_selGDB->setMaximumSize(QSize(50, 16777215));
        QFont font4;
        font4.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        rb_selGDB->setFont(font4);
        rb_selGDB->setChecked(false);
        rb_selGDA = new QRadioButton(gbx_PicFlip_4);
        rb_selGDA->setObjectName(QStringLiteral("rb_selGDA"));
        rb_selGDA->setEnabled(true);
        rb_selGDA->setGeometry(QRect(9, 34, 70, 21));
        sizePolicy3.setHeightForWidth(rb_selGDA->sizePolicy().hasHeightForWidth());
        rb_selGDA->setSizePolicy(sizePolicy3);
        rb_selGDA->setMinimumSize(QSize(70, 0));
        rb_selGDA->setMaximumSize(QSize(50, 16777215));
        rb_selGDA->setFont(font4);
        rb_selGDA->setChecked(true);
        groupBox_12 = new QGroupBox(tab_7);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(810, 10, 111, 191));
        groupBox_12->setMaximumSize(QSize(150, 191));
        groupBox_12->setFont(font1);
        gridLayout_2 = new QGridLayout(groupBox_12);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        rb_SelDetectMtd1 = new QRadioButton(groupBox_12);
        rb_SelDetectMtd1->setObjectName(QStringLiteral("rb_SelDetectMtd1"));
        rb_SelDetectMtd1->setEnabled(true);
        sizePolicy3.setHeightForWidth(rb_SelDetectMtd1->sizePolicy().hasHeightForWidth());
        rb_SelDetectMtd1->setSizePolicy(sizePolicy3);
        rb_SelDetectMtd1->setMinimumSize(QSize(70, 0));
        rb_SelDetectMtd1->setMaximumSize(QSize(70, 16777215));
        rb_SelDetectMtd1->setFont(font4);
        rb_SelDetectMtd1->setChecked(false);

        gridLayout_2->addWidget(rb_SelDetectMtd1, 1, 0, 1, 1);

        cbxStartTracking = new QCheckBox(groupBox_12);
        cbxStartTracking->setObjectName(QStringLiteral("cbxStartTracking"));
        cbxStartTracking->setMinimumSize(QSize(0, 60));
        cbxStartTracking->setFont(font1);

        gridLayout_2->addWidget(cbxStartTracking, 3, 0, 1, 1);

        rb_SelDetectNone = new QRadioButton(groupBox_12);
        rb_SelDetectNone->setObjectName(QStringLiteral("rb_SelDetectNone"));
        rb_SelDetectNone->setEnabled(true);
        sizePolicy3.setHeightForWidth(rb_SelDetectNone->sizePolicy().hasHeightForWidth());
        rb_SelDetectNone->setSizePolicy(sizePolicy3);
        rb_SelDetectNone->setMinimumSize(QSize(70, 0));
        rb_SelDetectNone->setMaximumSize(QSize(70, 16777215));
        rb_SelDetectNone->setFont(font4);
        rb_SelDetectNone->setChecked(false);

        gridLayout_2->addWidget(rb_SelDetectNone, 0, 0, 1, 1);

        rb_SelDetectMtd2 = new QRadioButton(groupBox_12);
        rb_SelDetectMtd2->setObjectName(QStringLiteral("rb_SelDetectMtd2"));
        rb_SelDetectMtd2->setEnabled(true);
        sizePolicy3.setHeightForWidth(rb_SelDetectMtd2->sizePolicy().hasHeightForWidth());
        rb_SelDetectMtd2->setSizePolicy(sizePolicy3);
        rb_SelDetectMtd2->setMinimumSize(QSize(70, 0));
        rb_SelDetectMtd2->setMaximumSize(QSize(70, 16777215));
        rb_SelDetectMtd2->setFont(font4);
        rb_SelDetectMtd2->setChecked(true);

        gridLayout_2->addWidget(rb_SelDetectMtd2, 2, 0, 1, 1);

        groupBox_5 = new QGroupBox(tab_7);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(810, 218, 121, 191));
        sizePolicy4.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy4);
        groupBox_5->setMinimumSize(QSize(100, 100));
        groupBox_5->setMaximumSize(QSize(150, 191));
        groupBox_5->setFont(font1);
        rb_trackStick = new QRadioButton(groupBox_5);
        rb_trackStick->setObjectName(QStringLiteral("rb_trackStick"));
        rb_trackStick->setGeometry(QRect(9, 27, 90, 21));
        sizePolicy3.setHeightForWidth(rb_trackStick->sizePolicy().hasHeightForWidth());
        rb_trackStick->setSizePolicy(sizePolicy3);
        rb_trackStick->setMinimumSize(QSize(90, 0));
        rb_trackStick->setMaximumSize(QSize(90, 16777215));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font5.setPointSize(12);
        font5.setBold(true);
        font5.setWeight(75);
        rb_trackStick->setFont(font5);
        rb_trackStick->setChecked(true);
        rb_Center = new QRadioButton(groupBox_5);
        rb_Center->setObjectName(QStringLiteral("rb_Center"));
        rb_Center->setEnabled(true);
        rb_Center->setGeometry(QRect(9, 54, 90, 21));
        sizePolicy3.setHeightForWidth(rb_Center->sizePolicy().hasHeightForWidth());
        rb_Center->setSizePolicy(sizePolicy3);
        rb_Center->setMinimumSize(QSize(90, 0));
        rb_Center->setMaximumSize(QSize(90, 16777215));
        rb_Center->setFont(font5);
        cbxLianDongFlag = new QCheckBox(groupBox_5);
        cbxLianDongFlag->setObjectName(QStringLiteral("cbxLianDongFlag"));
        cbxLianDongFlag->setGeometry(QRect(10, 120, 81, 16));
        cbxLianDongFlag->setFont(font1);
        cbxLianDongFlag->setChecked(false);
        cbxIgnorCenterCmd = new QCheckBox(groupBox_5);
        cbxIgnorCenterCmd->setObjectName(QStringLiteral("cbxIgnorCenterCmd"));
        cbxIgnorCenterCmd->setGeometry(QRect(10, 150, 91, 16));
        cbxIgnorCenterCmd->setFont(font1);
        cbxIgnorCenterCmd->setChecked(false);
        label_15 = new QLabel(tab_7);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(1065, 280, 54, 20));
        label_15->setFont(font1);
        label_15->setAlignment(Qt::AlignCenter);
        vsld_speed = new QSlider(tab_7);
        vsld_speed->setObjectName(QStringLiteral("vsld_speed"));
        vsld_speed->setGeometry(QRect(1126, 310, 121, 21));
        vsld_speed->setMinimum(1);
        vsld_speed->setMaximum(32);
        vsld_speed->setValue(24);
        vsld_speed->setOrientation(Qt::Horizontal);
        vsld_speed->setTickPosition(QSlider::TicksBothSides);
        vsld_speed->setTickInterval(5);
        label_3 = new QLabel(tab_7);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(1065, 310, 54, 20));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignCenter);
        vsld_SetBeiShu = new QSlider(tab_7);
        vsld_SetBeiShu->setObjectName(QStringLiteral("vsld_SetBeiShu"));
        vsld_SetBeiShu->setGeometry(QRect(1125, 280, 121, 21));
        vsld_SetBeiShu->setMinimum(1);
        vsld_SetBeiShu->setMaximum(90);
        vsld_SetBeiShu->setValue(1);
        vsld_SetBeiShu->setOrientation(Qt::Horizontal);
        vsld_SetBeiShu->setTickPosition(QSlider::TicksBothSides);
        vsld_SetBeiShu->setTickInterval(5);
        groupBox_2 = new QGroupBox(tab_7);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(940, 10, 121, 131));
        groupBox_2->setFont(font1);
        pbSetMotor = new QPushButton(groupBox_2);
        pbSetMotor->setObjectName(QStringLiteral("pbSetMotor"));
        pbSetMotor->setGeometry(QRect(40, 90, 70, 30));
        sizePolicy3.setHeightForWidth(pbSetMotor->sizePolicy().hasHeightForWidth());
        pbSetMotor->setSizePolicy(sizePolicy3);
        pbSetMotor->setMinimumSize(QSize(70, 30));
        pbSetMotor->setMaximumSize(QSize(70, 30));
        pbSetMotor->setFont(font1);
        pbSetMotor->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        cbxPanMotor = new QCheckBox(groupBox_2);
        cbxPanMotor->setObjectName(QStringLiteral("cbxPanMotor"));
        cbxPanMotor->setGeometry(QRect(10, 35, 101, 16));
        cbxPanMotor->setFont(font2);
        cbxPanMotor->setChecked(true);
        cbxTiltMotor = new QCheckBox(groupBox_2);
        cbxTiltMotor->setObjectName(QStringLiteral("cbxTiltMotor"));
        cbxTiltMotor->setGeometry(QRect(10, 60, 101, 16));
        cbxTiltMotor->setFont(font2);
        cbxTiltMotor->setChecked(true);
        groupBox_7 = new QGroupBox(tab_7);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(940, 145, 121, 131));
        groupBox_7->setFont(font1);
        pbSetMotor_B = new QPushButton(groupBox_7);
        pbSetMotor_B->setObjectName(QStringLiteral("pbSetMotor_B"));
        pbSetMotor_B->setGeometry(QRect(40, 90, 70, 30));
        sizePolicy3.setHeightForWidth(pbSetMotor_B->sizePolicy().hasHeightForWidth());
        pbSetMotor_B->setSizePolicy(sizePolicy3);
        pbSetMotor_B->setMinimumSize(QSize(70, 30));
        pbSetMotor_B->setMaximumSize(QSize(70, 30));
        pbSetMotor_B->setFont(font1);
        pbSetMotor_B->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        cbxPanMotor_B = new QCheckBox(groupBox_7);
        cbxPanMotor_B->setObjectName(QStringLiteral("cbxPanMotor_B"));
        cbxPanMotor_B->setGeometry(QRect(10, 35, 101, 16));
        cbxPanMotor_B->setFont(font2);
        cbxPanMotor_B->setChecked(true);
        cbxTiltMotor_B = new QCheckBox(groupBox_7);
        cbxTiltMotor_B->setObjectName(QStringLiteral("cbxTiltMotor_B"));
        cbxTiltMotor_B->setGeometry(QRect(10, 60, 101, 16));
        cbxTiltMotor_B->setFont(font2);
        cbxTiltMotor_B->setChecked(true);
        layoutWidget = new QWidget(tab_7);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(1070, 17, 169, 254));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pbRec = new QPushButton(layoutWidget);
        pbRec->setObjectName(QStringLiteral("pbRec"));
        pbRec->setMinimumSize(QSize(80, 30));
        pbRec->setMaximumSize(QSize(70, 30));
        pbRec->setFont(font1);
        pbRec->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbRec, 0, 0, 1, 1);

        pbToZero = new QPushButton(layoutWidget);
        pbToZero->setObjectName(QStringLiteral("pbToZero"));
        sizePolicy3.setHeightForWidth(pbToZero->sizePolicy().hasHeightForWidth());
        pbToZero->setSizePolicy(sizePolicy3);
        pbToZero->setMinimumSize(QSize(80, 30));
        pbToZero->setMaximumSize(QSize(70, 30));
        pbToZero->setFont(font1);
        pbToZero->setLayoutDirection(Qt::LeftToRight);
        pbToZero->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbToZero, 0, 1, 1, 1);

        pbAutoFoc = new QPushButton(layoutWidget);
        pbAutoFoc->setObjectName(QStringLiteral("pbAutoFoc"));
        sizePolicy3.setHeightForWidth(pbAutoFoc->sizePolicy().hasHeightForWidth());
        pbAutoFoc->setSizePolicy(sizePolicy3);
        pbAutoFoc->setMinimumSize(QSize(80, 30));
        pbAutoFoc->setMaximumSize(QSize(70, 30));
        pbAutoFoc->setFont(font1);
        pbAutoFoc->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbAutoFoc, 1, 0, 1, 1);

        pbFocusN = new QPushButton(layoutWidget);
        pbFocusN->setObjectName(QStringLiteral("pbFocusN"));
        pbFocusN->setMinimumSize(QSize(80, 30));
        pbFocusN->setMaximumSize(QSize(70, 30));
        pbFocusN->setFont(font1);
        pbFocusN->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbFocusN, 2, 0, 1, 1);

        pbZoomOut = new QPushButton(layoutWidget);
        pbZoomOut->setObjectName(QStringLiteral("pbZoomOut"));
        sizePolicy3.setHeightForWidth(pbZoomOut->sizePolicy().hasHeightForWidth());
        pbZoomOut->setSizePolicy(sizePolicy3);
        pbZoomOut->setMinimumSize(QSize(80, 30));
        pbZoomOut->setMaximumSize(QSize(70, 30));
        pbZoomOut->setFont(font1);
        pbZoomOut->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbZoomOut, 2, 1, 1, 1);

        pbFocusF = new QPushButton(layoutWidget);
        pbFocusF->setObjectName(QStringLiteral("pbFocusF"));
        pbFocusF->setMinimumSize(QSize(80, 30));
        pbFocusF->setMaximumSize(QSize(70, 30));
        pbFocusF->setFont(font1);
        pbFocusF->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbFocusF, 3, 0, 1, 1);

        pbZoomIn = new QPushButton(layoutWidget);
        pbZoomIn->setObjectName(QStringLiteral("pbZoomIn"));
        sizePolicy3.setHeightForWidth(pbZoomIn->sizePolicy().hasHeightForWidth());
        pbZoomIn->setSizePolicy(sizePolicy3);
        pbZoomIn->setMinimumSize(QSize(80, 30));
        pbZoomIn->setMaximumSize(QSize(70, 30));
        pbZoomIn->setFont(font1);
        pbZoomIn->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbZoomIn, 3, 1, 1, 1);

        pbUp = new QPushButton(layoutWidget);
        pbUp->setObjectName(QStringLiteral("pbUp"));
        sizePolicy3.setHeightForWidth(pbUp->sizePolicy().hasHeightForWidth());
        pbUp->setSizePolicy(sizePolicy3);
        pbUp->setMinimumSize(QSize(80, 30));
        pbUp->setMaximumSize(QSize(16777215, 30));
        pbUp->setFont(font1);
        pbUp->setLayoutDirection(Qt::LeftToRight);
        pbUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbUp->setAutoRepeatDelay(10);
        pbUp->setAutoRepeatInterval(10);

        gridLayout->addWidget(pbUp, 4, 0, 1, 1);

        pbDown = new QPushButton(layoutWidget);
        pbDown->setObjectName(QStringLiteral("pbDown"));
        sizePolicy3.setHeightForWidth(pbDown->sizePolicy().hasHeightForWidth());
        pbDown->setSizePolicy(sizePolicy3);
        pbDown->setMinimumSize(QSize(80, 30));
        pbDown->setMaximumSize(QSize(16777215, 30));
        pbDown->setFont(font1);
        pbDown->setLayoutDirection(Qt::LeftToRight);
        pbDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbDown->setAutoRepeatDelay(10);
        pbDown->setAutoRepeatInterval(10);

        gridLayout->addWidget(pbDown, 4, 1, 1, 1);

        pbLeft = new QPushButton(layoutWidget);
        pbLeft->setObjectName(QStringLiteral("pbLeft"));
        sizePolicy3.setHeightForWidth(pbLeft->sizePolicy().hasHeightForWidth());
        pbLeft->setSizePolicy(sizePolicy3);
        pbLeft->setMinimumSize(QSize(80, 30));
        pbLeft->setMaximumSize(QSize(16777215, 30));
        pbLeft->setFont(font1);
        pbLeft->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbLeft->setAutoRepeatDelay(10);
        pbLeft->setAutoRepeatInterval(10);

        gridLayout->addWidget(pbLeft, 5, 0, 1, 1);

        pbRight = new QPushButton(layoutWidget);
        pbRight->setObjectName(QStringLiteral("pbRight"));
        sizePolicy3.setHeightForWidth(pbRight->sizePolicy().hasHeightForWidth());
        pbRight->setSizePolicy(sizePolicy3);
        pbRight->setMinimumSize(QSize(80, 30));
        pbRight->setMaximumSize(QSize(16777215, 30));
        pbRight->setFont(font1);
        pbRight->setLayoutDirection(Qt::LeftToRight);
        pbRight->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbRight->setAutoRepeatDelay(10);
        pbRight->setAutoRepeatInterval(10);

        gridLayout->addWidget(pbRight, 5, 1, 1, 1);

        pbStop = new QPushButton(layoutWidget);
        pbStop->setObjectName(QStringLiteral("pbStop"));
        sizePolicy3.setHeightForWidth(pbStop->sizePolicy().hasHeightForWidth());
        pbStop->setSizePolicy(sizePolicy3);
        pbStop->setMinimumSize(QSize(80, 30));
        pbStop->setMaximumSize(QSize(60, 30));
        pbStop->setFont(font1);
        pbStop->setLayoutDirection(Qt::LeftToRight);
        pbStop->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        gridLayout->addWidget(pbStop, 6, 1, 1, 1);

        groupBox_19 = new QGroupBox(tab_7);
        groupBox_19->setObjectName(QStringLiteral("groupBox_19"));
        groupBox_19->setGeometry(QRect(620, 10, 181, 191));
        groupBox_19->setFont(font1);
        lbl_lensTitle_6 = new QLabel(groupBox_19);
        lbl_lensTitle_6->setObjectName(QStringLiteral("lbl_lensTitle_6"));
        lbl_lensTitle_6->setGeometry(QRect(20, 71, 31, 31));
        QFont font6;
        font6.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221 Light"));
        font6.setPointSize(10);
        font6.setBold(true);
        font6.setWeight(75);
        lbl_lensTitle_6->setFont(font6);
        lbl_lenVal = new QLabel(groupBox_19);
        lbl_lenVal->setObjectName(QStringLiteral("lbl_lenVal"));
        lbl_lenVal->setGeometry(QRect(60, 76, 70, 21));
        lbl_lenVal->setMinimumSize(QSize(70, 21));
        lbl_lenVal->setMaximumSize(QSize(70, 21));
        lbl_lenVal->setFont(font3);
        lbl_lenVal->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblBeishu = new QLabel(groupBox_19);
        lblBeishu->setObjectName(QStringLiteral("lblBeishu"));
        lblBeishu->setGeometry(QRect(60, 53, 70, 21));
        lblBeishu->setMinimumSize(QSize(70, 21));
        lblBeishu->setMaximumSize(QSize(60, 21));
        QFont font7;
        font7.setPointSize(14);
        lblBeishu->setFont(font7);
        lblBeishu->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_lensTitle = new QLabel(groupBox_19);
        lbl_lensTitle->setObjectName(QStringLiteral("lbl_lensTitle"));
        lbl_lensTitle->setGeometry(QRect(20, 27, 31, 31));
        lbl_lensTitle->setFont(font6);
        lbl_lensTitle_5 = new QLabel(groupBox_19);
        lbl_lensTitle_5->setObjectName(QStringLiteral("lbl_lensTitle_5"));
        lbl_lensTitle_5->setGeometry(QRect(20, 49, 31, 31));
        lbl_lensTitle_5->setFont(font6);
        lblLens = new QLabel(groupBox_19);
        lblLens->setObjectName(QStringLiteral("lblLens"));
        lblLens->setGeometry(QRect(60, 30, 70, 21));
        lblLens->setMinimumSize(QSize(70, 21));
        lblLens->setMaximumSize(QSize(60, 21));
        lblLens->setFont(font7);
        lblLens->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblDirection = new QLabel(groupBox_19);
        lblDirection->setObjectName(QStringLiteral("lblDirection"));
        lblDirection->setGeometry(QRect(60, 126, 110, 21));
        lblDirection->setMinimumSize(QSize(0, 21));
        lblDirection->setMaximumSize(QSize(110, 21));
        lblDirection->setFont(font3);
        lblDirection->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblDirection->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblPitch = new QLabel(groupBox_19);
        lblPitch->setObjectName(QStringLiteral("lblPitch"));
        lblPitch->setGeometry(QRect(60, 150, 110, 21));
        lblPitch->setMinimumSize(QSize(0, 21));
        lblPitch->setMaximumSize(QSize(110, 21));
        lblPitch->setFont(font3);
        lblPitch->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblPitch->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_lensTitle_7 = new QLabel(groupBox_19);
        lbl_lensTitle_7->setObjectName(QStringLiteral("lbl_lensTitle_7"));
        lbl_lensTitle_7->setGeometry(QRect(20, 121, 31, 31));
        lbl_lensTitle_7->setFont(font6);
        lbl_lensTitle_8 = new QLabel(groupBox_19);
        lbl_lensTitle_8->setObjectName(QStringLiteral("lbl_lensTitle_8"));
        lbl_lensTitle_8->setGeometry(QRect(20, 143, 31, 31));
        lbl_lensTitle_8->setFont(font6);
        lbl_focalVal = new QLabel(groupBox_19);
        lbl_focalVal->setObjectName(QStringLiteral("lbl_focalVal"));
        lbl_focalVal->setGeometry(QRect(60, 100, 70, 21));
        lbl_focalVal->setMinimumSize(QSize(70, 0));
        lbl_focalVal->setMaximumSize(QSize(70, 16777215));
        lbl_focalVal->setFont(font3);
        lbl_focalVal->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_lensTitle_11 = new QLabel(groupBox_19);
        lbl_lensTitle_11->setObjectName(QStringLiteral("lbl_lensTitle_11"));
        lbl_lensTitle_11->setGeometry(QRect(20, 100, 31, 22));
        lbl_lensTitle_11->setMaximumSize(QSize(16777215, 22));
        lbl_lensTitle_11->setFont(font6);
        groupBox_20 = new QGroupBox(tab_7);
        groupBox_20->setObjectName(QStringLiteral("groupBox_20"));
        groupBox_20->setGeometry(QRect(620, 218, 181, 191));
        groupBox_20->setFont(font1);
        lblBeishu_B = new QLabel(groupBox_20);
        lblBeishu_B->setObjectName(QStringLiteral("lblBeishu_B"));
        lblBeishu_B->setGeometry(QRect(60, 44, 70, 21));
        lblBeishu_B->setMinimumSize(QSize(70, 21));
        lblBeishu_B->setMaximumSize(QSize(60, 21));
        lblBeishu_B->setFont(font7);
        lblBeishu_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_lensTitle_2 = new QLabel(groupBox_20);
        lbl_lensTitle_2->setObjectName(QStringLiteral("lbl_lensTitle_2"));
        lbl_lensTitle_2->setGeometry(QRect(20, 22, 31, 22));
        lbl_lensTitle_2->setMaximumSize(QSize(16777215, 22));
        lbl_lensTitle_2->setFont(font6);
        lbl_lenVal_B = new QLabel(groupBox_20);
        lbl_lenVal_B->setObjectName(QStringLiteral("lbl_lenVal_B"));
        lbl_lenVal_B->setGeometry(QRect(60, 67, 70, 21));
        lbl_lenVal_B->setMinimumSize(QSize(70, 21));
        lbl_lenVal_B->setMaximumSize(QSize(70, 21));
        lbl_lenVal_B->setFont(font7);
        lbl_lenVal_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblLens_B = new QLabel(groupBox_20);
        lblLens_B->setObjectName(QStringLiteral("lblLens_B"));
        lblLens_B->setGeometry(QRect(60, 21, 70, 21));
        lblLens_B->setMinimumSize(QSize(70, 21));
        lblLens_B->setMaximumSize(QSize(60, 21));
        lblLens_B->setFont(font7);
        lblLens_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_lensTitle_3 = new QLabel(groupBox_20);
        lbl_lensTitle_3->setObjectName(QStringLiteral("lbl_lensTitle_3"));
        lbl_lensTitle_3->setGeometry(QRect(20, 44, 31, 22));
        lbl_lensTitle_3->setMaximumSize(QSize(16777215, 22));
        lbl_lensTitle_3->setFont(font6);
        lbl_lensTitle_4 = new QLabel(groupBox_20);
        lbl_lensTitle_4->setObjectName(QStringLiteral("lbl_lensTitle_4"));
        lbl_lensTitle_4->setGeometry(QRect(20, 67, 31, 22));
        lbl_lensTitle_4->setMaximumSize(QSize(16777215, 22));
        lbl_lensTitle_4->setFont(font6);
        lblDirection_B = new QLabel(groupBox_20);
        lblDirection_B->setObjectName(QStringLiteral("lblDirection_B"));
        lblDirection_B->setGeometry(QRect(59, 116, 110, 21));
        lblDirection_B->setMaximumSize(QSize(110, 21));
        QFont font8;
        font8.setPointSize(16);
        lblDirection_B->setFont(font8);
        lblDirection_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblDirection_B->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblPitch_B = new QLabel(groupBox_20);
        lblPitch_B->setObjectName(QStringLiteral("lblPitch_B"));
        lblPitch_B->setGeometry(QRect(59, 140, 110, 21));
        lblPitch_B->setMaximumSize(QSize(110, 21));
        lblPitch_B->setFont(font8);
        lblPitch_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblPitch_B->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_lensTitle_9 = new QLabel(groupBox_20);
        lbl_lensTitle_9->setObjectName(QStringLiteral("lbl_lensTitle_9"));
        lbl_lensTitle_9->setGeometry(QRect(21, 111, 31, 31));
        lbl_lensTitle_9->setFont(font6);
        lbl_lensTitle_10 = new QLabel(groupBox_20);
        lbl_lensTitle_10->setObjectName(QStringLiteral("lbl_lensTitle_10"));
        lbl_lensTitle_10->setGeometry(QRect(21, 133, 31, 31));
        lbl_lensTitle_10->setFont(font6);
        lbl_focalVal_B = new QLabel(groupBox_20);
        lbl_focalVal_B->setObjectName(QStringLiteral("lbl_focalVal_B"));
        lbl_focalVal_B->setGeometry(QRect(60, 90, 70, 21));
        lbl_focalVal_B->setMinimumSize(QSize(70, 0));
        lbl_focalVal_B->setMaximumSize(QSize(70, 16777215));
        lbl_focalVal_B->setFont(font3);
        lbl_focalVal_B->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_lensTitle_12 = new QLabel(groupBox_20);
        lbl_lensTitle_12->setObjectName(QStringLiteral("lbl_lensTitle_12"));
        lbl_lensTitle_12->setGeometry(QRect(20, 90, 31, 22));
        lbl_lensTitle_12->setMaximumSize(QSize(16777215, 22));
        lbl_lensTitle_12->setFont(font6);
        tabWidConfig->addTab(tab_7, QString());
        layoutWidget->raise();
        te_Output->raise();
        groupBox_11->raise();
        groupBox_13->raise();
        gbx_PicFlip_5->raise();
        pClearbDbgOut->raise();
        groupBox_18->raise();
        gbx_PicFlip_4->raise();
        groupBox_12->raise();
        groupBox_5->raise();
        label_15->raise();
        vsld_speed->raise();
        label_3->raise();
        vsld_SetBeiShu->raise();
        groupBox_2->raise();
        groupBox_7->raise();
        groupBox_19->raise();
        groupBox_20->raise();
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        groupBox = new QGroupBox(tab_4);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 441, 391));
        groupBox->setFont(font1);
        rb_ExpoIris = new QRadioButton(groupBox);
        rb_ExpoIris->setObjectName(QStringLiteral("rb_ExpoIris"));
        rb_ExpoIris->setEnabled(true);
        rb_ExpoIris->setGeometry(QRect(10, 109, 101, 21));
        sizePolicy3.setHeightForWidth(rb_ExpoIris->sizePolicy().hasHeightForWidth());
        rb_ExpoIris->setSizePolicy(sizePolicy3);
        rb_ExpoIris->setFont(font4);
        rb_ExpoManual = new QRadioButton(groupBox);
        rb_ExpoManual->setObjectName(QStringLiteral("rb_ExpoManual"));
        rb_ExpoManual->setEnabled(true);
        rb_ExpoManual->setGeometry(QRect(10, 59, 81, 21));
        sizePolicy3.setHeightForWidth(rb_ExpoManual->sizePolicy().hasHeightForWidth());
        rb_ExpoManual->setSizePolicy(sizePolicy3);
        rb_ExpoManual->setFont(font4);
        rb_ExpoAuto = new QRadioButton(groupBox);
        rb_ExpoAuto->setObjectName(QStringLiteral("rb_ExpoAuto"));
        rb_ExpoAuto->setEnabled(true);
        rb_ExpoAuto->setGeometry(QRect(10, 34, 81, 21));
        sizePolicy3.setHeightForWidth(rb_ExpoAuto->sizePolicy().hasHeightForWidth());
        rb_ExpoAuto->setSizePolicy(sizePolicy3);
        rb_ExpoAuto->setFont(font4);
        rb_ExpoAuto->setChecked(true);
        rb_ExpoShutter = new QRadioButton(groupBox);
        rb_ExpoShutter->setObjectName(QStringLiteral("rb_ExpoShutter"));
        rb_ExpoShutter->setEnabled(true);
        rb_ExpoShutter->setGeometry(QRect(10, 84, 101, 21));
        sizePolicy3.setHeightForWidth(rb_ExpoShutter->sizePolicy().hasHeightForWidth());
        rb_ExpoShutter->setSizePolicy(sizePolicy3);
        rb_ExpoShutter->setFont(font4);
        gbx_WB = new QGroupBox(groupBox);
        gbx_WB->setObjectName(QStringLiteral("gbx_WB"));
        gbx_WB->setGeometry(QRect(26, 310, 90, 71));
        sizePolicy4.setHeightForWidth(gbx_WB->sizePolicy().hasHeightForWidth());
        gbx_WB->setSizePolicy(sizePolicy4);
        gbx_WB->setMinimumSize(QSize(90, 60));
        gbx_WB->setMaximumSize(QSize(90, 300));
        gbx_WB->setFont(font1);
        cbx_WbMode = new QComboBox(gbx_WB);
        cbx_WbMode->setObjectName(QStringLiteral("cbx_WbMode"));
        cbx_WbMode->setGeometry(QRect(4, 30, 81, 30));
        cbx_WbMode->setFont(font1);
        cbx_WbMode->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        gbx_Sharpen = new QGroupBox(groupBox);
        gbx_Sharpen->setObjectName(QStringLiteral("gbx_Sharpen"));
        gbx_Sharpen->setGeometry(QRect(25, 155, 90, 141));
        sizePolicy4.setHeightForWidth(gbx_Sharpen->sizePolicy().hasHeightForWidth());
        gbx_Sharpen->setSizePolicy(sizePolicy4);
        gbx_Sharpen->setMinimumSize(QSize(90, 130));
        gbx_Sharpen->setMaximumSize(QSize(90, 300));
        gbx_Sharpen->setFont(font1);
        pbSharpenReset = new QPushButton(gbx_Sharpen);
        pbSharpenReset->setObjectName(QStringLiteral("pbSharpenReset"));
        pbSharpenReset->setGeometry(QRect(16, 102, 70, 30));
        sizePolicy3.setHeightForWidth(pbSharpenReset->sizePolicy().hasHeightForWidth());
        pbSharpenReset->setSizePolicy(sizePolicy3);
        pbSharpenReset->setMinimumSize(QSize(70, 30));
        pbSharpenReset->setMaximumSize(QSize(70, 30));
        pbSharpenReset->setFont(font2);
        pbSharpenReset->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbSharpenDown = new QPushButton(gbx_Sharpen);
        pbSharpenDown->setObjectName(QStringLiteral("pbSharpenDown"));
        pbSharpenDown->setGeometry(QRect(17, 67, 70, 30));
        sizePolicy3.setHeightForWidth(pbSharpenDown->sizePolicy().hasHeightForWidth());
        pbSharpenDown->setSizePolicy(sizePolicy3);
        pbSharpenDown->setMinimumSize(QSize(70, 30));
        pbSharpenDown->setMaximumSize(QSize(70, 30));
        pbSharpenDown->setFont(font2);
        pbSharpenDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbSharpenUp = new QPushButton(gbx_Sharpen);
        pbSharpenUp->setObjectName(QStringLiteral("pbSharpenUp"));
        pbSharpenUp->setGeometry(QRect(17, 33, 70, 30));
        sizePolicy3.setHeightForWidth(pbSharpenUp->sizePolicy().hasHeightForWidth());
        pbSharpenUp->setSizePolicy(sizePolicy3);
        pbSharpenUp->setMinimumSize(QSize(70, 30));
        pbSharpenUp->setMaximumSize(QSize(70, 30));
        pbSharpenUp->setFont(font2);
        pbSharpenUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        gbx_Gamma = new QGroupBox(groupBox);
        gbx_Gamma->setObjectName(QStringLiteral("gbx_Gamma"));
        gbx_Gamma->setGeometry(QRect(132, 310, 90, 71));
        sizePolicy4.setHeightForWidth(gbx_Gamma->sizePolicy().hasHeightForWidth());
        gbx_Gamma->setSizePolicy(sizePolicy4);
        gbx_Gamma->setMinimumSize(QSize(90, 60));
        gbx_Gamma->setMaximumSize(QSize(90, 300));
        gbx_Gamma->setFont(font1);
        spb_Gamma = new QSpinBox(gbx_Gamma);
        spb_Gamma->setObjectName(QStringLiteral("spb_Gamma"));
        spb_Gamma->setGeometry(QRect(11, 34, 71, 22));
        spb_Gamma->setFont(font3);
        spb_Gamma->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        spb_Gamma->setMaximum(15);
        gbx_Expo = new QGroupBox(groupBox);
        gbx_Expo->setObjectName(QStringLiteral("gbx_Expo"));
        gbx_Expo->setGeometry(QRect(131, 10, 90, 131));
        sizePolicy4.setHeightForWidth(gbx_Expo->sizePolicy().hasHeightForWidth());
        gbx_Expo->setSizePolicy(sizePolicy4);
        gbx_Expo->setMinimumSize(QSize(90, 60));
        gbx_Expo->setMaximumSize(QSize(90, 300));
        gbx_Expo->setFont(font1);
        pbExpDown = new QPushButton(gbx_Expo);
        pbExpDown->setObjectName(QStringLiteral("pbExpDown"));
        pbExpDown->setGeometry(QRect(13, 55, 70, 30));
        sizePolicy3.setHeightForWidth(pbExpDown->sizePolicy().hasHeightForWidth());
        pbExpDown->setSizePolicy(sizePolicy3);
        pbExpDown->setMinimumSize(QSize(70, 30));
        pbExpDown->setMaximumSize(QSize(70, 30));
        pbExpDown->setFont(font2);
        pbExpDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbExpUp = new QPushButton(gbx_Expo);
        pbExpUp->setObjectName(QStringLiteral("pbExpUp"));
        pbExpUp->setGeometry(QRect(13, 21, 70, 30));
        sizePolicy3.setHeightForWidth(pbExpUp->sizePolicy().hasHeightForWidth());
        pbExpUp->setSizePolicy(sizePolicy3);
        pbExpUp->setMinimumSize(QSize(70, 30));
        pbExpUp->setMaximumSize(QSize(70, 30));
        pbExpUp->setFont(font2);
        pbExpUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbExpReset = new QPushButton(gbx_Expo);
        pbExpReset->setObjectName(QStringLiteral("pbExpReset"));
        pbExpReset->setGeometry(QRect(13, 90, 70, 30));
        sizePolicy3.setHeightForWidth(pbExpReset->sizePolicy().hasHeightForWidth());
        pbExpReset->setSizePolicy(sizePolicy3);
        pbExpReset->setMinimumSize(QSize(70, 30));
        pbExpReset->setMaximumSize(QSize(70, 30));
        pbExpReset->setFont(font2);
        pbExpReset->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        gbx_backLight = new QGroupBox(groupBox);
        gbx_backLight->setObjectName(QStringLiteral("gbx_backLight"));
        gbx_backLight->setGeometry(QRect(238, 155, 90, 141));
        sizePolicy4.setHeightForWidth(gbx_backLight->sizePolicy().hasHeightForWidth());
        gbx_backLight->setSizePolicy(sizePolicy4);
        gbx_backLight->setMinimumSize(QSize(90, 60));
        gbx_backLight->setMaximumSize(QSize(90, 300));
        gbx_backLight->setFont(font1);
        pbBackLightSend = new QPushButton(gbx_backLight);
        pbBackLightSend->setObjectName(QStringLiteral("pbBackLightSend"));
        pbBackLightSend->setGeometry(QRect(10, 106, 70, 30));
        sizePolicy3.setHeightForWidth(pbBackLightSend->sizePolicy().hasHeightForWidth());
        pbBackLightSend->setSizePolicy(sizePolicy3);
        pbBackLightSend->setMinimumSize(QSize(70, 30));
        pbBackLightSend->setMaximumSize(QSize(70, 30));
        pbBackLightSend->setFont(font2);
        pbBackLightSend->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        cbx_backLightMode = new QComboBox(gbx_backLight);
        cbx_backLightMode->setObjectName(QStringLiteral("cbx_backLightMode"));
        cbx_backLightMode->setGeometry(QRect(10, 32, 71, 30));
        cbx_backLightMode->setFont(font2);
        cbx_backLightMode->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        sbx_cbx_backLightVal = new QSpinBox(gbx_backLight);
        sbx_cbx_backLightVal->setObjectName(QStringLiteral("sbx_cbx_backLightVal"));
        sbx_cbx_backLightVal->setGeometry(QRect(10, 68, 71, 30));
        sbx_cbx_backLightVal->setFont(font2);
        sbx_cbx_backLightVal->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        sbx_cbx_backLightVal->setMaximum(100);
        sbx_cbx_backLightVal->setValue(50);
        gbx_Expo_2 = new QGroupBox(groupBox);
        gbx_Expo_2->setObjectName(QStringLiteral("gbx_Expo_2"));
        gbx_Expo_2->setGeometry(QRect(238, 10, 90, 131));
        sizePolicy4.setHeightForWidth(gbx_Expo_2->sizePolicy().hasHeightForWidth());
        gbx_Expo_2->setSizePolicy(sizePolicy4);
        gbx_Expo_2->setMinimumSize(QSize(90, 130));
        gbx_Expo_2->setMaximumSize(QSize(90, 300));
        gbx_Expo_2->setFont(font1);
        pbApReset = new QPushButton(gbx_Expo_2);
        pbApReset->setObjectName(QStringLiteral("pbApReset"));
        pbApReset->setGeometry(QRect(15, 90, 70, 30));
        sizePolicy3.setHeightForWidth(pbApReset->sizePolicy().hasHeightForWidth());
        pbApReset->setSizePolicy(sizePolicy3);
        pbApReset->setMinimumSize(QSize(70, 30));
        pbApReset->setMaximumSize(QSize(70, 30));
        pbApReset->setFont(font2);
        pbApReset->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbApDown = new QPushButton(gbx_Expo_2);
        pbApDown->setObjectName(QStringLiteral("pbApDown"));
        pbApDown->setGeometry(QRect(16, 55, 70, 30));
        sizePolicy3.setHeightForWidth(pbApDown->sizePolicy().hasHeightForWidth());
        pbApDown->setSizePolicy(sizePolicy3);
        pbApDown->setMinimumSize(QSize(70, 30));
        pbApDown->setMaximumSize(QSize(70, 30));
        pbApDown->setFont(font2);
        pbApDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbApUp = new QPushButton(gbx_Expo_2);
        pbApUp->setObjectName(QStringLiteral("pbApUp"));
        pbApUp->setGeometry(QRect(16, 21, 70, 30));
        sizePolicy3.setHeightForWidth(pbApUp->sizePolicy().hasHeightForWidth());
        pbApUp->setSizePolicy(sizePolicy3);
        pbApUp->setMinimumSize(QSize(70, 30));
        pbApUp->setMaximumSize(QSize(70, 30));
        pbApUp->setFont(font2);
        pbApUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        gbx_Shutter = new QGroupBox(groupBox);
        gbx_Shutter->setObjectName(QStringLiteral("gbx_Shutter"));
        gbx_Shutter->setGeometry(QRect(131, 155, 90, 141));
        sizePolicy4.setHeightForWidth(gbx_Shutter->sizePolicy().hasHeightForWidth());
        gbx_Shutter->setSizePolicy(sizePolicy4);
        gbx_Shutter->setMinimumSize(QSize(90, 130));
        gbx_Shutter->setMaximumSize(QSize(90, 300));
        gbx_Shutter->setFont(font1);
        pbShutterReset = new QPushButton(gbx_Shutter);
        pbShutterReset->setObjectName(QStringLiteral("pbShutterReset"));
        pbShutterReset->setGeometry(QRect(10, 103, 70, 30));
        sizePolicy3.setHeightForWidth(pbShutterReset->sizePolicy().hasHeightForWidth());
        pbShutterReset->setSizePolicy(sizePolicy3);
        pbShutterReset->setMinimumSize(QSize(70, 30));
        pbShutterReset->setMaximumSize(QSize(70, 30));
        pbShutterReset->setFont(font2);
        pbShutterReset->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbApShutterDown = new QPushButton(gbx_Shutter);
        pbApShutterDown->setObjectName(QStringLiteral("pbApShutterDown"));
        pbApShutterDown->setGeometry(QRect(11, 68, 70, 30));
        sizePolicy3.setHeightForWidth(pbApShutterDown->sizePolicy().hasHeightForWidth());
        pbApShutterDown->setSizePolicy(sizePolicy3);
        pbApShutterDown->setMinimumSize(QSize(70, 30));
        pbApShutterDown->setMaximumSize(QSize(70, 30));
        pbApShutterDown->setFont(font2);
        pbApShutterDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbShutterUp = new QPushButton(gbx_Shutter);
        pbShutterUp->setObjectName(QStringLiteral("pbShutterUp"));
        pbShutterUp->setGeometry(QRect(11, 34, 70, 30));
        sizePolicy3.setHeightForWidth(pbShutterUp->sizePolicy().hasHeightForWidth());
        pbShutterUp->setSizePolicy(sizePolicy3);
        pbShutterUp->setMinimumSize(QSize(70, 30));
        pbShutterUp->setMaximumSize(QSize(70, 30));
        pbShutterUp->setFont(font2);
        pbShutterUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        gbx_PicFlip = new QGroupBox(groupBox);
        gbx_PicFlip->setObjectName(QStringLiteral("gbx_PicFlip"));
        gbx_PicFlip->setGeometry(QRect(238, 310, 90, 71));
        sizePolicy4.setHeightForWidth(gbx_PicFlip->sizePolicy().hasHeightForWidth());
        gbx_PicFlip->setSizePolicy(sizePolicy4);
        gbx_PicFlip->setMinimumSize(QSize(90, 71));
        gbx_PicFlip->setMaximumSize(QSize(90, 61));
        gbx_PicFlip->setFont(font1);
        rb_flipPicOff = new QRadioButton(gbx_PicFlip);
        rb_flipPicOff->setObjectName(QStringLiteral("rb_flipPicOff"));
        rb_flipPicOff->setGeometry(QRect(15, 23, 50, 21));
        sizePolicy3.setHeightForWidth(rb_flipPicOff->sizePolicy().hasHeightForWidth());
        rb_flipPicOff->setSizePolicy(sizePolicy3);
        rb_flipPicOff->setMinimumSize(QSize(50, 0));
        rb_flipPicOff->setMaximumSize(QSize(50, 16777215));
        rb_flipPicOff->setFont(font4);
        rb_flipPicOff->setChecked(true);
        rb_flipPicOn = new QRadioButton(gbx_PicFlip);
        rb_flipPicOn->setObjectName(QStringLiteral("rb_flipPicOn"));
        rb_flipPicOn->setEnabled(true);
        rb_flipPicOn->setGeometry(QRect(15, 43, 50, 21));
        sizePolicy3.setHeightForWidth(rb_flipPicOn->sizePolicy().hasHeightForWidth());
        rb_flipPicOn->setSizePolicy(sizePolicy3);
        rb_flipPicOn->setMinimumSize(QSize(50, 0));
        rb_flipPicOn->setMaximumSize(QSize(50, 16777215));
        rb_flipPicOn->setFont(font4);
        gbx_PicFlip_2 = new QGroupBox(groupBox);
        gbx_PicFlip_2->setObjectName(QStringLiteral("gbx_PicFlip_2"));
        gbx_PicFlip_2->setGeometry(QRect(340, 310, 90, 71));
        sizePolicy4.setHeightForWidth(gbx_PicFlip_2->sizePolicy().hasHeightForWidth());
        gbx_PicFlip_2->setSizePolicy(sizePolicy4);
        gbx_PicFlip_2->setMinimumSize(QSize(90, 71));
        gbx_PicFlip_2->setMaximumSize(QSize(90, 61));
        gbx_PicFlip_2->setFont(font1);
        rb_DefogOff = new QRadioButton(gbx_PicFlip_2);
        rb_DefogOff->setObjectName(QStringLiteral("rb_DefogOff"));
        rb_DefogOff->setGeometry(QRect(15, 23, 50, 21));
        sizePolicy3.setHeightForWidth(rb_DefogOff->sizePolicy().hasHeightForWidth());
        rb_DefogOff->setSizePolicy(sizePolicy3);
        rb_DefogOff->setMinimumSize(QSize(50, 0));
        rb_DefogOff->setMaximumSize(QSize(50, 16777215));
        rb_DefogOff->setFont(font4);
        rb_DefogOff->setChecked(true);
        rb_DefogOn = new QRadioButton(gbx_PicFlip_2);
        rb_DefogOn->setObjectName(QStringLiteral("rb_DefogOn"));
        rb_DefogOn->setEnabled(true);
        rb_DefogOn->setGeometry(QRect(15, 43, 50, 21));
        sizePolicy3.setHeightForWidth(rb_DefogOn->sizePolicy().hasHeightForWidth());
        rb_DefogOn->setSizePolicy(sizePolicy3);
        rb_DefogOn->setMinimumSize(QSize(50, 0));
        rb_DefogOn->setMaximumSize(QSize(50, 16777215));
        rb_DefogOn->setFont(font4);
        tabWidConfig->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pbOsdWordSave = new QPushButton(tab);
        pbOsdWordSave->setObjectName(QStringLiteral("pbOsdWordSave"));
        pbOsdWordSave->setGeometry(QRect(1060, 115, 91, 30));
        pbOsdWordSave->setFont(font2);
        pbOsdWordSave->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        gbxPalSelect_8 = new QGroupBox(tab);
        gbxPalSelect_8->setObjectName(QStringLiteral("gbxPalSelect_8"));
        gbxPalSelect_8->setGeometry(QRect(720, 10, 200, 100));
        gbxPalSelect_8->setMinimumSize(QSize(200, 100));
        gbxPalSelect_8->setMaximumSize(QSize(200, 70));
        gbxPalSelect_8->setFont(font2);
        gbxPalSelect_8->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        cbxOSDFontSize = new QComboBox(gbxPalSelect_8);
        cbxOSDFontSize->setObjectName(QStringLiteral("cbxOSDFontSize"));
        cbxOSDFontSize->setGeometry(QRect(76, 60, 50, 22));
        cbxOSDFontSize->setMinimumSize(QSize(50, 0));
        cbxOSDFontSize->setMaximumSize(QSize(50, 16777215));
        cbxOSDFont = new QComboBox(gbxPalSelect_8);
        cbxOSDFont->setObjectName(QStringLiteral("cbxOSDFont"));
        cbxOSDFont->setGeometry(QRect(76, 30, 100, 22));
        cbxOSDFont->setMinimumSize(QSize(100, 0));
        cbxOSDFont->setMaximumSize(QSize(100, 16777215));
        lblCurrentPos_22 = new QLabel(gbxPalSelect_8);
        lblCurrentPos_22->setObjectName(QStringLiteral("lblCurrentPos_22"));
        lblCurrentPos_22->setGeometry(QRect(13, 34, 51, 20));
        lblCurrentPos_22->setFont(font2);
        lblCurrentPos_23 = new QLabel(gbxPalSelect_8);
        lblCurrentPos_23->setObjectName(QStringLiteral("lblCurrentPos_23"));
        lblCurrentPos_23->setGeometry(QRect(12, 61, 51, 20));
        lblCurrentPos_23->setFont(font2);
        groupBox_9 = new QGroupBox(tab);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(20, 10, 150, 146));
        groupBox_9->setMinimumSize(QSize(150, 146));
        groupBox_9->setMaximumSize(QSize(150, 146));
        groupBox_9->setFont(font2);
        groupBox_9->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        cbxCrosser = new QCheckBox(groupBox_9);
        cbxCrosser->setObjectName(QStringLiteral("cbxCrosser"));
        cbxCrosser->setGeometry(QRect(17, 20, 120, 16));
        cbxCrosser->setMinimumSize(QSize(120, 0));
        cbxCrosser->setMaximumSize(QSize(120, 16777215));
        cbxCrosser->setFont(font2);
        cbxCrosser->setChecked(true);
        cbxFullScreen = new QCheckBox(groupBox_9);
        cbxFullScreen->setObjectName(QStringLiteral("cbxFullScreen"));
        cbxFullScreen->setGeometry(QRect(17, 110, 120, 22));
        cbxFullScreen->setMinimumSize(QSize(120, 0));
        cbxFullScreen->setMaximumSize(QSize(120, 16777215));
        QFont font9;
        font9.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font9.setPointSize(10);
        font9.setBold(true);
        font9.setWeight(75);
        cbxFullScreen->setFont(font9);
        cbxFullScreen->setChecked(false);
        cbxRoiBox = new QCheckBox(groupBox_9);
        cbxRoiBox->setObjectName(QStringLiteral("cbxRoiBox"));
        cbxRoiBox->setGeometry(QRect(18, 60, 120, 16));
        cbxRoiBox->setMinimumSize(QSize(120, 0));
        cbxRoiBox->setMaximumSize(QSize(80, 16777215));
        cbxRoiBox->setFont(font2);
        cbxRoiBox->setChecked(true);
        cbxMarkline = new QCheckBox(groupBox_9);
        cbxMarkline->setObjectName(QStringLiteral("cbxMarkline"));
        cbxMarkline->setEnabled(false);
        cbxMarkline->setGeometry(QRect(18, 40, 120, 16));
        cbxMarkline->setMinimumSize(QSize(120, 0));
        cbxMarkline->setMaximumSize(QSize(80, 16777215));
        cbxMarkline->setFont(font2);
        cbxMarkline->setCheckable(false);
        cbxMarkline->setChecked(false);
        cbxAzimuthScale = new QCheckBox(groupBox_9);
        cbxAzimuthScale->setObjectName(QStringLiteral("cbxAzimuthScale"));
        cbxAzimuthScale->setEnabled(false);
        cbxAzimuthScale->setGeometry(QRect(19, 80, 80, 16));
        cbxAzimuthScale->setMinimumSize(QSize(80, 0));
        cbxAzimuthScale->setMaximumSize(QSize(80, 16777215));
        cbxAzimuthScale->setFont(font2);
        cbxAzimuthScale->setCheckable(false);
        cbxAzimuthScale->setChecked(false);
        gbx_RoiSize_2 = new QGroupBox(tab);
        gbx_RoiSize_2->setObjectName(QStringLiteral("gbx_RoiSize_2"));
        gbx_RoiSize_2->setGeometry(QRect(180, 10, 120, 146));
        sizePolicy4.setHeightForWidth(gbx_RoiSize_2->sizePolicy().hasHeightForWidth());
        gbx_RoiSize_2->setSizePolicy(sizePolicy4);
        gbx_RoiSize_2->setMinimumSize(QSize(120, 146));
        gbx_RoiSize_2->setMaximumSize(QSize(130, 36));
        gbx_RoiSize_2->setFont(font2);
        rb_OSDColorGreen = new QRadioButton(gbx_RoiSize_2);
        rb_OSDColorGreen->setObjectName(QStringLiteral("rb_OSDColorGreen"));
        rb_OSDColorGreen->setGeometry(QRect(20, 26, 80, 21));
        sizePolicy3.setHeightForWidth(rb_OSDColorGreen->sizePolicy().hasHeightForWidth());
        rb_OSDColorGreen->setSizePolicy(sizePolicy3);
        rb_OSDColorGreen->setMinimumSize(QSize(80, 0));
        rb_OSDColorGreen->setMaximumSize(QSize(80, 16777215));
        QFont font10;
        font10.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font10.setPointSize(10);
        font10.setBold(true);
        font10.setWeight(75);
        rb_OSDColorGreen->setFont(font10);
        rb_OSDColorGreen->setChecked(true);
        rb_0SDColorBlk = new QRadioButton(gbx_RoiSize_2);
        rb_0SDColorBlk->setObjectName(QStringLiteral("rb_0SDColorBlk"));
        rb_0SDColorBlk->setEnabled(true);
        rb_0SDColorBlk->setGeometry(QRect(20, 67, 80, 21));
        sizePolicy3.setHeightForWidth(rb_0SDColorBlk->sizePolicy().hasHeightForWidth());
        rb_0SDColorBlk->setSizePolicy(sizePolicy3);
        rb_0SDColorBlk->setMinimumSize(QSize(80, 0));
        rb_0SDColorBlk->setMaximumSize(QSize(80, 16777215));
        rb_0SDColorBlk->setFont(font10);
        rb_OSDColorRed = new QRadioButton(gbx_RoiSize_2);
        rb_OSDColorRed->setObjectName(QStringLiteral("rb_OSDColorRed"));
        rb_OSDColorRed->setEnabled(true);
        rb_OSDColorRed->setGeometry(QRect(20, 46, 80, 21));
        sizePolicy3.setHeightForWidth(rb_OSDColorRed->sizePolicy().hasHeightForWidth());
        rb_OSDColorRed->setSizePolicy(sizePolicy3);
        rb_OSDColorRed->setMinimumSize(QSize(80, 0));
        rb_OSDColorRed->setMaximumSize(QSize(80, 16777215));
        rb_OSDColorRed->setFont(font10);
        rb_OSDColorRed->setChecked(false);
        rb_0SDColorWhite = new QRadioButton(gbx_RoiSize_2);
        rb_0SDColorWhite->setObjectName(QStringLiteral("rb_0SDColorWhite"));
        rb_0SDColorWhite->setEnabled(true);
        rb_0SDColorWhite->setGeometry(QRect(20, 90, 80, 21));
        sizePolicy3.setHeightForWidth(rb_0SDColorWhite->sizePolicy().hasHeightForWidth());
        rb_0SDColorWhite->setSizePolicy(sizePolicy3);
        rb_0SDColorWhite->setMinimumSize(QSize(80, 0));
        rb_0SDColorWhite->setMaximumSize(QSize(80, 16777215));
        rb_0SDColorWhite->setFont(font10);
        rb_0SDColorWhite->setChecked(false);
        tabWidConfig->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        pbSetDirection = new QPushButton(tab_2);
        pbSetDirection->setObjectName(QStringLiteral("pbSetDirection"));
        pbSetDirection->setEnabled(true);
        pbSetDirection->setGeometry(QRect(450, 325, 141, 30));
        pbSetDirection->setMinimumSize(QSize(75, 30));
        pbSetDirection->setMaximumSize(QSize(200, 30));
        pbSetDirection->setFont(font2);
        pbSetDirection->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbSetPitch = new QPushButton(tab_2);
        pbSetPitch->setObjectName(QStringLiteral("pbSetPitch"));
        pbSetPitch->setGeometry(QRect(450, 361, 141, 30));
        pbSetPitch->setMinimumSize(QSize(75, 30));
        pbSetPitch->setMaximumSize(QSize(200, 30));
        pbSetPitch->setFont(font2);
        pbSetPitch->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        groupBox_14 = new QGroupBox(tab_2);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        groupBox_14->setGeometry(QRect(20, 162, 201, 181));
        groupBox_14->setFont(font2);
        leGD1Blh2A = new QLineEdit(groupBox_14);
        leGD1Blh2A->setObjectName(QStringLiteral("leGD1Blh2A"));
        leGD1Blh2A->setGeometry(QRect(73, 51, 100, 20));
        sizePolicy3.setHeightForWidth(leGD1Blh2A->sizePolicy().hasHeightForWidth());
        leGD1Blh2A->setSizePolicy(sizePolicy3);
        leGD1Blh2A->setMinimumSize(QSize(100, 0));
        leGD1Blh2A->setMaximumSize(QSize(100, 16777215));
        leGD1Blh2A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_47 = new QLabel(groupBox_14);
        lblCurrentPos_47->setObjectName(QStringLiteral("lblCurrentPos_47"));
        lblCurrentPos_47->setGeometry(QRect(29, 54, 35, 16));
        lblCurrentPos_47->setMinimumSize(QSize(35, 0));
        lblCurrentPos_47->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_47->setFont(font2);
        lblCurrentPos_47->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_48 = new QLabel(groupBox_14);
        lblCurrentPos_48->setObjectName(QStringLiteral("lblCurrentPos_48"));
        lblCurrentPos_48->setGeometry(QRect(28, 81, 35, 16));
        lblCurrentPos_48->setMinimumSize(QSize(35, 0));
        lblCurrentPos_48->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_48->setFont(font2);
        lblCurrentPos_48->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD1Blh2E = new QLineEdit(groupBox_14);
        leGD1Blh2E->setObjectName(QStringLiteral("leGD1Blh2E"));
        leGD1Blh2E->setGeometry(QRect(72, 78, 100, 20));
        sizePolicy3.setHeightForWidth(leGD1Blh2E->sizePolicy().hasHeightForWidth());
        leGD1Blh2E->setSizePolicy(sizePolicy3);
        leGD1Blh2E->setMinimumSize(QSize(100, 0));
        leGD1Blh2E->setMaximumSize(QSize(100, 16777215));
        leGD1Blh2E->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_49 = new QLabel(groupBox_14);
        lblCurrentPos_49->setObjectName(QStringLiteral("lblCurrentPos_49"));
        lblCurrentPos_49->setGeometry(QRect(28, 107, 35, 16));
        lblCurrentPos_49->setMinimumSize(QSize(35, 0));
        lblCurrentPos_49->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_49->setFont(font2);
        lblCurrentPos_49->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD1Blh2R = new QLineEdit(groupBox_14);
        leGD1Blh2R->setObjectName(QStringLiteral("leGD1Blh2R"));
        leGD1Blh2R->setGeometry(QRect(72, 104, 100, 20));
        sizePolicy3.setHeightForWidth(leGD1Blh2R->sizePolicy().hasHeightForWidth());
        leGD1Blh2R->setSizePolicy(sizePolicy3);
        leGD1Blh2R->setMinimumSize(QSize(100, 0));
        leGD1Blh2R->setMaximumSize(QSize(100, 16777215));
        leGD1Blh2R->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pbSetGD1Direction = new QPushButton(groupBox_14);
        pbSetGD1Direction->setObjectName(QStringLiteral("pbSetGD1Direction"));
        pbSetGD1Direction->setGeometry(QRect(74, 20, 100, 30));
        pbSetGD1Direction->setMinimumSize(QSize(75, 30));
        pbSetGD1Direction->setMaximumSize(QSize(200, 30));
        pbSetGD1Direction->setFont(font2);
        pbSetGD1Direction->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        lblCurrentPos_59 = new QLabel(groupBox_14);
        lblCurrentPos_59->setObjectName(QStringLiteral("lblCurrentPos_59"));
        lblCurrentPos_59->setGeometry(QRect(173, 54, 16, 16));
        lblCurrentPos_59->setMinimumSize(QSize(0, 0));
        lblCurrentPos_59->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_59->setFont(font2);
        lblCurrentPos_59->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_60 = new QLabel(groupBox_14);
        lblCurrentPos_60->setObjectName(QStringLiteral("lblCurrentPos_60"));
        lblCurrentPos_60->setGeometry(QRect(173, 80, 16, 16));
        lblCurrentPos_60->setMinimumSize(QSize(0, 0));
        lblCurrentPos_60->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_60->setFont(font2);
        lblCurrentPos_60->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_61 = new QLabel(groupBox_14);
        lblCurrentPos_61->setObjectName(QStringLiteral("lblCurrentPos_61"));
        lblCurrentPos_61->setGeometry(QRect(173, 104, 16, 16));
        lblCurrentPos_61->setMinimumSize(QSize(0, 0));
        lblCurrentPos_61->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_61->setFont(font2);
        lblCurrentPos_61->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD1AntennaA = new QLineEdit(groupBox_14);
        leGD1AntennaA->setObjectName(QStringLiteral("leGD1AntennaA"));
        leGD1AntennaA->setGeometry(QRect(72, 130, 100, 20));
        sizePolicy3.setHeightForWidth(leGD1AntennaA->sizePolicy().hasHeightForWidth());
        leGD1AntennaA->setSizePolicy(sizePolicy3);
        leGD1AntennaA->setMinimumSize(QSize(100, 0));
        leGD1AntennaA->setMaximumSize(QSize(100, 16777215));
        leGD1AntennaA->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGD1AntennaE = new QLineEdit(groupBox_14);
        leGD1AntennaE->setObjectName(QStringLiteral("leGD1AntennaE"));
        leGD1AntennaE->setGeometry(QRect(71, 157, 100, 20));
        sizePolicy3.setHeightForWidth(leGD1AntennaE->sizePolicy().hasHeightForWidth());
        leGD1AntennaE->setSizePolicy(sizePolicy3);
        leGD1AntennaE->setMinimumSize(QSize(100, 0));
        leGD1AntennaE->setMaximumSize(QSize(100, 16777215));
        leGD1AntennaE->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_68 = new QLabel(groupBox_14);
        lblCurrentPos_68->setObjectName(QStringLiteral("lblCurrentPos_68"));
        lblCurrentPos_68->setGeometry(QRect(172, 159, 16, 16));
        lblCurrentPos_68->setMinimumSize(QSize(0, 0));
        lblCurrentPos_68->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_68->setFont(font2);
        lblCurrentPos_68->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_69 = new QLabel(groupBox_14);
        lblCurrentPos_69->setObjectName(QStringLiteral("lblCurrentPos_69"));
        lblCurrentPos_69->setGeometry(QRect(172, 133, 16, 16));
        lblCurrentPos_69->setMinimumSize(QSize(0, 0));
        lblCurrentPos_69->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_69->setFont(font2);
        lblCurrentPos_69->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_56 = new QLabel(groupBox_14);
        lblCurrentPos_56->setObjectName(QStringLiteral("lblCurrentPos_56"));
        lblCurrentPos_56->setGeometry(QRect(5, 160, 60, 16));
        lblCurrentPos_56->setMinimumSize(QSize(60, 0));
        lblCurrentPos_56->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_56->setFont(font2);
        lblCurrentPos_56->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_57 = new QLabel(groupBox_14);
        lblCurrentPos_57->setObjectName(QStringLiteral("lblCurrentPos_57"));
        lblCurrentPos_57->setGeometry(QRect(6, 133, 60, 16));
        lblCurrentPos_57->setMinimumSize(QSize(60, 0));
        lblCurrentPos_57->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_57->setFont(font2);
        lblCurrentPos_57->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_15 = new QGroupBox(tab_2);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));
        groupBox_15->setGeometry(QRect(20, 10, 411, 151));
        groupBox_15->setFont(font2);
        leGd1SiteB = new QLineEdit(groupBox_15);
        leGd1SiteB->setObjectName(QStringLiteral("leGd1SiteB"));
        leGd1SiteB->setGeometry(QRect(51, 51, 100, 20));
        sizePolicy3.setHeightForWidth(leGd1SiteB->sizePolicy().hasHeightForWidth());
        leGd1SiteB->setSizePolicy(sizePolicy3);
        leGd1SiteB->setMinimumSize(QSize(100, 0));
        leGd1SiteB->setMaximumSize(QSize(100, 16777215));
        leGd1SiteB->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd1SiteL = new QLineEdit(groupBox_15);
        leGd1SiteL->setObjectName(QStringLiteral("leGd1SiteL"));
        leGd1SiteL->setGeometry(QRect(160, 51, 100, 20));
        sizePolicy3.setHeightForWidth(leGd1SiteL->sizePolicy().hasHeightForWidth());
        leGd1SiteL->setSizePolicy(sizePolicy3);
        leGd1SiteL->setMinimumSize(QSize(100, 0));
        leGd1SiteL->setMaximumSize(QSize(100, 16777215));
        leGd1SiteL->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_50 = new QLabel(groupBox_15);
        lblCurrentPos_50->setObjectName(QStringLiteral("lblCurrentPos_50"));
        lblCurrentPos_50->setGeometry(QRect(7, 53, 35, 16));
        lblCurrentPos_50->setMinimumSize(QSize(35, 0));
        lblCurrentPos_50->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_50->setFont(font2);
        lblCurrentPos_50->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_51 = new QLabel(groupBox_15);
        lblCurrentPos_51->setObjectName(QStringLiteral("lblCurrentPos_51"));
        lblCurrentPos_51->setGeometry(QRect(7, 80, 35, 16));
        lblCurrentPos_51->setMinimumSize(QSize(35, 0));
        lblCurrentPos_51->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_51->setFont(font2);
        lblCurrentPos_51->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGd2SiteL = new QLineEdit(groupBox_15);
        leGd2SiteL->setObjectName(QStringLiteral("leGd2SiteL"));
        leGd2SiteL->setGeometry(QRect(160, 78, 100, 20));
        sizePolicy3.setHeightForWidth(leGd2SiteL->sizePolicy().hasHeightForWidth());
        leGd2SiteL->setSizePolicy(sizePolicy3);
        leGd2SiteL->setMinimumSize(QSize(100, 0));
        leGd2SiteL->setMaximumSize(QSize(100, 16777215));
        leGd2SiteL->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd2SiteB = new QLineEdit(groupBox_15);
        leGd2SiteB->setObjectName(QStringLiteral("leGd2SiteB"));
        leGd2SiteB->setGeometry(QRect(51, 78, 100, 20));
        sizePolicy3.setHeightForWidth(leGd2SiteB->sizePolicy().hasHeightForWidth());
        leGd2SiteB->setSizePolicy(sizePolicy3);
        leGd2SiteB->setMinimumSize(QSize(100, 0));
        leGd2SiteB->setMaximumSize(QSize(100, 16777215));
        leGd2SiteB->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leStdPoleSiteL = new QLineEdit(groupBox_15);
        leStdPoleSiteL->setObjectName(QStringLiteral("leStdPoleSiteL"));
        leStdPoleSiteL->setGeometry(QRect(160, 105, 100, 20));
        sizePolicy3.setHeightForWidth(leStdPoleSiteL->sizePolicy().hasHeightForWidth());
        leStdPoleSiteL->setSizePolicy(sizePolicy3);
        leStdPoleSiteL->setMinimumSize(QSize(100, 0));
        leStdPoleSiteL->setMaximumSize(QSize(100, 16777215));
        leStdPoleSiteL->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leStdPoleSiteB = new QLineEdit(groupBox_15);
        leStdPoleSiteB->setObjectName(QStringLiteral("leStdPoleSiteB"));
        leStdPoleSiteB->setGeometry(QRect(51, 105, 100, 20));
        sizePolicy3.setHeightForWidth(leStdPoleSiteB->sizePolicy().hasHeightForWidth());
        leStdPoleSiteB->setSizePolicy(sizePolicy3);
        leStdPoleSiteB->setMinimumSize(QSize(100, 0));
        leStdPoleSiteB->setMaximumSize(QSize(100, 16777215));
        leStdPoleSiteB->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_52 = new QLabel(groupBox_15);
        lblCurrentPos_52->setObjectName(QStringLiteral("lblCurrentPos_52"));
        lblCurrentPos_52->setGeometry(QRect(7, 108, 35, 16));
        lblCurrentPos_52->setMinimumSize(QSize(35, 0));
        lblCurrentPos_52->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_52->setFont(font2);
        lblCurrentPos_52->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_53 = new QLabel(groupBox_15);
        lblCurrentPos_53->setObjectName(QStringLiteral("lblCurrentPos_53"));
        lblCurrentPos_53->setGeometry(QRect(180, 30, 40, 16));
        lblCurrentPos_53->setMinimumSize(QSize(40, 0));
        lblCurrentPos_53->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_53->setFont(font2);
        lblCurrentPos_53->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_54 = new QLabel(groupBox_15);
        lblCurrentPos_54->setObjectName(QStringLiteral("lblCurrentPos_54"));
        lblCurrentPos_54->setGeometry(QRect(70, 30, 40, 16));
        lblCurrentPos_54->setMinimumSize(QSize(40, 0));
        lblCurrentPos_54->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_54->setFont(font2);
        lblCurrentPos_54->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_55 = new QLabel(groupBox_15);
        lblCurrentPos_55->setObjectName(QStringLiteral("lblCurrentPos_55"));
        lblCurrentPos_55->setGeometry(QRect(290, 30, 30, 16));
        lblCurrentPos_55->setMinimumSize(QSize(30, 0));
        lblCurrentPos_55->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_55->setFont(font2);
        lblCurrentPos_55->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGd2SiteH = new QLineEdit(groupBox_15);
        leGd2SiteH->setObjectName(QStringLiteral("leGd2SiteH"));
        leGd2SiteH->setGeometry(QRect(269, 78, 100, 20));
        sizePolicy3.setHeightForWidth(leGd2SiteH->sizePolicy().hasHeightForWidth());
        leGd2SiteH->setSizePolicy(sizePolicy3);
        leGd2SiteH->setMinimumSize(QSize(100, 0));
        leGd2SiteH->setMaximumSize(QSize(100, 16777215));
        leGd2SiteH->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd1SiteH = new QLineEdit(groupBox_15);
        leGd1SiteH->setObjectName(QStringLiteral("leGd1SiteH"));
        leGd1SiteH->setGeometry(QRect(269, 51, 100, 20));
        sizePolicy3.setHeightForWidth(leGd1SiteH->sizePolicy().hasHeightForWidth());
        leGd1SiteH->setSizePolicy(sizePolicy3);
        leGd1SiteH->setMinimumSize(QSize(100, 0));
        leGd1SiteH->setMaximumSize(QSize(100, 16777215));
        leGd1SiteH->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leStdPoleSiteH = new QLineEdit(groupBox_15);
        leStdPoleSiteH->setObjectName(QStringLiteral("leStdPoleSiteH"));
        leStdPoleSiteH->setGeometry(QRect(269, 104, 100, 20));
        sizePolicy3.setHeightForWidth(leStdPoleSiteH->sizePolicy().hasHeightForWidth());
        leStdPoleSiteH->setSizePolicy(sizePolicy3);
        leStdPoleSiteH->setMinimumSize(QSize(100, 0));
        leStdPoleSiteH->setMaximumSize(QSize(100, 16777215));
        leStdPoleSiteH->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pbcalcBlh2Rae = new QPushButton(tab_2);
        pbcalcBlh2Rae->setObjectName(QStringLiteral("pbcalcBlh2Rae"));
        pbcalcBlh2Rae->setGeometry(QRect(450, 80, 75, 30));
        pbcalcBlh2Rae->setMinimumSize(QSize(75, 30));
        pbcalcBlh2Rae->setMaximumSize(QSize(75, 30));
        pbcalcBlh2Rae->setFont(font2);
        pbcalcBlh2Rae->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        groupBox_16 = new QGroupBox(tab_2);
        groupBox_16->setObjectName(QStringLiteral("groupBox_16"));
        groupBox_16->setGeometry(QRect(230, 162, 201, 181));
        groupBox_16->setFont(font2);
        leGD2Blh2A = new QLineEdit(groupBox_16);
        leGD2Blh2A->setObjectName(QStringLiteral("leGD2Blh2A"));
        leGD2Blh2A->setGeometry(QRect(75, 51, 100, 20));
        sizePolicy3.setHeightForWidth(leGD2Blh2A->sizePolicy().hasHeightForWidth());
        leGD2Blh2A->setSizePolicy(sizePolicy3);
        leGD2Blh2A->setMinimumSize(QSize(100, 0));
        leGD2Blh2A->setMaximumSize(QSize(100, 16777215));
        leGD2Blh2A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_62 = new QLabel(groupBox_16);
        lblCurrentPos_62->setObjectName(QStringLiteral("lblCurrentPos_62"));
        lblCurrentPos_62->setGeometry(QRect(31, 54, 35, 16));
        lblCurrentPos_62->setMinimumSize(QSize(35, 0));
        lblCurrentPos_62->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_62->setFont(font2);
        lblCurrentPos_62->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_63 = new QLabel(groupBox_16);
        lblCurrentPos_63->setObjectName(QStringLiteral("lblCurrentPos_63"));
        lblCurrentPos_63->setGeometry(QRect(30, 81, 35, 16));
        lblCurrentPos_63->setMinimumSize(QSize(35, 0));
        lblCurrentPos_63->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_63->setFont(font2);
        lblCurrentPos_63->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD2Blh2E = new QLineEdit(groupBox_16);
        leGD2Blh2E->setObjectName(QStringLiteral("leGD2Blh2E"));
        leGD2Blh2E->setGeometry(QRect(74, 78, 100, 20));
        sizePolicy3.setHeightForWidth(leGD2Blh2E->sizePolicy().hasHeightForWidth());
        leGD2Blh2E->setSizePolicy(sizePolicy3);
        leGD2Blh2E->setMinimumSize(QSize(100, 0));
        leGD2Blh2E->setMaximumSize(QSize(100, 16777215));
        leGD2Blh2E->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_64 = new QLabel(groupBox_16);
        lblCurrentPos_64->setObjectName(QStringLiteral("lblCurrentPos_64"));
        lblCurrentPos_64->setGeometry(QRect(30, 107, 35, 16));
        lblCurrentPos_64->setMinimumSize(QSize(35, 0));
        lblCurrentPos_64->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_64->setFont(font2);
        lblCurrentPos_64->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD2Blh2R = new QLineEdit(groupBox_16);
        leGD2Blh2R->setObjectName(QStringLiteral("leGD2Blh2R"));
        leGD2Blh2R->setGeometry(QRect(74, 104, 100, 20));
        sizePolicy3.setHeightForWidth(leGD2Blh2R->sizePolicy().hasHeightForWidth());
        leGD2Blh2R->setSizePolicy(sizePolicy3);
        leGD2Blh2R->setMinimumSize(QSize(100, 0));
        leGD2Blh2R->setMaximumSize(QSize(100, 16777215));
        leGD2Blh2R->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pbSetGD2Direction = new QPushButton(groupBox_16);
        pbSetGD2Direction->setObjectName(QStringLiteral("pbSetGD2Direction"));
        pbSetGD2Direction->setGeometry(QRect(76, 20, 100, 30));
        pbSetGD2Direction->setMinimumSize(QSize(75, 30));
        pbSetGD2Direction->setMaximumSize(QSize(200, 30));
        pbSetGD2Direction->setFont(font2);
        pbSetGD2Direction->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        lblCurrentPos_65 = new QLabel(groupBox_16);
        lblCurrentPos_65->setObjectName(QStringLiteral("lblCurrentPos_65"));
        lblCurrentPos_65->setGeometry(QRect(175, 54, 16, 16));
        lblCurrentPos_65->setMinimumSize(QSize(0, 0));
        lblCurrentPos_65->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_65->setFont(font2);
        lblCurrentPos_65->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_66 = new QLabel(groupBox_16);
        lblCurrentPos_66->setObjectName(QStringLiteral("lblCurrentPos_66"));
        lblCurrentPos_66->setGeometry(QRect(175, 80, 16, 16));
        lblCurrentPos_66->setMinimumSize(QSize(0, 0));
        lblCurrentPos_66->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_66->setFont(font2);
        lblCurrentPos_66->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_67 = new QLabel(groupBox_16);
        lblCurrentPos_67->setObjectName(QStringLiteral("lblCurrentPos_67"));
        lblCurrentPos_67->setGeometry(QRect(175, 104, 16, 16));
        lblCurrentPos_67->setMinimumSize(QSize(0, 0));
        lblCurrentPos_67->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_67->setFont(font2);
        lblCurrentPos_67->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_70 = new QLabel(groupBox_16);
        lblCurrentPos_70->setObjectName(QStringLiteral("lblCurrentPos_70"));
        lblCurrentPos_70->setGeometry(QRect(175, 133, 16, 16));
        lblCurrentPos_70->setMinimumSize(QSize(0, 0));
        lblCurrentPos_70->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_70->setFont(font2);
        lblCurrentPos_70->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_71 = new QLabel(groupBox_16);
        lblCurrentPos_71->setObjectName(QStringLiteral("lblCurrentPos_71"));
        lblCurrentPos_71->setGeometry(QRect(175, 159, 16, 16));
        lblCurrentPos_71->setMinimumSize(QSize(0, 0));
        lblCurrentPos_71->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_71->setFont(font2);
        lblCurrentPos_71->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD2AntennaA = new QLineEdit(groupBox_16);
        leGD2AntennaA->setObjectName(QStringLiteral("leGD2AntennaA"));
        leGD2AntennaA->setGeometry(QRect(75, 130, 100, 20));
        sizePolicy3.setHeightForWidth(leGD2AntennaA->sizePolicy().hasHeightForWidth());
        leGD2AntennaA->setSizePolicy(sizePolicy3);
        leGD2AntennaA->setMinimumSize(QSize(100, 0));
        leGD2AntennaA->setMaximumSize(QSize(100, 16777215));
        leGD2AntennaA->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_58 = new QLabel(groupBox_16);
        lblCurrentPos_58->setObjectName(QStringLiteral("lblCurrentPos_58"));
        lblCurrentPos_58->setGeometry(QRect(9, 133, 60, 16));
        lblCurrentPos_58->setMinimumSize(QSize(60, 0));
        lblCurrentPos_58->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_58->setFont(font2);
        lblCurrentPos_58->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD2AntennaE = new QLineEdit(groupBox_16);
        leGD2AntennaE->setObjectName(QStringLiteral("leGD2AntennaE"));
        leGD2AntennaE->setGeometry(QRect(74, 157, 100, 20));
        sizePolicy3.setHeightForWidth(leGD2AntennaE->sizePolicy().hasHeightForWidth());
        leGD2AntennaE->setSizePolicy(sizePolicy3);
        leGD2AntennaE->setMinimumSize(QSize(100, 0));
        leGD2AntennaE->setMaximumSize(QSize(100, 16777215));
        leGD2AntennaE->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_72 = new QLabel(groupBox_16);
        lblCurrentPos_72->setObjectName(QStringLiteral("lblCurrentPos_72"));
        lblCurrentPos_72->setGeometry(QRect(8, 160, 60, 16));
        lblCurrentPos_72->setMinimumSize(QSize(60, 0));
        lblCurrentPos_72->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_72->setFont(font2);
        lblCurrentPos_72->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbcalcRaeClear = new QPushButton(tab_2);
        pbcalcRaeClear->setObjectName(QStringLiteral("pbcalcRaeClear"));
        pbcalcRaeClear->setGeometry(QRect(450, 240, 75, 30));
        pbcalcRaeClear->setMinimumSize(QSize(75, 30));
        pbcalcRaeClear->setMaximumSize(QSize(75, 30));
        pbcalcRaeClear->setFont(font2);
        pbcalcRaeClear->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        layoutWidget_2 = new QWidget(tab_2);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(20, 360, 305, 35));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font1);

        horizontalLayout->addWidget(label_9);

        le_setDirecion = new QLineEdit(layoutWidget_2);
        le_setDirecion->setObjectName(QStringLiteral("le_setDirecion"));
        sizePolicy3.setHeightForWidth(le_setDirecion->sizePolicy().hasHeightForWidth());
        le_setDirecion->setSizePolicy(sizePolicy3);
        le_setDirecion->setMaximumSize(QSize(60, 16777215));
        le_setDirecion->setFont(font1);
        le_setDirecion->setFocusPolicy(Qt::ClickFocus);
        le_setDirecion->setAcceptDrops(false);
        le_setDirecion->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        le_setDirecion->setMaxLength(7);

        horizontalLayout->addWidget(le_setDirecion);

        label_10 = new QLabel(layoutWidget_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font1);

        horizontalLayout->addWidget(label_10);

        le_setPitch = new QLineEdit(layoutWidget_2);
        le_setPitch->setObjectName(QStringLiteral("le_setPitch"));
        sizePolicy3.setHeightForWidth(le_setPitch->sizePolicy().hasHeightForWidth());
        le_setPitch->setSizePolicy(sizePolicy3);
        le_setPitch->setMaximumSize(QSize(60, 16777215));
        le_setPitch->setFont(font1);
        le_setPitch->setFocusPolicy(Qt::ClickFocus);
        le_setPitch->setAcceptDrops(false);
        le_setPitch->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        le_setPitch->setInputMethodHints(Qt::ImhNone);
        le_setPitch->setMaxLength(7);

        horizontalLayout->addWidget(le_setPitch);

        pbSendDirection = new QPushButton(layoutWidget_2);
        pbSendDirection->setObjectName(QStringLiteral("pbSendDirection"));
        pbSendDirection->setMinimumSize(QSize(75, 30));
        pbSendDirection->setMaximumSize(QSize(75, 30));
        pbSendDirection->setFont(font1);
        pbSendDirection->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));

        horizontalLayout->addWidget(pbSendDirection);

        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(820, 10, 430, 180));
        sizePolicy4.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy4);
        groupBox_3->setMinimumSize(QSize(430, 180));
        groupBox_3->setMaximumSize(QSize(430, 180));
        groupBox_3->setFont(font1);
        groupBox_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(71, 18, 41, 16));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(68, 111, 31, 16));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(124, 61, 21, 16));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(8, 58, 25, 20));
        lblPadDirection = new QLabel(groupBox_3);
        lblPadDirection->setObjectName(QStringLiteral("lblPadDirection"));
        lblPadDirection->setGeometry(QRect(5, 21, 420, 110));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lblPadDirection->sizePolicy().hasHeightForWidth());
        lblPadDirection->setSizePolicy(sizePolicy5);
        lblPadDirection->setMinimumSize(QSize(420, 110));
        lblPadDirection->setMaximumSize(QSize(420, 110));
        lblPadDirection->setFrameShape(QFrame::NoFrame);
        lblPadDirection->raise();
        label_2->raise();
        label_4->raise();
        label_7->raise();
        label_8->raise();
        groupBox_8 = new QGroupBox(tab_2);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(820, 200, 430, 180));
        sizePolicy4.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy4);
        groupBox_8->setMinimumSize(QSize(430, 180));
        groupBox_8->setMaximumSize(QSize(430, 180));
        groupBox_8->setFont(font1);
        groupBox_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_11 = new QLabel(groupBox_8);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(72, 19, 41, 16));
        label_12 = new QLabel(groupBox_8);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(68, 111, 31, 16));
        label_13 = new QLabel(groupBox_8);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(124, 61, 21, 16));
        label_14 = new QLabel(groupBox_8);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(8, 58, 25, 20));
        lblPadDirection_B = new QLabel(groupBox_8);
        lblPadDirection_B->setObjectName(QStringLiteral("lblPadDirection_B"));
        lblPadDirection_B->setGeometry(QRect(5, 19, 420, 110));
        sizePolicy5.setHeightForWidth(lblPadDirection_B->sizePolicy().hasHeightForWidth());
        lblPadDirection_B->setSizePolicy(sizePolicy5);
        lblPadDirection_B->setMinimumSize(QSize(420, 110));
        lblPadDirection_B->setMaximumSize(QSize(420, 110));
        lblPadDirection_B->setFrameShape(QFrame::NoFrame);
        lblPadDirection_B->raise();
        label_11->raise();
        label_12->raise();
        label_13->raise();
        label_14->raise();
        groupBox_21 = new QGroupBox(tab_2);
        groupBox_21->setObjectName(QStringLiteral("groupBox_21"));
        groupBox_21->setGeometry(QRect(605, 13, 201, 191));
        pbAdjDriftDown = new QPushButton(groupBox_21);
        pbAdjDriftDown->setObjectName(QStringLiteral("pbAdjDriftDown"));
        pbAdjDriftDown->setGeometry(QRect(80, 100, 50, 30));
        pbAdjDriftDown->setMinimumSize(QSize(50, 30));
        pbAdjDriftDown->setMaximumSize(QSize(50, 30));
        pbAdjDriftDown->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbAdjDriftUp = new QPushButton(groupBox_21);
        pbAdjDriftUp->setObjectName(QStringLiteral("pbAdjDriftUp"));
        pbAdjDriftUp->setGeometry(QRect(80, 30, 50, 30));
        pbAdjDriftUp->setMinimumSize(QSize(50, 30));
        pbAdjDriftUp->setMaximumSize(QSize(50, 30));
        pbAdjDriftUp->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbAAdjDriftSave = new QPushButton(groupBox_21);
        pbAAdjDriftSave->setObjectName(QStringLiteral("pbAAdjDriftSave"));
        pbAAdjDriftSave->setEnabled(true);
        pbAAdjDriftSave->setGeometry(QRect(70, 140, 80, 40));
        sizePolicy3.setHeightForWidth(pbAAdjDriftSave->sizePolicy().hasHeightForWidth());
        pbAAdjDriftSave->setSizePolicy(sizePolicy3);
        pbAAdjDriftSave->setMinimumSize(QSize(70, 40));
        pbAAdjDriftSave->setMaximumSize(QSize(80, 40));
        pbAAdjDriftSave->setFont(font2);
        pbAAdjDriftSave->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbAdjDriftLeft = new QPushButton(groupBox_21);
        pbAdjDriftLeft->setObjectName(QStringLiteral("pbAdjDriftLeft"));
        pbAdjDriftLeft->setGeometry(QRect(20, 66, 50, 30));
        pbAdjDriftLeft->setMinimumSize(QSize(50, 30));
        pbAdjDriftLeft->setMaximumSize(QSize(50, 30));
        pbAdjDriftLeft->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbAdjDriftRight = new QPushButton(groupBox_21);
        pbAdjDriftRight->setObjectName(QStringLiteral("pbAdjDriftRight"));
        pbAdjDriftRight->setGeometry(QRect(140, 66, 50, 30));
        pbAdjDriftRight->setMinimumSize(QSize(50, 30));
        pbAdjDriftRight->setMaximumSize(QSize(50, 30));
        pbAdjDriftRight->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbAdjDriftStop = new QPushButton(groupBox_21);
        pbAdjDriftStop->setObjectName(QStringLiteral("pbAdjDriftStop"));
        pbAdjDriftStop->setGeometry(QRect(80, 66, 50, 30));
        pbAdjDriftStop->setMinimumSize(QSize(50, 30));
        pbAdjDriftStop->setMaximumSize(QSize(50, 30));
        pbAdjDriftStop->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        tabWidConfig->addTab(tab_2, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        groupBox_4 = new QGroupBox(tab_8);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(17, 1, 481, 151));
        groupBox_4->setMinimumSize(QSize(300, 0));
        groupBox_4->setFont(font2);
        lblCurrentPos_15 = new QLabel(groupBox_4);
        lblCurrentPos_15->setObjectName(QStringLiteral("lblCurrentPos_15"));
        lblCurrentPos_15->setGeometry(QRect(25, 44, 70, 16));
        lblCurrentPos_15->setMinimumSize(QSize(70, 0));
        lblCurrentPos_15->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_15->setFont(font2);
        lblCurrentPos_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetHdCamPort = new QLineEdit(groupBox_4);
        leSetHdCamPort->setObjectName(QStringLiteral("leSetHdCamPort"));
        leSetHdCamPort->setGeometry(QRect(104, 70, 51, 20));
        sizePolicy3.setHeightForWidth(leSetHdCamPort->sizePolicy().hasHeightForWidth());
        leSetHdCamPort->setSizePolicy(sizePolicy3);
        leSetHdCamPort->setMaximumSize(QSize(100, 16777215));
        leSetHdCamPort->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetHdCamIP = new QLineEdit(groupBox_4);
        leSetHdCamIP->setObjectName(QStringLiteral("leSetHdCamIP"));
        leSetHdCamIP->setGeometry(QRect(104, 43, 100, 20));
        sizePolicy3.setHeightForWidth(leSetHdCamIP->sizePolicy().hasHeightForWidth());
        leSetHdCamIP->setSizePolicy(sizePolicy3);
        leSetHdCamIP->setMaximumSize(QSize(100, 16777215));
        leSetHdCamIP->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pb_OpenSelfTest = new QPushButton(groupBox_4);
        pb_OpenSelfTest->setObjectName(QStringLiteral("pb_OpenSelfTest"));
        pb_OpenSelfTest->setGeometry(QRect(220, 10, 110, 23));
        pb_OpenSelfTest->setMaximumSize(QSize(110, 16777215));
        pb_OpenSelfTest->setFont(font2);
        leSetIP = new QLineEdit(groupBox_4);
        leSetIP->setObjectName(QStringLiteral("leSetIP"));
        leSetIP->setGeometry(QRect(104, 16, 100, 20));
        sizePolicy3.setHeightForWidth(leSetIP->sizePolicy().hasHeightForWidth());
        leSetIP->setSizePolicy(sizePolicy3);
        leSetIP->setMaximumSize(QSize(100, 16777215));
        leSetIP->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_12 = new QLabel(groupBox_4);
        lblCurrentPos_12->setObjectName(QStringLiteral("lblCurrentPos_12"));
        lblCurrentPos_12->setGeometry(QRect(26, 71, 70, 20));
        lblCurrentPos_12->setMinimumSize(QSize(70, 0));
        lblCurrentPos_12->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_12->setFont(font2);
        lblCurrentPos_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_9 = new QLabel(groupBox_4);
        lblCurrentPos_9->setObjectName(QStringLiteral("lblCurrentPos_9"));
        lblCurrentPos_9->setGeometry(QRect(25, 19, 70, 16));
        lblCurrentPos_9->setMinimumSize(QSize(70, 0));
        lblCurrentPos_9->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_9->setFont(font2);
        lblCurrentPos_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pb_CloseSelfTest = new QPushButton(groupBox_4);
        pb_CloseSelfTest->setObjectName(QStringLiteral("pb_CloseSelfTest"));
        pb_CloseSelfTest->setGeometry(QRect(220, 40, 110, 23));
        pb_CloseSelfTest->setMaximumSize(QSize(110, 16777215));
        pb_CloseSelfTest->setFont(font2);
        leSetDiffY_default_A = new QLineEdit(groupBox_4);
        leSetDiffY_default_A->setObjectName(QStringLiteral("leSetDiffY_default_A"));
        leSetDiffY_default_A->setGeometry(QRect(105, 96, 81, 20));
        sizePolicy3.setHeightForWidth(leSetDiffY_default_A->sizePolicy().hasHeightForWidth());
        leSetDiffY_default_A->setSizePolicy(sizePolicy3);
        leSetDiffY_default_A->setMaximumSize(QSize(100, 16777215));
        leSetDiffY_default_A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_DiffY_default = new QLabel(groupBox_4);
        lbl_DiffY_default->setObjectName(QStringLiteral("lbl_DiffY_default"));
        lbl_DiffY_default->setGeometry(QRect(26, 97, 70, 20));
        lbl_DiffY_default->setMinimumSize(QSize(70, 0));
        lbl_DiffY_default->setMaximumSize(QSize(40, 16777215));
        lbl_DiffY_default->setFont(font2);
        lbl_DiffY_default->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_DiffX_default = new QLabel(groupBox_4);
        lbl_DiffX_default->setObjectName(QStringLiteral("lbl_DiffX_default"));
        lbl_DiffX_default->setGeometry(QRect(26, 122, 70, 20));
        lbl_DiffX_default->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default->setFont(font2);
        lbl_DiffX_default->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetDiffX_default_A = new QLineEdit(groupBox_4);
        leSetDiffX_default_A->setObjectName(QStringLiteral("leSetDiffX_default_A"));
        leSetDiffX_default_A->setGeometry(QRect(105, 123, 81, 20));
        sizePolicy3.setHeightForWidth(leSetDiffX_default_A->sizePolicy().hasHeightForWidth());
        leSetDiffX_default_A->setSizePolicy(sizePolicy3);
        leSetDiffX_default_A->setMaximumSize(QSize(100, 16777215));
        leSetDiffX_default_A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetRadarDiffX_A = new QLineEdit(groupBox_4);
        leSetRadarDiffX_A->setObjectName(QStringLiteral("leSetRadarDiffX_A"));
        leSetRadarDiffX_A->setGeometry(QRect(266, 96, 51, 20));
        sizePolicy3.setHeightForWidth(leSetRadarDiffX_A->sizePolicy().hasHeightForWidth());
        leSetRadarDiffX_A->setSizePolicy(sizePolicy3);
        leSetRadarDiffX_A->setMaximumSize(QSize(100, 16777215));
        leSetRadarDiffX_A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_DiffX_default_3 = new QLabel(groupBox_4);
        lbl_DiffX_default_3->setObjectName(QStringLiteral("lbl_DiffX_default_3"));
        lbl_DiffX_default_3->setGeometry(QRect(197, 97, 70, 20));
        lbl_DiffX_default_3->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default_3->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default_3->setFont(font2);
        lbl_DiffX_default_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_DiffX_default_5 = new QLabel(groupBox_4);
        lbl_DiffX_default_5->setObjectName(QStringLiteral("lbl_DiffX_default_5"));
        lbl_DiffX_default_5->setGeometry(QRect(196, 123, 70, 20));
        lbl_DiffX_default_5->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default_5->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default_5->setFont(font2);
        lbl_DiffX_default_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetRadarDiffY_A = new QLineEdit(groupBox_4);
        leSetRadarDiffY_A->setObjectName(QStringLiteral("leSetRadarDiffY_A"));
        leSetRadarDiffY_A->setGeometry(QRect(265, 124, 51, 20));
        sizePolicy3.setHeightForWidth(leSetRadarDiffY_A->sizePolicy().hasHeightForWidth());
        leSetRadarDiffY_A->setSizePolicy(sizePolicy3);
        leSetRadarDiffY_A->setMaximumSize(QSize(100, 16777215));
        leSetRadarDiffY_A->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_18 = new QLabel(groupBox_4);
        lblCurrentPos_18->setObjectName(QStringLiteral("lblCurrentPos_18"));
        lblCurrentPos_18->setGeometry(QRect(338, 16, 70, 20));
        lblCurrentPos_18->setMinimumSize(QSize(70, 0));
        lblCurrentPos_18->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_18->setFont(font2);
        lblCurrentPos_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leHdaDefFoc = new QLineEdit(groupBox_4);
        leHdaDefFoc->setObjectName(QStringLiteral("leHdaDefFoc"));
        leHdaDefFoc->setGeometry(QRect(416, 15, 51, 20));
        sizePolicy3.setHeightForWidth(leHdaDefFoc->sizePolicy().hasHeightForWidth());
        leHdaDefFoc->setSizePolicy(sizePolicy3);
        leHdaDefFoc->setMaximumSize(QSize(100, 16777215));
        leHdaDefFoc->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        cbxUseDefFocA = new QCheckBox(groupBox_4);
        cbxUseDefFocA->setObjectName(QStringLiteral("cbxUseDefFocA"));
        cbxUseDefFocA->setGeometry(QRect(350, 43, 111, 16));
        cbxUseDefFocA->setFont(font2);
        cbxUseDefFocA->setChecked(false);
        pb_CloseSelfTest->raise();
        lblCurrentPos_15->raise();
        leSetHdCamPort->raise();
        leSetHdCamIP->raise();
        pb_OpenSelfTest->raise();
        leSetIP->raise();
        lblCurrentPos_12->raise();
        lblCurrentPos_9->raise();
        leSetDiffY_default_A->raise();
        lbl_DiffY_default->raise();
        lbl_DiffX_default->raise();
        leSetDiffX_default_A->raise();
        leSetRadarDiffX_A->raise();
        lbl_DiffX_default_3->raise();
        lbl_DiffX_default_5->raise();
        leSetRadarDiffY_A->raise();
        lblCurrentPos_18->raise();
        leHdaDefFoc->raise();
        cbxUseDefFocA->raise();
        groupBox_6 = new QGroupBox(tab_8);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(16, 151, 481, 141));
        groupBox_6->setMinimumSize(QSize(300, 0));
        groupBox_6->setFont(font2);
        lblCurrentPos_16 = new QLabel(groupBox_6);
        lblCurrentPos_16->setObjectName(QStringLiteral("lblCurrentPos_16"));
        lblCurrentPos_16->setGeometry(QRect(25, 41, 70, 16));
        lblCurrentPos_16->setMinimumSize(QSize(70, 0));
        lblCurrentPos_16->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_16->setFont(font2);
        lblCurrentPos_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetHdCamPort_B = new QLineEdit(groupBox_6);
        leSetHdCamPort_B->setObjectName(QStringLiteral("leSetHdCamPort_B"));
        leSetHdCamPort_B->setGeometry(QRect(104, 65, 51, 20));
        sizePolicy3.setHeightForWidth(leSetHdCamPort_B->sizePolicy().hasHeightForWidth());
        leSetHdCamPort_B->setSizePolicy(sizePolicy3);
        leSetHdCamPort_B->setMaximumSize(QSize(100, 16777215));
        leSetHdCamPort_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetHdCamIP_B = new QLineEdit(groupBox_6);
        leSetHdCamIP_B->setObjectName(QStringLiteral("leSetHdCamIP_B"));
        leSetHdCamIP_B->setGeometry(QRect(104, 40, 100, 20));
        sizePolicy3.setHeightForWidth(leSetHdCamIP_B->sizePolicy().hasHeightForWidth());
        leSetHdCamIP_B->setSizePolicy(sizePolicy3);
        leSetHdCamIP_B->setMaximumSize(QSize(100, 16777215));
        leSetHdCamIP_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pb_OpenSelfTest_B = new QPushButton(groupBox_6);
        pb_OpenSelfTest_B->setObjectName(QStringLiteral("pb_OpenSelfTest_B"));
        pb_OpenSelfTest_B->setGeometry(QRect(220, 20, 110, 23));
        pb_OpenSelfTest_B->setMaximumSize(QSize(110, 16777215));
        pb_OpenSelfTest_B->setFont(font2);
        leSetIP_B = new QLineEdit(groupBox_6);
        leSetIP_B->setObjectName(QStringLiteral("leSetIP_B"));
        leSetIP_B->setGeometry(QRect(104, 15, 100, 20));
        sizePolicy3.setHeightForWidth(leSetIP_B->sizePolicy().hasHeightForWidth());
        leSetIP_B->setSizePolicy(sizePolicy3);
        leSetIP_B->setMaximumSize(QSize(100, 16777215));
        leSetIP_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_13 = new QLabel(groupBox_6);
        lblCurrentPos_13->setObjectName(QStringLiteral("lblCurrentPos_13"));
        lblCurrentPos_13->setGeometry(QRect(27, 66, 70, 20));
        lblCurrentPos_13->setMinimumSize(QSize(70, 0));
        lblCurrentPos_13->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_13->setFont(font2);
        lblCurrentPos_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_11 = new QLabel(groupBox_6);
        lblCurrentPos_11->setObjectName(QStringLiteral("lblCurrentPos_11"));
        lblCurrentPos_11->setGeometry(QRect(25, 18, 70, 16));
        lblCurrentPos_11->setMinimumSize(QSize(70, 0));
        lblCurrentPos_11->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_11->setFont(font2);
        lblCurrentPos_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pb_CloseSelfTest_B = new QPushButton(groupBox_6);
        pb_CloseSelfTest_B->setObjectName(QStringLiteral("pb_CloseSelfTest_B"));
        pb_CloseSelfTest_B->setGeometry(QRect(220, 50, 110, 23));
        pb_CloseSelfTest_B->setMaximumSize(QSize(110, 16777215));
        pb_CloseSelfTest_B->setFont(font2);
        leSetDiffY_default_B = new QLineEdit(groupBox_6);
        leSetDiffY_default_B->setObjectName(QStringLiteral("leSetDiffY_default_B"));
        leSetDiffY_default_B->setGeometry(QRect(105, 90, 81, 20));
        sizePolicy3.setHeightForWidth(leSetDiffY_default_B->sizePolicy().hasHeightForWidth());
        leSetDiffY_default_B->setSizePolicy(sizePolicy3);
        leSetDiffY_default_B->setMaximumSize(QSize(100, 16777215));
        leSetDiffY_default_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetDiffX_default_B = new QLineEdit(groupBox_6);
        leSetDiffX_default_B->setObjectName(QStringLiteral("leSetDiffX_default_B"));
        leSetDiffX_default_B->setGeometry(QRect(105, 114, 81, 20));
        sizePolicy3.setHeightForWidth(leSetDiffX_default_B->sizePolicy().hasHeightForWidth());
        leSetDiffX_default_B->setSizePolicy(sizePolicy3);
        leSetDiffX_default_B->setMaximumSize(QSize(100, 16777215));
        leSetDiffX_default_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_DiffY_default_2 = new QLabel(groupBox_6);
        lbl_DiffY_default_2->setObjectName(QStringLiteral("lbl_DiffY_default_2"));
        lbl_DiffY_default_2->setGeometry(QRect(27, 90, 70, 20));
        lbl_DiffY_default_2->setMinimumSize(QSize(70, 0));
        lbl_DiffY_default_2->setMaximumSize(QSize(40, 16777215));
        lbl_DiffY_default_2->setFont(font2);
        lbl_DiffY_default_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_DiffX_default_2 = new QLabel(groupBox_6);
        lbl_DiffX_default_2->setObjectName(QStringLiteral("lbl_DiffX_default_2"));
        lbl_DiffX_default_2->setGeometry(QRect(27, 112, 70, 20));
        lbl_DiffX_default_2->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default_2->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default_2->setFont(font2);
        lbl_DiffX_default_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbl_DiffX_default_4 = new QLabel(groupBox_6);
        lbl_DiffX_default_4->setObjectName(QStringLiteral("lbl_DiffX_default_4"));
        lbl_DiffX_default_4->setGeometry(QRect(196, 89, 70, 20));
        lbl_DiffX_default_4->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default_4->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default_4->setFont(font2);
        lbl_DiffX_default_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetRadarDiffX_B = new QLineEdit(groupBox_6);
        leSetRadarDiffX_B->setObjectName(QStringLiteral("leSetRadarDiffX_B"));
        leSetRadarDiffX_B->setGeometry(QRect(265, 90, 51, 20));
        sizePolicy3.setHeightForWidth(leSetRadarDiffX_B->sizePolicy().hasHeightForWidth());
        leSetRadarDiffX_B->setSizePolicy(sizePolicy3);
        leSetRadarDiffX_B->setMaximumSize(QSize(100, 16777215));
        leSetRadarDiffX_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetRadarDiffY_B = new QLineEdit(groupBox_6);
        leSetRadarDiffY_B->setObjectName(QStringLiteral("leSetRadarDiffY_B"));
        leSetRadarDiffY_B->setGeometry(QRect(265, 114, 51, 20));
        sizePolicy3.setHeightForWidth(leSetRadarDiffY_B->sizePolicy().hasHeightForWidth());
        leSetRadarDiffY_B->setSizePolicy(sizePolicy3);
        leSetRadarDiffY_B->setMaximumSize(QSize(100, 16777215));
        leSetRadarDiffY_B->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_DiffX_default_6 = new QLabel(groupBox_6);
        lbl_DiffX_default_6->setObjectName(QStringLiteral("lbl_DiffX_default_6"));
        lbl_DiffX_default_6->setGeometry(QRect(196, 113, 70, 20));
        lbl_DiffX_default_6->setMinimumSize(QSize(70, 0));
        lbl_DiffX_default_6->setMaximumSize(QSize(40, 16777215));
        lbl_DiffX_default_6->setFont(font2);
        lbl_DiffX_default_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leHdbDefFoc_2 = new QLineEdit(groupBox_6);
        leHdbDefFoc_2->setObjectName(QStringLiteral("leHdbDefFoc_2"));
        leHdbDefFoc_2->setGeometry(QRect(418, 19, 51, 20));
        sizePolicy3.setHeightForWidth(leHdbDefFoc_2->sizePolicy().hasHeightForWidth());
        leHdbDefFoc_2->setSizePolicy(sizePolicy3);
        leHdbDefFoc_2->setMaximumSize(QSize(100, 16777215));
        leHdbDefFoc_2->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_37 = new QLabel(groupBox_6);
        lblCurrentPos_37->setObjectName(QStringLiteral("lblCurrentPos_37"));
        lblCurrentPos_37->setGeometry(QRect(340, 20, 70, 20));
        lblCurrentPos_37->setMinimumSize(QSize(70, 0));
        lblCurrentPos_37->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_37->setFont(font2);
        lblCurrentPos_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cbxUseDefFocB = new QCheckBox(groupBox_6);
        cbxUseDefFocB->setObjectName(QStringLiteral("cbxUseDefFocB"));
        cbxUseDefFocB->setGeometry(QRect(350, 50, 111, 16));
        cbxUseDefFocB->setFont(font2);
        cbxUseDefFocB->setChecked(false);
        pb_CloseSelfTest_B->raise();
        lblCurrentPos_16->raise();
        leSetHdCamPort_B->raise();
        leSetHdCamIP_B->raise();
        pb_OpenSelfTest_B->raise();
        leSetIP_B->raise();
        lblCurrentPos_13->raise();
        lblCurrentPos_11->raise();
        leSetDiffY_default_B->raise();
        leSetDiffX_default_B->raise();
        lbl_DiffY_default_2->raise();
        lbl_DiffX_default_2->raise();
        lbl_DiffX_default_4->raise();
        leSetRadarDiffX_B->raise();
        leSetRadarDiffY_B->raise();
        lbl_DiffX_default_6->raise();
        leHdbDefFoc_2->raise();
        lblCurrentPos_37->raise();
        cbxUseDefFocB->raise();
        pbConfigSave = new QPushButton(tab_8);
        pbConfigSave->setObjectName(QStringLiteral("pbConfigSave"));
        pbConfigSave->setGeometry(QRect(680, 357, 75, 30));
        pbConfigSave->setMinimumSize(QSize(75, 30));
        pbConfigSave->setMaximumSize(QSize(75, 30));
        pbConfigSave->setFont(font2);
        pbConfigSave->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        cbxEditLock_sys = new QCheckBox(tab_8);
        cbxEditLock_sys->setObjectName(QStringLiteral("cbxEditLock_sys"));
        cbxEditLock_sys->setGeometry(QRect(580, 363, 91, 16));
        cbxEditLock_sys->setFont(font2);
        groupBox_17 = new QGroupBox(tab_8);
        groupBox_17->setObjectName(QStringLiteral("groupBox_17"));
        groupBox_17->setGeometry(QRect(17, 291, 321, 121));
        sizePolicy4.setHeightForWidth(groupBox_17->sizePolicy().hasHeightForWidth());
        groupBox_17->setSizePolicy(sizePolicy4);
        groupBox_17->setMinimumSize(QSize(300, 110));
        groupBox_17->setMaximumSize(QSize(321, 136));
        groupBox_17->setFont(font2);
        lblCurrentPos_28 = new QLabel(groupBox_17);
        lblCurrentPos_28->setObjectName(QStringLiteral("lblCurrentPos_28"));
        lblCurrentPos_28->setGeometry(QRect(5, 28, 40, 16));
        lblCurrentPos_28->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_28->setFont(font2);
        lblCurrentPos_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_29 = new QLabel(groupBox_17);
        lblCurrentPos_29->setObjectName(QStringLiteral("lblCurrentPos_29"));
        lblCurrentPos_29->setGeometry(QRect(5, 52, 40, 16));
        lblCurrentPos_29->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_29->setFont(font2);
        lblCurrentPos_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_30 = new QLabel(groupBox_17);
        lblCurrentPos_30->setObjectName(QStringLiteral("lblCurrentPos_30"));
        lblCurrentPos_30->setGeometry(QRect(5, 75, 40, 16));
        lblCurrentPos_30->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_30->setFont(font2);
        lblCurrentPos_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_31 = new QLabel(groupBox_17);
        lblCurrentPos_31->setObjectName(QStringLiteral("lblCurrentPos_31"));
        lblCurrentPos_31->setGeometry(QRect(4, 97, 40, 16));
        lblCurrentPos_31->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_31->setFont(font2);
        lblCurrentPos_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_32 = new QLabel(groupBox_17);
        lblCurrentPos_32->setObjectName(QStringLiteral("lblCurrentPos_32"));
        lblCurrentPos_32->setGeometry(QRect(60, 10, 40, 16));
        lblCurrentPos_32->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_32->setFont(font2);
        lblCurrentPos_32->setAlignment(Qt::AlignCenter);
        lblCurrentPos_33 = new QLabel(groupBox_17);
        lblCurrentPos_33->setObjectName(QStringLiteral("lblCurrentPos_33"));
        lblCurrentPos_33->setGeometry(QRect(126, 10, 40, 16));
        lblCurrentPos_33->setMaximumSize(QSize(40, 16777215));
        lblCurrentPos_33->setFont(font2);
        lblCurrentPos_33->setAlignment(Qt::AlignCenter);
        splitter = new QSplitter(groupBox_17);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(50, 27, 50, 88));
        splitter->setOrientation(Qt::Vertical);
        lePid_X_Scale = new QLineEdit(splitter);
        lePid_X_Scale->setObjectName(QStringLiteral("lePid_X_Scale"));
        sizePolicy3.setHeightForWidth(lePid_X_Scale->sizePolicy().hasHeightForWidth());
        lePid_X_Scale->setSizePolicy(sizePolicy3);
        lePid_X_Scale->setMinimumSize(QSize(50, 0));
        lePid_X_Scale->setMaximumSize(QSize(50, 16777215));
        lePid_X_Scale->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_X_Scale->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(lePid_X_Scale);
        lePid_X_P = new QLineEdit(splitter);
        lePid_X_P->setObjectName(QStringLiteral("lePid_X_P"));
        sizePolicy3.setHeightForWidth(lePid_X_P->sizePolicy().hasHeightForWidth());
        lePid_X_P->setSizePolicy(sizePolicy3);
        lePid_X_P->setMinimumSize(QSize(50, 0));
        lePid_X_P->setMaximumSize(QSize(50, 16777215));
        lePid_X_P->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_X_P->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(lePid_X_P);
        lePid_X_I = new QLineEdit(splitter);
        lePid_X_I->setObjectName(QStringLiteral("lePid_X_I"));
        sizePolicy3.setHeightForWidth(lePid_X_I->sizePolicy().hasHeightForWidth());
        lePid_X_I->setSizePolicy(sizePolicy3);
        lePid_X_I->setMinimumSize(QSize(50, 0));
        lePid_X_I->setMaximumSize(QSize(50, 16777215));
        lePid_X_I->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_X_I->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(lePid_X_I);
        lePid_X_D = new QLineEdit(splitter);
        lePid_X_D->setObjectName(QStringLiteral("lePid_X_D"));
        sizePolicy3.setHeightForWidth(lePid_X_D->sizePolicy().hasHeightForWidth());
        lePid_X_D->setSizePolicy(sizePolicy3);
        lePid_X_D->setMinimumSize(QSize(50, 0));
        lePid_X_D->setMaximumSize(QSize(50, 16777215));
        lePid_X_D->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_X_D->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(lePid_X_D);
        splitter_2 = new QSplitter(groupBox_17);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(120, 27, 50, 88));
        splitter_2->setOrientation(Qt::Vertical);
        lePid_Y_Scale = new QLineEdit(splitter_2);
        lePid_Y_Scale->setObjectName(QStringLiteral("lePid_Y_Scale"));
        sizePolicy3.setHeightForWidth(lePid_Y_Scale->sizePolicy().hasHeightForWidth());
        lePid_Y_Scale->setSizePolicy(sizePolicy3);
        lePid_Y_Scale->setMinimumSize(QSize(50, 0));
        lePid_Y_Scale->setMaximumSize(QSize(50, 16777215));
        lePid_Y_Scale->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_Y_Scale->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter_2->addWidget(lePid_Y_Scale);
        lePid_Y_P = new QLineEdit(splitter_2);
        lePid_Y_P->setObjectName(QStringLiteral("lePid_Y_P"));
        sizePolicy3.setHeightForWidth(lePid_Y_P->sizePolicy().hasHeightForWidth());
        lePid_Y_P->setSizePolicy(sizePolicy3);
        lePid_Y_P->setMinimumSize(QSize(50, 0));
        lePid_Y_P->setMaximumSize(QSize(50, 16777215));
        lePid_Y_P->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_Y_P->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter_2->addWidget(lePid_Y_P);
        lePid_Y_I = new QLineEdit(splitter_2);
        lePid_Y_I->setObjectName(QStringLiteral("lePid_Y_I"));
        sizePolicy3.setHeightForWidth(lePid_Y_I->sizePolicy().hasHeightForWidth());
        lePid_Y_I->setSizePolicy(sizePolicy3);
        lePid_Y_I->setMinimumSize(QSize(50, 0));
        lePid_Y_I->setMaximumSize(QSize(50, 16777215));
        lePid_Y_I->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_Y_I->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter_2->addWidget(lePid_Y_I);
        lePid_Y_D = new QLineEdit(splitter_2);
        lePid_Y_D->setObjectName(QStringLiteral("lePid_Y_D"));
        sizePolicy3.setHeightForWidth(lePid_Y_D->sizePolicy().hasHeightForWidth());
        lePid_Y_D->setSizePolicy(sizePolicy3);
        lePid_Y_D->setMinimumSize(QSize(50, 0));
        lePid_Y_D->setMaximumSize(QSize(50, 16777215));
        lePid_Y_D->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePid_Y_D->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter_2->addWidget(lePid_Y_D);
        lePidMaxOut = new QLineEdit(groupBox_17);
        lePidMaxOut->setObjectName(QStringLiteral("lePidMaxOut"));
        lePidMaxOut->setGeometry(QRect(190, 30, 50, 19));
        sizePolicy3.setHeightForWidth(lePidMaxOut->sizePolicy().hasHeightForWidth());
        lePidMaxOut->setSizePolicy(sizePolicy3);
        lePidMaxOut->setMinimumSize(QSize(50, 0));
        lePidMaxOut->setMaximumSize(QSize(50, 16777215));
        lePidMaxOut->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePidMaxOut->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblMaxOut = new QLabel(groupBox_17);
        lblMaxOut->setObjectName(QStringLiteral("lblMaxOut"));
        lblMaxOut->setGeometry(QRect(175, 11, 80, 16));
        lblMaxOut->setMinimumSize(QSize(80, 0));
        lblMaxOut->setMaximumSize(QSize(80, 16777215));
        lblMaxOut->setFont(font2);
        lblMaxOut->setAlignment(Qt::AlignCenter);
        lblMinOut = new QLabel(groupBox_17);
        lblMinOut->setObjectName(QStringLiteral("lblMinOut"));
        lblMinOut->setGeometry(QRect(175, 55, 80, 16));
        lblMinOut->setMinimumSize(QSize(80, 0));
        lblMinOut->setMaximumSize(QSize(80, 16777215));
        lblMinOut->setFont(font2);
        lblMinOut->setAlignment(Qt::AlignCenter);
        lePidMinOut = new QLineEdit(groupBox_17);
        lePidMinOut->setObjectName(QStringLiteral("lePidMinOut"));
        lePidMinOut->setGeometry(QRect(190, 74, 50, 19));
        sizePolicy3.setHeightForWidth(lePidMinOut->sizePolicy().hasHeightForWidth());
        lePidMinOut->setSizePolicy(sizePolicy3);
        lePidMinOut->setMinimumSize(QSize(50, 0));
        lePidMinOut->setMaximumSize(QSize(50, 16777215));
        lePidMinOut->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePidMinOut->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblDefaultSpeed = new QLabel(groupBox_17);
        lblDefaultSpeed->setObjectName(QStringLiteral("lblDefaultSpeed"));
        lblDefaultSpeed->setGeometry(QRect(239, 11, 80, 16));
        lblDefaultSpeed->setMinimumSize(QSize(80, 0));
        lblDefaultSpeed->setMaximumSize(QSize(80, 16777215));
        lblDefaultSpeed->setFont(font2);
        lblDefaultSpeed->setAlignment(Qt::AlignCenter);
        lePtzDefSpeed = new QLineEdit(groupBox_17);
        lePtzDefSpeed->setObjectName(QStringLiteral("lePtzDefSpeed"));
        lePtzDefSpeed->setGeometry(QRect(260, 30, 50, 19));
        sizePolicy3.setHeightForWidth(lePtzDefSpeed->sizePolicy().hasHeightForWidth());
        lePtzDefSpeed->setSizePolicy(sizePolicy3);
        lePtzDefSpeed->setMinimumSize(QSize(50, 0));
        lePtzDefSpeed->setMaximumSize(QSize(50, 16777215));
        lePtzDefSpeed->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lePtzDefSpeed->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->raise();
        splitter_2->raise();
        lblCurrentPos_33->raise();
        lblCurrentPos_32->raise();
        lblCurrentPos_28->raise();
        lblCurrentPos_29->raise();
        lblCurrentPos_30->raise();
        lblCurrentPos_31->raise();
        lePidMaxOut->raise();
        lblMaxOut->raise();
        lblMinOut->raise();
        lePidMinOut->raise();
        lblDefaultSpeed->raise();
        lePtzDefSpeed->raise();
        groupBox_10 = new QGroupBox(tab_8);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(520, 200, 231, 141));
        groupBox_10->setFont(font2);
        leGd1InstallX = new QLineEdit(groupBox_10);
        leGd1InstallX->setObjectName(QStringLiteral("leGd1InstallX"));
        leGd1InstallX->setGeometry(QRect(51, 51, 51, 20));
        sizePolicy3.setHeightForWidth(leGd1InstallX->sizePolicy().hasHeightForWidth());
        leGd1InstallX->setSizePolicy(sizePolicy3);
        leGd1InstallX->setMaximumSize(QSize(100, 16777215));
        leGd1InstallX->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd1InstallY = new QLineEdit(groupBox_10);
        leGd1InstallY->setObjectName(QStringLiteral("leGd1InstallY"));
        leGd1InstallY->setGeometry(QRect(111, 51, 51, 20));
        sizePolicy3.setHeightForWidth(leGd1InstallY->sizePolicy().hasHeightForWidth());
        leGd1InstallY->setSizePolicy(sizePolicy3);
        leGd1InstallY->setMaximumSize(QSize(100, 16777215));
        leGd1InstallY->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_20 = new QLabel(groupBox_10);
        lblCurrentPos_20->setObjectName(QStringLiteral("lblCurrentPos_20"));
        lblCurrentPos_20->setGeometry(QRect(7, 53, 35, 16));
        lblCurrentPos_20->setMinimumSize(QSize(35, 0));
        lblCurrentPos_20->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_20->setFont(font2);
        lblCurrentPos_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_21 = new QLabel(groupBox_10);
        lblCurrentPos_21->setObjectName(QStringLiteral("lblCurrentPos_21"));
        lblCurrentPos_21->setGeometry(QRect(7, 80, 35, 16));
        lblCurrentPos_21->setMinimumSize(QSize(35, 0));
        lblCurrentPos_21->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_21->setFont(font2);
        lblCurrentPos_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGd2InstallY = new QLineEdit(groupBox_10);
        leGd2InstallY->setObjectName(QStringLiteral("leGd2InstallY"));
        leGd2InstallY->setGeometry(QRect(111, 78, 51, 20));
        sizePolicy3.setHeightForWidth(leGd2InstallY->sizePolicy().hasHeightForWidth());
        leGd2InstallY->setSizePolicy(sizePolicy3);
        leGd2InstallY->setMaximumSize(QSize(100, 16777215));
        leGd2InstallY->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd2InstallX = new QLineEdit(groupBox_10);
        leGd2InstallX->setObjectName(QStringLiteral("leGd2InstallX"));
        leGd2InstallX->setGeometry(QRect(51, 78, 51, 20));
        sizePolicy3.setHeightForWidth(leGd2InstallX->sizePolicy().hasHeightForWidth());
        leGd2InstallX->setSizePolicy(sizePolicy3);
        leGd2InstallX->setMaximumSize(QSize(100, 16777215));
        leGd2InstallX->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetBorderEnd = new QLineEdit(groupBox_10);
        leSetBorderEnd->setObjectName(QStringLiteral("leSetBorderEnd"));
        leSetBorderEnd->setGeometry(QRect(111, 105, 51, 20));
        sizePolicy3.setHeightForWidth(leSetBorderEnd->sizePolicy().hasHeightForWidth());
        leSetBorderEnd->setSizePolicy(sizePolicy3);
        leSetBorderEnd->setMaximumSize(QSize(100, 16777215));
        leSetBorderEnd->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetBorderBeg = new QLineEdit(groupBox_10);
        leSetBorderBeg->setObjectName(QStringLiteral("leSetBorderBeg"));
        leSetBorderBeg->setGeometry(QRect(51, 105, 51, 20));
        sizePolicy3.setHeightForWidth(leSetBorderBeg->sizePolicy().hasHeightForWidth());
        leSetBorderBeg->setSizePolicy(sizePolicy3);
        leSetBorderBeg->setMaximumSize(QSize(100, 16777215));
        leSetBorderBeg->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_19 = new QLabel(groupBox_10);
        lblCurrentPos_19->setObjectName(QStringLiteral("lblCurrentPos_19"));
        lblCurrentPos_19->setGeometry(QRect(7, 108, 35, 16));
        lblCurrentPos_19->setMinimumSize(QSize(35, 0));
        lblCurrentPos_19->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_19->setFont(font2);
        lblCurrentPos_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_24 = new QLabel(groupBox_10);
        lblCurrentPos_24->setObjectName(QStringLiteral("lblCurrentPos_24"));
        lblCurrentPos_24->setGeometry(QRect(50, 30, 30, 16));
        lblCurrentPos_24->setMinimumSize(QSize(30, 0));
        lblCurrentPos_24->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_24->setFont(font2);
        lblCurrentPos_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_25 = new QLabel(groupBox_10);
        lblCurrentPos_25->setObjectName(QStringLiteral("lblCurrentPos_25"));
        lblCurrentPos_25->setGeometry(QRect(115, 30, 30, 16));
        lblCurrentPos_25->setMinimumSize(QSize(30, 0));
        lblCurrentPos_25->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_25->setFont(font2);
        lblCurrentPos_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_26 = new QLabel(groupBox_10);
        lblCurrentPos_26->setObjectName(QStringLiteral("lblCurrentPos_26"));
        lblCurrentPos_26->setGeometry(QRect(170, 30, 30, 16));
        lblCurrentPos_26->setMinimumSize(QSize(30, 0));
        lblCurrentPos_26->setMaximumSize(QSize(30, 16777215));
        lblCurrentPos_26->setFont(font2);
        lblCurrentPos_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGd2InstallZ = new QLineEdit(groupBox_10);
        leGd2InstallZ->setObjectName(QStringLiteral("leGd2InstallZ"));
        leGd2InstallZ->setGeometry(QRect(170, 78, 51, 20));
        sizePolicy3.setHeightForWidth(leGd2InstallZ->sizePolicy().hasHeightForWidth());
        leGd2InstallZ->setSizePolicy(sizePolicy3);
        leGd2InstallZ->setMaximumSize(QSize(100, 16777215));
        leGd2InstallZ->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGd1InstallZ = new QLineEdit(groupBox_10);
        leGd1InstallZ->setObjectName(QStringLiteral("leGd1InstallZ"));
        leGd1InstallZ->setGeometry(QRect(170, 51, 51, 20));
        sizePolicy3.setHeightForWidth(leGd1InstallZ->sizePolicy().hasHeightForWidth());
        leGd1InstallZ->setSizePolicy(sizePolicy3);
        leGd1InstallZ->setMaximumSize(QSize(100, 16777215));
        leGd1InstallZ->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetListenGyroPort = new QLineEdit(tab_8);
        leSetListenGyroPort->setObjectName(QStringLiteral("leSetListenGyroPort"));
        leSetListenGyroPort->setGeometry(QRect(697, 106, 50, 22));
        sizePolicy3.setHeightForWidth(leSetListenGyroPort->sizePolicy().hasHeightForWidth());
        leSetListenGyroPort->setSizePolicy(sizePolicy3);
        leSetListenGyroPort->setMinimumSize(QSize(50, 0));
        leSetListenGyroPort->setMaximumSize(QSize(50, 16777215));
        leSetListenGyroPort->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetGyroPort = new QLineEdit(tab_8);
        leSetGyroPort->setObjectName(QStringLiteral("leSetGyroPort"));
        leSetGyroPort->setGeometry(QRect(598, 106, 50, 22));
        sizePolicy3.setHeightForWidth(leSetGyroPort->sizePolicy().hasHeightForWidth());
        leSetGyroPort->setSizePolicy(sizePolicy3);
        leSetGyroPort->setMinimumSize(QSize(50, 0));
        leSetGyroPort->setMaximumSize(QSize(50, 16777215));
        leSetGyroPort->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leRecFileDir = new QLineEdit(tab_8);
        leRecFileDir->setObjectName(QStringLiteral("leRecFileDir"));
        leRecFileDir->setGeometry(QRect(598, 163, 101, 22));
        leRecFileDir->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetMyIP = new QLineEdit(tab_8);
        leSetMyIP->setObjectName(QStringLiteral("leSetMyIP"));
        leSetMyIP->setGeometry(QRect(598, 134, 100, 22));
        sizePolicy3.setHeightForWidth(leSetMyIP->sizePolicy().hasHeightForWidth());
        leSetMyIP->setSizePolicy(sizePolicy3);
        leSetMyIP->setMaximumSize(QSize(100, 16777215));
        leSetMyIP->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_17 = new QLabel(tab_8);
        lblCurrentPos_17->setObjectName(QStringLiteral("lblCurrentPos_17"));
        lblCurrentPos_17->setGeometry(QRect(522, 134, 70, 16));
        lblCurrentPos_17->setMinimumSize(QSize(70, 0));
        lblCurrentPos_17->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_17->setFont(font2);
        lblCurrentPos_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetMyPort = new QLineEdit(tab_8);
        leSetMyPort->setObjectName(QStringLiteral("leSetMyPort"));
        leSetMyPort->setGeometry(QRect(697, 50, 50, 22));
        sizePolicy3.setHeightForWidth(leSetMyPort->sizePolicy().hasHeightForWidth());
        leSetMyPort->setSizePolicy(sizePolicy3);
        leSetMyPort->setMinimumSize(QSize(50, 0));
        leSetMyPort->setMaximumSize(QSize(50, 16777215));
        leSetMyPort->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_8 = new QLabel(tab_8);
        lblCurrentPos_8->setObjectName(QStringLiteral("lblCurrentPos_8"));
        lblCurrentPos_8->setGeometry(QRect(522, 163, 70, 16));
        lblCurrentPos_8->setMinimumSize(QSize(70, 0));
        lblCurrentPos_8->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_8->setFont(font2);
        lblCurrentPos_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_34 = new QLabel(tab_8);
        lblCurrentPos_34->setObjectName(QStringLiteral("lblCurrentPos_34"));
        lblCurrentPos_34->setGeometry(QRect(522, 106, 70, 16));
        lblCurrentPos_34->setMinimumSize(QSize(70, 0));
        lblCurrentPos_34->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_34->setFont(font2);
        lblCurrentPos_34->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetZhiKongIP = new QLineEdit(tab_8);
        leSetZhiKongIP->setObjectName(QStringLiteral("leSetZhiKongIP"));
        leSetZhiKongIP->setGeometry(QRect(598, 22, 100, 22));
        sizePolicy3.setHeightForWidth(leSetZhiKongIP->sizePolicy().hasHeightForWidth());
        leSetZhiKongIP->setSizePolicy(sizePolicy3);
        leSetZhiKongIP->setMaximumSize(QSize(100, 16777215));
        leSetZhiKongIP->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_10 = new QLabel(tab_8);
        lblCurrentPos_10->setObjectName(QStringLiteral("lblCurrentPos_10"));
        lblCurrentPos_10->setGeometry(QRect(522, 22, 70, 16));
        lblCurrentPos_10->setMinimumSize(QSize(70, 0));
        lblCurrentPos_10->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_10->setFont(font2);
        lblCurrentPos_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_27 = new QLabel(tab_8);
        lblCurrentPos_27->setObjectName(QStringLiteral("lblCurrentPos_27"));
        lblCurrentPos_27->setGeometry(QRect(522, 78, 70, 16));
        lblCurrentPos_27->setMinimumSize(QSize(70, 0));
        lblCurrentPos_27->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_27->setFont(font2);
        lblCurrentPos_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_14 = new QLabel(tab_8);
        lblCurrentPos_14->setObjectName(QStringLiteral("lblCurrentPos_14"));
        lblCurrentPos_14->setGeometry(QRect(522, 50, 70, 16));
        lblCurrentPos_14->setMinimumSize(QSize(70, 0));
        lblCurrentPos_14->setMaximumSize(QSize(70, 16777215));
        lblCurrentPos_14->setFont(font2);
        lblCurrentPos_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leSetZhiKongPort = new QLineEdit(tab_8);
        leSetZhiKongPort->setObjectName(QStringLiteral("leSetZhiKongPort"));
        leSetZhiKongPort->setGeometry(QRect(598, 50, 50, 22));
        sizePolicy3.setHeightForWidth(leSetZhiKongPort->sizePolicy().hasHeightForWidth());
        leSetZhiKongPort->setSizePolicy(sizePolicy3);
        leSetZhiKongPort->setMinimumSize(QSize(50, 0));
        leSetZhiKongPort->setMaximumSize(QSize(50, 16777215));
        leSetZhiKongPort->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leSetGyroIP = new QLineEdit(tab_8);
        leSetGyroIP->setObjectName(QStringLiteral("leSetGyroIP"));
        leSetGyroIP->setGeometry(QRect(598, 78, 100, 22));
        sizePolicy3.setHeightForWidth(leSetGyroIP->sizePolicy().hasHeightForWidth());
        leSetGyroIP->setSizePolicy(sizePolicy3);
        leSetGyroIP->setMaximumSize(QSize(100, 16777215));
        leSetGyroIP->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lblCurrentPos_35 = new QLabel(tab_8);
        lblCurrentPos_35->setObjectName(QStringLiteral("lblCurrentPos_35"));
        lblCurrentPos_35->setGeometry(QRect(653, 54, 41, 16));
        lblCurrentPos_35->setMinimumSize(QSize(30, 0));
        lblCurrentPos_35->setMaximumSize(QSize(50, 16777215));
        lblCurrentPos_35->setFont(font2);
        lblCurrentPos_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lblCurrentPos_36 = new QLabel(tab_8);
        lblCurrentPos_36->setObjectName(QStringLiteral("lblCurrentPos_36"));
        lblCurrentPos_36->setGeometry(QRect(653, 110, 41, 16));
        lblCurrentPos_36->setMinimumSize(QSize(30, 0));
        lblCurrentPos_36->setMaximumSize(QSize(50, 16777215));
        lblCurrentPos_36->setFont(font2);
        lblCurrentPos_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        tabWidConfig->addTab(tab_8, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        lbDiffY = new QLabel(tab_3);
        lbDiffY->setObjectName(QStringLiteral("lbDiffY"));
        lbDiffY->setEnabled(true);
        lbDiffY->setGeometry(QRect(120, 310, 80, 21));
        sizePolicy2.setHeightForWidth(lbDiffY->sizePolicy().hasHeightForWidth());
        lbDiffY->setSizePolicy(sizePolicy2);
        lbDiffY->setMinimumSize(QSize(80, 21));
        lbDiffY->setMaximumSize(QSize(80, 21));
        lbDiffY->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_20 = new QLabel(tab_3);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setEnabled(true);
        label_20->setGeometry(QRect(10, 312, 100, 16));
        sizePolicy3.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy3);
        label_20->setMinimumSize(QSize(100, 0));
        label_20->setMaximumSize(QSize(100, 16777215));
        label_20->setFont(font2);
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(tab_3);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setEnabled(true);
        label_19->setGeometry(QRect(10, 289, 100, 16));
        sizePolicy3.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy3);
        label_19->setMinimumSize(QSize(100, 0));
        label_19->setMaximumSize(QSize(100, 16777215));
        label_19->setFont(font2);
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbDiffX = new QLabel(tab_3);
        lbDiffX->setObjectName(QStringLiteral("lbDiffX"));
        lbDiffX->setEnabled(true);
        lbDiffX->setGeometry(QRect(120, 287, 80, 21));
        sizePolicy2.setHeightForWidth(lbDiffX->sizePolicy().hasHeightForWidth());
        lbDiffX->setSizePolicy(sizePolicy2);
        lbDiffX->setMinimumSize(QSize(80, 21));
        lbDiffX->setMaximumSize(QSize(80, 21));
        lbDiffX->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbl_SimJoystick = new QLabel(tab_3);
        lbl_SimJoystick->setObjectName(QStringLiteral("lbl_SimJoystick"));
        lbl_SimJoystick->setGeometry(QRect(20, 0, 160, 160));
        lbl_SimJoystick->setMinimumSize(QSize(160, 160));
        lbl_SimJoystick->setMaximumSize(QSize(160, 160));
        lbl_SimJoystick->setFrameShape(QFrame::NoFrame);
        lbSendUdpDataA = new QLabel(tab_3);
        lbSendUdpDataA->setObjectName(QStringLiteral("lbSendUdpDataA"));
        lbSendUdpDataA->setEnabled(true);
        lbSendUdpDataA->setGeometry(QRect(260, 288, 110, 21));
        sizePolicy2.setHeightForWidth(lbSendUdpDataA->sizePolicy().hasHeightForWidth());
        lbSendUdpDataA->setSizePolicy(sizePolicy2);
        lbSendUdpDataA->setMinimumSize(QSize(110, 21));
        lbSendUdpDataA->setMaximumSize(QSize(110, 21));
        lbSendUdpDataA->setFont(font3);
        lbSendUdpDataA->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lbldataE = new QLabel(tab_3);
        lbldataE->setObjectName(QStringLiteral("lbldataE"));
        lbldataE->setEnabled(true);
        lbldataE->setGeometry(QRect(226, 310, 30, 20));
        sizePolicy3.setHeightForWidth(lbldataE->sizePolicy().hasHeightForWidth());
        lbldataE->setSizePolicy(sizePolicy3);
        lbldataE->setMinimumSize(QSize(30, 0));
        lbldataE->setMaximumSize(QSize(30, 16777215));
        lbldataE->setFont(font2);
        lbldataE->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbldataA = new QLabel(tab_3);
        lbldataA->setObjectName(QStringLiteral("lbldataA"));
        lbldataA->setEnabled(true);
        lbldataA->setGeometry(QRect(226, 289, 30, 20));
        sizePolicy3.setHeightForWidth(lbldataA->sizePolicy().hasHeightForWidth());
        lbldataA->setSizePolicy(sizePolicy3);
        lbldataA->setMinimumSize(QSize(30, 0));
        lbldataA->setMaximumSize(QSize(30, 16777215));
        lbldataA->setFont(font2);
        lbldataA->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbSendUdpDataE = new QLabel(tab_3);
        lbSendUdpDataE->setObjectName(QStringLiteral("lbSendUdpDataE"));
        lbSendUdpDataE->setEnabled(true);
        lbSendUdpDataE->setGeometry(QRect(260, 310, 110, 21));
        sizePolicy2.setHeightForWidth(lbSendUdpDataE->sizePolicy().hasHeightForWidth());
        lbSendUdpDataE->setSizePolicy(sizePolicy2);
        lbSendUdpDataE->setMinimumSize(QSize(110, 21));
        lbSendUdpDataE->setMaximumSize(QSize(110, 21));
        lbSendUdpDataE->setFont(font3);
        lbSendUdpDataE->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        gbx_PicFlip_3 = new QGroupBox(tab_3);
        gbx_PicFlip_3->setObjectName(QStringLiteral("gbx_PicFlip_3"));
        gbx_PicFlip_3->setGeometry(QRect(220, 50, 100, 65));
        sizePolicy4.setHeightForWidth(gbx_PicFlip_3->sizePolicy().hasHeightForWidth());
        gbx_PicFlip_3->setSizePolicy(sizePolicy4);
        gbx_PicFlip_3->setMinimumSize(QSize(100, 65));
        gbx_PicFlip_3->setMaximumSize(QSize(100, 65));
        gbx_PicFlip_3->setFont(font2);
        rb_HGyroUnLock = new QRadioButton(gbx_PicFlip_3);
        rb_HGyroUnLock->setObjectName(QStringLiteral("rb_HGyroUnLock"));
        rb_HGyroUnLock->setGeometry(QRect(9, 41, 50, 21));
        sizePolicy3.setHeightForWidth(rb_HGyroUnLock->sizePolicy().hasHeightForWidth());
        rb_HGyroUnLock->setSizePolicy(sizePolicy3);
        rb_HGyroUnLock->setMinimumSize(QSize(50, 0));
        rb_HGyroUnLock->setMaximumSize(QSize(50, 16777215));
        rb_HGyroUnLock->setFont(font10);
        rb_HGyroUnLock->setChecked(true);
        rb_HGyroLock = new QRadioButton(gbx_PicFlip_3);
        rb_HGyroLock->setObjectName(QStringLiteral("rb_HGyroLock"));
        rb_HGyroLock->setEnabled(true);
        rb_HGyroLock->setGeometry(QRect(9, 20, 50, 21));
        sizePolicy3.setHeightForWidth(rb_HGyroLock->sizePolicy().hasHeightForWidth());
        rb_HGyroLock->setSizePolicy(sizePolicy3);
        rb_HGyroLock->setMinimumSize(QSize(50, 0));
        rb_HGyroLock->setMaximumSize(QSize(50, 16777215));
        rb_HGyroLock->setFont(font10);
        pbDbgSendZhiKong = new QPushButton(tab_3);
        pbDbgSendZhiKong->setObjectName(QStringLiteral("pbDbgSendZhiKong"));
        pbDbgSendZhiKong->setGeometry(QRect(510, 295, 91, 30));
        pbDbgSendZhiKong->setMinimumSize(QSize(75, 30));
        pbDbgSendZhiKong->setMaximumSize(QSize(100, 30));
        pbDbgSendZhiKong->setFont(font2);
        pbDbgSendZhiKong->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbDbgSend = new QPushButton(tab_3);
        pbDbgSend->setObjectName(QStringLiteral("pbDbgSend"));
        pbDbgSend->setGeometry(QRect(509, 210, 75, 30));
        pbDbgSend->setMinimumSize(QSize(75, 30));
        pbDbgSend->setMaximumSize(QSize(75, 30));
        pbDbgSend->setFont(font2);
        pbDbgSend->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        le_dbgSend = new QLineEdit(tab_3);
        le_dbgSend->setObjectName(QStringLiteral("le_dbgSend"));
        le_dbgSend->setGeometry(QRect(260, 215, 241, 20));
        le_dbgSend->setFont(font2);
        le_dbgSend->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        pbDbgSendBorderChk = new QPushButton(tab_3);
        pbDbgSendBorderChk->setObjectName(QStringLiteral("pbDbgSendBorderChk"));
        pbDbgSendBorderChk->setGeometry(QRect(510, 259, 91, 30));
        pbDbgSendBorderChk->setMinimumSize(QSize(75, 30));
        pbDbgSendBorderChk->setMaximumSize(QSize(100, 30));
        pbDbgSendBorderChk->setFont(font2);
        pbDbgSendBorderChk->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        leTestPan = new QLineEdit(tab_3);
        leTestPan->setObjectName(QStringLiteral("leTestPan"));
        leTestPan->setGeometry(QRect(386, 264, 51, 20));
        sizePolicy3.setHeightForWidth(leTestPan->sizePolicy().hasHeightForWidth());
        leTestPan->setSizePolicy(sizePolicy3);
        leTestPan->setMaximumSize(QSize(100, 16777215));
        leTestPan->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leTestTilt = new QLineEdit(tab_3);
        leTestTilt->setObjectName(QStringLiteral("leTestTilt"));
        leTestTilt->setGeometry(QRect(446, 264, 51, 20));
        sizePolicy3.setHeightForWidth(leTestTilt->sizePolicy().hasHeightForWidth());
        leTestTilt->setSizePolicy(sizePolicy3);
        leTestTilt->setMaximumSize(QSize(100, 16777215));
        leTestTilt->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        gbx_RoiSize = new QGroupBox(tab_3);
        gbx_RoiSize->setObjectName(QStringLiteral("gbx_RoiSize"));
        gbx_RoiSize->setGeometry(QRect(220, 120, 100, 80));
        sizePolicy4.setHeightForWidth(gbx_RoiSize->sizePolicy().hasHeightForWidth());
        gbx_RoiSize->setSizePolicy(sizePolicy4);
        gbx_RoiSize->setMinimumSize(QSize(100, 80));
        gbx_RoiSize->setMaximumSize(QSize(200, 200));
        gbx_RoiSize->setFont(font2);
        rb_trackRoi40 = new QRadioButton(gbx_RoiSize);
        rb_trackRoi40->setObjectName(QStringLiteral("rb_trackRoi40"));
        rb_trackRoi40->setGeometry(QRect(9, 16, 90, 21));
        sizePolicy3.setHeightForWidth(rb_trackRoi40->sizePolicy().hasHeightForWidth());
        rb_trackRoi40->setSizePolicy(sizePolicy3);
        rb_trackRoi40->setMinimumSize(QSize(90, 0));
        rb_trackRoi40->setMaximumSize(QSize(90, 16777215));
        rb_trackRoi40->setFont(font10);
        rb_trackRoi40->setChecked(true);
        rb_trackRoi10 = new QRadioButton(gbx_RoiSize);
        rb_trackRoi10->setObjectName(QStringLiteral("rb_trackRoi10"));
        rb_trackRoi10->setEnabled(true);
        rb_trackRoi10->setGeometry(QRect(9, 57, 90, 21));
        sizePolicy3.setHeightForWidth(rb_trackRoi10->sizePolicy().hasHeightForWidth());
        rb_trackRoi10->setSizePolicy(sizePolicy3);
        rb_trackRoi10->setMinimumSize(QSize(90, 0));
        rb_trackRoi10->setMaximumSize(QSize(90, 16777215));
        rb_trackRoi10->setFont(font10);
        rb_trackRoi20 = new QRadioButton(gbx_RoiSize);
        rb_trackRoi20->setObjectName(QStringLiteral("rb_trackRoi20"));
        rb_trackRoi20->setEnabled(true);
        rb_trackRoi20->setGeometry(QRect(9, 36, 90, 21));
        sizePolicy3.setHeightForWidth(rb_trackRoi20->sizePolicy().hasHeightForWidth());
        rb_trackRoi20->setSizePolicy(sizePolicy3);
        rb_trackRoi20->setMinimumSize(QSize(90, 0));
        rb_trackRoi20->setMaximumSize(QSize(90, 16777215));
        rb_trackRoi20->setFont(font10);
        lblTrackingFps = new QLabel(tab_3);
        lblTrackingFps->setObjectName(QStringLiteral("lblTrackingFps"));
        lblTrackingFps->setGeometry(QRect(260, 340, 70, 21));
        lblTrackingFps->setMinimumSize(QSize(70, 0));
        lblTrackingFps->setMaximumSize(QSize(70, 16777215));
        lblTrackingFps->setFont(font3);
        lblTrackingFps->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_22 = new QLabel(tab_3);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setEnabled(true);
        label_22->setGeometry(QRect(159, 342, 100, 16));
        sizePolicy3.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy3);
        label_22->setMinimumSize(QSize(100, 0));
        label_22->setMaximumSize(QSize(100, 16777215));
        label_22->setFont(font2);
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        rb_Tracking = new QRadioButton(tab_3);
        rb_Tracking->setObjectName(QStringLiteral("rb_Tracking"));
        rb_Tracking->setEnabled(true);
        rb_Tracking->setGeometry(QRect(340, 180, 90, 21));
        sizePolicy3.setHeightForWidth(rb_Tracking->sizePolicy().hasHeightForWidth());
        rb_Tracking->setSizePolicy(sizePolicy3);
        rb_Tracking->setMinimumSize(QSize(90, 0));
        rb_Tracking->setMaximumSize(QSize(90, 16777215));
        rb_Tracking->setFont(font9);
        pbDbgSendconvAE = new QPushButton(tab_3);
        pbDbgSendconvAE->setObjectName(QStringLiteral("pbDbgSendconvAE"));
        pbDbgSendconvAE->setGeometry(QRect(510, 330, 91, 30));
        pbDbgSendconvAE->setMinimumSize(QSize(75, 30));
        pbDbgSendconvAE->setMaximumSize(QSize(100, 30));
        pbDbgSendconvAE->setFont(font2);
        pbDbgSendconvAE->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        leGD_B_IP = new QLineEdit(tab_3);
        leGD_B_IP->setObjectName(QStringLiteral("leGD_B_IP"));
        leGD_B_IP->setGeometry(QRect(384, 91, 100, 22));
        sizePolicy3.setHeightForWidth(leGD_B_IP->sizePolicy().hasHeightForWidth());
        leGD_B_IP->setSizePolicy(sizePolicy3);
        leGD_B_IP->setMaximumSize(QSize(100, 16777215));
        leGD_B_IP->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGD_B_IP->setInputMethodHints(Qt::ImhNone);
        pbDisConnect = new QPushButton(tab_3);
        pbDisConnect->setObjectName(QStringLiteral("pbDisConnect"));
        pbDisConnect->setGeometry(QRect(510, 124, 70, 30));
        sizePolicy3.setHeightForWidth(pbDisConnect->sizePolicy().hasHeightForWidth());
        pbDisConnect->setSizePolicy(sizePolicy3);
        pbDisConnect->setMinimumSize(QSize(70, 30));
        pbDisConnect->setMaximumSize(QSize(70, 30));
        pbDisConnect->setFont(font2);
        pbDisConnect->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbDisConnect_B = new QPushButton(tab_3);
        pbDisConnect_B->setObjectName(QStringLiteral("pbDisConnect_B"));
        pbDisConnect_B->setGeometry(QRect(510, 160, 70, 30));
        sizePolicy3.setHeightForWidth(pbDisConnect_B->sizePolicy().hasHeightForWidth());
        pbDisConnect_B->setSizePolicy(sizePolicy3);
        pbDisConnect_B->setMinimumSize(QSize(70, 30));
        pbDisConnect_B->setMaximumSize(QSize(70, 30));
        pbDisConnect_B->setFont(font2);
        pbDisConnect_B->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        label_21 = new QLabel(tab_3);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(321, 93, 61, 20));
        label_21->setMaximumSize(QSize(100, 16777215));
        label_21->setFont(font9);
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(tab_3);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(320, 59, 61, 20));
        label_18->setMaximumSize(QSize(100, 16777215));
        label_18->setFont(font9);
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        leGD_A_IP = new QLineEdit(tab_3);
        leGD_A_IP->setObjectName(QStringLiteral("leGD_A_IP"));
        leGD_A_IP->setGeometry(QRect(384, 56, 100, 22));
        sizePolicy3.setHeightForWidth(leGD_A_IP->sizePolicy().hasHeightForWidth());
        leGD_A_IP->setSizePolicy(sizePolicy3);
        leGD_A_IP->setMaximumSize(QSize(100, 16777215));
        leGD_A_IP->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        leGD_A_IP->setInputMethodHints(Qt::ImhNone);
        pbConnect = new QPushButton(tab_3);
        pbConnect->setObjectName(QStringLiteral("pbConnect"));
        pbConnect->setGeometry(QRect(510, 54, 70, 30));
        sizePolicy3.setHeightForWidth(pbConnect->sizePolicy().hasHeightForWidth());
        pbConnect->setSizePolicy(sizePolicy3);
        pbConnect->setMinimumSize(QSize(70, 30));
        pbConnect->setMaximumSize(QSize(70, 30));
        pbConnect->setFont(font2);
        pbConnect->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        pbConnect_B = new QPushButton(tab_3);
        pbConnect_B->setObjectName(QStringLiteral("pbConnect_B"));
        pbConnect_B->setGeometry(QRect(510, 90, 70, 30));
        sizePolicy3.setHeightForWidth(pbConnect_B->sizePolicy().hasHeightForWidth());
        pbConnect_B->setSizePolicy(sizePolicy3);
        pbConnect_B->setMinimumSize(QSize(70, 30));
        pbConnect_B->setMaximumSize(QSize(70, 30));
        pbConnect_B->setFont(font2);
        pbConnect_B->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        lbl_Version = new QLabel(tab_3);
        lbl_Version->setObjectName(QStringLiteral("lbl_Version"));
        lbl_Version->setGeometry(QRect(360, 380, 211, 16));
        lbl_Version->setFont(font2);
        lbl_Version->setFrameShape(QFrame::NoFrame);
        lbl_Version->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbSysClose = new QPushButton(tab_3);
        pbSysClose->setObjectName(QStringLiteral("pbSysClose"));
        pbSysClose->setGeometry(QRect(510, 20, 70, 30));
        sizePolicy3.setHeightForWidth(pbSysClose->sizePolicy().hasHeightForWidth());
        pbSysClose->setSizePolicy(sizePolicy3);
        pbSysClose->setMinimumSize(QSize(70, 30));
        pbSysClose->setMaximumSize(QSize(70, 30));
        pbSysClose->setFont(font2);
        pbSysClose->setStyleSheet(QLatin1String("QPushButton{background-color: rgb(225, 225, 225);border:3px groove gray;border-radius:6px;padding:1px 4px;border-style: outset;}\n"
"QPushButton:hover{background-color:rgb(200, 200, 200); color: black;}\n"
"QPushButton:pressed{background-color:rgb(150, 150, 150);border-style: inset;}"));
        cbxSetDbgDistance = new QCheckBox(tab_3);
        cbxSetDbgDistance->setObjectName(QStringLiteral("cbxSetDbgDistance"));
        cbxSetDbgDistance->setGeometry(QRect(120, 253, 121, 16));
        cbxSetDbgDistance->setFont(font1);
        cbxSetDbgDistance->setChecked(false);
        leSetDbgDistance = new QLineEdit(tab_3);
        leSetDbgDistance->setObjectName(QStringLiteral("leSetDbgDistance"));
        leSetDbgDistance->setGeometry(QRect(50, 250, 51, 20));
        sizePolicy3.setHeightForWidth(leSetDbgDistance->sizePolicy().hasHeightForWidth());
        leSetDbgDistance->setSizePolicy(sizePolicy3);
        leSetDbgDistance->setMaximumSize(QSize(100, 16777215));
        leSetDbgDistance->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setEnabled(true);
        label_6->setGeometry(QRect(40, 373, 16, 21));
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);
        lbMousePosition_x = new QLabel(tab_3);
        lbMousePosition_x->setObjectName(QStringLiteral("lbMousePosition_x"));
        lbMousePosition_x->setEnabled(true);
        lbMousePosition_x->setGeometry(QRect(60, 350, 30, 23));
        sizePolicy2.setHeightForWidth(lbMousePosition_x->sizePolicy().hasHeightForWidth());
        lbMousePosition_x->setSizePolicy(sizePolicy2);
        lbMousePosition_x->setMinimumSize(QSize(30, 0));
        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setEnabled(true);
        label_5->setGeometry(QRect(40, 356, 6, 12));
        sizePolicy3.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy3);
        lbMousePosition_y = new QLabel(tab_3);
        lbMousePosition_y->setObjectName(QStringLiteral("lbMousePosition_y"));
        lbMousePosition_y->setEnabled(true);
        lbMousePosition_y->setGeometry(QRect(60, 370, 30, 21));
        sizePolicy2.setHeightForWidth(lbMousePosition_y->sizePolicy().hasHeightForWidth());
        lbMousePosition_y->setSizePolicy(sizePolicy2);
        lbMousePosition_y->setMinimumSize(QSize(30, 0));
        label_31 = new QLabel(tab_3);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setEnabled(true);
        label_31->setGeometry(QRect(190, 382, 100, 16));
        sizePolicy3.setHeightForWidth(label_31->sizePolicy().hasHeightForWidth());
        label_31->setSizePolicy(sizePolicy3);
        label_31->setMinimumSize(QSize(100, 0));
        label_31->setMaximumSize(QSize(100, 16777215));
        label_31->setFont(font2);
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lbGyroPitch_A = new QLabel(tab_3);
        lbGyroPitch_A->setObjectName(QStringLiteral("lbGyroPitch_A"));
        lbGyroPitch_A->setEnabled(true);
        lbGyroPitch_A->setGeometry(QRect(300, 380, 80, 21));
        sizePolicy2.setHeightForWidth(lbGyroPitch_A->sizePolicy().hasHeightForWidth());
        lbGyroPitch_A->setSizePolicy(sizePolicy2);
        lbGyroPitch_A->setMinimumSize(QSize(80, 21));
        lbGyroPitch_A->setMaximumSize(QSize(80, 21));
        lbGyroPitch_A->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lerawdira = new QLabel(tab_3);
        lerawdira->setObjectName(QStringLiteral("lerawdira"));
        lerawdira->setEnabled(true);
        lerawdira->setGeometry(QRect(690, 210, 80, 21));
        sizePolicy2.setHeightForWidth(lerawdira->sizePolicy().hasHeightForWidth());
        lerawdira->setSizePolicy(sizePolicy2);
        lerawdira->setMinimumSize(QSize(80, 21));
        lerawdira->setMaximumSize(QSize(80, 21));
        lerawdira->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        lerawdirb = new QLabel(tab_3);
        lerawdirb->setObjectName(QStringLiteral("lerawdirb"));
        lerawdirb->setEnabled(true);
        lerawdirb->setGeometry(QRect(690, 240, 80, 21));
        sizePolicy2.setHeightForWidth(lerawdirb->sizePolicy().hasHeightForWidth());
        lerawdirb->setSizePolicy(sizePolicy2);
        lerawdirb->setMinimumSize(QSize(80, 21));
        lerawdirb->setMaximumSize(QSize(80, 21));
        lerawdirb->setStyleSheet(QLatin1String("background-color: rgb(0, 0, 0);\n"
"color: rgb(0, 255, 0);\n"
"border:2px gray;padding:1px 4px;border-style: inset;"));
        tabWidConfig->addTab(tab_3, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1380, 26));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(menubar->sizePolicy().hasHeightForWidth());
        menubar->setSizePolicy(sizePolicy6);
        menubar->setNativeMenuBar(true);
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidConfig->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GD280S\345\256\242\346\210\267\347\253\257 V2.00", Q_NULLPTR));
        lblPad_B->setText(QString());
        lblPad->setText(QString());
        groupBox_11->setTitle(QApplication::translate("MainWindow", "\347\275\221\347\273\234\350\256\241\346\225\260", Q_NULLPTR));
        lbSendZhiKongPkt->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_28->setText(QApplication::translate("MainWindow", "\345\217\221\346\214\207\346\216\247", Q_NULLPTR));
        lbSendServoPkt->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_29->setText(QApplication::translate("MainWindow", "\345\217\221\344\274\272\346\234\215", Q_NULLPTR));
        lbRecvGyroPkt->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "\346\224\266\346\203\257\345\257\274", Q_NULLPTR));
        pbClearDbgCnt->setText(QApplication::translate("MainWindow", "\346\270\205\351\233\266", Q_NULLPTR));
        label_24->setText(QApplication::translate("MainWindow", "\346\224\266\350\247\206\351\242\221\345\270\247GD A", Q_NULLPTR));
        lbRecvPkt_A->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_23->setText(QApplication::translate("MainWindow", "\346\224\266GD A", Q_NULLPTR));
        lbRecvFrameA->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_26->setText(QApplication::translate("MainWindow", "\346\224\266GD B", Q_NULLPTR));
        lbRecvPkt_B->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_25->setText(QApplication::translate("MainWindow", "\346\224\266\350\247\206\351\242\221\345\270\247GD B", Q_NULLPTR));
        lbRecvZhiKongPkt->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbRecvFrameB->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_27->setText(QApplication::translate("MainWindow", "\346\224\266\346\214\207\346\216\247", Q_NULLPTR));
        pClearbDbgOut->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        groupBox_13->setTitle(QApplication::translate("MainWindow", "\345\217\221\344\274\272\346\234\215\346\225\260\346\215\256", Q_NULLPTR));
        lbldataA_2->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265A\346\226\271\344\275\215", Q_NULLPTR));
        lbGDASendServoA->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE_2->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265A\344\277\257\344\273\260", Q_NULLPTR));
        lbGDASendServoE->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbGDBSendServoA->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE_3->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265B\344\277\257\344\273\260", Q_NULLPTR));
        lbGDBSendServoE->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataA_3->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265B\346\226\271\344\275\215", Q_NULLPTR));
        gbx_PicFlip_5->setTitle(QApplication::translate("MainWindow", "\346\224\266\346\203\257\345\257\274\346\225\260\346\215\256", Q_NULLPTR));
        lbRecvGyroWeiDu->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE_4->setText(QApplication::translate("MainWindow", "\347\273\217\345\272\246", Q_NULLPTR));
        lbRecvGyroJingdu->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataA_4->setText(QApplication::translate("MainWindow", "\347\272\254\345\272\246", Q_NULLPTR));
        lbRecvGyroGpsDirect->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataA_5->setText(QApplication::translate("MainWindow", "\351\253\230\345\272\246", Q_NULLPTR));
        lbRecvGyroGaodu->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE_5->setText(QApplication::translate("MainWindow", "GPS\346\226\271\345\220\221", Q_NULLPTR));
        lbRecvGyroPitch->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataA_6->setText(QApplication::translate("MainWindow", "\350\275\246\344\275\223\345\200\276\346\226\234", Q_NULLPTR));
        lbRecvGyroRoll->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbRecvGyroYaw->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE_6->setText(QApplication::translate("MainWindow", "\350\275\246\344\275\223\344\277\257\344\273\260", Q_NULLPTR));
        lbldataA_7->setText(QApplication::translate("MainWindow", "IMU\346\226\271\345\220\221", Q_NULLPTR));
        cbxIgnoreGyroData->setText(QApplication::translate("MainWindow", "\344\270\215\344\275\277\347\224\250\346\203\257\345\257\274\346\225\260\346\215\256", Q_NULLPTR));
        groupBox_18->setTitle(QApplication::translate("MainWindow", "\346\224\266\346\214\207\346\216\247\345\274\225\345\257\274", Q_NULLPTR));
        lbldataA_8->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276\346\226\271\344\275\215", Q_NULLPTR));
        lbRecvZhiKongFangwei->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbRecvRadarFuyang->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276\344\277\257\344\273\260", Q_NULLPTR));
        lbRecvZhiKongFuyang->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbRecvZhiKongDist->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbRecvRadarDist->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276\350\267\235\347\246\273", Q_NULLPTR));
        gbx_PicFlip_4->setTitle(QApplication::translate("MainWindow", "\346\216\247\345\210\266\351\200\211\346\213\251", Q_NULLPTR));
        rb_selGDB->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\220\216", Q_NULLPTR));
        rb_selGDA->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\211\215", Q_NULLPTR));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "\350\257\206\345\210\253", Q_NULLPTR));
        rb_SelDetectMtd1->setText(QApplication::translate("MainWindow", "\347\256\227\346\263\2251", Q_NULLPTR));
        cbxStartTracking->setText(QApplication::translate("MainWindow", "\351\224\201\345\256\232\347\233\256\346\240\207", Q_NULLPTR));
        rb_SelDetectNone->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", Q_NULLPTR));
        rb_SelDetectMtd2->setText(QApplication::translate("MainWindow", "\347\256\227\346\263\2252", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\346\250\241\345\274\217", Q_NULLPTR));
        rb_trackStick->setText(QApplication::translate("MainWindow", "\346\211\213\345\212\250", Q_NULLPTR));
        rb_Center->setText(QApplication::translate("MainWindow", "\346\214\207\346\216\247\345\274\225\345\257\274", Q_NULLPTR));
        cbxLianDongFlag->setText(QApplication::translate("MainWindow", "\350\201\224\345\212\250", Q_NULLPTR));
        cbxIgnorCenterCmd->setText(QApplication::translate("MainWindow", "\350\266\205\350\266\212\346\214\207\346\216\247", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "\345\217\230\345\200\215", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\351\200\237\345\272\246", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\211\215\344\274\272\346\234\215\346\216\247\345\210\266", Q_NULLPTR));
        pbSetMotor->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        cbxPanMotor->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\346\226\271\344\275\215\347\224\265\346\234\272", Q_NULLPTR));
        cbxTiltMotor->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\344\277\257\344\273\260\347\224\265\346\234\272", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "\345\220\216\344\274\272\346\234\215\346\216\247\345\210\266", Q_NULLPTR));
        pbSetMotor_B->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        cbxPanMotor_B->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\346\226\271\344\275\215\347\224\265\346\234\272", Q_NULLPTR));
        cbxTiltMotor_B->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\344\277\257\344\273\260\347\224\265\346\234\272", Q_NULLPTR));
        pbRec->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\345\275\225\345\203\217", Q_NULLPTR));
        pbToZero->setText(QApplication::translate("MainWindow", "\345\275\222\351\233\266", Q_NULLPTR));
        pbAutoFoc->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\257\271\347\204\246", Q_NULLPTR));
        pbFocusN->setText(QApplication::translate("MainWindow", "\345\257\271\347\204\246+", Q_NULLPTR));
        pbZoomOut->setText(QApplication::translate("MainWindow", "\345\217\230\347\204\246+", Q_NULLPTR));
        pbFocusF->setText(QApplication::translate("MainWindow", "\345\257\271\347\204\246-", Q_NULLPTR));
        pbZoomIn->setText(QApplication::translate("MainWindow", "\345\217\230\347\204\246-", Q_NULLPTR));
        pbUp->setText(QApplication::translate("MainWindow", "\344\270\212", Q_NULLPTR));
        pbDown->setText(QApplication::translate("MainWindow", "\344\270\213", Q_NULLPTR));
        pbLeft->setText(QApplication::translate("MainWindow", "\345\267\246", Q_NULLPTR));
        pbRight->setText(QApplication::translate("MainWindow", "\345\217\263", Q_NULLPTR));
        pbStop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", Q_NULLPTR));
        groupBox_19->setTitle(QApplication::translate("MainWindow", "\345\211\215\345\205\211\347\224\265", Q_NULLPTR));
        lbl_lensTitle_6->setText(QApplication::translate("MainWindow", "\347\204\246\350\267\235", Q_NULLPTR));
        lbl_lenVal->setText(QApplication::translate("MainWindow", "15", Q_NULLPTR));
        lblBeishu->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle->setText(QApplication::translate("MainWindow", "\350\247\206\345\234\272", Q_NULLPTR));
        lbl_lensTitle_5->setText(QApplication::translate("MainWindow", "\345\200\215\346\225\260", Q_NULLPTR));
        lblLens->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lblDirection->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lblPitch->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_7->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215", Q_NULLPTR));
        lbl_lensTitle_8->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260", Q_NULLPTR));
        lbl_focalVal->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_11->setText(QApplication::translate("MainWindow", "\345\257\271\347\204\246", Q_NULLPTR));
        groupBox_20->setTitle(QApplication::translate("MainWindow", "\345\220\216\345\205\211\347\224\265", Q_NULLPTR));
        lblBeishu_B->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_2->setText(QApplication::translate("MainWindow", "\350\247\206\345\234\272", Q_NULLPTR));
        lbl_lenVal_B->setText(QApplication::translate("MainWindow", "15", Q_NULLPTR));
        lblLens_B->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_3->setText(QApplication::translate("MainWindow", "\345\200\215\346\225\260", Q_NULLPTR));
        lbl_lensTitle_4->setText(QApplication::translate("MainWindow", "\347\204\246\350\267\235", Q_NULLPTR));
        lblDirection_B->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lblPitch_B->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_9->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215", Q_NULLPTR));
        lbl_lensTitle_10->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260", Q_NULLPTR));
        lbl_focalVal_B->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        lbl_lensTitle_12->setText(QApplication::translate("MainWindow", "\345\257\271\347\204\246", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab_7), QApplication::translate("MainWindow", "\345\270\270\347\224\250", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\347\233\270\346\234\272", Q_NULLPTR));
        rb_ExpoIris->setText(QApplication::translate("MainWindow", "\345\205\211\345\234\210\344\274\230\345\205\210", Q_NULLPTR));
        rb_ExpoManual->setText(QApplication::translate("MainWindow", "\346\211\213\345\212\250", Q_NULLPTR));
        rb_ExpoAuto->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250", Q_NULLPTR));
        rb_ExpoShutter->setText(QApplication::translate("MainWindow", "\345\277\253\351\227\250\344\274\230\345\205\210", Q_NULLPTR));
        gbx_WB->setTitle(QApplication::translate("MainWindow", "\347\231\275\345\271\263\350\241\241", Q_NULLPTR));
        cbx_WbMode->clear();
        cbx_WbMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\346\210\267\345\244\226", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\256\244\345\206\205", Q_NULLPTR)
         << QApplication::translate("MainWindow", "ATW", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\211\213\345\212\250", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\210\267\345\244\226\350\207\252\345\212\250", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\350\267\257\347\201\257\350\207\252\345\212\250", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\350\267\257\347\201\257", Q_NULLPTR)
        );
        gbx_Sharpen->setTitle(QApplication::translate("MainWindow", "\351\224\220\345\214\226", Q_NULLPTR));
        pbSharpenReset->setText(QApplication::translate("MainWindow", "\345\244\215\344\275\215", Q_NULLPTR));
        pbSharpenDown->setText(QApplication::translate("MainWindow", "\351\224\220\345\214\226-", Q_NULLPTR));
        pbSharpenUp->setText(QApplication::translate("MainWindow", "\351\224\220\345\214\226+", Q_NULLPTR));
        gbx_Gamma->setTitle(QApplication::translate("MainWindow", "\344\274\275\351\251\254", Q_NULLPTR));
        gbx_Expo->setTitle(QApplication::translate("MainWindow", "\346\233\235\345\205\211", Q_NULLPTR));
        pbExpDown->setText(QApplication::translate("MainWindow", "\346\233\235\345\205\211-", Q_NULLPTR));
        pbExpUp->setText(QApplication::translate("MainWindow", "\346\233\235\345\205\211+", Q_NULLPTR));
        pbExpReset->setText(QApplication::translate("MainWindow", "\345\244\215\344\275\215", Q_NULLPTR));
        gbx_backLight->setTitle(QApplication::translate("MainWindow", "\345\274\272\345\205\211\346\212\221\345\210\266", Q_NULLPTR));
        pbBackLightSend->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        cbx_backLightMode->clear();
        cbx_backLightMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\205\263\351\227\255", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\350\203\214\345\205\211\350\241\245\345\201\277", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\274\272\345\205\211\346\212\221\345\210\266", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\256\275\345\212\250\346\200\201", Q_NULLPTR)
        );
        gbx_Expo_2->setTitle(QApplication::translate("MainWindow", "\345\205\211\345\234\210", Q_NULLPTR));
        pbApReset->setText(QApplication::translate("MainWindow", "\345\244\215\344\275\215", Q_NULLPTR));
        pbApDown->setText(QApplication::translate("MainWindow", "\345\205\211\345\234\210-", Q_NULLPTR));
        pbApUp->setText(QApplication::translate("MainWindow", "\345\205\211\345\234\210+", Q_NULLPTR));
        gbx_Shutter->setTitle(QApplication::translate("MainWindow", "\345\277\253\351\227\250", Q_NULLPTR));
        pbShutterReset->setText(QApplication::translate("MainWindow", "\345\244\215\344\275\215", Q_NULLPTR));
        pbApShutterDown->setText(QApplication::translate("MainWindow", "\345\277\253\351\227\250-", Q_NULLPTR));
        pbShutterUp->setText(QApplication::translate("MainWindow", "\345\277\253\351\227\250+", Q_NULLPTR));
        gbx_PicFlip->setTitle(QApplication::translate("MainWindow", "\347\277\273\350\275\254\345\233\276\345\203\217", Q_NULLPTR));
        rb_flipPicOff->setText(QApplication::translate("MainWindow", "Off", Q_NULLPTR));
        rb_flipPicOn->setText(QApplication::translate("MainWindow", "On", Q_NULLPTR));
        gbx_PicFlip_2->setTitle(QApplication::translate("MainWindow", "\351\200\217\351\233\276", Q_NULLPTR));
        rb_DefogOff->setText(QApplication::translate("MainWindow", "Off", Q_NULLPTR));
        rb_DefogOn->setText(QApplication::translate("MainWindow", "On", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab_4), QApplication::translate("MainWindow", "\347\233\270\346\234\272", Q_NULLPTR));
        pbOsdWordSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
        gbxPalSelect_8->setTitle(QApplication::translate("MainWindow", "\345\255\227\344\275\223", Q_NULLPTR));
        cbxOSDFontSize->clear();
        cbxOSDFontSize->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "32", Q_NULLPTR)
         << QApplication::translate("MainWindow", "24", Q_NULLPTR)
         << QApplication::translate("MainWindow", "23", Q_NULLPTR)
         << QApplication::translate("MainWindow", "22", Q_NULLPTR)
         << QApplication::translate("MainWindow", "30", Q_NULLPTR)
         << QApplication::translate("MainWindow", "29", Q_NULLPTR)
         << QApplication::translate("MainWindow", "28", Q_NULLPTR)
         << QApplication::translate("MainWindow", "27", Q_NULLPTR)
         << QApplication::translate("MainWindow", "26", Q_NULLPTR)
         << QApplication::translate("MainWindow", "25", Q_NULLPTR)
         << QApplication::translate("MainWindow", "31", Q_NULLPTR)
         << QApplication::translate("MainWindow", "34", Q_NULLPTR)
         << QApplication::translate("MainWindow", "36", Q_NULLPTR)
         << QApplication::translate("MainWindow", "38", Q_NULLPTR)
         << QApplication::translate("MainWindow", "40", Q_NULLPTR)
         << QApplication::translate("MainWindow", "41", Q_NULLPTR)
         << QApplication::translate("MainWindow", "42", Q_NULLPTR)
        );
        cbxOSDFont->clear();
        cbxOSDFont->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\351\273\221\344\275\223", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\215\216\346\226\207\344\273\277\345\256\213", Q_NULLPTR)
         << QApplication::translate("MainWindow", "MS Shell Dlg 2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\345\256\213\344\275\223", Q_NULLPTR)
        );
        lblCurrentPos_22->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223", Q_NULLPTR));
        lblCurrentPos_23->setText(QApplication::translate("MainWindow", "\345\255\227\351\253\230", Q_NULLPTR));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "OSD", Q_NULLPTR));
        cbxCrosser->setText(QApplication::translate("MainWindow", "\345\215\201\345\255\227", Q_NULLPTR));
        cbxFullScreen->setText(QApplication::translate("MainWindow", "\350\276\223\345\207\272\345\205\250\345\261\217", Q_NULLPTR));
        cbxRoiBox->setText(QApplication::translate("MainWindow", "\350\267\237\350\270\252\346\241\206", Q_NULLPTR));
        cbxMarkline->setText(QApplication::translate("MainWindow", "\346\240\207\345\260\272\347\272\277", Q_NULLPTR));
        cbxAzimuthScale->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215\345\210\273\345\272\246", Q_NULLPTR));
        gbx_RoiSize_2->setTitle(QApplication::translate("MainWindow", "\351\242\234\350\211\262", Q_NULLPTR));
        rb_OSDColorGreen->setText(QApplication::translate("MainWindow", "\347\273\277", Q_NULLPTR));
        rb_0SDColorBlk->setText(QApplication::translate("MainWindow", "\351\273\221", Q_NULLPTR));
        rb_OSDColorRed->setText(QApplication::translate("MainWindow", "\347\272\242", Q_NULLPTR));
        rb_0SDColorWhite->setText(QApplication::translate("MainWindow", "\347\231\275", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab), QApplication::translate("MainWindow", "OSD\350\256\276\347\275\256", Q_NULLPTR));
        pbSetDirection->setText(QApplication::translate("MainWindow", "\345\260\206\345\275\223\345\211\215\346\226\271\344\275\215\347\275\256\344\270\2720\302\260", Q_NULLPTR));
        pbSetPitch->setText(QApplication::translate("MainWindow", "\345\260\206\345\275\223\345\211\215\344\277\257\344\273\260\347\275\256\344\270\2720\302\260", Q_NULLPTR));
        groupBox_14->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265A", Q_NULLPTR));
        lblCurrentPos_47->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215", Q_NULLPTR));
        lblCurrentPos_48->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260", Q_NULLPTR));
        lblCurrentPos_49->setText(QApplication::translate("MainWindow", "\350\267\235\347\246\273", Q_NULLPTR));
        pbSetGD1Direction->setText(QApplication::translate("MainWindow", "\347\275\256\344\270\272\345\275\223\345\211\215\346\226\271\344\275\215", Q_NULLPTR));
        lblCurrentPos_59->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_60->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_61->setText(QApplication::translate("MainWindow", "m", Q_NULLPTR));
        lblCurrentPos_68->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_69->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_56->setText(QApplication::translate("MainWindow", "\345\244\251\347\272\277\344\277\257\344\273\260", Q_NULLPTR));
        lblCurrentPos_57->setText(QApplication::translate("MainWindow", "\345\244\251\347\272\277\346\226\271\344\275\215", Q_NULLPTR));
        groupBox_15->setTitle(QApplication::translate("MainWindow", "\345\235\220\346\240\207\345\217\202\346\225\260", Q_NULLPTR));
        lblCurrentPos_50->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265A", Q_NULLPTR));
        lblCurrentPos_51->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265B", Q_NULLPTR));
        lblCurrentPos_52->setText(QApplication::translate("MainWindow", "\346\240\207\346\235\206", Q_NULLPTR));
        lblCurrentPos_53->setText(QApplication::translate("MainWindow", "\347\273\217\345\272\246L", Q_NULLPTR));
        lblCurrentPos_54->setText(QApplication::translate("MainWindow", "\347\272\254\345\272\246B", Q_NULLPTR));
        lblCurrentPos_55->setText(QApplication::translate("MainWindow", "\351\253\230H", Q_NULLPTR));
        pbcalcBlh2Rae->setText(QApplication::translate("MainWindow", "\350\256\241\347\256\227", Q_NULLPTR));
        groupBox_16->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265B", Q_NULLPTR));
        lblCurrentPos_62->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215", Q_NULLPTR));
        lblCurrentPos_63->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260", Q_NULLPTR));
        lblCurrentPos_64->setText(QApplication::translate("MainWindow", "\350\267\235\347\246\273", Q_NULLPTR));
        pbSetGD2Direction->setText(QApplication::translate("MainWindow", "\347\275\256\344\270\272\345\275\223\345\211\215\346\226\271\344\275\215", Q_NULLPTR));
        lblCurrentPos_65->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_66->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_67->setText(QApplication::translate("MainWindow", "m", Q_NULLPTR));
        lblCurrentPos_70->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_71->setText(QApplication::translate("MainWindow", "\302\260", Q_NULLPTR));
        lblCurrentPos_58->setText(QApplication::translate("MainWindow", "\345\244\251\347\272\277\346\226\271\344\275\215", Q_NULLPTR));
        lblCurrentPos_72->setText(QApplication::translate("MainWindow", "\345\244\251\347\272\277\344\277\257\344\273\260", Q_NULLPTR));
        pbcalcRaeClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "\346\226\271\344\275\215", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        le_setDirecion->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        le_setDirecion->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        le_setPitch->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        le_setPitch->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        pbSendDirection->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\211\215-\346\226\271\344\275\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "0\302\260", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "180\302\260", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "90\302\260", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "270\302\260", Q_NULLPTR));
        lblPadDirection->setText(QString());
        groupBox_8->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\220\216-\346\226\271\344\275\215", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "0\302\260", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "180\302\260", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "90\302\260", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "270\302\260", Q_NULLPTR));
        lblPadDirection_B->setText(QString());
        groupBox_21->setTitle(QApplication::translate("MainWindow", "\346\274\202\347\247\273\350\260\203\346\225\264", Q_NULLPTR));
        pbAdjDriftDown->setText(QApplication::translate("MainWindow", "\344\270\213", Q_NULLPTR));
        pbAdjDriftUp->setText(QApplication::translate("MainWindow", "\344\270\212", Q_NULLPTR));
        pbAAdjDriftSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
        pbAdjDriftLeft->setText(QApplication::translate("MainWindow", "\345\267\246", Q_NULLPTR));
        pbAdjDriftRight->setText(QApplication::translate("MainWindow", "\345\217\263", Q_NULLPTR));
        pbAdjDriftStop->setText(QApplication::translate("MainWindow", "\345\201\234", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab_2), QApplication::translate("MainWindow", "\345\257\271\346\240\207\350\260\203\351\233\266", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265A", Q_NULLPTR));
        lblCurrentPos_15->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272IP", Q_NULLPTR));
        pb_OpenSelfTest->setText(QApplication::translate("MainWindow", "\344\275\277\350\203\275\344\270\212\347\224\265\350\207\252\346\243\200", Q_NULLPTR));
        lblCurrentPos_12->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272Port", Q_NULLPTR));
        lblCurrentPos_9->setText(QApplication::translate("MainWindow", "\350\275\254\345\217\260A IP", Q_NULLPTR));
        pb_CloseSelfTest->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210\344\270\212\347\224\265\350\207\252\346\243\200", Q_NULLPTR));
        lbl_DiffY_default->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260\350\241\245\345\201\277A", Q_NULLPTR));
        lbl_DiffX_default->setText(QApplication::translate("MainWindow", "\346\260\264\345\271\263\351\233\266\344\275\215A", Q_NULLPTR));
        lbl_DiffX_default_3->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276X\350\241\245\345\201\277A", Q_NULLPTR));
        lbl_DiffX_default_5->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276Y\350\241\245\345\201\277A", Q_NULLPTR));
        lblCurrentPos_18->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272A\345\257\271\347\204\246", Q_NULLPTR));
        leHdaDefFoc->setText(QApplication::translate("MainWindow", "4400", Q_NULLPTR));
        cbxUseDefFocA->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\351\273\230\350\256\244\345\257\271\347\204\246\347\202\271", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\345\205\211\347\224\265B", Q_NULLPTR));
        lblCurrentPos_16->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272IP", Q_NULLPTR));
        pb_OpenSelfTest_B->setText(QApplication::translate("MainWindow", "\344\275\277\350\203\275\344\270\212\347\224\265\350\207\252\346\243\200", Q_NULLPTR));
        lblCurrentPos_13->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272Port", Q_NULLPTR));
        lblCurrentPos_11->setText(QApplication::translate("MainWindow", "\350\275\254\345\217\260B IP", Q_NULLPTR));
        pb_CloseSelfTest_B->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210\344\270\212\347\224\265\350\207\252\346\243\200", Q_NULLPTR));
        lbl_DiffY_default_2->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260\350\241\245\345\201\277B", Q_NULLPTR));
        lbl_DiffX_default_2->setText(QApplication::translate("MainWindow", "\346\260\264\345\271\263\351\233\266\344\275\215B", Q_NULLPTR));
        lbl_DiffX_default_4->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276X\350\241\245\345\201\277B", Q_NULLPTR));
        lbl_DiffX_default_6->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276Y\350\241\245\345\201\277B", Q_NULLPTR));
        leHdbDefFoc_2->setText(QApplication::translate("MainWindow", "4400", Q_NULLPTR));
        lblCurrentPos_37->setText(QApplication::translate("MainWindow", "\347\233\270\346\234\272B\345\257\271\347\204\246", Q_NULLPTR));
        cbxUseDefFocB->setText(QApplication::translate("MainWindow", "\344\275\277\347\224\250\351\273\230\350\256\244\345\257\271\347\204\246\347\202\271", Q_NULLPTR));
        pbConfigSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
        cbxEditLock_sys->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221", Q_NULLPTR));
        groupBox_17->setTitle(QApplication::translate("MainWindow", "PID", Q_NULLPTR));
        lblCurrentPos_28->setText(QApplication::translate("MainWindow", "Scale", Q_NULLPTR));
        lblCurrentPos_29->setText(QApplication::translate("MainWindow", "P", Q_NULLPTR));
        lblCurrentPos_30->setText(QApplication::translate("MainWindow", "I", Q_NULLPTR));
        lblCurrentPos_31->setText(QApplication::translate("MainWindow", "D", Q_NULLPTR));
        lblCurrentPos_32->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        lblCurrentPos_33->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        lblMaxOut->setText(QApplication::translate("MainWindow", "\346\234\200\345\244\247\351\200\237\345\272\246", Q_NULLPTR));
        lblMinOut->setText(QApplication::translate("MainWindow", "\346\234\200\345\260\217\351\200\237\345\272\246", Q_NULLPTR));
        lblDefaultSpeed->setText(QApplication::translate("MainWindow", "\351\273\230\350\256\244\351\200\237\345\272\246", Q_NULLPTR));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "\345\256\211\350\243\205\344\275\215\347\275\256", Q_NULLPTR));
        lblCurrentPos_20->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265A", Q_NULLPTR));
        lblCurrentPos_21->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265B", Q_NULLPTR));
        lblCurrentPos_19->setText(QApplication::translate("MainWindow", "\345\210\206\347\225\214", Q_NULLPTR));
        lblCurrentPos_24->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        lblCurrentPos_25->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        lblCurrentPos_26->setText(QApplication::translate("MainWindow", "Z", Q_NULLPTR));
        lblCurrentPos_17->setText(QApplication::translate("MainWindow", "\346\234\254\346\234\272IP", Q_NULLPTR));
        lblCurrentPos_8->setText(QApplication::translate("MainWindow", "\345\275\225\345\203\217\344\277\235\345\255\230", Q_NULLPTR));
        lblCurrentPos_34->setText(QApplication::translate("MainWindow", "\346\203\257\345\257\274Port", Q_NULLPTR));
        lblCurrentPos_10->setText(QApplication::translate("MainWindow", "\346\214\207\346\216\247IP", Q_NULLPTR));
        lblCurrentPos_27->setText(QApplication::translate("MainWindow", "\346\203\257\345\257\274IP", Q_NULLPTR));
        lblCurrentPos_14->setText(QApplication::translate("MainWindow", "\346\214\207\346\216\247Port", Q_NULLPTR));
        lblCurrentPos_35->setText(QApplication::translate("MainWindow", "Listen", Q_NULLPTR));
        lblCurrentPos_36->setText(QApplication::translate("MainWindow", "Listen", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab_8), QApplication::translate("MainWindow", "\351\205\215\347\275\256", Q_NULLPTR));
        lbDiffY->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "\350\241\245\345\201\277Y", Q_NULLPTR));
        label_19->setText(QApplication::translate("MainWindow", "\350\241\245\345\201\277X", Q_NULLPTR));
        lbDiffX->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbl_SimJoystick->setText(QString());
        lbSendUdpDataA->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lbldataE->setText(QApplication::translate("MainWindow", "E", Q_NULLPTR));
        lbldataA->setText(QApplication::translate("MainWindow", "A", Q_NULLPTR));
        lbSendUdpDataE->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        gbx_PicFlip_3->setTitle(QApplication::translate("MainWindow", "\346\260\264\345\271\263\351\231\200\350\236\272", Q_NULLPTR));
        rb_HGyroUnLock->setText(QApplication::translate("MainWindow", "\350\247\243\351\224\201", Q_NULLPTR));
        rb_HGyroLock->setText(QApplication::translate("MainWindow", "\351\224\201\345\256\232", Q_NULLPTR));
        pbDbgSendZhiKong->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\214\207\346\216\247\346\250\241\346\213\237", Q_NULLPTR));
        pbDbgSend->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", Q_NULLPTR));
        le_dbgSend->setText(QApplication::translate("MainWindow", "81 01 04 07 03 FF", Q_NULLPTR));
        pbDbgSendBorderChk->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\345\210\207\346\215\242\346\250\241\346\213\237", Q_NULLPTR));
        gbx_RoiSize->setTitle(QApplication::translate("MainWindow", "\350\267\237\350\270\252\346\241\206", Q_NULLPTR));
        rb_trackRoi40->setText(QApplication::translate("MainWindow", "160 x 160", Q_NULLPTR));
        rb_trackRoi10->setText(QApplication::translate("MainWindow", "40 x 40", Q_NULLPTR));
        rb_trackRoi20->setText(QApplication::translate("MainWindow", "80 x 80", Q_NULLPTR));
        lblTrackingFps->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "\350\257\206\345\210\253\345\270\247\347\216\207", Q_NULLPTR));
        rb_Tracking->setText(QApplication::translate("MainWindow", "\350\267\237\350\270\252", Q_NULLPTR));
        pbDbgSendconvAE->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\345\235\220\346\240\207\350\275\254\346\215\242", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        leGD_B_IP->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        pbDisConnect->setText(QApplication::translate("MainWindow", "\351\207\215\350\277\236\351\200\232\350\256\257", Q_NULLPTR));
        pbDisConnect_B->setText(QApplication::translate("MainWindow", "\351\207\215\350\277\236\351\200\232\350\256\257", Q_NULLPTR));
        label_21->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\220\216", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", "\345\205\211\347\224\265\345\211\215", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        leGD_A_IP->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        pbConnect->setText(QApplication::translate("MainWindow", "\351\207\215\350\277\236\350\247\206\351\242\221", Q_NULLPTR));
        pbConnect_B->setText(QApplication::translate("MainWindow", "\351\207\215\350\277\236\350\247\206\351\242\221", Q_NULLPTR));
        lbl_Version->setText(QApplication::translate("MainWindow", "\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        pbSysClose->setText(QApplication::translate("MainWindow", "\345\205\263\346\234\272", Q_NULLPTR));
        cbxSetDbgDistance->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\350\260\203\350\257\225\350\267\235\347\246\273", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        lbMousePosition_x->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        lbMousePosition_y->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        label_31->setText(QApplication::translate("MainWindow", "mGyroPitch_A", Q_NULLPTR));
        lbGyroPitch_A->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lerawdira->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        lerawdirb->setText(QApplication::translate("MainWindow", "_", Q_NULLPTR));
        tabWidConfig->setTabText(tabWidConfig->indexOf(tab_3), QApplication::translate("MainWindow", "\350\260\203\350\257\225", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
