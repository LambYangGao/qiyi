#include "trackerthread.h"

extern pthread_mutex_t gTrackingMutex;
extern pthread_cond_t gTrackingTransImg;



TrackerThread::TrackerThread()
{
    // 初始化多种跟踪算法
    HOG = true;
    FIXEDWINDOW = false;
    MULTISCALE = false;
    SILENT = false;
    LAB = true;
    kcfTracker = new KCFTracker(HOG,FIXEDWINDOW,MULTISCALE,LAB);
    //dlcmTracker = new DLCMTracker();
    thTracker = new ThTracker(); // TopHat算法

    // 初始化状态变量
    trackingMethod = TRACKING_METHOD_NONE;  // 初始无跟踪
    bRunflag = true;                        // 线程运行标志
    boxChanged = true;                      // 跟踪框变化标志
    //toCenter = false;
    roi_width = ROI_WIDTH_DEFAULT;          // 默认ROI大小：80
    roi_height = ROI_HEIGHT_DEFAULT;        // 默认ROI大小：80
    bTrackerStart = false;
    gPreTime = 0;                           // 跟踪目标数量

    fps=0;
    lastTime=0;
    frameCount=0;
    mCentroidWhMax = 240;
    mCentroidBlkMin = 80;
    roi_num = 0;

    irDetector = new CDetector(); // 初始化小目标检测器
    //cv::Point2f center =cv::Point2f(320, 256);
    //irDetector->initCenter(center);
    irParams.tarLmax = 9;  // 设置默认参数
    irParams.k1 = 2;
    irParams.k2 = 1;
    irParams.scoreThreshold = 3.0f;

    useCustomCenter = false;     // 默认不使用自定义中心点
    customCenter = cv::Point2f(320, 240);  // 默认中心点
    roi_width_ = 320;            // ROI窗口宽度
    roi_height_ = 256;           // ROI窗口高度
    /*
    cv::String modelTxt = "D:/ObjTracking/build-centroidtracker-Desktop_Qt_5_9_9_MinGW_32bit-Debug/debug/model/deploy.prototxt";
    cv::String modelBin = "D:/ObjTracking/build-centroidtracker-Desktop_Qt_5_9_9_MinGW_32bit-Debug/debug/model/res10_300x300_ssd_iter_140000.caffemodel";
    qDebug()<< "Loading model.." << endl;
    net = cv::dnn::readNetFromCaffe(modelTxt, modelBin);*/

    //initOpenCVTracker();
    // 创建共享内存用于YOLO检测
    sharedMemory.setKey("QSharedMemoryExample");
    m_semaphore = new QSystemSemaphore("market", 1, QSystemSemaphore::Open);

    sharedMemoryRetu.setKey("QSharedMemoryRetu");
    m_semaphoreRetu = new QSystemSemaphore("semYoloRetu", 1, QSystemSemaphore::Open);

    //创建指定大小的共享内存段
    if(!sharedMemory.create(1440000))   //
    {
        qDebug()<<"unable create shared memory ";
        return;
    }

    //openTcp();    //GD280s mianyang 项目不用tcp方式
}

TrackerThread::~TrackerThread()
{
    qDebug()<<"quit from TrackerThread()";
    delete(kcfTracker);
    //delete(dlcmTracker);
    delete(thTracker);
    delete(irDetector);
    quit();
    wait();
}


int TrackerThread::initCenter(cv::Point2f &center_)
{
    center_x=center_.x;
    center_y=center_.y;
}

void TrackerThread::setCustomCenter(cv::Point2f center)
{
    customCenter = center;
    useCustomCenter = true;
    center_x = center.x;  // 同时更新center_x和center_y
    center_y = center.y;
    //qDebug() << "设置自定义跟踪中心点: (" << center.x << "," << center.y << ")";
}

