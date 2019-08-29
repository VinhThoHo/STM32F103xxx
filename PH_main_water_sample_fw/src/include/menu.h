#ifndef __MENU_H
#define __MENU_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"
#include "key.h"
#include "debug.h"

typedef struct menu_struct menu_t;

struct menu_struct {
	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);
};

extern menu_t menu1;
	extern menu_t sub_menu1_1;
	extern menu_t sub_menu1_2;

extern menu_t menu2;
	extern menu_t sub_menu2_1;
	extern menu_t sub_menu2_2;
	
extern menu_t menu3;
	extern menu_t sub_menu3_1;
	extern menu_t sub_menu3_2;

extern menu_t menu4;
	extern menu_t sub_menu4_1;
	extern menu_t sub_menu4_2;

extern menu_t menu5;
	extern menu_t sub_menu5_1;
	extern menu_t sub_menu5_2;
		extern menu_t sub_menu5_2_1;
		extern menu_t sub_menu5_2_2;

// extern menu_t menu6;

void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void menu_back(void);

#endif /* __MENU_H */