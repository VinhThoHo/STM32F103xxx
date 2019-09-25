#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

#include "main.h"

void Lamp_Status(uint8_t lampStatus);
void UV_Status(uint8_t uvStatus);
void Fan_Status(uint8_t fanStatus);
void Socket_Status(uint8_t sStatus);
void Filter(uint8_t FStatus);
void Horizontal(void);
void Interface_Component(void);
#endif