void TrackerThread::roiTrackbydetect(cv::Mat frame8,cv::Rect &box,int tarLmax, int k1, int k2)
{
      int u16Width= frame8.cols;
      int u16Height=frame8.rows;

      //利用Center位置 和OPENCV进行ROI扣图
      int xmin =center_x - roi_width_ / 2;
      int ymin = center_y - roi_height_/ 2;
      int xmax = center_x +roi_width_ / 2;
      int ymax = center_y + roi_height_ / 2;

      xmin = xmin < 0 ? 0 : xmin;
      ymin = ymin < 0 ? 0 : ymin;
      xmax = xmax > (u16Width - 1) ? u16Width - 1 : xmax;
      ymax = ymax > (u16Height - 1) ? u16Height - 1 : ymax;

      cv::Rect roiBox(xmin, ymin, xmax - xmin, ymax - ymin);
      cv::Mat roimat = frame8(roiBox).clone();


      //ROI 转格式
      irDetector->detect(roimat,tarLmax,k1,k2);
      for(int i=0;i<irDetector->m_targetRects.size();i++)
      {
         irDetector->m_targetRects[i].x+=xmin;
         irDetector->m_targetRects[i].y+=ymin;
      }

      //输出目标并判断可信目标 距离中心最近的目标
      float min_dis = 65536;
      int min_id = -1;
      for (int i = 0; i < irDetector->m_targetRects.size(); i++)
      {
          cv::Point2f objcen = cv::Point2f(irDetector->m_targetRects[i].x+ irDetector->m_targetRects[i].width/2,
                                           irDetector->m_targetRects[i].y+ irDetector->m_targetRects[i].height/2);
          float dis = sqrt((objcen.x - center_x) * (objcen.x - center_x) +
              (objcen.y - center_y) * (objcen.y - center_y));

          if (min_dis > dis)
          {
              min_dis = dis;
              min_id = i;
          }
      }
      if (min_id > -1)
      {

          cv::Rect objbox=cv::Rect(irDetector->m_targetRects[min_id].x,irDetector->m_targetRects[min_id].y,
                                   irDetector->m_targetRects[min_id].width,irDetector->m_targetRects[min_id].height);
          cv::Point2f objcen = cv::Point2f(objbox.x+objbox.width/2,objbox.y+objbox.height/2);

          //TODO：根据目标尺寸自适应开窗大小，若目标宽高接近窗口大小
          if (objbox.width > roi_width / 2
              ||objbox.height > roi_height / 2)
          {
              roi_width = roi_width * 1.5;
              roi_height = roi_height * 1.5;
          }

          center_x = objcen.x;
          center_y = objcen.y;

          box=objbox;
      }
}

int TrackerThread::irSmallTargetDetection(cv::Mat& frame)
{
    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat grayFrame;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    } else {
        grayFrame = frame.clone();
    }

    cv::Mat img8;
    if (grayFrame.depth() != CV_8U) {
        grayFrame.convertTo(img8, CV_8U);
    } else {
        img8 = grayFrame;
    }

    // 根据自定义中心点来设置检测中心
    cv::Point2f center;
    if (useCustomCenter) {
        // 使用用户鼠标点击的位置作为中心点
        center = customCenter;
        // 确保中心点在图像范围内
        center.x = std::max(0.0f, std::min((float)img8.cols - 1, center.x));
        center.y = std::max(0.0f, std::min((float)img8.rows - 1, center.y));

        //qDebug() << "使用自定义中心点: (" << center.x << "," << center.y << ")";
    } else {
        // 使用图像中心作为检测中心
        center = cv::Point2f(img8.cols / 2, img8.rows / 2);
    }

    //cv::Point2f center =cv::Point2f(img8.cols / 2, img8.rows / 2);
    //qDebug() << "目标检测 - 输入图像尺寸:" << img8.cols << "x" << img8.rows << ", 参数: tarLmax=" << irParams.tarLmax << ", k1=" << irParams.k1 << ", k2=" << irParams.k2 << ", 阈值=" << irParams.scoreThreshold;
    initCenter(center);
    cv::Rect objbox;
    roiTrackbydetect(img8,objbox,irParams.tarLmax, irParams.k1, irParams.k2);
    //irDetector->detect(img8, irParams.tarLmax, irParams.k1, irParams.k2);// 调用检测算法

    // 处理检测结果
    //roi_num = 0;
//    cv::Rect bestTarget;
//    float bestScore = 0;

//    for (int i = 0; i < irDetector->m_targetRects.size(); i++) {
//        cv::Rect tarRect = irDetector->m_targetRects[i];
//        float targetScore = irDetector->m_targetScores[i];

//        //qDebug() << "候选目标" << i << ": 位置(" << tarRect.x << "," << tarRect.y << "), 尺寸" << tarRect.width << "x" << tarRect.height << ", 得分" << targetScore;

//        // 筛选高置信度目标
//        if (targetScore > irParams.scoreThreshold) {
//            if (targetScore > bestScore) {
//                bestScore = targetScore;
//                bestTarget = tarRect;
//                roi_num = 1;
//            }
//        }

//        cv::rectangle(frame, tarRect, cv::Scalar(0, 0, 255), 2);
//        char txt[64];
//        sprintf(txt, "%.2f", targetScore);
//        cv::putText(frame, txt, tarRect.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
//    }

     //在原图上直接画框调试
    //cv::rectangle(frame, objbox, cv::Scalar(0, 0, 255), 2);

    //char txt[64];
    //sprintf(txt, "%.2f", bestScore);
    //cv::putText(frame, txt, objbox.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));

    // 更新跟踪框
