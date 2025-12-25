#ifndef NETWORKCOMM_H
#define NETWORKCOMM_H
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include <QThread>
#include <iostream>
#include <stdint.h>
#include <QTime>
#include <QDateTime>
#include <QMutexLocker>
#include <QNetworkDatagram>
#include <QReadWriteLock>

#define RECVZHIKONGMSG_LEN  21
#define RECVGYROMSG_LEN  58
#define CTRMSG_BUF_LEN  0x51
#define PTMSG_BUF_LEN  49
#define SYSHEADER  0x7a
#define CTRHEADER  0x5a
#define CTRHEADER2  0x47

#define ANGLENET2DEG   (65535.00000/360.00000)

//GD280绵阳项目上送状态报文
typedef struct PktToZhiKong_s {
    uint8_t sId;                //0x47 发送设备Id
    uint8_t destId;             //0x5a 接收设备Id
    uint8_t pktType;            //填0
    uint8_t pktLen;             //报文长度，共40字节

    uint8_t status;             //状态标志
    uint16_t gd1BianBei;          //设备1变倍值
    uint16_t gd1DuiJiao;          //设备1对焦值
    uint8_t gd1ZheDangFlag;       //设备1遮挡标志
    uint16_t gd1FangWei;          //设备1方位信息
    uint16_t gd1FuYang;          //设备1俯仰信息

    uint8_t res;                //保留

    uint16_t gd2BianBei;          //设备1变倍值
    uint16_t gd2DuiJiao;          //设备1对焦值
    uint8_t gd2ZheDangFlag;       //设备1遮挡标志
    uint16_t gd2FangWei;          //设备1方位信息
    uint16_t gd2FuYang;          //设备1俯仰信息

    uint8_t switchTrack;          //GD是否转图像跟踪

    uint8_t numOfObj;          //识别目标数量，最多3个
    int16_t obj1PosX;          //识别目标1 水平位置
    int16_t obj1PosY;          //识别目标1 垂直位置
    int16_t obj2PosX;          //识别目标1 水平位置
    int16_t obj2PosY;          //识别目标1 垂直位置
    uint8_t roiWidth;           //obj1,2共用的宽高
    uint8_t roiHeight;          //
    int16_t res2;                //保留
    uint16_t crc;               //校验字，除校验字外所有字节之和的低十六位
}stPktToZhiKong;

//typedef struct PktFromZhiKong_s {
//    uint8_t sId;                //0x5a 发送设备Id
//    uint8_t destId;             //0x47 接收设备Id
//    uint8_t pktType;            //填0
//    uint8_t pktLen;             //报文长度，共16字节

//    uint8_t cmdId;             /*   0：停止
//                                    1：引导光电设备
//                                    2：备用
//                                     */
//    uint8_t startTracking;      //bit0:是否启动跟踪，bit1~2:识别算法选择
//    uint16_t zkFangWei;         //引导方位信息
//    uint16_t zkFuYang;          //引导俯仰信息
//    uint16_t zkDistence;        //距离， 量化 0.1米

//    uint8_t zkBianBeiDuiJiao;   //保留
//    uint8_t res;                //保留
//    uint16_t crc;               //校验字，除校验字外所有字节之和的低十六位,低字节在前
//}stPktFromZhiKong;

typedef struct PktFromZhiKong_s {
    uint8_t sId;                //0x5a 发送设备Id
    uint8_t destId;             //0x47 接收设备Id
    uint8_t pktType;            // bit0:cmdId；bit1:startTracking；bit2: suanfa；bit3：fangweifuyangjuli；bit4：变倍对焦;bit5:shubiao;
    uint8_t pktLen;             //报文长度，共16字节

    uint8_t cmdId;             /*   0：停止(+一下伺服停止)
                                    1：引导光电设备
                                    2：备用
                                     */
    uint8_t startTracking;      //bit0:是否启动跟踪
    uint8_t suanfa;            //识别算法选择
    uint16_t zkFangWei;         //引导方位信息
    uint16_t zkFuYang;          //引导俯仰信息
    uint16_t zkDistence;        //距离， 量化 0.1米
    uint8_t zkBianBeiDuiJiao;   //保留
    uint16_t shubiaox;           //鼠标坐标x
    uint16_t shubiaoy;          //鼠标坐标y
    uint8_t res;                //保留
    uint16_t crc;               //校验字，除校验字外所有字节之和的低十六位,低字节在前
}stPktFromZhiKong;


