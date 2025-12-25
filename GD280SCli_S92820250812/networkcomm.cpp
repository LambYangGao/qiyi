#include "networkcomm.h"
#include "CoordTranser.h"

//4.1指挥命令(0xEa/0x40)（FW2发送到光电经纬仪）
unsigned char cmd_cmd[] = { CTRHEADER, 0x00, 0x01, 0x07, 0x01, 0x2a, 0xe8, 0xea, 0x40, 0x08
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00};

//4.2点目标分配数据段格式（RAE）
unsigned char cmd_target_rae[] = { CTRHEADER, 0x00, 0x01, 0x07, 0x01, 0x2a, 0xe8, 0xea, 0x1e, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00};

//4.3指挥命令回复(0xEa/0x41) （光电经纬仪发送到FW2）
unsigned char cmd_cmd_reply[] = { CTRHEADER, 0x07, 0x01, 0x00, 0x01, 0x2a, 0xe8, 0xea, 0x41, 0x0f
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00};

//4.4心跳状态信息（含装备位置,0xEa/0x05）（光电经纬仪发送到FW2）
unsigned char cmd_heartbeat[] = { CTRHEADER, 0x07, 0x01, 0x49, 0x01, 0x2a, 0xe8, 0xea, 0x05, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                            ,0x00, 0x00};
extern QReadWriteLock netBufLock;

NetworkComm::NetworkComm(int role)
{
    ctrConnectFlag = false;
    pitchVal=0;
    directionVal=0;
    recvPacketErrCnt=0;
    recvPacketCnt=0;
    sendHeartbeatCnt=0;
    sendZhiKongPktCnt = 0;
    recvZhiKongPktCnt = 0;

    sendServoPktCnt = 0;
    servo422ConnectFlag = false;

    if(0 == role)
    {
        cmd_heartbeat[4] = 0x01;    //光电球机设备号0x4901
    }
    else if(1 == role)
    {
        cmd_heartbeat[4] = 0x02;
    }
    //timerHeartBeat = new QTimer();
    //connect(timerHeartBeat,SIGNAL(timeout()),this,SLOT(heartBeat_timeOut()));
    //timerHeartBeat->start(50);
    initToZhiKongPacket();
    initToServoPkt422();
    memset(&msgFromZhiKong,0,sizeof(msgFromZhiKong));
}

/*
int NetworkComm::connectCenter()
{

}
*/

// 在 networkcomm.cpp 的 centerSocket_Read_Data() 函数中的修改

int NetworkComm::centerSocket_Read_Data()
{
    int chk=0,len=0,m=0;
    QByteArray buffer;
    buffer = centerUdpSocket->readAll();
    len=buffer.length();

    qDebug()<<"******收到指控指令长度为："<<len<<"*******";

    // 1. 数据包长度校验 - 更新为新的长度
    if(len< RECVZHIKONGMSG_LEN)
    {
        //qDebug()<<"ZhiKong message len="<<buffer.length();
        recvPacketErrCnt++;
        return 50;
    }

    if( (len%RECVZHIKONGMSG_LEN) == 0 && len >=RECVZHIKONGMSG_LEN)
    {
       m = len-RECVZHIKONGMSG_LEN;
    }
    else
    {
        qDebug()<<"len err"<<len;
        recvPacketErrCnt++;
        return 100;
    }

    // 2. 报文头校验
    if(CTRHEADER != (uint8_t)buffer[m+0] || CTRHEADER2 != (uint8_t)buffer[m+1]  )
    {
        qDebug()<<"CTRHEADER != (uint8_t)buffer[0]";
        recvPacketErrCnt++;
        return 100;
    }

    // 3. CRC校验 - 更新CRC字段位置（现在在最后2个字节：buffer[m+19]和buffer[m+20]）
    chk = calcChecksum(buffer,RECVZHIKONGMSG_LEN,m);
    if((((chk>>8)&0xff) != buffer.at(m+20)) && (chk&0xff) != buffer.at(m+19))
    {
         qDebug()<<"chk err:"<<chk<<"buf[19]"<<QString::number(buffer.at(19)) <<":"<<QString::number(buffer.at(20));
         recvPacketErrCnt++;
        return 100;
    }

    recvZhiKongPktCnt++;

    // 4. 数据解析
    msgFromZhiKong.pktType = (uint8_t)buffer[m+2];
    msgFromZhiKong.cmdId = (uint8_t)buffer[m+4];
    msgFromZhiKong.startTracking = (uint8_t)buffer[m+5];
    msgFromZhiKong.suanfa = (uint8_t)buffer[m+6];

    // 后续字段位置都向后偏移1字节
    msgFromZhiKong.zkFangWei = ((buffer[m+7]<<8)& 0x0000FF00) + (buffer[m+8]&0xff);
    msgFromZhiKong.zkFuYang =  ((buffer[m+9]<<8)& 0x0000FF00) + (buffer[m+10]&0xff);
    msgFromZhiKong.zkDistence = (( (buffer[m+11]<<8)& 0x0000FF00) + (uint8_t)buffer[m+12] ) / 10;
    qDebug()<<"zkDistence:"<<msgFromZhiKong.zkDistence;
    msgFromZhiKong.zkBianBeiDuiJiao =(uint8_t)buffer[m+13];

    // 新增：鼠标坐标字段解析
    msgFromZhiKong.shubiaox = ((buffer[m+14]<<8)& 0x0000FF00) + (buffer[m+15]&0xff);
    msgFromZhiKong.shubiaoy = ((buffer[m+16]<<8)& 0x0000FF00) + (buffer[m+17]&0xff);

    // 调试输出：调试验证解析对不对
    qDebug()<<"*****Received ZhiKong data:"
           <<"cmdId:"<<msgFromZhiKong.cmdId
           <<"startTracking:"<<msgFromZhiKong.startTracking
           <<"suanfa:"<<msgFromZhiKong.suanfa
           <<"zkFangWei:"<<msgFromZhiKong.zkFangWei
           <<"zkFuYang:"<<msgFromZhiKong.zkFuYang
           <<"zkDistence:"<<msgFromZhiKong.zkDistence
           <<"shubiaox:"<<msgFromZhiKong.shubiaox
             <<"shubiaoy:"<<msgFromZhiKong.shubiaoy<<"***";


    return 0;
}