//    if (roi_num > 0) {
//        bbox = bestTarget;
//        emit sig_UpdateDetectingBox(roi_num, 0, bbox.x, bbox.y, bbox.width, bbox.height, bestScore);
//    } else {
//            emit sig_UpdateDetectingBox(0, 0, 0, 0, 0, 0, 0.0f);
//    }

    if (objbox.width > 0 && objbox.height > 0) {
        roi_num = 1;
        bbox = objbox;

//        //尝试改变跟踪框大小
//        int orig_center_x = objbox.x + objbox.width/2;   // 记录原始中心X
//        int orig_center_y = objbox.y + objbox.height/2;  // 记录原始中心Y
//        if (objbox.width < 32)  objbox.width = 32;
//        if (objbox.height < 18) objbox.height = 18;
//        objbox.x = orig_center_x - objbox.width/2;
//        objbox.y = orig_center_y - objbox.height/2;

        emit sig_UpdateTrackingBox(roi_num, objbox.x, objbox.y, objbox.width, objbox.height);
        //706测延时
        // QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        // qDebug() << "跟踪算法发出信号:" << timestamp;

        // 检测成功后，更新中心点为检测到的目标中心
        cv::Point2f newCenter = cv::Point2f(objbox.x + objbox.width/2, objbox.y + objbox.height/2);
        customCenter = newCenter;
        center_x = newCenter.x;
        center_y = newCenter.y;
    } else {
        roi_num = 0;
        emit sig_UpdateTrackingBox(0, 0, 0, 0, 0);
    }

    // 显示调试图像
//    cv::imshow("Detection Result", frame);
//    cv::waitKey(1);



//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double, std::milli> elapsed = end - start;
//    //fps = calcFps();
//    //qDebug() << "检测耗时:" << elapsed.count() << "ms, 当前fps:";
//    qDebug() << "检测耗时:" << elapsed.count() << "ms";

    return 0;
}

int TrackerThread::kcfTrackingFunc(cv::Mat & frame)
{
    long curTime;

    if(true == bTrackerStart)
    {
        //qDebug()<<"boxChanged = " << boxChanged;
        if (boxChanged == true)
        {
            qDebug()<<"init KCF tracker";
            kcfTracker->padding = 3.5;   //4.5;  //2.5; //跟踪框比较小时，可以设比较大的搜索区域，相反则小，以降低计算量
            kcfTracker->sigma = 0.6;
            kcfTracker->template_size = 40;    //96;

            kcfTracker->lambda = 0.0001;
            kcfTracker->cell_size = 2;
            kcfTracker->cell_sizeQ = kcfTracker->cell_size * kcfTracker->cell_size;
            kcfTracker->output_sigma_factor = 0.15;//0.125;
            kcfTracker->interp_factor = 0.012;   //0.012;

            int px = roi_min_width/1;
            int py = roi_min_height/1;

            if(frame.cols < px) px = frame.cols;
            if(frame.rows < py) py = frame.rows;

            float xMin = px - roi_width/2;     //frame.cols/2 - roi_width/2;
            float yMin = py - roi_height/2;    //frame.rows/2 - roi_height/2;
            float width = roi_width;
            float height = roi_height;
            //qDebug()<<"xMin:"<<xMin<<"  yMin:"<<yMin<<"  width:"<<width<<"  height:"<<height;
            //cv::rectangle(frame, cv::Rect(xMin, yMin, width, height), cv::Scalar(255, 0, 0), 2, 1);
            kcfTracker->init( cv::Rect(xMin, yMin, width, height), frame );
            boxChanged = false;
        }

        bbox = kcfTracker->update(frame);
        //bbox.x = bbox.x + bbox.width/2;
        //bbox.y = bbox.y + bbox.height/2;
        //qDebug()<<"peak_value:"<<kcfTracker->peak_value;
        if(kcfTracker->peak_value < 0.10)
        {
            curTime = time(NULL);
            //qDebug()<<"curTime:"<<curTime<<"    gPreTime:"<<gPreTime<<" diff:"<<curTime - gPreTime;
            if(gPreTime == 0)
            {
                gPreTime = curTime;
            }
            else if(curTime - gPreTime > 1 )
            {
                qDebug()<<"lost";
                bbox.x = 0;
                bbox.y = -1;
                roi_num = 0;
                emit sig_UpdateTrackingBox(roi_num,bbox.x,bbox.y,bbox.width,bbox.height);
                bTrackerStart = false;
                gPreTime= 0;
                goto ENDOFTRK;
            }

        }
        // 绘制目标位置
        //cv::rectangle(frame, bbox, cv::Scalar(255, 0, 0), 2, 1);
        //cv::imshow("roi",frame);
        roi_num = 1;
        fps = calcFps();
        emit sig_UpdateTrackingBox(roi_num,bbox.x,bbox.y,bbox.width,bbox.height);
        //qDebug()<<"fps:"<<fps;
    }
    else
    {
        gPreTime = 0;
        //bbox.x = gMouse.posX;
        //bbox.y = gMouse.posY;
        bbox.width= roi_width;
        bbox.height = roi_height;
        boxChanged = true;
    }
ENDOFTRK:

    return 0;
}