typedef struct PktFromZhiKongCalib_s {
    uint8_t sId;                //0x5a 发送设备Id
    uint8_t destId;             //0x47 接收设备Id
    uint8_t pktType;            //填1
    uint8_t pktLen;             //报文长度，共16字节

    uint8_t cmdId;             /*   0：备用
                                    1：备用
                                    2：备用
                                    3：光电球归零
                                    4：调整零位（将光电1的当前指向设置为0度）
                                    5：调整零位（将光电2的当前指向设置为0度） */
    uint8_t res0;               //保留
    uint16_t setFangWei;        //方位角
    uint16_t res;               //保留
    uint16_t res2;              //保留
    uint16_t res3;              //保留
    uint16_t crc;               //校验字，除校验字外所有字节之和的低十六位,低字节在前
}stPktFromZhiKongCalib;

typedef struct PktFromGyro_s {
    double roll;                //横滚角（右手螺旋，前，右倾为正），±60°
    double yaw;                //航向角（右手螺旋，上，逆时针为正），0~360°
    double pitch;              //俯仰角（右手螺旋，右，抬头为正），±60°
    double B;               //纬度(北纬为正)，±Pi rad        rad	 Pi/(2^31 -1)
    double L;               //经度(东经为正)，±Pi rad
    double H;               //高度,单位为mm  LSB=0.001m
    double direction;       //参考航向（双天线航向角，0~2Pi  rad）
}stPktFromGyro;

typedef   union flot2char_u{
    float fData;
    int cData;
}f2c;

typedef struct PktToServo422_s {
    uint16_t head;                //报文起始标志	0x5AA5
    uint16_t pktLen;                //报文长度	32
    uint8_t devIdServo;                //随动伺服编号	0x50
    uint8_t devIdGD;                //微光电视编号	0x57
    uint32_t loopNo;                //报文序号
    f2c gd1FangWei;               //微光1引导方位角度值	[0°,360°] 精确到0.01°    低字节先发，高字节后发
    f2c gd1FuYang;               //微光1引导俯仰角度值	[-2°,90°] 精确到0.01°    低字节先发，高字节后发
    f2c gd2FangWei;               //微光2引导方位角度值	[0°,360°] 精确到0.01°    低字节先发，高字节后发
    f2c gd2FuYang;               //微光2引导俯仰角度值	[-2°,90°] 精确到0.01°    低字节先发，高字节后发
    uint32_t res;
    uint16_t crc;           //校验和	—	[30-31]	无符号整型	2	低字节在前，高字节在后
}stPktToServo422;

class NetworkComm
{
public slots:
   // void heartBeat_timeOut();

public:
    QUdpSocket * centerUdpSocket;
    QString centerIpAddr;
    int centerPortNum;
    int pitchVal;
    int directionVal;
    int distanceVal;
    int timeInterval;
    bool ctrConnectFlag;
    int localRoleFlag;
    int sendHeartbeatCnt;
    int recvPacketCnt;
    int recvPacketErrCnt;
    int sendZhiKongPktCnt;
    int recvZhiKongPktCnt;



    stPktToZhiKong msgToZhiKong;
    stPktFromZhiKong msgFromZhiKong;
    stPktFromGyro msgFromGyro;
    uint8_t pktToZhiKong[40];

    stPktToServo422 msgToServo422;
    uint8_t pktToServo422[32];
    int sendServoPktCnt;
    bool servo422ConnectFlag;

    NetworkComm(int role);
    int connectCenter();
    void disconnectCenter();
    //int recvMsg();
    int sendMsg();
    //int sendReply();
    //void sendHeartbeat();
    int centerSocket_Read_Data();
    void gyroSocket_Read_Data();
    int updateToZhiKongPacket();
    int initToZhiKongPacket();
    int initToServoPkt422();
    int convComm422PktToByte();

private:

    int calcChecksum(QByteArray buf,int len,int m);
    int calcChecksum(unsigned char * buf,int len);

    QTimer * timerHeartBeat;
};






//----------------------------子线程中使用socket------------------------------------------------------------------
typedef struct Packet {
    uint8_t pktNum;
    uint8_t frameNum;
    uint8_t res;
    uint16_t serialNo;
    uint8_t counter;
    uint16_t pktLen;

    uint8_t status;         //光电球运行状态
    uint8_t mode;           //模式
    uint8_t trackingStat;   //跟踪状态
    uint8_t res2;
    float direction;        //方位
    float pitch;            //俯仰
    uint8_t res3;
    uint8_t res4;
    uint8_t res5;
    uint8_t res6;
}stPkt;


typedef struct PktFromPCBodyTransparent {
    uint8_t buf[31];
}stPktFromPCBodyTransparent;


