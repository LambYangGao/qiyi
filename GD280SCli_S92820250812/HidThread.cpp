#include "HidThread.h"

#ifdef OS_KYLIN


#elif OS_LINUX

#else
// Callback functions' prototypes
void hidApiErrorCb(HidError err);
void deviceAddedCb(int index, HidDevice dev);
void deviceRemovedCb(int index, HidDevice dev);
void hidDeviceErrorCb(HidDevice dev, HidError err);



// Global variables
extern QReadWriteLock hidLock;
HidApi        api(&hidApiErrorCb);
HidDeviceList devList;
// ######################################### CALLBACK FUNCTIONS' DEFINITIONS BEGIN ######################################### //

void hidApiErrorCb(HidError err)
{
    std::cout << "Api error occured:     " << err.getErrorCode() << " - " << err.getErrorString() << std::endl;
}

void deviceAddedCb(int index, HidDevice dev)
{
    std::cout << "\nThis device added:   " << dev.getPath() << std::endl;
    devList = api.getDeviceList();
}

void deviceRemovedCb(int index, HidDevice dev)
{
    std::cout << "\nThis device removed: " << dev.getPath() << std::endl;
    devList = api.getDeviceList();
}

void hidDeviceErrorCb(HidDevice dev, HidError err)
{
    std::cout << "Device error occured:  " << err.getErrorCode() << " - " << err.getErrorString() << " - " << dev.getPath() << std::endl;
}

// ########################################## CALLBACK FUNCTIONS' DEFINITIONS END ########################################## //
#endif



HidProcThd::HidProcThd()
{
    runflag =true;
    panVal=0;
    tiltVal=0;
    btnVal=0;
    btnVal2=0;
    axialVal=0;
    hidCount=0;
    //speedGear=1;
}

HidProcThd::~HidProcThd()
{
    hidCount=0;
    kbc312.close();
    quit();
    wait();
}


void HidProcThd::run() // 线程入口函数
{
#ifdef OS_KYLIN
    //openSerialPort();
    while(runflag)
    {
        usleep(200*1000);
    }
    //closeSerialPort();
#elif OS_LINUX
    hotplug_sock = init_hotplug_sock();

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(0x4d8, 0x3f, L"12345");
    openHidDev();

    while(runflag)
    {
        if (hidOpend == true)
        {
            hidloop();
        }
        else
        {
            checkHotplug();
        }
    }

#else
    qDebug() << objectName() << " : " << "run() begin";
    //HID摇杆键盘设置

    api.registerApiErrorCallback( &hidApiErrorCb );
    api.registerDeviceErrorCallback( &hidDeviceErrorCb );
    api.registerDeviceAddCallback( &deviceAddedCb );
    api.registerDeviceRemoveCallback( &deviceRemovedCb );
    // Scan(enumarate) devices with filter parameters
    devList = api.scanDevices( ANY,                // vendor id
                               //0x5710,                // product id - keyboard C312
                               0x61,                // product id - 3D
                               ANY,     // serial
                               ANY,                // manufacturer
                               ANY,                // product string
                               ANY,               // release
                               ANY,                // usage page
                               ANY                 // usage
                               );





    std::cout << "HidAPi is inited? : " << std::boolalpha << api.isInitialized() << std::endl;
    std::cout << "Found device count: " << devList.size() << std::endl;

    // Prints found devices' properties
    for( size_t i=0 ; i<devList.size() ; i++ )
    {
        //if( devList[i].getProductId() ==  0x5710)	//12886
        if( devList[i].getProductId() ==  0x61)	//3d joystick pid
        {
            kbc312 = devList[i];
            std::cout << "3D Joystick found" << std::endl;
            hidCount=1;
        }
    }

    qDebug()<<"hid thd:"<<this->thread()->currentThreadId();
    if( kbc312.isInitialized() )
    {
        bool isOpen = kbc312.open();
        if(false == isOpen)
        {
            std::cout << "Joystick open result : " << std::boolalpha << isOpen << std::endl;
        }
        else
        {
            while(runflag)
            {
                hidloop();
            }
        }
    }
    std::cout << "Rechecking hid devices : " << std::endl;
    kbc312.close();
    hidCount=0;
#endif
}

void HidProcThd::hidloop()
{

    std::string str;

    if(kbc312.readAvailable()>0)
    {
        str = kbc312.read(0);
        /*QByteArray babuf;
        babuf.resize(str.length());
        memcpy(babuf.data(),str.data(),20);
        qDebug()<<"str.length():"<<str.length()<<"str:"<<babuf.toHex() ; */
        if (str.length() == 9)      //3D JoyStick on laptop
        {
            hidLock.lockForWrite();
            panVal= ((str.at(2))<<8) +((str.at(1))&0xff);
            tiltVal=((str.at(4))<<8) +((str.at(3))&0xff);
            axialVal=((str.at(6))<<8) +((str.at(5))&0xff);
            //btnVal = (str.at(7))&0xff;
            btnVal2= (str.at(8))&0xff;
            hidLock.unlock();
            //qDebug()<<"panVal:"<<panVal <<" tiltVal:"<<tiltVal;
            //emit sig_UpdateJoystick();"
        }
    }
    //--------------------------------------------仅作测试
   /* {
        static int tmpPan =100;
        tmpPan=tmpPan+1;
        if(tmpPan >800) tmpPan =100;

        //hidLock.lockForWrite();
        panVal= 512;//tmpPan;
        tiltVal=512;
        //btnVal = (str.at(7))&0xff;
        btnVal2= 0;
        //hidLock.unlock();
        emit sig_UpdateJoystick();

    }
    QThread::msleep(5);*/
    //--------------------------------------------仅作测试

    return;
}


