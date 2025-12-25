#ifndef HIDTHREAD_H
#define HIDTHREAD_H

#include <QtCore/QCoreApplication>
#include <QThread>
#include <QDebug>

#include <QPainter>
#include <synchapi.h>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QDirModel>
#include <QDateTime>
#include <QtCore/QList>
#include <QTextStream>
#include <QMessageBox>

#include <iostream>
#include <stdint.h>
#include <QReadWriteLock>
#include "HidApi.h"

class HidProcThd : public QThread
{
    Q_OBJECT
public:
    HidProcThd();
    ~HidProcThd();
    int panVal;
    int tiltVal;
    int btnVal;
    int btnVal2;
    int axialVal;
    bool runflag;
    int hidCount;

    //int speedGear;


signals:
    void sig_UpdateJoystick();

protected:
    void run(); // 线程入口函数
    void hidloop();

    HidDevice kbc312;


};

#endif // HIDTHREAD_H