#if 0
int NetworkComm::centerSocket_Read_Data()
{
    int chk=0,len=0,m=0;
    QByteArray buffer;
    buffer = centerUdpSocket->readAll();
    len=buffer.length();
/*
    unsigned char data[] = {0x5a, 0x47, 0x00, 0x10, 0x01, 0x00, 0x72, 0x7c, 0x24, 0xfa, 0x0b, 0xb8, 0x00, 0x00, 0x81, 0x03};

    buffer.resize(16);
    for(int i=0;i<16;i++)
    {
        buffer[i]=data[i];
    }
    len = 16; */
    // 1. 数据包长度校验
    if(len< RECVZHIKONGMSG_LEN)
    {
        //qDebug()<<"ZhiKong message len="<<buffer.length();
        recvPacketErrCnt++;
        return 50;
    }

    if( (len%RECVZHIKONGMSG_LEN) == 0 && len >=RECVZHIKONGMSG_LEN)
    {
       m = len-RECVZHIKONGMSG_LEN;
    }
    else
    {
        qDebug()<<"len err"<<len;
        recvPacketErrCnt++;
        return 100;
    }
//qDebug()<<"aaa m:"<<m<<"    :"<<buffer.toHex(' ');
    // 2. 报文头校验
    if(CTRHEADER != (uint8_t)buffer[m+0] || CTRHEADER2 != (uint8_t)buffer[m+1]  )
    {
        qDebug()<<"CTRHEADER != (uint8_t)buffer[0]";
        recvPacketErrCnt++;
        return 100;
    }
//qDebug()<<"bbb crc" <<" :"<<(uint16_t)(((uint8_t)buffer.at(m+14)<<8) + (uint8_t)buffer.at(m+15));
    // 3. CRC校验
    chk = calcChecksum(buffer,RECVZHIKONGMSG_LEN,m);
    //if(chk != (uint16_t)((buffer.at(m+14)<<8) + buffer.at(m+15)))
    if((((chk>>8)&0xff) != buffer.at(m+15)) && (chk&0xff) != buffer.at(m+14))
    {
         qDebug()<<"chk err:"<<chk<<"buf[14]"<<QString::number(buffer.at(14)) <<":"<<QString::number(buffer.at(15));
         recvPacketErrCnt++;
        return 100;
    }

    recvZhiKongPktCnt++;

    // 4. 数据解析
    msgFromZhiKong.cmdId = (uint8_t)buffer[m+4];
    msgFromZhiKong.startTracking = (uint8_t)buffer[m+5];
    msgFromZhiKong.zkFangWei = ((buffer[m+6]<<8)& 0x0000FF00) + (buffer[m+7]&0xff);
    msgFromZhiKong.zkFuYang =  ((buffer[m+8]<<8)& 0x0000FF00) + (buffer[m+9]&0xff);
    msgFromZhiKong.zkDistence = (( (buffer[m+10]<<8)& 0x0000FF00) + (uint8_t)buffer[m+11] ) / 10;
    msgFromZhiKong.zkBianBeiDuiJiao =(uint8_t)buffer[m+12];

//qDebug()<<"cmdId:"<<msgFromZhiKong.cmdId <<"    zkFangWei:"<<msgFromZhiKong.zkFangWei<<" zkFuYang:"<<msgFromZhiKong.zkFuYang <<" zkDistence:"<<msgFromZhiKong.zkDistence;
    return 0;
}
#endif