/*
int TrackerThread::dlcmTrackingFunc(cv::Mat & frame)
{
    bbox = dlcmTracker->DLcmFunc(frame);
    fps = calcFps();
    emit sig_UpdateTrackingBox(bbox.x,bbox.y,bbox.width,bbox.height);
    return 0;
}

*/
int TrackerThread::thTrackingFunc(cv::Mat & frame)
{
    bbox = thTracker->tophatFunc(frame);
    if(bbox.x > SEARCHWINDOWX && bbox.y > SEARCHWINDOWY && bbox.x < SEARCHWINDOWX+SEARCHWINDOWW && bbox.y < SEARCHWINDOWY+SEARCHWINDOWH )
        roi_num = 1;
    else
        roi_num = 0;
    fps = calcFps();
    emit sig_UpdateTrackingBox(roi_num,bbox.x-bbox.width/2,bbox.y-bbox.height/2,bbox.width,bbox.height);
    return 0;
}

//算法2修改为新的检测算法
int TrackerThread::thTrackingFunc706(cv::Mat & frame)
{
    cv::Mat grayFrame;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
    } else {
        grayFrame = frame.clone();
    }
    cv::Mat img8;
    if (grayFrame.depth() != CV_8U) {
        grayFrame.convertTo(img8, CV_8U);
    } else {
        img8 = grayFrame;}
    cv::Rect objbox;
    irDetector->detect(img8,irParams.tarLmax, irParams.k1, irParams.k2);

#if 1  //输出得分值最高的目标
    float tar_sco = 0;
    int min_id = -1;
    for (int i = 0; i < irDetector->m_targetRects.size(); i++)
    {
        float score = computeScore(img8, irDetector->m_targetRects[i]);

        int cy = irDetector->m_targetRects[i].y + irDetector->m_targetRects[i].height / 2;
        if (score > tar_sco && 50<cy && cy < 1030) {
            tar_sco = score;
            min_id = i;
        }
    }
#endif

#if 0  //输出目标并判断可信目标 距离中心最近的目标
    float min_dis = 65536;
    int min_id = -1;
    int c_x = 1920/2;
    int c_y = 1080/2;
    for (int i = 0; i < irDetector->m_targetRects.size(); i++)
    {
        cv::Point2f objcen = cv::Point2f(irDetector->m_targetRects[i].x+ irDetector->m_targetRects[i].width/2,
                                         irDetector->m_targetRects[i].y+ irDetector->m_targetRects[i].height/2);
        float dis = sqrt((objcen.x - c_x) * (objcen.x - c_x) +
                         (objcen.y - c_y) * (objcen.y - c_y));
        if (min_dis > dis)
        {
            min_dis = dis;
            min_id = i;
        }
    }
#endif
    qDebug()<<"*****id:"<<min_id;
    if (min_id > -1)
    {
        objbox=cv::Rect(irDetector->m_targetRects[min_id].x,irDetector->m_targetRects[min_id].y,
                                   irDetector->m_targetRects[min_id].width,irDetector->m_targetRects[min_id].height);
        qDebug()<<"**x:"<<irDetector->m_targetRects[min_id].x<<"**y:"<<irDetector->m_targetRects[min_id].y<<"**width:"<<irDetector->m_targetRects[min_id].width<<"**height:"<<irDetector->m_targetRects[min_id].height;
    }

    if (objbox.width > 0 && objbox.height > 0) {
        roi_num = 1;
        bbox = objbox;
        emit sig_UpdateTrackingBox(roi_num, bbox.x, bbox.y, bbox.width, bbox.height);}
        return 0;
}

//算法2使用的得分策略
float TrackerThread::computeScore(cv::Mat img8uc1, cv::Rect rect)
{
    float score = 0;

    cv::Rect outerRect;
    outerRect.x = rect.x - rect.width/2;
    outerRect.y = rect.y - rect.height/2;
    outerRect.width = rect.width * 2;
    outerRect.height = rect.height * 2;
    if (outerRect.x<0 || outerRect.y<0 || outerRect.x + outerRect.width>img8uc1.cols || outerRect.y + outerRect.height>img8uc1.rows)
    {
        return 0;
    }


    cv::Mat innerPatch = img8uc1(rect).clone();
    cv::Scalar sumInner = cv::sum(innerPatch);
    double sumInnerVal = sumInner.val[0];
    float meanInnerVal = sumInnerVal / rect.area();

    cv::Mat outerPatch = img8uc1(outerRect).clone();

    cv::Scalar sumOuter = cv::sum(outerPatch);
    double sumOuterVal = sumOuter.val[0];
    float meanOuterVal = (sumOuterVal- sumInnerVal) / (outerRect.area()-rect.area());


    score = (meanOuterVal - meanInnerVal)/ meanOuterVal;


    return score;
}