typedef struct PktFromPC {
    uint8_t hd1;                //0x5a
    uint8_t hd2;                //0x6b
    uint8_t fromDev;            //发送设备Id    填2 （0:无效   1:转台  2:PC1 3:PC2 ...）
    uint8_t toDev;              //接收设备Id    填1
    uint8_t pktType;            //报文Type
    uint8_t pktLen;
    uint8_t loopNum;            //循环码
    uint8_t cameraNum;          //机芯数量
    uint8_t cam1BianBeiH;       //机芯1变倍值H
    uint8_t cam1BianBeiL;       //机芯1变倍值L
    uint8_t cam1BianBeiNum;     //机芯1变倍倍数
    uint8_t cam1BianBeiZoom;    //机芯1变倍+-
    uint8_t cam1DuiJiaoH;       //机芯1对焦值H
    uint8_t cam1DuiJiaoL;       //机芯1对焦值L
    uint8_t cam1DuiJiao;        //机芯1对焦+-

    uint8_t cam2BianBeiH;       //机芯2变倍值H
    uint8_t cam2BianBeiL;       //机芯2变倍值L
    uint8_t cam2BianBeiNum;     //机芯2变倍倍数
    uint8_t cam2BianBeiZoom;    //机芯2变倍+-
    uint8_t cam2DuiJiaoH;       //机芯2对焦值H
    uint8_t cam2DuiJiaoL;       //机芯2对焦值L
    uint8_t cam2DuiJiao;        //机芯2对焦+-
    uint8_t res1;               //保留 光圈？
    uint8_t res2;               //保留

    uint8_t panMotorEn;         //方位伺服使能    0,1,2	1:使能 2:直接置角度
    uint8_t Ah;                 //方位Ah
    uint8_t Al;                 //方位Al
    uint8_t tiltMotorEn;        //俯仰伺服使能    0,1,2	1:使能 2:直接置角度
    uint8_t Eh;                 //俯仰Eh
    uint8_t El;                 //俯仰El

    uint8_t trackerMode;        //跟踪模式	0,1,2	0:off 1:算法1 2:算法2
    uint8_t rectx;              //跟踪框x
    uint8_t recty;              //跟踪框y
    uint8_t rectWidth;          //跟踪框width
    uint8_t rectHeight;         //跟踪框height
    uint8_t mouseBtn;           //鼠标Button	0,1,2,3,4	0鼠标无效     1鼠标有效 2左 3右 4中
    uint8_t mousePosx;          //鼠标Pos x
    uint8_t mousePosy;          //鼠标Pos y
    uint8_t res3;

    uint8_t pktTail;            //0xa5		帧尾
}stPktFromPC;

typedef struct PktFromPCTr {
    uint8_t hd1;                //0x5a
    uint8_t hd2;                //0x6b
    uint8_t fromDev;            //发送设备Id    填2 （0:无效   1:转台  2:PC1 3:PC2 ...）
    uint8_t toDev;              //接收设备Id    填1
    uint8_t pktType;            //报文Type
    uint8_t pktLen;
    uint8_t loopNum;            //循环码
    uint8_t buf[31];

    uint8_t pktTail;            //0xa5		帧尾
}stPktFromPCTr;

//https://blog.csdn.net/jlxbc/article/details/130382560
class UdpRecvThread: public QObject
{
    Q_OBJECT
public:
    explicit UdpRecvThread(QObject *parent = nullptr);
    ~UdpRecvThread();
    bool m_bStop;
    QUdpSocket * udpSocket;
    int m_srcPort;

    QHostAddress groupAddress;//组播地址
    int mGroupPort;
    stPkt recvPkt;

private:

    //void run();
    int mcBind(QString ipaddr,int Port);
    void mcUnbind();
    QBasicMutex m_lock;   //定义一个互斥锁变量

public slots:
    void crtMcSocket();
    void readPendingDatagrams();
};


class UdpSendThread: public QObject
{
    Q_OBJECT
public:
    //UdpSendThread();
    explicit UdpSendThread(QObject *parent = nullptr);
    ~UdpSendThread();

    QUdpSocket * udpSocket;
    bool m_bStop;
    int m_dstPort;
    //QString m_dstIP;

    QHostAddress groupAddress;//组播地址
    int mGroupPort;

    QHostAddress mPeerAddress;//单播地址
    int mPeerPort;  //单播Port

    //stPkt sendPkt;
    stPktFromPC sendPkt;
    QTimer * timerTest;  //定时器


    QString getLocalIp();//获取本机IP地址

    int mcBind(QString ipaddr,int Port);
    void mcUnbind();
    int mcSend(stPktFromPC * pkt);
    void initSendPkt(stPktFromPC * pkt);

    int ucBind(QString ipaddr,int Port);
    void ucUnbind();
    int ucSend(stPktFromPC * pkt);
    void initSendUcPkt(stPktFromPC * pkt);
private:
    void run();
    int findInterface();

public slots:
    void crtTimer();
    void onTimeOut_Func();

signals:

};

#endif // NETWORKCOMM_H