/*
 * 惯导数据：EB 90 76 00 AF 1F B8 FF 50 CA 4C 00 18 29 B6 FF 28 28 D5 FB 80 18 CB 18 63 F6 75 FF 00 00 00 70 B8 27 00 A3 2C C4 30 E5 44 40 4D C4 53 06 00 00 00 08 00 08 00 02 00 00 BF BB
 */
void NetworkComm::gyroSocket_Read_Data()
{
    int chk=0,len=0,m=0;
    QByteArray buffer;
    buffer = centerUdpSocket->readAll();
    len=buffer.length();
    //qDebug()<<"recv Gyro message len="<<len;
/*
    unsigned char data[] = {0x5a, 0x47, 0x00, 0x10, 0x01, 0x00, 0x72, 0x7c, 0x24, 0xfa, 0x0b, 0xb8, 0x00, 0x00, 0x81, 0x03};

    buffer.resize(16);
    for(int i=0;i<16;i++)
    {
        buffer[i]=data[i];
    }
    len = 16; */

    /*unsigned char data[] = {0xEB, 0x90, 0x76, 0x00, 0xAF, 0x1F, 0xB8, 0xFF, 0x50, 0xCA, 0x4C, 0x00, 0x18, 0x29, 0xB6, 0xFF, 0x28, 0x28, \
                            0xD5, 0xFB, 0x80, 0x18, 0xCB, 0x18, 0x63, 0xF6, 0x75, 0xFF, 0x00, 0x00, 0x00, 0x70, 0xB8, 0x27, 0x00, 0xA3, \
                            0x2C, 0xC4, 0x30, 0xE5, 0x44, 0x40, 0x4D, 0xC4, 0x53, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x02, \
                            0x00, 0x00, 0xBF, 0xBB};
    buffer.resize(58);
    for(int i=0;i<58;i++)
    {
        buffer[i]=data[i];
    }
    len = 58; */

    if(len< RECVGYROMSG_LEN)
    {
        qDebug()<<"Gyro message len="<<buffer.length();
        recvPacketErrCnt++;
        return;
    }

    if( (len%RECVGYROMSG_LEN) == 0 && len >=RECVGYROMSG_LEN)
    {
       m = len - RECVGYROMSG_LEN;
    }
    else
    {
        qDebug()<<"len err"<<len;
        recvPacketErrCnt++;
        return;
    }
    // 1. 报文头校验 (0xEB 0x90 ... 0xBB)
    if(0xeb != (uint8_t)buffer[m+0] || 0x90 != (uint8_t)buffer[m+1]  || 0xbb != (uint8_t)buffer[m+57] )  //惯导报文头尾
    {
        qDebug()<<"0xeb != buffer[0]";
        recvPacketErrCnt++;
        return;
    }
    //qDebug()<<"recv Gyro message len="<<len <<" m:"<<m <<"  "<<buffer.toHex(' ');
    recvPacketCnt++;    //借用

    //int rollInt = (int)((buffer[m+16]<<0) + (buffer[m+17]<<8) + (buffer[m+18]<<16)  + (buffer[m+19]<<24));
    double dbUnit = 2147483647;

    // 2. 姿态角解析 (32位有符号整数转角度)
    msgFromGyro.roll =   (int)(((buffer[m+16]<<0)& 0x000000FF) + ((buffer[m+17]<<8)& 0x0000FF00) + ((buffer[m+18]<<16)& 0x00FF0000)  + ((buffer[m+19]<<24)& 0xFF000000)) * (60.0 / dbUnit);
    msgFromGyro.yaw =    (int)(((buffer[m+20]<<0)& 0x000000FF) + ((buffer[m+21]<<8)& 0x0000FF00) + ((buffer[m+22]<<16)& 0x00FF0000)  + ((buffer[m+23]<<24)& 0xFF000000)) * (360.0 / dbUnit);
    msgFromGyro.pitch =  (int)(((buffer[m+24]<<0)& 0x000000FF) + ((buffer[m+25]<<8)& 0x0000FF00) + ((buffer[m+26]<<16)& 0x00FF0000)  + ((buffer[m+27]<<24)& 0xFF000000)) * (60.0 / dbUnit);

    // 3. 位置信息解析
    msgFromGyro.B =  Rad2Deg((int)(((buffer[m+35]<<0)& 0x000000FF) + ((buffer[m+36]<<8)& 0x0000FF00) + ((buffer[m+37]<<16)& 0x00FF0000)  + ((buffer[m+38]<<24)& 0xFF000000)) * (PI/2.0 / dbUnit));
    msgFromGyro.L =  Rad2Deg((int)(((buffer[m+39]<<0)& 0x000000FF) + ((buffer[m+40]<<8)& 0x0000FF00) + ((buffer[m+41]<<16)& 0x00FF0000)  + ((buffer[m+42]<<24)& 0xFF000000)) * (PI / dbUnit));
    msgFromGyro.H = (((buffer[m+43]<<0)& 0x000000FF) + ((buffer[m+44]<<8)& 0x0000FF00) + ((buffer[m+45]<<16)& 0x00FF0000)  + ((buffer[m+46]<<24)& 0xFF000000)) * (0.001);
    //msgFromGyro.direction = Rad2Deg((((buffer[m+47]<<0)& 0x000000FF) + ((buffer[m+48]<<8)& 0x0000FF00)) * (2 * PI /65535.0 ));
    msgFromGyro.direction = (((buffer[m+47]<<0)& 0x000000FF) + ((buffer[m+48]<<8)& 0x0000FF00)) * (360.0 /65535.0);

    //qDebug()<<"   Gyro B:"<<(msgFromGyro.B) <<"   L:"<<(msgFromGyro.L)<<"   H:"<<msgFromGyro.H <<"   direct:"<<msgFromGyro.direction;
    //qDebug()<<"   Gyro roll:"<<QString::number(msgFromGyro.roll,'f',2) <<"    yaw:"<<msgFromGyro.yaw <<"    pitch:"<<msgFromGyro.pitch;
    return;
}
#if 0
//4.3指挥命令回复(0xEa/0x41) （光电经纬仪发送到FW2）
int NetworkComm::sendReply()
{
    if(ctrConnectFlag == false) return 100;


    int chk = calcChecksum(cmd_cmd_reply,CTRMSG_BUF_LEN);
    cmd_cmd_reply[41] = chk & 0xff;
    centerUdpSocket->write((char *)cmd_cmd_reply,CTRMSG_BUF_LEN);
    return 0;
}
#endif