void TrackerThread::initOpenCVTracker()
{
    TrackerKCF::Params param;
        param.desc_pca = TrackerKCF::GRAY | TrackerKCF::CN;
        param.desc_npca = 0;
        param.compress_feature = true;
        param.compressed_size = 2;
        //! [param]

        //Ê¹ÓÃTrackerMIL¸ú×Ù
        //Ptr<TrackerMIL> tracker= TrackerMIL::create();
        //Ptr<TrackerTLD> tracker= TrackerTLD::create();
        //Ptr<TrackerKCF> tracker = TrackerKCF::create(param);
        //opencvtracker = TrackerMedianFlow::create();
        //Ptr<TrackerBoosting> tracker = TrackerBoosting::create();
        //Ptr<TrackerMOSSE> tracker = TrackerMOSSE::create();
        //Ptr<TrackerCSRT> tracker = TrackerCSRT::create();
        //Ptr<TrackerGOTURN> tracker = TrackerGOTURN::create();

        //capture.read(frame);
        //tracker->init(frame, bbox);
}
void TrackerThread::opencvTracker(cv::Mat & frame)
{
    long curTime;
    Rect2d lbox;
    //Rect kcfrect;

    if(true == bTrackerStart)
    {
        //qDebug()<<"boxChanged = " << boxChanged;
        if (boxChanged == true)
        {
            qDebug()<<"init opencv tracker";

            // 计算初始跟踪框
            int px = roi_min_width/1;
            int py = roi_min_height/1;
            if(frame.cols < px) px = frame.cols;
            if(frame.rows < py) py = frame.rows;

            float xMin = px;// - roi_width/2;     //frame.cols/2 - roi_width/2;
            float yMin = py;// - roi_height/2;    //frame.rows/2 - roi_height/2;
            float width = roi_width;
            float height = roi_height;
            //qDebug()<<"xMin:"<<xMin<<"  yMin:"<<yMin<<"  width:"<<width<<"  height:"<<height;
            //cv::rectangle(frame, cv::Rect(xMin, yMin, width, height), cv::Scalar(255, 0, 0), 2, 1);
            //opencvtracker = TrackerMIL::create();
#if 0
            TrackerKCF::Params param;
            param.desc_pca = TrackerKCF::GRAY | TrackerKCF::CN;
            param.desc_npca = 0;
            param.compress_feature = true;
            param.compressed_size = 2;
            param.max_patch_size = 3;
            /*param.sigma = 0.6;
            param.lambda = 0.0001;
            param.resize = true;
            param.output_sigma_factor = 0.15; */

            opencvtracker = TrackerKCF::create();

            lbox=selectROI(frame);
            lbox.x=xMin;
            lbox.y=yMin;
            lbox.width=width;
            lbox.height=yMin;

            //opencvtracker->init(frame, kcfrect);
            opencvtracker->init(frame, lbox);
#endif

#if 1
            // CSRT参数优化
            TrackerCSRT::Params param;
            param.padding = 4;
            param.use_hog = true;
            param.scale_step = 0.9;
            param.use_rgb = false;
            param.psr_threshold = 0.02;  // lost the target, if the psr is lower than this.
            opencvtracker = TrackerCSRT::create(param);
#endif
#if 0
            TrackerBoosting::Params param;
            param.iterationInit =1;
            //param.numClassifiers = 1;
            param.samplerOverlap = 10;
            //param.samplerSearchFactor = 1;
            //param.featureSetNumFeatures = 1;
            opencvtracker = TrackerBoosting::create(param);
#endif
            //opencvtracker = TrackerTLD::create();
            //opencvtracker = TrackerMedianFlow::create();
            //opencvtracker = TrackerBoosting::create();
            //opencvtracker = TrackerMOSSE::create();
            //opencvtracker = TrackerCSRT::create();
            // 初始化跟踪器
            opencvtracker->init(frame, cv::Rect(xMin, yMin, width, height));
            boxChanged = false;
        }

        // 执行跟踪更新
        bool retu =opencvtracker->update(frame, lbox);
        //opencvtracker->update(frame, kcfrect);

        // 绘制目标位置
        /*cv::rectangle(frame, lbox, cv::Scalar(255, 0, 0), 2, 1);
        cv::imshow("roi",frame); */
        if(true == retu)
        {
            roi_num = 1;  // 跟踪成功
        }
        else
        {
            roi_num = 0;
        }
        fps = calcFps();
        // 发送跟踪结果到主线程
        emit sig_UpdateTrackingBox(roi_num,lbox.x,lbox.y,lbox.width,lbox.height);
    }
    else
    {
        gPreTime = 0;
        //bbox.x = gMouse.posX;
        //bbox.y = gMouse.posY;
        bbox.width= roi_width;
        bbox.height = roi_height;
        boxChanged = true;
        opencvtracker.release();
    }
ENDOFTRK:


    fps = calcFps();

    return;
}

