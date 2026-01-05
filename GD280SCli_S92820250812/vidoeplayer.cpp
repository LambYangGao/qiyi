#include "vidoeplayer.h"
#include <QDebug>

#include <stdio.h>
#include <iostream>
using namespace std;


#define NVIDIA_H264_DECODER "h264_cuvid"

vidoeplayer::vidoeplayer()
{
    runflag =true;
    recflag = false;
    replayflag = false;
    //paintThdFree=true;
    channelUrl = "";
    num_frame = 0;
    mImageGetNew = false;

    enableFrameRateControl = false;  // 默认启用帧率控制
    targetFPS = 25;                 // 默认25fps
    lastFrameTime = QTime::currentTime();

    // 初始化延时测量变量
    m_frameCount = 0;
    m_lastFrameTime = 0;
    m_totalDecodeTime = 0;
    m_totalEmitDelay = 0;
    m_frameTimer.start();  // 启动计时器
}
vidoeplayer::~vidoeplayer()
{
    quit();
    wait();
}

void vidoeplayer::startPlay()
{
	this->start();
}

void vidoeplayer::pauseReplay()
{
    replayPauseflag = !replayPauseflag;
}

void vidoeplayer::jumpToFrameReplay(int n)
{

}
int vidoeplayer::elapsedToString(int elapsed,char * sElapsed)
{
    if(NULL == sElapsed) return -1;
    int min = 0;
    int sec = 0;
    int msec = 0;

    min = elapsed/60000;
    sec = (elapsed % 60000)/1000;
    msec = (elapsed % 60000) % 1000;
    sprintf(sElapsed,"%02d:%02d:%02d,%03d",0,min,sec,msec);
    return 0;
}

void vidoeplayer::writeSrtFile(int curframeNo)
{
    char str[128];
    char sLastElapsed[16];
    char sCurElapsed[16];
    QString ParamText;

    if(recflag == false)
    {
        return;
    }
    QTextStream stream(&srtFileHandle);
    stream.seek(srtFileHandle.size());   //移动到文件末尾

    if(channelNo == 1)
    {
        static QTime lastframe_time ;
        static QTime curframe_time ;
        static int serialNo = 0;
        if(curframeNo == 0)
        {
            lastframe_time = CurrentRecFileStartTime;
            serialNo = 0;
        }
        serialNo++;

        curframe_time = OsdTime;
        //qDebug()<<"CurStartTime:"<<CurrentRecFileStartTime<<" OsdTime:"<<OsdTime;
        int lastElapsed = CurrentRecFileStartTime.msecsTo(lastframe_time);
        int curElapsed = CurrentRecFileStartTime.msecsTo(curframe_time);
        elapsedToString(lastElapsed,sLastElapsed);
        //qDebug()<<"lastElapsed:"<<lastElapsed << "  sLastElapsed:"<<sLastElapsed;
        elapsedToString(curElapsed,sCurElapsed);
        lastframe_time = curframe_time;

        sprintf(str,"%d", serialNo);
        stream <<str <<"\n";
        sprintf(str,"%s --> %s",sLastElapsed,sCurElapsed);
        stream <<str <<"\n";

    }
    else
    {
        static QTime lastframe_time ;
        static QTime curframe_time ;
        static int serialNo = 0;
        if(curframeNo == 0)
        {
            lastframe_time = CurrentRecFileStartTime;
            serialNo = 0;
        }
        serialNo++;

        curframe_time = OsdTime;
        //qDebug()<<"CurStartTime:"<<CurrentRecFileStartTime<<" OsdTime:"<<OsdTime;
        int lastElapsed = CurrentRecFileStartTime.msecsTo(lastframe_time);
        int curElapsed = CurrentRecFileStartTime.msecsTo(curframe_time);
        elapsedToString(lastElapsed,sLastElapsed);
        //qDebug()<<"lastElapsed:"<<lastElapsed << "  sLastElapsed:"<<sLastElapsed;
        elapsedToString(curElapsed,sCurElapsed);
        lastframe_time = curframe_time;

        sprintf(str,"%d", serialNo);
        stream <<str <<"\n";
        sprintf(str,"%s --> %s",sLastElapsed,sCurElapsed);
        stream <<str <<"\n";

    }
    ParamText =OSDText + QString("%1").arg(OsdTime.hour(), 2,10,QLatin1Char('0'))+":" + \
            QString("%1").arg(OsdTime.minute(), 2,10,QLatin1Char('0'))+":" + \
            QString("%1").arg(OsdTime.second(), 2,10,QLatin1Char('0'))+":" + \
            QString("%1").arg(OsdTime.msec(), 3,10,QLatin1Char('0'));
    ParamText = ParamText + " A:" + QString::number(OsdA, 'f',3)+" E:" + QString::number(OsdE, 'f',3)+ \
            " R:"+ QString::number(OsdR,10)+"m" + " L:";
    ParamText = ParamText + QString::number(OsdLens,10)+"mm";
    ParamText = ParamText + " V:" + QString::number(OsdViewAngle,'f',1);

    stream <<ParamText<<"\n";
    stream <<"\n";
}

