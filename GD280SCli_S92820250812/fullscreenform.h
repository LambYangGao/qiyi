#ifndef FULLSCREENFORM_H
#define FULLSCREENFORM_H

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QTime>

#include <QDebug>


namespace Ui {
class FullScreenForm;
}

class FullScreenForm : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenForm(QWidget *parent = nullptr);
    ~FullScreenForm();
    QImage mImageCh1;                  //记录当前的图像
    bool mImageCh1GetNew;              //通道1图像更新标记
    QString OSDText;
    QTime OsdTime;
    double OsdA;
    double OsdE;
    int RelativeTime;
    int num_frame;

    unsigned int channelNo;
    bool recflag;
    QString recDir;

    bool replayflag;
    //bool replaying;
    QString srtFilename;

    QString ParamText;
    int lastFilePtr;
    int frameNum;
    bool mdispOSDTime;
    QString OSDFont;
    int OSDFontHeight;
    int OSDLocationY;


private:
    Ui::FullScreenForm *ui;



protected:
    virtual void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sig_backToMainWin();
};

#endif // FULLSCREENFORM_H