int TrackerThread::yoloDetectFunc()
{
    //将图片加载到共享内存
    QBuffer buffer;

    if(qImg.width() == 1920)
    {
        qImg = qImg.scaled(480,270, Qt::KeepAspectRatio);
    }
    //QImage imgMono = GraizeImage(qImg);

    buffer.open(QBuffer::ReadWrite);    //将图片暂存到buffer中
    QDataStream out(&buffer);       //获取图片数据的指针
    out<<qImg;

    int size = buffer.size();       //获取图片的大小
    //qDebug()<<"buf size:"<<size;
    //在共享内存段的操作时，需要先加锁
    sharedMemory.lock();
    char * to = (char*)sharedMemory.data();
    const char * from = buffer.data().data();
    memcpy(to,from, qMin(sharedMemory.size(),size));
    //解锁
    sharedMemory.unlock();
    m_semaphore->release();


    m_semaphoreRetu->acquire();     //等待返回检测结果
    //qDebug()<<"Retu acquire:";

    if(!sharedMemoryRetu.attach())
    {
        //ui->label->setText(tr("无法连接到共享内存段，请先加载一张图片！"));
        qDebug()<<"-->Return attach failed";
        return -1;
    }

    QBuffer bufferRetu;
    QDataStream in(&bufferRetu);

    sharedMemoryRetu.lock();
    bufferRetu.setData((char*)sharedMemoryRetu.constData(),sharedMemoryRetu.size());    //读取Return内存段中的数据
    bufferRetu.open(QBuffer::ReadOnly);
    in>> mDRetu.id;
    in>> mDRetu.x;
    in>> mDRetu.y;
    in>> mDRetu.w;
    in>> mDRetu.h;
    in>> mDRetu.prob;

    sharedMemoryRetu.unlock();
    sharedMemoryRetu.detach();

    //qDebug()<<"id:"<<mDRetu.id<<"   x:"<<mDRetu.x<<"    y:"<<mDRetu.y<<"    w:"<<mDRetu.w<<"    h:"<<mDRetu.h<<"    prob:"<<mDRetu.prob;
    if(mDRetu.prob>0.1)
    {
        roi_num = 1;

    }
    else
    {
        roi_num = 0;
    }
    emit sig_UpdateDetectingBox(roi_num, mDRetu.id, mDRetu.x, mDRetu.y, mDRetu.w, mDRetu.h, mDRetu.prob);
/*
    cv::Scalar color = {255,255,255} ;
    cv::rectangle(frame, cv::Rect(mDRetu.x, mDRetu.y, mDRetu.w, mDRetu.h), color, 1);

    cv::Scalar colorRed = {0,0,255} ;
    cv::rectangle(frame, cv::Rect(mDRetu.x + mDRetu.w/2-1, mDRetu.y + mDRetu.h/2-1, 2,2), colorRed, 2);

    cv::namedWindow("test", CV_WINDOW_NORMAL);
    cv::imshow("test", frame);
    cv::waitKey(1);*/
    fps = calcFps();
    return 0;
}

/* 通过 TCP socket 方式传输一帧图*/
int TrackerThread::yoloDetectFunc2()
{
    /*if(qImg.width() == 1920)
    {
        qImg = qImg.scaled(480,270, Qt::KeepAspectRatio);
    } */
    sendOneImage(qImg);
    return 0;
}
void TrackerThread::run() // 线程入口函数
{
    cv::Mat mat;
    //cv::Mat matSmall=cv::Mat::zeros(540,960,CV_8UC3);
    qDebug()<<"starting TrackerThread()";

    QElapsedTimer algorithmTimer;
    algorithmTimer.start();

    while(bRunflag)
    {
        {

        // 线程同步：等待新图像
        pthread_mutex_lock(&gTrackingMutex);
        pthread_cond_wait(&gTrackingTransImg, &gTrackingMutex);  // 阻塞等待新图像
        //qDebug()<<"convert img to mat"<<"   width:" <<qImg.width()<<"   height:"<<qImg.height() << "    img format:"<<qImg.format();
        // QImage → cv::Mat转换

        mat = imageCvtMat();

        pthread_mutex_unlock(&gTrackingMutex);
        //qDebug()<<"trackingFunc()";
        //resize(mat,matSmall,matSmall.size());

        // 算法处理开始
        qint64 algorithmStartTime = algorithmTimer.elapsed();

        // 图像预处理
        cv::Mat median;
        cv::medianBlur(mat,median, 5); // 中值滤波降噪
        //cv::imshow("median",median);

        // 算法分发
        if(trackingMethod == TRACKING_METHOD_KCF)
        {
            //kcfTrackingFunc(median);
            opencvTracker(median);
        }
        else if(trackingMethod == TRACKING_METHOD_DETECT)
        {
            yoloDetectFunc();
        }
        else if(trackingMethod == TRACKING_METHOD_TOPHAT)
        {
            thTrackingFunc(median);
        }
        else if(trackingMethod == TRACKING_METHOD_TOPHAT706)
        {
            thTrackingFunc706(median);
        }
        else if(trackingMethod == TRACKING_METHOD_IR_SMALL) {  // 新增
            //706测延时
            // QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
            // qDebug() << "进入跟踪算法:" << timestamp;
            irSmallTargetDetection(median);
        }
        else if(trackingMethod == TRACKING_METHOD_NONE) {
            emit sig_UpdateTrackingBox(0,0,0,0, 0);
        }

        qint64 algorithmEndTime = algorithmTimer.elapsed();
        qint64 algorithmDelay = algorithmEndTime - algorithmStartTime;

        static int algorithmFrameCount = 0;
        static qint64 totalAlgorithmDelay = 0;

        // algorithmFrameCount++;
        // totalAlgorithmDelay += algorithmDelay;
//        if(algorithmFrameCount % 30 == 0)
//        {
//            qDebug() << QString("=== 跟踪算法延时统计 (帧#%1) ===").arg(algorithmFrameCount);
//            qDebug() << QString("算法处理延时: %1 ms (方法:%2)").arg(algorithmDelay).arg(trackingMethod);
//            qDebug() << QString("平均算法延时: %1 ms").arg(totalAlgorithmDelay / algorithmFrameCount);
//            qDebug() << "===============================";
//        }

        /*else if(trackingMethod == TRACKING_METHOD_CENTROIDWH)
        {
             WhCentroTrackingFunc(median, 100, 100, median.cols-200 , median.rows-200);
        }
        else if(trackingMethod == TRACKING_METHOD_CENTROIDBLK)
        {
            BlkCentroTrackingFunc(median,  100, 100, median.cols-200 , median.rows-200);
        } */
        }
    }
}

