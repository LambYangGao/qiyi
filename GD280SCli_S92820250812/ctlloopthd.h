#ifndef CTLLOOPTHD_H
#define CTLLOOPTHD_H
#include <QtCore/QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStandardItem>
#include <QStandardItemModel>
#include <iostream>

class ctlLoopThd:public QThread
{
    Q_OBJECT
public:
    ctlLoopThd();
    ~ctlLoopThd();

    bool bRunflag;

signals:
    void sig_UpdateWorkState(int,int);

protected:
    void run(); // 线程入口函数
};

#endif // CTLLOOPTHD_H
