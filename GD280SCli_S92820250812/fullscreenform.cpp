#include "fullscreenform.h"
#include "ui_fullscreenform.h"

FullScreenForm::FullScreenForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenForm)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
   // this->grabKeyboard();
    replayflag = false;
    mdispOSDTime = false;
    OSDFont="黑体";
    OSDFontHeight=40;
    OSDLocationY = 950;
}

FullScreenForm::~FullScreenForm()
{
    delete ui;
    this->close();
}


void FullScreenForm::paintEvent(QPaintEvent *event)
{

    QImage img;
    QPixmap fitPixmap;


    if(true== mImageCh1GetNew)
    {
        //将图像按比例缩放成和小窗口一样大小
        img = mImageCh1.scaled(ui->lblfullPic->size(), Qt::KeepAspectRatio);
        //QImage image = mImage.mirrored(false, true);
        fitPixmap=QPixmap::fromImage(img);

        QFont font(OSDFont,OSDFontHeight);
        //设置显示字体的大小
        //font.setPixelSize(40);
        QPainter painter(&fitPixmap);
        //painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        QPen pen = painter.pen();
        pen.setColor( QColor(255, 255, 255,255));
        painter.setPen(pen);
        painter.setFont(font);
        //painter.drawText(fitPixmap.rect(), Qt::AlignCenter, imageText);
        //painter.drawText(100,1030, OSDText);
        if(false == replayflag)
        {
            ParamText =OSDText;
            if(mdispOSDTime)
            {
                ParamText = ParamText + "  " + QString("%1").arg(OsdTime.hour(), 2,10,QLatin1Char('0'))+":" + \
                        QString("%1").arg(OsdTime.minute(), 2,10,QLatin1Char('0'))+":" + \
                        QString("%1").arg(OsdTime.second(), 2,10,QLatin1Char('0'));
                /*ParamText = ParamText + " A:" + QString::number(OsdA, 'f',3)+"° E:" + QString::number(OsdE, 'f',3)+"° ";
                    ParamText = ParamText + "T:" + QString::number(RelativeTime/1000, 10) + "s";*/
            }


        }
        else
        {
            if(num_frame+6 >= frameNum)
            {
                QFile file(srtFilename);

                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream stream(&file);
                    stream.seek(lastFilePtr);   //移动到上次位置
                    QString  line;
                    int pos=0;
                    for(int i=0;i<5;i++)
                    {
                        line = file.readLine();
                        lastFilePtr += line.size();
                        pos = line.indexOf("->");
                        if(pos!= -1) break;
                    }
                    QString strFrameNum=line.section("->",-1);
                    frameNum=strFrameNum.toInt();
                    if(num_frame < frameNum)
                    {
                        ParamText = file.readLine();
                        lastFilePtr += ParamText.size();
                    }
                    file.close();
                }
            }
        }
        painter.drawText(QRectF(0, OSDLocationY, fitPixmap.width(), OSDFontHeight+8), Qt::AlignCenter, ParamText);//居中显示 1020
        ui->lblfullPic->setPixmap(fitPixmap);
        mImageCh1GetNew = false;
    }

}

void FullScreenForm::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        emit sig_backToMainWin();
        return;
    }
    QWidget::keyReleaseEvent(event);
}
