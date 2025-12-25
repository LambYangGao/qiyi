#include "pod_cmd.h"

pod_cmd::pod_cmd()
{

    memset(cmd_blank,0,sizeof(cmd_blank));
    cmd_blank[0] = SYNCSH;
    cmd_blank[1] = 0;

}


uint8_t * pod_cmd::crtSteplessCmd(int offsetx,int offsety)
{
    uint8_t * cmd;
    cmd = cmd_blank;
    cmd[1] = PTZ_SH_OFFSET;
    cmd[2] = (offsetx>>8) & 0xff;
    cmd[3] = offsetx & 0xff;
    cmd[4] = (offsety>>8) & 0xff;
    cmd[5] = offsety & 0xff;
    cmd[6] = 0xff & ((uint)cmd[1] + (uint)cmd[2] + (uint)cmd[3] + (uint)cmd[4] + (uint)cmd[5]);
    //qDebug()<<"cmd[2]:"<<cmd[2] <<" "<<cmd[3];
    /*
    QByteArray mBuf;
    mBuf.resize(7);
    memcpy(mBuf.data(),cmd, 7);
    qDebug()<<mBuf.toHex(' '); */
    return cmd;
}

uint8_t * pod_cmd::crtOffsetCmd(int offsetXDeg,int offsetYDeg)
{
    uint8_t * cmd;
    cmd = cmd_blank;
    cmd[1] = PTZ_SH_OFFSETDEG;
    cmd[2] = (offsetXDeg>>8) & 0xff;
    cmd[3] = offsetXDeg & 0xff;
    cmd[4] = (offsetYDeg>>8) & 0xff;
    cmd[5] = offsetYDeg & 0xff;
    cmd[6] = 0xff & ((uint)cmd[1] + (uint)cmd[2] + (uint)cmd[3] + (uint)cmd[4] + (uint)cmd[5]);
    //qDebug()<<"cmd[2]:"<<cmd[2] <<" "<<cmd[3];
    /*
    QByteArray mBuf;
    mBuf.resize(7);
    memcpy(mBuf.data(),cmd, 7);
    qDebug()<<mBuf.toHex(' '); */
    return cmd;
}