void vidoeplayer::run()
{
    //static clock_t timeOld = 0;
    //clock_t timeCur = 0;
	static double timeOld = 0;
    double timeCur = 0;

    static struct SwsContext *img_convert_ctxCh1;
    static struct SwsContext *img_convert_ctxCh2;
    //FILE *fp_yuv = fopen("output.yuv", "wb");
	int videoStream, i, numBytes;
	int ret, got_picture;
    //int num_frame = 0;


    // FFmpeg初始化
    av_register_all();         //初始化FFMPEG  调用了这个才能正常适用编码器和解码器
    avformat_network_init();   //初始化FFmpeg网络模块，


    // RTSP连接参数
    AVDictionary *avdic = NULL;
    //在打开码流前指定各种参数比如:探测时间/超时时间/最大延时等
    //设置缓存大小,1080p可将值调大
    av_dict_set(&avdic, "buffer_size", "8192000", 0); // 8MB缓存
    //以tcp方式打开,如果以udp方式打开将tcp替换为udp
    av_dict_set(&avdic, "rtsp_transport", "tcp", 0);  // TCP传输
    //设置超时断开连接时间,单位微秒,3000000表示3秒
    av_dict_set(&avdic, "stimeout", "3000000", 0);    // 3秒超时
    //设置最大时延,单位微秒,1000000表示1秒
    av_dict_set(&avdic, "max_delay", "1000000", 0);   // 1秒最大延迟
    //自动开启线程数
    av_dict_set(&avdic, "threads", "auto", 0);

    //测试用rtsp地址，根据实际情况修改
    //char url[]="rtsp://192.168.0.39:8553/PSIA/Streaming/channels/0?videoCodecType=H.264";
    //char url[]="rtsp://192.168.0.39:8554/PSIA/Streaming/channels/1?videoCodecType=H.264";
    //char url[] = "D:/jump.mp4";

    //Allocate an AVFormatContext.
    // 打开RTSP流
    pFormatCtx = avformat_alloc_context();
    qDebug()<<"url : "<<channelUrl.toLatin1().data();
    if (avformat_open_input(&pFormatCtx, channelUrl.toLatin1().data(), NULL, &avdic) != 0) {
        printf("can't open the file.%s \n",channelUrl.toLatin1().data());
		return;
	}
    //释放设置参数
    if (avdic != NULL) {
        av_dict_free(&avdic);
    }

    //获取流信息
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		printf("Could't find stream infomation.\n");
		return;
	}

    // 查找视频流和解码器
    videoStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pCodec, 0);
    if (videoStream < 0) {
        qDebug() <<  "find video stream index error";
        return;
    }
    //获取视频流
    AVStream * videoStreammmm = pFormatCtx->streams[videoStream];
    //获取视频流解码器,或者指定解码器
    pCodecCtx = videoStreammmm->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    //videoDecoder = avcodec_find_decoder_by_name("h264_qsv");
    if (pCodec == NULL) {
        qDebug() <<  "video decoder not found";
        return ;
    }

    //设置加速解码
    pCodecCtx->lowres = pCodec->max_lowres;
    pCodecCtx->flags2 |= AV_CODEC_FLAG2_FAST; // 快速解码模式

	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		printf("Could not open codec.\n");
		return;
	}
    else
    {
        qDebug()<<"Codec: "<<pCodec->name<<" opened success!";
    }
    //int fn=av_seek_frame(pFormatCtx,videoStream,(int64_t)10,AVSEEK_FLAG_BACKWARD);
    //qDebug()<<"videoStream:"<<videoStream<<" av_seek_frame "<<fn;


    //获取分辨率大小
    int videoWidth = videoStreammmm->codec->width;
    int videoHeight = videoStreammmm->codec->height;

    //如果没有获取到宽高则返回
    if (videoWidth == 0 || videoHeight == 0) {
        qDebug() << "find width height error";
        return;
    }

    QString videoInfo = QString("视频流信息 -> 索引: %1  解码: %2  格式: %3  时长: %4 秒  分辨率: %5*%6")
                        .arg(videoStream).arg(pCodec->name).arg(pFormatCtx->iformat->name)
                        .arg((pFormatCtx->duration) / 1000000).arg(videoWidth).arg(videoHeight);
    qDebug() << videoInfo;

    //预分配好内存
    packet = av_packet_alloc();
	pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    pFrameYUV = av_frame_alloc();

    //比较上一次文件的宽度高度,当改变时,需要重新分配内存
    //if (oldWidth != videoWidth || oldHeight != videoHeight)
    {
        int byte = avpicture_get_size(AV_PIX_FMT_RGB32, videoWidth, videoHeight);
        out_buffer = (uint8_t *)av_malloc(byte * sizeof(uint8_t));
        //oldWidth = videoWidth;
        //oldHeight = videoHeight;
    }

    //定义像素格式
    AVPixelFormat srcFormat = AV_PIX_FMT_YUV420P;
    AVPixelFormat dstFormat = AV_PIX_FMT_RGB32;
    //通过解码器获取解码格式
    srcFormat = pCodecCtx->pix_fmt;

    //默认最快速度的解码采用的SWS_FAST_BILINEAR参数,可能会丢失部分图片数据,可以自行更改成其他参数
    int flags = SWS_FAST_BILINEAR;

    //开辟缓存存储一帧数据
    //avpicture_fill((AVPicture *)avFrame3, buffer, dstFormat, videoWidth, videoHeight);
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, dstFormat, videoWidth, videoHeight, 1);

  /*  //图像转换
    swsContext = sws_getContext(videoWidth, videoHeight, srcFormat, videoWidth, videoHeight, dstFormat, flags, NULL, NULL, NULL);
    */




