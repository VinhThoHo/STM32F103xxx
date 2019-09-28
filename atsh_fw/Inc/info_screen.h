#ifndef __INFO_SCREEN_H
#define __INFO_SCREEN_H

#include "main.h"

#define D_Level 3 

typedef struct {
    uint8_t SysTime;
    uint8_t SysWork;
    uint8_t Filter_Pa;
    uint8_t Filter_Time;
    uint8_t Flow;
    uint8_t UV;
    uint8_t Door;
    uint32_t tick;
} INFO_SCREEN_STR;

extern INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void);
void SysTime(uint8_t TimeStatus);
void Filter_Pa(uint8_t PaStatus);
void Flow(uint8_t FlowStatus);
void Door(uint8_t DStatus);
void Info_Screen_Manage(void);
// void SysWork(uint8_t WorkStatus);
// void Filter_Time(uint8_t FTStatus);
// void UV_Time(uint8_t UvStatus);

#endif