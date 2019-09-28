#ifndef __MAIN_SCREEN_H__
#define __MAIN_SCREEN_H__

#include "main.h"
#include "user_bitmap.h"

typedef struct {
    uint8_t lampStatus;
    uint8_t uvStatus;
    uint8_t fanStatus;
    uint8_t socketStatus;
    uint8_t fanRotate;
    uint32_t tick;
} MAIN_SCREEN_STR;

extern MAIN_SCREEN_STR main_scr;

void main_screen_init(void);
void Lamp_Status(uint8_t lampStatus);
void UV_Status(uint8_t uvStatus);
void Fan_Status(uint8_t fanStatus);
void Socket_Status(uint8_t sStatus);
void Filter(uint8_t FStatus);
void Horizontal(void);
void Main_Screen_Manage(void);

#endif