/*
    //保存YUV数据
	out_buffer_YUV = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer_YUV, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	img_convert_yuv = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
*/
    //图像转换 将解码后的YUV数据转换成RGB32
    if(channelNo==1)
    {
        img_convert_ctxCh1 = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            srcFormat, pCodecCtx->width, pCodecCtx->height,
            AV_PIX_FMT_RGB32, flags, NULL, NULL, NULL);
    }
    else if(channelNo==2)
    {
        img_convert_ctxCh2 = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            srcFormat, pCodecCtx->width, pCodecCtx->height,
            AV_PIX_FMT_RGB32, flags, NULL, NULL, NULL);
    }

#if 1
    //如果要写文件或者硬解码，要将SPS和PPS加到I帧的头部，因此需要对分好的数据包进行处理
    //1. 找到相应解码器的过滤器
    const AVBitStreamFilter* bsf = av_bsf_get_by_name("h264_mp4toannexb");
    AVBSFContext* ctx;
    //2.过滤器分配内存
    int retu = av_bsf_alloc(bsf, &ctx);
    //3. 添加解码器属性
    AVCodecParameters* codecpar = NULL;
    if (pFormatCtx->streams[videoStream]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        codecpar = pFormatCtx->streams[videoStream]->codecpar;
    }
    avcodec_parameters_copy(ctx->par_in, codecpar);
    //4. 初始化过滤器上下文
    av_bsf_init(ctx);
