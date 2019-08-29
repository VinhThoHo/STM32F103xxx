#ifndef __MENU_H
#define __MENU_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include "tick.h"
#include "key.h"
#include "debug.h"


#define TITLE_TEXT (" ** WATER SAMPLE ** ")
#define MENU_TEXT ("        MENU")
#define ABOUT_TEXT ("ABOUT")

#define SET_CLOCK_TEXT ("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")
#define SET_BOTTLE_TEXT ("> 2.SET BOTTLE"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")
#define SET_TRIGGER_TEXT ("> 3.SET TRIGGER"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")
#define SET_VIEWLOG_TEXT ("> 4.SET VIEWLOG"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")
#define SET_RESTORE_TEXT ("> 5.SET RESTORE"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")
#define SET_BACK_TEXT ("> 6.BACK"),("> 1.SET CLOCK"),("> 1.SET CLOCK"),("> 1.SET CLOCK")

#define DISPLAY_SET_CLOCK (" *Setting DateTime* ")
#define DISPLAY_SET_DATE ("     DD/MM/YYYY     ")
#define DISPLAY_SET_TIME ("      HH:MM:SS      ")

typedef enum {
	/*Menu Level 1*/
	Init = 0,
	Exe_Idle,
	MAIN_MENU,
	Exe_Info,

	/*Main menu level 1*/
	SET_CLOCK,
	MANUAL_TRIGGER,
	MOVE_HOME,
	MOVE_CURRENT,
	VIEW_LOG,
	BACK,

	/*menu level 2*/
	SET_CLOCK_MENU,
	MANUAL_TRIGGER_MENU,
	MOVE_HOME_MENU,
	MOVE_CURRENT_MENU,
	VIEW_LOG_MENU,

	/*menu level 2*/
	SET_DATE_MENU,
	SET_TIME_MENU,

	SET_DATE,
	SET_TIME,

	SET_DAY,
	SET_MONTH,
	SET_YEAR,

	SET_HOUR,
	SET_MIN,
	SET_SEC,

	SET_DAY_UP,
	SET_DAY_DOWN,

	SET_MONTH_UP,
	SET_MONTH_DOWN,

	SET_YEAR_UP,
	SET_YEAR_DOWN,

	SET_SAVE_DATE,
	SAVE_DATE,
	

	SET_HOUR_DOWN,
	SET_HOUR_UP,

	SET_MIN_DOWN,
	SET_MIN_UP,

	SET_SEC_DOWN,
	SET_SEC_UP,

	SET_SAVE_TIME,
	SAVE_TIME,

	SET_BOT_UP,
	SET_BOT_DOWN,
	MANUAL_MOVING,

	MOVE_TO_HOME,
	MOVE_TO_CURRENT,

	VIEW_LOG_UP,
	VIEW_LOG_DOWN,

}MENU_STATE_ID;

extern MENU_STATE_ID State;
/** Menu NextState */
typedef struct
{
  MENU_STATE_ID State; //Current State
  BUTTON_ID Key;	//event Key
  MENU_STATE_ID NextState; //Next State
} MENU_NEXTSTATE;

/** Menu state */
typedef struct
{
	MENU_STATE_ID State; //Current State
	void (*pFunc)(void); // Excute Function
	uint8_t* pText[4]; //Message Display
} MENU_STATE;


void MenuInit(void); //Init
void MenuUpdate(BUTTON_ID key); //Update State
void MenuDisplay(void); //Display State
uint8_t MenuDisplayWelcomeMessage(void); 
void Screen_Menu_Exe(void);
void Screen_Info_Exec(void);
void Screen_Idle_Exec(void);
void SelMenu_Exec(void);


void data_time_update(void);
void temp_update(void);
void menu_manager(void);

void view_set_date_time(void);
void view_set_date(void);

void get_date_time(void);
void set_day(void);
void set_month(void);
void set_year(void);
void set_hour(void);
void set_min(void);
void set_sec(void);

void day_dec(void);
void day_inc(void);

void month_dec(void);
void month_inc(void);

void year_dec(void);
void year_inc(void);

void hour_dec(void);
void hour_inc(void);

void min_dec(void);
void min_inc(void);

void sec_dec(void);
void sec_inc(void);

void save_date(void);
void save_time(void);

void lcd_display(uint8_t* state[]);

void set_bottle_up(void);
void set_bottle_down(void);
void view_bottle(void);
void manual_moving(void);
void move_to_home(void);
void move_to_current(void);
void view_log_up(void);
void view_log_down(void);
void view_log(void);

#endif /* __MENU_H */