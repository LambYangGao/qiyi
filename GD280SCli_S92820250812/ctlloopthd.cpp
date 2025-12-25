#include "ctlloopthd.h"

ctlLoopThd::ctlLoopThd()
{
    bRunflag = true;
}

ctlLoopThd::~ctlLoopThd()
{
    qDebug()<<"quit from ctlLoopThd()";
    quit();
    wait();
}


void ctlLoopThd::run() // 线程入口函数
{
    while(bRunflag)
    {

    }
}