#endif

    //保存一段时间的视频流，写入文件中
    FILE* fpSave;
    bool recStarted = false;

    //creatFilename();

    while (runflag)
    {
        if(replayflag && replayPauseflag)
        {
            msleep(40);
            continue;
        }

        // 读取帧开始时间
        qint64 readFrameStartTime = m_frameTimer.elapsed();

        if (av_read_frame(pFormatCtx, packet) < 0)
        {
            msleep(40);
            av_packet_unref(packet);
            if(true == replayflag)
            {
                break;
            }
            continue;
        }


        if (packet->stream_index == videoStream)
        {
#if 0
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0)
            {
                printf("decode error.\n");
                av_packet_unref(packet);
                continue;
            }
                //avcodec_decode_video2(videoCodec, avFrame2, &got_picture, avPacket);
#else


            // 2. 记录解码开始时间
            m_decodeStartTime = m_frameTimer.elapsed();

            // H.264解码
            got_picture = avcodec_send_packet(pCodecCtx, packet);
            if (got_picture < 0)
            {
                qDebug()<<"1 got_picture < 0";
                continue;
            }

            got_picture = avcodec_receive_frame(pCodecCtx, pFrameRGB);
            if (got_picture < 0)
            {
                qDebug()<<"2 got_picture < 0";
                continue;
            }
#endif
            // 3. 记录解码结束时间
            m_decodeEndTime = m_frameTimer.elapsed();
            qint64 decodeDelay = m_decodeEndTime - m_decodeStartTime;

            if (got_picture >= 0)
            {

                //if(pCodecCtx->width !=pFrame->width)
                //    printf("====>%d linesize=%d wid=%d h=%d  pfWidth=%d\n",channelNo,pFrame->linesize,pCodecCtx->width,pCodecCtx->height,pFrame->width);
                // 图像格式转换：YUV420P → RGB32
                if(channelNo==1)
                {
                    sws_scale(img_convert_ctxCh1,
                        (uint8_t const * const *)pFrameRGB->data,
                        pFrameRGB->linesize, 0, pCodecCtx->height, pFrameYUV->data,
                        pFrameYUV->linesize);
                }
                else if(channelNo==2)
                {
                    sws_scale(img_convert_ctxCh2,
                        (uint8_t const * const *)pFrameRGB->data,
                        pFrameRGB->linesize, 0, pCodecCtx->height, pFrameYUV->data,
                        pFrameYUV->linesize);
                }


                //把这个RGB数据 用QImage加载--------转换为QImage并发送信号
                QImage image((uchar *)out_buffer, pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32);
                 //QImage image = tmpImg.copy();   //把图像复制一份 传递给界面显示

                //image.save(QString("frame_ch%2.png").arg(channelNo));

                if(image.isNull())
                {
                    qDebug()<<"player image is null";
                }
                else
                {
                    // 发送到UI显示
                    if(1==channelNo)
                    {
                        //printf("emit1:%s,%d\n",channelUrl.toLatin1().data(),channelNo);
                        emit sig_GetOneFrameCh1(image);    //ch1发送信号--------发送到主线程
                        //printf("ch1 wid=%d\n",pCodecCtx->width);
                    }
                    else
                    {
                        //printf("%---->2:%s,%d\n",channelUrl.toLatin1().data(),channelNo);
                        emit sig_GetOneFrameCh2(image);    //ch2发送信号
                        //printf("ch2 wid=%d\n",pCodecCtx->width);
                    }


                    // 4 记录发送信号后时间
                    qint64 afterEmitTime = m_frameTimer.elapsed();

                    // 计算总延时
                    qint64 totalFrameDelay = afterEmitTime - readFrameStartTime;
                    // 更新统计信息
                    m_frameCount++;
                    m_totalDecodeTime += decodeDelay;

                    if(enableFrameRateControl)
                    {
                        // 计算帧间隔时间（毫秒）
                        int frameInterval = 1000 / targetFPS;  // 25fps = 40ms

                        // 计算从上一帧到现在的时间差
                        QTime currentTime = QTime::currentTime();
                        int elapsed = lastFrameTime.msecsTo(currentTime);

                        // 如果时间差小于目标间隔，则等待
                        if(elapsed < frameInterval)
                        {
                            int sleepTime = frameInterval - elapsed;
                            msleep(sleepTime);
                        }

                        lastFrameTime = QTime::currentTime();
                    }
#if 1
                    // 录像处理
                    if(true==recflag)
                    {
                        if(mFileSegTimeOut == true)
                        {
                            if(recStarted == true)
                            {
                                fclose(fpSave);
                                srtFileHandle.close();
                                recStarted = false;
                            }
                            creatFilename();
                            fpSave= fopen(mVideoFilename, "wb");
                            if (fpSave ==NULL )
                            //if (fopen_s(&fpSave, mVideoFilename, "wb")!=NULL)
                            {
                                printf("创建视频文件 %s 失败！\n",mVideoFilename);
                                continue;
                            }

                            srtFileHandle.setFileName(mSrtFilename);
                            if (srtFileHandle.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) == false)
                            {
                                printf("创建srt文件 %s 失败！\n",mSrtFilename.data());
                                continue;
                            }

                            recStarted = true;
                            CurrentRecFileStartTime = OsdTime; //取当前帧的时间为本Srt文件起始时间
                            num_frame=0;
                            mFileSegTimeOut = false;
                            OsdUpdate = true;
                        }

                        av_bsf_send_packet(ctx, packet);//发送分好的数据包
                        if (av_bsf_receive_packet(ctx, packet) == 0)//接收分好的数据包
                        {
                            //写入SPS和PPS信息
                            fwrite(pFormatCtx->streams[videoStream]->codecpar->extradata, 1, pFormatCtx->streams[videoStream]->codecpar->extradata_size, fpSave);
                        }
                        fwrite(packet->data, 1, packet->size, fpSave);
                        if(true == OsdUpdate)
                        {
                            writeSrtFile(num_frame);
                            OsdUpdate=false;
                        }
                    }
                    else
                    {
                        if(recStarted == true)
                        {
                            fclose(fpSave);
                            srtFileHandle.close();
                            recStarted = false;
                            qDebug()<<tr("Write Recording file: ")<<channelNo<<" _ "<<mVideoFilename;
                        }
                    }
#ifdef SIMUCAMERA
                    msleep(33);     //只在模拟相机时打开
#endif
                    num_frame++;
                    if(true == replayflag)
                    {
#ifdef OS_LINUX
                        timeOld = clock();
                        for(int i=0;i<40;i++)
                        {
                            timeCur = clock();
                            if( ((timeCur   - timeOld)/1000) >= 15)         //相机25帧, 40ms间隔 //Linux 版本使用这个clock计算方式
                            {
                                i=40;
                            }
                            else
                            {
                                msleep(1);
                            }
                        }
#elif OS_KYLIN
                        /*for(int i=0;i<40;i++)
                        {
                            timeCur = clock();
                            if(timeCur - timeOld >= 39)         //相机25帧, 40ms间隔
                            {
                                i=40;
                            }
                            else
                            {
                                msleep(1);
                            }
                        } */
                        msleep(33);
#elif _WIN32
                        //msleep(30);
                        /*for(int i=0;i<40;i++)
                        {
                            timeCur = clock();
                            if(timeCur - timeOld >= 39)         //相机25帧, 40ms间隔
                            {
                                i=40;
                            }
                            else
                            {
                                msleep(1);
                            }
                        }*/
#endif
                        timeOld = timeCur;
                    }
#endif
                }

            }

		}
        av_packet_unref(packet);
	}
    av_free_packet(packet);