int TrackerThread::calcFps()
{
    ++frameCount;

    int cTime= time(NULL);
    if(cTime - lastTime >=1)
    {
        fps=frameCount;
        frameCount=0;
        lastTime=cTime;
    }
    return fps;
}

//原文链接：https://blog.csdn.net/qq_44464101/article/details/128247822
cv::Mat TrackerThread::imageCvtMat()

{
    QImage inImage = qImg;
    switch ( inImage.format() )
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return  mat.clone();
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {

        cv::Mat  mat( inImage.height(), inImage.width(),
                  CV_8UC4,
                  const_cast<uchar*>(inImage.bits()),
                  static_cast<size_t>(inImage.bytesPerLine())
                  );

        cv::Mat  matNoAlpha;

        cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        QImage   swapped = inImage.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                    CV_8UC3,
                    const_cast<uchar*>(swapped.bits()),
                    static_cast<size_t>(swapped.bytesPerLine())
                    ).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Grayscale8:
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                  CV_8UC1,
                  const_cast<uchar*>(inImage.bits()),
                  static_cast<size_t>(inImage.bytesPerLine())
                  );

        return mat.clone() ;
    }
    default:
        qDebug() << "没有专门所属类型，请完善！！！" << inImage.format()<<endl;
        break;
    }

    return cv::Mat();

}


//===========================形心跟踪==================================
// 计算图像中目标的形心
int get_pixel(cv::Mat & img, cv::Point pt)
{
    int width = img.cols; //图片宽度
    int height = img.rows; //图片宽度t;//图片高度
    // x 是 列 ， y 是 行
    uchar * ptr = (uchar *) img.data + pt.y * width; //获得灰度值数据指针
    int intensity = ptr[pt.x];
    return intensity;
}


cv::Rect TrackerThread::WhCentroTrackingFunc(cv::Mat & inframe, int x, int y, int width, int height)
{
    QImage inImage = qImg;
    QImage gray_image;
    gray_image = inImage.convertToFormat(QImage::Format_Grayscale8,Qt::AutoColor);

    int i, j;
    int sum_x = 0, sum_y = 0, total = 1;

    uchar * val;
    cv::Mat frame = inframe.clone();
#ifdef _WIN32
    cvtColor(frame, frame, CV_RGB2GRAY);
#elif OS_KYLIN
	cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
#endif

    // 遍历目标区域
    for (i = y; i < y + height; i++)
    {
        val = gray_image.scanLine(i);
        for (j = x; j < x + width; j++)
        {
            uchar v = val[j];
            if (v > mCentroidWhMax)
            {
                sum_x += j;
                sum_y += i;
                total++;
            }
        }
    }

    bbox.x = (int)(sum_x / total);
    bbox.y = (int)(sum_y / total);
    bbox.width = 10;
    bbox.height = 10;

    /*cv::Scalar clr = cv::Scalar(255, 255, 255);
    rectangle(frame, cv::Point(bbox.x, bbox.y), cv::Point(bbox.x + bbox.width, bbox.y + bbox.height), clr);
    cv::imshow("center",frame);*/

    if(bbox.x > 0 && bbox.y >0 && bbox.x < frame.cols && bbox.y < frame.rows ) roi_num = 1;
    else roi_num = 0;
    fps = calcFps();
    emit sig_UpdateTrackingBox(roi_num,bbox.x,bbox.y,bbox.width,bbox.height);
    return bbox;
}