int NetworkComm::sendMsg()
{
    if(ctrConnectFlag == false) return 100;

    //initToZhiKongPacket();
    //memcpy(&pktToZhiKong, &msgToZhiKong,sizeof(stPktToZhiKong));
    // 1. 数据结构转字节数组
    pktToZhiKong[0] = msgToZhiKong.sId & 0xff;
    pktToZhiKong[1] = msgToZhiKong.destId & 0xff;
    pktToZhiKong[2] = msgToZhiKong.pktType & 0xff;
    pktToZhiKong[3] = msgToZhiKong.pktLen & 0xff;
    pktToZhiKong[4] = msgToZhiKong.status & 0xff;

    pktToZhiKong[5] = (msgToZhiKong.gd1BianBei>>8) & 0xff;
    pktToZhiKong[6] = (msgToZhiKong.gd1BianBei) & 0xff;
    pktToZhiKong[7] = (msgToZhiKong.gd1DuiJiao>>8) & 0xff;
    pktToZhiKong[8] = (msgToZhiKong.gd1DuiJiao) & 0xff;
    pktToZhiKong[9] =  msgToZhiKong.gd1ZheDangFlag & 0xff;
    pktToZhiKong[10] = (msgToZhiKong.gd1FangWei>>8) & 0xff;   // 方位高字节
    pktToZhiKong[11] = (msgToZhiKong.gd1FangWei) & 0xff;      // 方位低字节
    pktToZhiKong[12] = (msgToZhiKong.gd1FuYang>>8) & 0xff;    // 俯仰高字节
    pktToZhiKong[13] = (msgToZhiKong.gd1FuYang) & 0xff;       // 俯仰低字节
    pktToZhiKong[14] =  msgToZhiKong.res & 0xff;

    pktToZhiKong[15] = (msgToZhiKong.gd2BianBei>>8) & 0xff;
    pktToZhiKong[16] = (msgToZhiKong.gd2BianBei) & 0xff;
    pktToZhiKong[17] = (msgToZhiKong.gd2DuiJiao>>8) & 0xff;
    pktToZhiKong[18] = (msgToZhiKong.gd2DuiJiao) & 0xff;
    pktToZhiKong[19] =  msgToZhiKong.gd2ZheDangFlag & 0xff;
    pktToZhiKong[20] = (msgToZhiKong.gd2FangWei>>8) & 0xff;
    pktToZhiKong[21] = (msgToZhiKong.gd2FangWei) & 0xff;
    pktToZhiKong[22] = (msgToZhiKong.gd2FuYang>>8) & 0xff;
    pktToZhiKong[23] = (msgToZhiKong.gd2FuYang) & 0xff;

    pktToZhiKong[24] = msgToZhiKong.switchTrack & 0xff;
    pktToZhiKong[25] = msgToZhiKong.numOfObj & 0xff;
    //qDebug()<<"================ num : ==========="<<pktToZhiKong[25];
    pktToZhiKong[26] = (msgToZhiKong.obj1PosX>>8) & 0xff;
    pktToZhiKong[27] = (msgToZhiKong.obj1PosX) & 0xff;
    pktToZhiKong[28] = (msgToZhiKong.obj1PosY>>8) & 0xff;
    pktToZhiKong[29] = (msgToZhiKong.obj1PosY) & 0xff;

    pktToZhiKong[30] = (msgToZhiKong.obj2PosX>>8) & 0xff;
    pktToZhiKong[31] = (msgToZhiKong.obj2PosX) & 0xff;
    pktToZhiKong[32] = (msgToZhiKong.obj2PosY>>8) & 0xff;
    pktToZhiKong[33] = (msgToZhiKong.obj2PosY) & 0xff;

    pktToZhiKong[34] = (msgToZhiKong.roiWidth) & 0xff;
    pktToZhiKong[35] = (msgToZhiKong.roiHeight) & 0xff;
    pktToZhiKong[36] = (msgToZhiKong.res2>>8) & 0xff;
    pktToZhiKong[37] = (msgToZhiKong.res2) & 0xff;
//qDebug()<<"sendMsg()"<<"crc:"<<chk <<"send Cnt:"<<sendZhiKongPktCnt;
    // 2. CRC计算与添加
    int chk = calcChecksum((unsigned char *)&pktToZhiKong,CTRMSG_BUF_LEN);

    pktToZhiKong[38] = (chk>>8) & 0xff;
    pktToZhiKong[39] = chk & 0xff;

    // 3. UDP发送
    centerUdpSocket->write((char *)&pktToZhiKong,CTRMSG_BUF_LEN);
    sendZhiKongPktCnt++;
    //qDebug()<<"sendMsg()"<<"crc:"<<chk <<"send Cnt:"<<sendZhiKongPktCnt;
    return 0;
}
/*
void NetworkComm::heartBeat_timeOut()
{

}
*/
#if 0
void NetworkComm::sendHeartbeat()
{
    if(ctrConnectFlag == false) return;

    int chk = calcChecksum(cmd_heartbeat,CTRMSG_BUF_LEN);
    cmd_heartbeat[41] = chk & 0xff;
    centerUdpSocket->write((char *)cmd_heartbeat,CTRMSG_BUF_LEN);
    sendHeartbeatCnt++;
    return;
}
#endif


