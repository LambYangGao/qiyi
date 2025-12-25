#ifndef TRACKERTHREAD_H
#define TRACKERTHREAD_H

#include <QtCore/QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStandardItem>
#include <QStandardItemModel>
#include <iostream>
#include "kcftracker.h"
#include "dlcmtracker.h"
#include "tophattracker.h"
#include <opencv2/dnn.hpp>
#include <opencv2/tracking/tracking.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QBuffer>
#include <QTime>
#include <QTcpSocket>
#include "detector/cdetector.h"
#include <chrono>
#include <iomanip>
#include <QElapsedTimer>

using namespace std;
using namespace cv;

class TrackerThread : public QThread
{

#define VPSS_PROC_SIZEX		416
#define VPSS_PROC_SIZEY		416

#define ROI_X_DEFAULT       (VPSS_PROC_SIZEX>>1)  //380   //940;//380
#define ROI_Y_DEFAULT       (VPSS_PROC_SIZEY>>1)  //280   //540;//280
#define ROI_WIDTH_DEFAULT   80
#define ROI_HEIGHT_DEFAULT  80

#define TRACKING_METHOD_NONE 0         // 无跟踪
#define TRACKING_METHOD_KCF 1          // KCF (Kernelized Correlation Filter) 跟踪
#define TRACKING_METHOD_TOPHAT 2       // Top-Hat 形态学算法
#define TRACKING_METHOD_CENTROIDWH 3   // 白色质心跟踪
#define TRACKING_METHOD_CENTROIDBLK 4  // 黑色质心跟踪
#define TRACKING_METHOD_DLCM 5
#define TRACKING_METHOD_DETECT 6       // 目标检测算法
#define TRACKING_METHOD_IR_SMALL 7     // 小目标检测
#define TRACKING_METHOD_TOPHAT706 8       //
#define TRANSFILEINFO 0xaa
#define TRANSFILEDATA 0xab

#define TCPBUFSIZE  1000*64

    Q_OBJECT
public:
    struct FileDate//文件传输结构
    {
        int type;
        int readCnt;
        int flowNo;
        int lastSize ;
        uint8_t data[TCPBUFSIZE];
    };

    struct DetectResult//引导数据
    {
        int id;
        int x;
        int y;
        int w;
        int h;
        float prob;
    };

    struct IRDetectionParams {//小目标检测参数
        int tarLmax;    // 目标最大尺寸
        int k1;         // 算法参数1
        int k2;         // 算法参数2
        float scoreThreshold;  // 阈值
    };

    IRDetectionParams irParams;  // 检测参数
    DetectResult mDRetu;
    QSystemSemaphore * m_semaphore;
    QSharedMemory sharedMemory;
    QSystemSemaphore * m_semaphoreRetu;     //读取返回值用
    QSharedMemory sharedMemoryRetu;

    QTcpSocket * tcpSocket;
    int openTcp();

    QImage qImg;
    cv::Mat oneFrame;
    cv::Rect bbox;
    TrackerThread();
    ~TrackerThread();
    bool bRunflag;
    bool boxChanged;
    int fps;
    //bool toCenter;
    int trackingMethod;
    bool bTrackerStart;
    int roi_width;
    int roi_height;
    int roi_min_width;
    int roi_min_height;
    bool HOG;
    bool FIXEDWINDOW;
    bool MULTISCALE;
    bool SILENT;
    bool LAB;
    cv::dnn::Net net;
    int mCentroidWhMax;
    int mCentroidBlkMin;
    int roi_num;
    QImage GraizeImage( QImage& image );

    bool useCustomCenter;        // 是否使用自定义中心点
    cv::Point2f customCenter;    // 自定义中心点
    int roi_width_=320;
    int roi_height_=256;
    void setCustomCenter(cv::Point2f center);     // 设置自定义中心点

signals:
    void sig_UpdateTrackingBox(int,int,int,int,int);
    void sig_UpdateDetectingBox(int,int,int,int,int,int,float);

protected:
    void run(); // 线程入口函数

private:
    long gPreTime;
    KCFTracker * kcfTracker;
    //DLCMTracker * dlcmTracker;
    ThTracker * thTracker;

    int kcfTrackingFunc(cv::Mat & frame);
    //int dlcmTrackingFunc(cv::Mat & frame);
    int thTrackingFunc(cv::Mat & frame);
    int thTrackingFunc706(cv::Mat & frame);
    float computeScore(cv::Mat img8uc1, cv::Rect rect);
    cv::Rect WhCentroTrackingFunc(cv::Mat & frame, int x, int y, int width, int height);
    cv::Rect BlkCentroTrackingFunc(cv::Mat & frame, int x, int y, int width, int height);
	int yoloDetectFunc();
    int yoloDetectFunc2();
	
    //Ptr<TrackerMIL> opencvtracker;    //帧速低，10，效果不好
    //Ptr<TrackerTLD> opencvtracker;
    //Ptr<TrackerKCF> opencvtracker;
    //Ptr<TrackerMedianFlow> opencvtracker; //速度快，不容易闪丢
    //Ptr<TrackerBoosting> opencvtracker;     //小面积框时速度快，小目标效果好，帧速30，受背景影响较小，框固定不自动变大小
    //Ptr<TrackerMOSSE> opencvtracker;        //速度快 50+，小目标效果好，受背景影响小，框固定不自动变大小 ,对比度低时不能框目标
    Ptr<TrackerCSRT> opencvtracker;     //小目标效果好，帧速30，，受背景影响小
    //Ptr<TrackerGOTURN> opencvtracker;
    void initOpenCVTracker();
    void opencvTracker(cv::Mat & frame);
    int lastTime;
    int frameCount;
    int calcFps();
    QTime mLastTime;
    cv::Mat imageCvtMat();
    int sendOneImage(QImage img);
    void socket_Read_Data();


    CDetector* irDetector = nullptr; // 小目标检测
    int irSmallTargetDetection(cv::Mat& frame); // 小目标检测函数
    //跟踪中心初始化
    int initCenter(cv::Point2f &center);
    //ROI开窗跟踪
    void roiTrackbydetect(cv::Mat frame8,cv::Rect &box,int tarLmax, int k1, int k2);
    float center_x;
    float center_y;

};

#endif // TRACKERTHREAD_H