cv::Rect TrackerThread::BlkCentroTrackingFunc(cv::Mat & inframe, int x, int y, int width, int height)
{
    QImage inImage = qImg;
    QImage gray_image;
    gray_image = inImage.convertToFormat(QImage::Format_Grayscale8,Qt::AutoColor);

    int i, j;
    int sum_x = 0, sum_y = 0, total = 1;

    uchar * val;
    cv::Mat frame = inframe.clone();
#ifdef _WIN32
    cvtColor(frame, frame, CV_RGB2GRAY);
#elif OS_KYLIN
	cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
#endif
	
    // 遍历目标区域
    for (i = y; i < y + height; i++)
    {
        val = gray_image.scanLine(i);
        for (j = x; j < x + width; j++)
        {
            uchar v = val[j];
            if (v < mCentroidBlkMin)
            {
                sum_x += j;
                sum_y += i;
                total++;
            }
        }
    }
    bbox.x = (int)(sum_x / total);
    bbox.y = (int)(sum_y / total);
    bbox.width = 10;
    bbox.height = 10;

    /*cv::Scalar clr = cv::Scalar(255, 255, 255);
    rectangle(frame, cv::Point(bbox.x, bbox.y), cv::Point(bbox.x + 10, bbox.y + 10), clr);
    //cv::imshow("center",frame);*/
    if(bbox.x > 0 && bbox.y >0 && bbox.x < frame.cols && bbox.y < frame.rows ) roi_num = 1;
    else roi_num = 0;
    fps = calcFps();
    emit sig_UpdateTrackingBox(roi_num,bbox.x,bbox.y,bbox.width,bbox.height);
    return bbox;
}

int TrackerThread::sendOneImage(QImage img)
{
    //mLastTime = QTime::currentTime();

    int imgDataSize = img.width() * img.height();

    int sendLastSize= imgDataSize%(TCPBUFSIZE);
    int sendCnt = imgDataSize/(TCPBUFSIZE)+ (sendLastSize > 0 ? 1 : 0);
    sendCnt = sendCnt - 1;
    QByteArray buf64k(TCPBUFSIZE+24,0);
    FileDate fileData;
    fileData.type = TRANSFILEDATA;
    fileData.readCnt = sendCnt;
    fileData.lastSize = sendLastSize;
    //qDebug()<<"imgDataSize:"<<imgDataSize <<" readCnt:"<<fileData.readCnt<<" lastSize:"<<fileData.lastSize;

    QImage imgMono = img;//GraizeImage(img);

    for(int i=0;i<sendCnt;i++)
    {
        fileData.flowNo = i;
        memcpy(fileData.data, imgMono.bits()+i*TCPBUFSIZE,TCPBUFSIZE);
        memcpy(buf64k.data(),&fileData,sizeof(fileData));   //把结构体存入数组
        tcpSocket->write(buf64k, buf64k.size());
        //tcpSocket->waitForBytesWritten(1000);
        QThread::usleep(2000);
        //QThread::msleep(10);
    }
    fileData.flowNo = sendCnt;
    memcpy(fileData.data, imgMono.bits() + sendCnt * TCPBUFSIZE, sendLastSize);
    memcpy(buf64k.data(),&fileData,sizeof(fileData));   //把结构体存入数组
    int ret = tcpSocket->write(buf64k, sendLastSize + 16);
    qDebug()<<"sendLastSize + 16:"<<sendLastSize + 16   <<" write:"<<ret;

   /* tcpSocket->waitForBytesWritten(); */
    return 0;
}


void TrackerThread::socket_Read_Data()
{
    QByteArray buffer;
    buffer = tcpSocket->readAll();
    DetectResult retuData;
    memcpy(&retuData, buffer.data(), sizeof(retuData));//转化到结构体
    qDebug()<<"id:"<<retuData.id<<"  x:"<<retuData.x<<"   y:"<<retuData.y<<"    w:"<<retuData.w<<"   h:"<<retuData.h<<"   prob:"<<retuData.prob;
    if(mDRetu.prob>0.1)
    {
        roi_num = 1;

    }
    else
    {
        roi_num = 0;
    }
    emit sig_UpdateDetectingBox(roi_num, mDRetu.id, mDRetu.x, mDRetu.y, mDRetu.w, mDRetu.h, mDRetu.prob);
}

int TrackerThread::openTcp()
{
    tcpSocket = new QTcpSocket(this);
    //QObject::connect(tcpSocket,&QTcpSocket::connected,this,&MainWindow::socket_Read_Data);
    QObject::connect(tcpSocket,&QTcpSocket::readyRead,this,&TrackerThread::socket_Read_Data);
    //与服务器连接
    tcpSocket->connectToHost("127.0.0.1",10088);


    return 0;
}

QImage TrackerThread::GraizeImage( QImage& image )
{
    int w =image.width();
    int h = image.height();
    QImage iGray(w,h, QImage::Format_Indexed8);
    int step = iGray.bytesPerLine();
    uchar * data= iGray.bits();
    for(int i=0; i<h;i++)
    {
        for(int j=0; j<w;j++)
        {
            QRgb pixel = image.pixel(j,i);
            unsigned char gray = qGray(pixel);
            data[i * step + j ] = gray;
        }
    }
    return iGray;
}