int NetworkComm::initToZhiKongPacket()
{
    msgToZhiKong.sId = 0x47;
    msgToZhiKong.destId = 0x5a;
    msgToZhiKong.pktType = 0;
    msgToZhiKong.pktLen = 40;

    msgToZhiKong.status = (uint16_t)1;
    msgToZhiKong.gd1BianBei =(uint16_t)1;
    msgToZhiKong.gd1DuiJiao =(uint16_t) 1000;

    msgToZhiKong.gd1FangWei =0;

    msgToZhiKong.gd1FuYang = 0;
    msgToZhiKong.gd1ZheDangFlag = 0;
//qDebug()<<"msgToZhiKong.gd1FangWei:"<<msgToZhiKong.gd1FangWei <<"   gd1FuYang:"<<msgToZhiKong.gd1FuYang;
    msgToZhiKong.gd2BianBei = 1;
    msgToZhiKong.gd2DuiJiao = 1000;
    msgToZhiKong.gd2FangWei = 0;
    msgToZhiKong.gd2FuYang = 0;
    msgToZhiKong.gd2ZheDangFlag = 1;

    msgToZhiKong.switchTrack = 1;
    msgToZhiKong.numOfObj = 0;
    msgToZhiKong.obj1PosX = 0;
    msgToZhiKong.obj1PosY = 0;
    msgToZhiKong.obj2PosX = 0;
    msgToZhiKong.obj2PosY = 0;
    msgToZhiKong.roiWidth = 0;
    msgToZhiKong.roiHeight = 0;
    return 0;
}

int NetworkComm::initToServoPkt422()
{
    msgToServo422.head = 0x5aa5;
    msgToServo422.pktLen = 32;
    msgToServo422.devIdServo = 0x50;
    msgToServo422.devIdGD = 0x57;
    msgToServo422.loopNo = 0;
    msgToServo422.gd1FangWei.fData = 0.0;
    msgToServo422.gd1FuYang.fData = 0.0;
    msgToServo422.gd2FangWei.fData = 0.0;
    msgToServo422.gd2FuYang.fData = 0.0;
    msgToServo422.res = 0;
    msgToServo422.crc = 0;
    return 0;
}