//    av_bsf_free(&ctx);
    av_free(out_buffer);
	av_free(pFrameRGB);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
    cout << "总帧数为：    " << num_frame << endl;

    // 打印最终统计信息
    //printLatencyStats();
}

void vidoeplayer::creatFilename()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd_hhmmss");
    recDirToday = recDir + "/" + current_date_time.toString("yyyyMMdd");

    // 创建日期目录
    QDir *folder = new QDir;
    if(false == folder->exists(recDirToday))   //判断创建文件夹是否存在
    {
        folder->mkdir(recDirToday);    //创建文件夹
    }

    if(true==recflag)
    {
        if(channelNo==1)
        {
            sprintf(mVideoFilename,(recDirToday + "/GD1"+ current_date+".h264").toLatin1());
            mSrtFilename = recDirToday + "/GD1"+ current_date+".srt";
        }
        else
        {
            sprintf(mVideoFilename,(recDirToday + "/GD2"+ current_date+".h264").toLatin1());
            mSrtFilename = recDirToday + "/GD2"+ current_date+".srt";
        }
        //qDebug()<<("mVideoFilename=")<<mVideoFilename<<("   mSrtFilename=")<<mSrtFilename;
    }
}


void vidoeplayer::printLatencyStats()
{
    if(m_frameCount > 0)
    {
        qDebug() << QString("=== 通道%1 最终延时统计 ===").arg(channelNo);
        qDebug() << QString("总处理帧数: %1").arg(m_frameCount);
        qDebug() << QString("平均解码延时: %1 ms").arg(m_totalDecodeTime / m_frameCount);
        qDebug() << QString("平均发送延时: %1 ms").arg(m_totalEmitDelay / m_frameCount);
        qDebug() << "==============================";
    }
}

