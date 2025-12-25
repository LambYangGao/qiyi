#ifndef POD_CMD_H
#define POD_CMD_H
//----------------------------2023.11.29 Pod proj ----------------------------------

#include <iostream>
#include <stdint.h>
#include <QList>
#include <QDebug>


#define SYNCSH	0xff

#define PTZ_SH_OFFSETDEG	16	//云台跟踪偏移指令  type 16 值xx1xx2: 方位偏移量(高位在前) xx3xx4: 俯仰偏移量(高位在前)   16位short 1LSB =0.002（度）
#define PTZ_SH_OFFSET       15		//云台速度指令  type 15 值0~+-127


class pod_cmd
{
public:
    pod_cmd();
    uint8_t cmd_blank[7];

    uint8_t * crtSteplessCmd(int offsetx,int offsety);
    uint8_t * crtOffsetCmd(int offsetXDeg,int offsetYDeg);
};

#endif // POD_CMD_H