int NetworkComm::convComm422PktToByte()
{
    //if(ctrConnectFlag == false) return 100;

    pktToServo422[0] = (msgToServo422.head) & 0xff;
    pktToServo422[1] = (msgToServo422.head>>8) & 0xff;
    pktToServo422[2] = (msgToServo422.pktLen>>0) & 0xff;
    pktToServo422[3] = (msgToServo422.pktLen>>8) & 0xff;
    pktToServo422[4] = msgToServo422.devIdServo & 0xff;
    pktToServo422[5] = msgToServo422.devIdGD & 0xff;
    pktToServo422[6] = (msgToServo422.loopNo) & 0xff;
    pktToServo422[7] = (msgToServo422.loopNo>>8) & 0xff;
    pktToServo422[8] = (msgToServo422.loopNo>>16) & 0xff;
    pktToServo422[9] = (msgToServo422.loopNo>>24) & 0xff;
    pktToServo422[10] = ((uint32_t)(msgToServo422.gd1FangWei.cData)) & 0xff;
    pktToServo422[11] = ((uint32_t)(msgToServo422.gd1FangWei.cData)>>8) & 0xff;
    pktToServo422[12] = ((uint32_t)(msgToServo422.gd1FangWei.cData)>>16) & 0xff;
    pktToServo422[13] = ((uint32_t)(msgToServo422.gd1FangWei.cData)>>24) & 0xff;
    pktToServo422[14] = ((uint32_t)(msgToServo422.gd1FuYang.cData)) & 0xff;
    pktToServo422[15] = ((uint32_t)(msgToServo422.gd1FuYang.cData)>>8) & 0xff;
    pktToServo422[16] = ((uint32_t)(msgToServo422.gd1FuYang.cData)>>16) & 0xff;
    pktToServo422[17] = ((uint32_t)(msgToServo422.gd1FuYang.cData)>>24) & 0xff;
    pktToServo422[18] = ((uint32_t)(msgToServo422.gd2FangWei.cData)) & 0xff;
    pktToServo422[19] = ((uint32_t)(msgToServo422.gd2FangWei.cData)>>8) & 0xff;
    pktToServo422[20] = ((uint32_t)(msgToServo422.gd2FangWei.cData)>>16) & 0xff;
    pktToServo422[21] = ((uint32_t)(msgToServo422.gd2FangWei.cData)>>24) & 0xff;
    pktToServo422[22] = ((uint32_t)(msgToServo422.gd2FuYang.cData)) & 0xff;
    pktToServo422[23] = ((uint32_t)(msgToServo422.gd2FuYang.cData)>>8) & 0xff;
    pktToServo422[24] = ((uint32_t)(msgToServo422.gd2FuYang.cData)>>16) & 0xff;
    pktToServo422[25] = ((uint32_t)(msgToServo422.gd2FuYang.cData)>>24) & 0xff;

    pktToServo422[26] = 0;
    pktToServo422[27] = 0;
    pktToServo422[28] = 0;
    pktToServo422[29] = 0;

    //qDebug()<<"10:"<<pktToServo422[10]<<" 14:"<<pktToServo422[14] << "  22:"<<pktToServo422[22];
    msgToServo422.crc = calcChecksum((unsigned char *)&pktToServo422,sizeof(pktToServo422));
//qDebug()<<"crc:"<<msgToServo422.crc;
    pktToServo422[30] = (msgToServo422.crc) & 0xff;
    pktToServo422[31] = (msgToServo422.crc>>8) & 0xff;
    return 0;
}

int NetworkComm::updateToZhiKongPacket()
{

    return 0;
}

void NetworkComm::disconnectCenter()
{
    //centerUdpSocket->close();
    ctrConnectFlag = false;
}

int NetworkComm::calcChecksum(QByteArray buf,int len,int m)
{
    int chk=0;

    if(buf.isEmpty())
        return 0;

    for(int i=0;i<(len-2);i++)
    {
        chk=chk+(uint8_t)buf.at(i+m);

    }
    chk=chk & 0xffff;

    return chk;
}

int NetworkComm::calcChecksum(unsigned char * buf,int len)
{
    int chk=0;

    if(buf == NULL)
        return 0;

    for(int i=0;i<(len-2);i++)
    {
        chk=chk+(uint8_t)buf[i];
    }
    chk=chk & 0xffff;

    return chk;
}


//----------------------------子线程中使用socket------------------------------------------------------------------
//https://blog.csdn.net/jlxbc/article/details/130382560
UdpRecvThread::UdpRecvThread(QObject *parent) : QObject(parent)
{
    m_bStop = false;

}

UdpRecvThread::~UdpRecvThread()
{qDebug()<<"~UdpRecvThread() thd:"<<QThread::currentThreadId();
    m_bStop = true;
    mcUnbind();
    delete udpSocket;
    udpSocket = nullptr;
    qDebug()<<"~UdpRecvThread()";
}

void UdpRecvThread::readPendingDatagrams()
{
    if(m_bStop) return;

    while (this->udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        if(datagram.senderAddress() == QHostAddress("192.168.1.121")) continue;
        QByteArray ba = datagram.data();

        qDebug() << "From [" <<datagram.senderAddress().toString()<<":"<< datagram.senderPort()<<"] "<< ba.size()<<" "<< ba.data()<<"  thd:"<<QThread::currentThreadId();
        if(ba.size() == 0)
            continue;
    }
}
/*
void UdpRecvThread::run()
{
    //crtMcSocket();
    while (!m_bStop)
    {
        //if(m_bPause)    continue;
        qDebug()<<"UdpRecvThread::run():";
        QThread::msleep(1000);
    }
    mcUnbind();
    delete udpSocket;
    udpSocket = nullptr;
    qDebug()<<"quit UdpRecvThread:";
    return;
    //exec();
}*/

