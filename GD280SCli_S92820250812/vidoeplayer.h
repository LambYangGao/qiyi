#pragma once

#include <QThread>
#include <QImage>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QReadWriteLock>
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <chrono>
#include <iomanip>
#include <QElapsedTimer>


extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
extern "C"
{
#pragma comment (lib, "Ws2_32.lib")    
#pragma comment (lib, "avcodec.lib")  
#pragma comment (lib, "avdevice.lib")  
#pragma comment (lib, "avfilter.lib")  
#pragma comment (lib, "avformat.lib")  
#pragma comment (lib, "avutil.lib")  
#pragma comment (lib, "swresample.lib")  
#pragma comment (lib, "swscale.lib")  
};

class vidoeplayer : public QThread
{
	Q_OBJECT

public:
	vidoeplayer();
	~vidoeplayer();
	void startPlay();
    void pauseReplay();
    void jumpToFrameReplay(int n);
    void stopReplay();
	//void writeFrame();

    QString channelUrl;
    unsigned int channelNo;
    bool runflag;
    bool recflag;
    bool replayflag;
    bool replayPauseflag;
    bool mFileSegTimeOut;
    bool mImageGetNew;
    QImage mImg;
    QString recDir;
    QString recDirToday;

    QString OSDText;
    QTime OsdTime;
    QTime CurrentRecFileStartTime;
    double OsdA;
    double OsdE;
    int OsdR;
    int OsdLens;
    int RelativeTime;
    double OsdViewAngle;
    bool OsdUpdate;
    int num_frame;

	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame *pFrame, *pFrameRGB;
	AVFrame *pFrameYUV;
	AVPacket *packet;
	uint8_t *out_buffer;
	uint8_t *out_buffer_YUV;
	struct SwsContext *img_convert_yuv;

    bool enableFrameRateControl;  // 是否启用帧率控制
    int targetFPS;                // 目标帧率
    QTime lastFrameTime;          // 上一帧时间


signals:
    void sig_GetOneFrameCh1(QImage); //每获取到一帧图像 就发送此信号
    void sig_GetOneFrameCh2(QImage); //每获取到一帧图像 就发送此信号

protected:
	void run();
    void creatFilename();

private:
    char mVideoFilename[256];
    QString mSrtFilename;
    QFile srtFileHandle;
    void writeSrtFile(int curframeNo);
    int elapsedToString(int elapsed,char * sElapsed);

    // 延时测量相关
    QElapsedTimer m_frameTimer;           // 帧定时器
    qint64 m_lastFrameTime;               // 上一帧时间
    qint64 m_decodeStartTime;             // 解码开始时间
    qint64 m_decodeEndTime;               // 解码结束时间
    qint64 m_emitTime;                    // 发送信号时间
    // 统计信息
    int m_frameCount;                     // 帧计数
    qint64 m_totalDecodeTime;             // 总解码时间
    qint64 m_totalEmitDelay;              // 总发送延时
    void printLatencyStats();             // 打印延时统计

};
