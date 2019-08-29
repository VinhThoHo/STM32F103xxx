#ifndef __SYSTEM__H__
#define __SYSTEM__H__

#include "HD44780.h"
#include "debug.h"
#include "key.h"
#include "movexy.h"

typedef struct {
	uint8_t x;
	uint8_t y;
} SYSTEM_STRUCT;

extern SYSTEM_STRUCT sys;
extern float tempGet;

void system_init(void);
void system_manager(void);
void ds18b20_manager(void);
void bot_to_xy(uint8_t bot);

#endif  /*__SYSTEM__H__*/