void UdpRecvThread::crtMcSocket()
{
    qDebug()<<"crtMcSocket() thd:"<<QThread::currentThreadId();
    udpSocket = new QUdpSocket();
    mcBind("224.0.0.10",38190);
    /*if(!udpSocket->bind(m_srcPort,QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint))
    {
        qDebug() << "udp recv socket bind error";
    }
    udpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,128*1024*1024);*/
    QObject::connect(udpSocket,&QUdpSocket::readyRead,this,&UdpRecvThread::readPendingDatagrams);
}

//加入组播
int UdpRecvThread::mcBind(QString ipaddr,int Port)
{
    groupAddress = QHostAddress(ipaddr);
    mGroupPort = Port;
    quint16 groupPort = mGroupPort;//端口
    qDebug()<<"UdpRecvThread() Join groupAddress:"<<groupAddress <<"     groupPort:"<<mGroupPort;
    if(udpSocket->bind(QHostAddress::AnyIPv4,groupPort,QUdpSocket::ShareAddress))
    {
        //findInterface();
        udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, QVariant(200));
        //udpSocket->setMulticastInterface(interface_outer);

        udpSocket->joinMulticastGroup(groupAddress);//加入多播组
        qDebug()<<"joinMulticastGroup() OK";
    }
    else
    {
        qDebug()<<"joinMulticastGroup() failed";
        return -1;
    }
    return 0;
}

void UdpRecvThread::mcUnbind()
{//退出组播
    udpSocket->leaveMulticastGroup(groupAddress);//退出组播
    udpSocket->abort();
    /*ui->bindBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    ui->comboTargetIp->setEnabled(true);
    ui->plainTextEdit->appendPlainText("**已退出组播，解除端口绑定"); */
}


//-------------------------------Mc发送线程------------------------------------------
UdpSendThread::UdpSendThread(QObject *parent) : QObject(parent)

{
    m_bStop = false;
    mPeerAddress = QHostAddress("192.168.1.101");
    mPeerPort = 10023;
    initSendPkt(&sendPkt);

}

UdpSendThread::~UdpSendThread()
{
    timerTest->stop();
    timerTest->deleteLater();
    delete udpSocket;
    qDebug()<<"~UdpSendThread()";
    udpSocket = nullptr;
}


void UdpSendThread::run()
{
/*    //bool m_bPause = false;
    udpSocket = new QUdpSocket();
    qDebug()<<"UdpSendThread::run() thd:"<<QThread::currentThreadId();
    groupAddress = QHostAddress("224.0.0.1");
    mGroupPort = 38190;
//    if(!udpSocket->bind(m_srcPort,QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint))
//    {
//        qDebug() << "udp recv socket bind error";
//    }

    //udpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,128*1024*1024);

    while (!m_bStop)
    {
        //if(m_bPause)    continue;

        sendPkt.serialNo++;
        if(sendPkt.serialNo>255) sendPkt.serialNo = 0;
        //qDebug()<<"serialNo:"<<sendPkt.serialNo;
        QThread::msleep(1000);
    }
qDebug()<<"quit UdpSendThread: 0"<<QThread::currentThreadId();
    delete udpSocket;
    qDebug()<<"quit UdpSendThread: 1";
    timerTest->stop();
    udpSocket = nullptr;*/
    qDebug()<<"quit UdpSendThread: 2";
    return;
}

void UdpSendThread::initSendPkt(stPktFromPC * pkt)
{
    netBufLock.lockForWrite();
    memset(pkt,0,sizeof(stPktFromPC));
    pkt->hd1 = 0x5a;
    pkt->hd2 = 0x6b;
    pkt->fromDev = 0x2;
    pkt->toDev = 0x1;
    pkt->pktType = 0x1;
    pkt->pktLen = 40;
    pkt->loopNum =0;
    pkt->cameraNum =2;
    pkt->panMotorEn =0;
    pkt->Ah =0;
    pkt->Al =0;
    pkt->tiltMotorEn =0;
    pkt->Eh =0;
    pkt->El =0;
    pkt->pktTail = 0xa5;
    netBufLock.unlock();
}

void UdpSendThread::crtTimer()
{
    udpSocket = new QUdpSocket();
    qDebug()<<"1 crtTimer thd:"<<QThread::currentThreadId();
    groupAddress = QHostAddress("224.0.0.10");
    mGroupPort = 38190;

    //定时器
    timerTest = new QTimer();
    timerTest->setInterval(10);  //10ms  100hz
    connect(timerTest,SIGNAL(timeout()),this,SLOT(onTimeOut_Func()));
    //connect(timerTest, SIGNAL(sig_startSendThdTimer()), this, SLOT(slotGetOneFrameCh1(QImage)));
    timerTest->start();
    qDebug()<<"crtTimer() thd:"<<QThread::currentThreadId();
}

void UdpSendThread::onTimeOut_Func()
{
    int len =0;
    int resendCnt = 0;
    if(m_bStop) return;
    do
    {
        netBufLock.lockForRead();
        sendPkt.loopNum++;
        if(sendPkt.loopNum>255) sendPkt.loopNum = 0;
        len = mcSend(&sendPkt);

        netBufLock.unlock();
        if(sendPkt.pktType>1)
        {
            initSendPkt(&sendPkt);
        }
        /*
        QByteArray buf;
        buf.resize(sizeof(sendPkt));
        memcpy(buf.data(),&sendPkt, sizeof(sendPkt));
        qDebug()<<"sendPkt:"<<buf.toHex(' ');
        */

        if(len == -1)
        {
            //QThread::usleep(30);
        }
        resendCnt++;
    }while(len == -1 && resendCnt <= 3);
    return;
}

//加入组播
int UdpSendThread::mcBind(QString ipaddr,int Port)
{
    groupAddress = QHostAddress(ipaddr);
    mGroupPort = Port;
    quint16 groupPort = mGroupPort;//端口
    qDebug()<<"Join groupAddress:"<<groupAddress <<"     groupPort:"<<mGroupPort;
    if(udpSocket->bind(QHostAddress::AnyIPv4,groupPort,QUdpSocket::ShareAddress))
    {
        //findInterface();
        udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, QVariant(200));
        //udpSocket->setMulticastInterface(interface_outer);

        udpSocket->joinMulticastGroup(groupAddress);//加入多播组
        qDebug()<<"joinMulticastGroup() OK";
    }
    else
    {
        qDebug()<<"joinMulticastGroup() failed";
        return -1;
    }
    return 0;
}

void UdpSendThread::mcUnbind()
{//退出组播
    udpSocket->leaveMulticastGroup(groupAddress);//退出组播
    udpSocket->abort();
    /*ui->bindBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    ui->comboTargetIp->setEnabled(true);
    ui->plainTextEdit->appendPlainText("**已退出组播，解除端口绑定"); */
}

//bind单播
int UdpSendThread::ucBind(QString ipaddr,int Port)
{
    //groupAddress = QHostAddress(ipaddr);
    //mGroupPort = Port;
    quint16 ucPort = mPeerPort;//端口
    qDebug()<<"Join groupAddress:"<<mPeerAddress <<"     groupPort:"<<mPeerPort;
    if(udpSocket->bind(QHostAddress::AnyIPv4,ucPort,QUdpSocket::ShareAddress))
    {
        //findInterface();
        udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, QVariant(200));
        //udpSocket->setMulticastInterface(interface_outer);

        //udpSocket->joinMulticastGroup(mPeerAddress);//加入多播组
        //qDebug()<<"joinMulticastGroup() OK";
    }
    else
    {
        qDebug()<<"joinMulticastGroup() failed";
        return -1;
    }
    return 0;
}

void UdpSendThread::ucUnbind()
{//退出单播
    udpSocket->leaveMulticastGroup(mPeerAddress);//退出组播
    udpSocket->abort();
    /*ui->bindBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    ui->comboTargetIp->setEnabled(true);
    ui->plainTextEdit->appendPlainText("**已退出组播，解除端口绑定"); */
}

//发送组播控制报文
int UdpSendThread::mcSend(stPktFromPC * pkt)
{
    QByteArray byArr;
    byArr.resize(sizeof(stPktFromPC));
    //qDebug()<<"sizeof(JJSK_Packet):"<<sizeof(stPktFromPC);
    memcpy(byArr.data(),pkt,sizeof(stPktFromPC));
    //len = udpSocket->writeDatagram(byArr,dataLen,QHostAddress(m_dstIP),m_dstPort);
    return udpSocket->writeDatagram(byArr,groupAddress,(quint16)mGroupPort);
}

int UdpSendThread::findInterface()
{
#if 0
    int iNetInterface_local_outer;
    QString ip_Local_Outer = "30.74.38.13";
     QList< QNetworkInterface > listAllNetInterface;
     listAllNetInterface = QNetworkInterface::allInterfaces();

    for(int index = 0; index < listAllNetInterface.size(); index++)  //循环将网卡名转换为人类可读，并追加到网卡列表后
    {
        if(listAllNetInterface.at(index).addressEntries().size()>=2)
        {
            if(ip_Local_Outer == listAllNetInterface.at(index).addressEntries().at(0).ip().toString() )  //这个系统下，0是ipv4地址，1是ipv6地址
            //if(localIP == listAllNetInterface.at(index).addressEntries().at(0).ip().toString() )  //这个系统下，0是ipv4地址，1是ipv6地址
            {
                iNetInterface_local_outer = index;
                break;
            }
        }
    }
    interface_outer = listAllNetInterface.at(iNetInterface_local_outer);
#endif
    return 0;
}
