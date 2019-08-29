#include "menu.h"
#include "HD44780.h"
#include <string.h>
#include <stdio.h>
#include "tick.h"
#include "clock_rtc.h"
#include "timerTick.h"
#include "key.h"
#include "lcd_display.h"
#include "system.h"
#include "eeprom.h"
#include "movexy.h"
#include "system.h"

TIME menuTick;
TIME displayTick;
uint8_t bottle = 1;
uint8_t bot_view = 1;

const MENU_NEXTSTATE MenuNextState[] = {
	/* Screen Main */
	{Exe_Idle, MENU, SET_CLOCK},
	{Exe_Idle, EXIT, Exe_Info},

	{SET_CLOCK, EXIT, Exe_Idle},
	{Exe_Info, EXIT, Exe_Idle},

	/* Main menu Level 1 */
	//{MAIN_MENU, DOWN, SET_CLOCK},
	{SET_CLOCK, DOWN, MANUAL_TRIGGER},
	{MANUAL_TRIGGER, DOWN, MOVE_HOME},
	{MOVE_HOME, DOWN, MOVE_CURRENT},
	{MOVE_CURRENT, DOWN, VIEW_LOG},
	{VIEW_LOG, DOWN, BACK},
	{BACK, DOWN, SET_CLOCK},

	//{MAIN_MENU, UP, SET_BACK},
	{BACK, UP, VIEW_LOG},
	{VIEW_LOG, UP, MOVE_CURRENT},
	{MOVE_CURRENT, UP, MOVE_HOME},
	{MOVE_HOME, UP, MANUAL_TRIGGER},
	{MANUAL_TRIGGER, UP, SET_CLOCK},
	{SET_CLOCK, UP, BACK},

	{SET_CLOCK, MENU, SET_DATE_MENU},
	{MANUAL_TRIGGER, MENU, MANUAL_TRIGGER_MENU},
	{MOVE_HOME, MENU, MOVE_HOME_MENU},
	{MOVE_CURRENT, MENU, MOVE_CURRENT_MENU},
	{VIEW_LOG, MENU, VIEW_LOG_MENU},
	{BACK, MENU, Exe_Idle},

	{SET_CLOCK, EXIT, Exe_Idle},
	{MANUAL_TRIGGER, EXIT, Exe_Idle},
	{MOVE_HOME, EXIT, Exe_Idle},
	{MOVE_CURRENT, EXIT, Exe_Idle},
	{VIEW_LOG, EXIT, Exe_Idle},
	{BACK, EXIT, Exe_Idle},

	{MANUAL_TRIGGER_MENU, EXIT, MANUAL_TRIGGER},
	{MOVE_HOME_MENU, EXIT, MOVE_HOME},
	{MOVE_CURRENT_MENU, EXIT, MOVE_CURRENT},
	{VIEW_LOG_MENU, EXIT, VIEW_LOG},

	/* Sub menu Level 2 */

	{SET_DATE_MENU, DOWN, SET_TIME_MENU},
	{SET_TIME_MENU, DOWN, SET_DATE_MENU},

	{SET_TIME_MENU, UP, SET_DATE_MENU},
	{SET_DATE_MENU, UP, SET_TIME_MENU},

	{SET_TIME_MENU, EXIT, SET_CLOCK},
	{SET_DATE_MENU, EXIT, SET_CLOCK},

	/* Set Date Time Menu */

	{SET_DATE_MENU, MENU, SET_DAY},
	{SET_TIME_MENU, MENU, SET_HOUR},

	{SET_DAY, EXIT, SET_DATE_MENU},

	{SET_DAY, MENU, SET_MONTH},
	{SET_MONTH, MENU, SET_YEAR},
	{SET_YEAR, MENU, SET_SAVE_DATE},
	{SET_SAVE_DATE, MENU, SAVE_DATE},
	{SAVE_DATE, MENU, SET_DATE_MENU},

	{SET_DAY, EXIT, SET_DATE_MENU},
	{SET_MONTH, EXIT, SET_DATE_MENU},
	{SET_YEAR, EXIT, SET_DATE_MENU},
	{SET_SAVE_DATE, EXIT, SET_DATE_MENU},
	{SAVE_DATE, EXIT, SET_DATE_MENU},

	/*SET +/- DAY*/
	{SET_DAY, DOWN, SET_DAY_DOWN},
	{SET_DAY_DOWN, DOWN, SET_DAY_DOWN},
	{SET_DAY_DOWN, UP, SET_DAY_UP},

	{SET_DAY, UP, SET_DAY_UP},
	{SET_DAY_UP, UP, SET_DAY_UP},
	{SET_DAY_UP, DOWN, SET_DAY_DOWN},

	{SET_DAY_DOWN, EXIT, SET_DATE_MENU},
	{SET_DAY_UP, EXIT, SET_DATE_MENU},

	{SET_DAY_DOWN, MENU, SET_MONTH},
	{SET_DAY_UP, MENU, SET_MONTH},

	/*SET +/- MONTH*/
	{SET_MONTH, DOWN, SET_MONTH_DOWN},
	{SET_MONTH_DOWN, DOWN, SET_MONTH_DOWN},
	{SET_MONTH_DOWN, UP, SET_MONTH_UP},

	{SET_MONTH, UP, SET_MONTH_UP},
	{SET_MONTH_UP, UP, SET_MONTH_UP},
	{SET_MONTH_UP, DOWN, SET_MONTH_DOWN},

	{SET_MONTH_DOWN, EXIT, SET_DATE_MENU},
	{SET_MONTH_UP, EXIT, SET_DATE_MENU},

	{SET_MONTH_DOWN, MENU, SET_YEAR},
	{SET_MONTH_UP, MENU, SET_YEAR},

	/*SET +/- YEAR*/
	{SET_YEAR, DOWN, SET_YEAR_DOWN},
	{SET_YEAR_DOWN, DOWN, SET_YEAR_DOWN},
	{SET_YEAR_DOWN, UP, SET_YEAR_UP},

	{SET_YEAR, UP, SET_YEAR_UP},
	{SET_YEAR_UP, UP, SET_YEAR_UP},
	{SET_MONTH_UP, DOWN, SET_YEAR_DOWN},

	{SET_YEAR_DOWN, EXIT, SET_DATE_MENU},
	{SET_YEAR_UP, EXIT, SET_DATE_MENU},

	{SET_YEAR_DOWN, MENU, SET_SAVE_DATE},
	{SET_YEAR_UP, MENU, SET_SAVE_DATE},


	/*SET_TIME*/
	{SET_HOUR, MENU, SET_MIN},
	{SET_MIN, MENU, SET_SEC},
	{SET_SEC, MENU, SET_SAVE_TIME},
	{SET_SAVE_TIME, MENU, SAVE_TIME},
	{SAVE_TIME, MENU, SET_TIME_MENU},

	{SET_HOUR, EXIT, SET_TIME_MENU},
	{SET_MIN, EXIT, SET_TIME_MENU},
	{SET_SEC, EXIT, SET_TIME_MENU},
	{SET_SAVE_TIME, EXIT, SET_TIME_MENU},
	{SAVE_TIME, EXIT, SET_TIME_MENU},

	/*SET +/- HOUR*/
	{SET_HOUR, DOWN, SET_HOUR_DOWN},
	{SET_HOUR_DOWN, DOWN, SET_HOUR_DOWN},
	{SET_HOUR_DOWN, UP, SET_HOUR_UP},

	{SET_HOUR, UP, SET_HOUR_UP},
	{SET_HOUR_UP, UP, SET_HOUR_UP},
	{SET_HOUR_UP, DOWN, SET_HOUR_DOWN},

	{SET_HOUR_DOWN, EXIT, SET_TIME_MENU},
	{SET_HOUR_UP, EXIT, SET_TIME_MENU},

	{SET_HOUR_DOWN, MENU, SET_MIN},
	{SET_HOUR_UP, MENU, SET_MIN},

	/*SET +/- MIN*/
	{SET_MIN, DOWN, SET_MIN_DOWN},
	{SET_MIN_DOWN, DOWN, SET_MIN_DOWN},
	{SET_MIN_DOWN, UP, SET_MIN_UP},

	{SET_MIN, UP, SET_MIN_UP},
	{SET_MIN_UP, UP, SET_MIN_UP},
	{SET_MIN_UP, DOWN, SET_MIN_DOWN},

	{SET_MIN_DOWN, EXIT, SET_TIME_MENU},
	{SET_MIN_UP, EXIT, SET_TIME_MENU},

	{SET_MIN_DOWN, MENU, SET_SEC},
	{SET_MIN_UP, MENU, SET_SEC},

	/*SET +/- SEC*/
	{SET_SEC, DOWN, SET_SEC_DOWN},
	{SET_SEC_DOWN, DOWN, SET_SEC_DOWN},
	{SET_SEC_DOWN, UP, SET_SEC_UP},

	{SET_SEC, UP, SET_SEC_UP},
	{SET_SEC_UP, UP, SET_SEC_UP},
	{SET_SEC_UP, DOWN, SET_SEC_DOWN},

	{SET_SEC_DOWN, EXIT, SET_TIME_MENU},
	{SET_SEC_UP, EXIT, SET_TIME_MENU},

	{SET_SEC_DOWN, MENU, SET_SAVE_TIME},
	{SET_SEC_UP, MENU, SET_SAVE_TIME},

	/*MANUAL_TRIGGER_MENU*/
	{MANUAL_TRIGGER_MENU, UP, SET_BOT_UP},
	{MANUAL_TRIGGER_MENU, DOWN, SET_BOT_DOWN},

	{SET_BOT_UP, UP, SET_BOT_UP},
	{SET_BOT_UP, DOWN, SET_BOT_DOWN},

	{SET_BOT_DOWN, UP, SET_BOT_UP},
	{SET_BOT_DOWN, DOWN, SET_BOT_DOWN},

	{SET_BOT_UP, EXIT, MANUAL_TRIGGER_MENU},
	{SET_BOT_DOWN, EXIT, MANUAL_TRIGGER_MENU},

	{SET_BOT_UP, MENU, MANUAL_MOVING},
	{SET_BOT_DOWN, MENU, MANUAL_MOVING},
	{MANUAL_TRIGGER_MENU, MENU, MANUAL_MOVING},

	{MANUAL_MOVING, EXIT, MANUAL_TRIGGER_MENU},

	/*MOVE HOME*/
	{MOVE_HOME_MENU, MENU, MOVE_TO_HOME},
	{MOVE_TO_HOME, EXIT, MOVE_HOME},

	/*MOVE CURRENT */
	{MOVE_CURRENT_MENU, MENU, MOVE_TO_CURRENT},
	{MOVE_TO_CURRENT, EXIT, MOVE_CURRENT},

	/*VIEW LOG*/
	{VIEW_LOG_MENU, UP, VIEW_LOG_UP},
	{VIEW_LOG_MENU, DOWN, VIEW_LOG_DOWN},

	{VIEW_LOG_UP, UP, VIEW_LOG_UP},
	{VIEW_LOG_UP, DOWN, VIEW_LOG_DOWN},

	{VIEW_LOG_DOWN, UP, VIEW_LOG_UP},
	{VIEW_LOG_DOWN, DOWN, VIEW_LOG_DOWN},

	{VIEW_LOG_DOWN, EXIT, VIEW_LOG},
	{VIEW_LOG_UP, EXIT, VIEW_LOG},

};

const MENU_STATE MenuState[] = {
	{Exe_Idle, 0, SCREEN_IDLE},
	{Exe_Info, Screen_Info_Exec, 0},

	{SET_CLOCK, 0, SCREEN_SET_CLOCK},
	{MANUAL_TRIGGER, 0, SCREEN_MANUAL_TRIGGER},
	{MOVE_HOME, 0, SCREEN_MOVE_HOME},
	{MOVE_CURRENT, 0, SCREEN_MOVE_CURRENT},
	{VIEW_LOG, 0, SCREEN_VIEW_LOG},
	{BACK, 0, SCREEN_BACK},

	{SET_DATE_MENU, get_date_time, SCREEN_SET_DATE_MENU},
	{SET_TIME_MENU, 0, SCREEN_SET_TIME_MENU},


	{MANUAL_TRIGGER_MENU, 0, SCREEN_MANUAL_TRIGGER_MENU},
	{MOVE_HOME_MENU, 0, SCREEN_MOVE_HOME_MENU},
	{MOVE_CURRENT_MENU, 0, SCREEN_MOVE_CURRENT_MENU},
	// {VIEW_LOG_MENU, 0, SCREEN_VIEW_LOG_MENU},

	/* SET DATE TIME */
	{SET_DATE, 0, SCREEN_SET_DATE_MENU},
	{SET_TIME, 0, SCREEN_SET_TIME_MENU},

	{SET_DAY, set_day, 0},
	{SET_MONTH, set_month, 0},
	{SET_YEAR, set_year, 0},

	{SET_DAY_DOWN, day_dec, 0},
	{SET_DAY_UP, day_inc, 0},

	{SET_MONTH_DOWN, month_dec, 0},
	{SET_MONTH_UP, month_inc, 0},

	{SET_YEAR_DOWN, year_dec, 0},
	{SET_YEAR_UP, year_inc, 0},

	{SET_SAVE_DATE, 0, SCREEN_SET_DATE},
	{SAVE_DATE, save_date, 0},

	{SET_HOUR, set_hour, 0},
	{SET_MIN, set_min, 0},
	{SET_SEC, set_sec, 0},

	{SET_HOUR_DOWN, hour_dec, 0},
	{SET_HOUR_UP, hour_inc, 0},

	{SET_MIN_DOWN, min_dec, 0},
	{SET_MIN_UP, min_inc, 0},

	{SET_SEC_DOWN, sec_dec, 0},
	{SET_SEC_UP, sec_inc, 0},

	{SET_SAVE_TIME, 0, SCREEN_SET_TIME},
	{SAVE_TIME, save_time, 0},

	/* SET MANUAL TRIGGER */
	{SET_BOT_UP, set_bottle_up, 0},
	{SET_BOT_DOWN, set_bottle_down, 0},
	{MANUAL_MOVING, manual_moving, 0},

	/* MOVE TO HOME EXE */
	{MOVE_TO_HOME, move_to_home, 0},

	/* MOVE TO CURRENT EXE */
	{MOVE_TO_CURRENT, move_to_current, 0},

	/* viewlog exe */
	{VIEW_LOG_MENU, view_log, 0},
	{VIEW_LOG_UP, view_log_up, 0},
	{VIEW_LOG_DOWN, view_log_down, 0},
};

MENU_STATE_ID State;
void (*pStateFunc)(void);

void MenuInit(void)
{
	RTC_Init();
	HD44780_Init(20, 4);
	LCD_LIGHT(1);
	HD44780_Clear();
	tick_wait_ms(200);
  	State = Exe_Idle;
  	pStateFunc = 0;
  	tick_initTime(&menuTick);
  	tick_initTime(&displayTick);
}

void lcd_display(uint8_t* text[])
{
	uint8_t i;
	for(i = 0; i < 4; i++)
	{
		HD44780_Puts(0, i, text[i]);
	}
}

void MenuDisplay(void)
{
	uint8_t index;
	uint8_t size_arr;
	if(State)
	{	
		size_arr = sizeof(MenuState)/21;
		//INFO("size MenuState: %d \n", size_arr);
		for(index =  0; index < size_arr; index++)
		{
			if(MenuState[index].State == State)
			{
				lcd_display(MenuState[index].pText);
				break;
			}
		}
	}
	for(index = 0; index < size_arr; index++)
	{
		if(MenuState[index].State == State)
		{
			pStateFunc = MenuState[index].pFunc;
			if(pStateFunc)
				pStateFunc();
			else
				pStateFunc = 0;
			break;
		}
		
	}
}

void MenuUpdate(BUTTON_ID key)
{
	uint16_t index;
	MENU_STATE_ID NextState;
	NextState = Init;
	
	if(key)
	{
		//INFO("Key: %d\n", key);
		for(index = 0; index < sizeof(MenuNextState); index++)
		{
			if((MenuNextState[index].State == State) && (MenuNextState[index].Key == key))
			{
				//INFO("State: %d\n", State);
				NextState = MenuNextState[index].NextState;
				//INFO("NextState: %d\n", NextState);
				break;
			}
		}
	
	}

	if(NextState)
	{
		State = NextState;
		//INFO("State new: %d\n", State);
		MenuDisplay();
	}
}

DATE_TIME setDateTime;
uint8_t flag_setDateTime = 0;

void get_date_time(void)
{
	setDateTime.mday = localTime.mday;
	setDateTime.month = localTime.month;
	setDateTime.year = localTime.year;
	setDateTime.hour = localTime.hour;
	setDateTime.min = localTime.min;
	setDateTime.sec = localTime.sec;
}

void set_day(void)
{
	uint8_t buff[10];
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "      ^             ");
}

void set_month(void)
{
	uint8_t buff[10];
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "         ^          ");
}


void set_year(void)
{
	uint16_t buff[10];
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "              ^    ");
}

void set_hour(void)
{
	uint8_t buff[10];
	sprintf(buff, " %02d:%02d:%02d ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "       ^            ");
}

void set_min(void)
{
	uint8_t buff[10];
	sprintf(buff, " %02d:%02d:%02d ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "          ^         ");
}

void set_sec(void)
{
	uint8_t buff[10];
	sprintf(buff, " %02d:%02d:%02d ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	HD44780_Puts(5, 1, buff);
	HD44780_Puts(0, 2, "             ^     ");	
}

void day_dec(void)
{
	//INFO("DAY -- \n");
	if(--setDateTime.mday < 0) setDateTime.mday = 31;
	//INFO("day: %d \n", setDateTime.mday);
}

void day_inc(void)
{
	//INFO("DAY ++ \n");
	if(++setDateTime.mday > 31) setDateTime.mday = 1;
	//INFO("day: %d \n", setDateTime.mday);
}


void month_dec(void)
{
	//INFO("MONTH -- \n");
	if(--setDateTime.month < 0) setDateTime.month = 12;
	//INFO("month: %d \n", setDateTime.month);
}

void month_inc(void)
{
	//INFO("MONTH ++ \n");
	if(++setDateTime.month > 12) setDateTime.month = 1;
	//INFO("month: %d \n", setDateTime.month);
}

void year_dec(void)
{
	//INFO("YEAR -- \n");
	if(--setDateTime.year < 2018) setDateTime.year = 2118;
	//INFO("year: %d \n", setDateTime.year);
}

void year_inc(void)
{
	//INFO("YEAR ++ \n");
	if(++setDateTime.year > 2118) setDateTime.year = 2018;
	//INFO("year: %d \n", setDateTime.year);
}

void hour_dec(void)
{
	//INFO("HOUR -- \n");
	if(--setDateTime.hour < 0) setDateTime.hour = 23;
	//INFO("hour: %d \n", setDateTime.hour);
}

void hour_inc(void)
{
	//INFO("HOUR ++ \n");
	if(++setDateTime.hour > 23) setDateTime.hour = 0;
	//INFO("hour: %d \n", setDateTime.hour);
}

void min_dec(void)
{
	//INFO("MIN -- \n");
	if(--setDateTime.min < 0) setDateTime.min = 59;
	//INFO("min: %d \n", setDateTime.min);
}

void min_inc(void)
{
	//INFO("MIN ++ \n");
	if(++setDateTime.min > 59) setDateTime.min = 0;
	//INFO("min: %d \n", setDateTime.min);
}

void sec_dec(void)
{
	//INFO("SEC -- \n");
	if(--setDateTime.sec < 0) setDateTime.sec = 59;
	//INFO("sec: %d \n", setDateTime.sec);
}

void sec_inc(void)
{
	//INFO("SEC ++ \n");
	if(++setDateTime.sec > 59) setDateTime.sec = 0;
	//INFO("sec: %d \n", setDateTime.sec);
}

void save_time(void)
{
	//INFO("SAVE TIME \n");
	pwr_disable_backup_domain_write_protect();
	setDateTime.hour = setDateTime.hour - 7;
	RTC_Sync(&setDateTime, +7);
	pwr_enable_backup_domain_write_protect();
}

void save_date(void)
{
	//INFO("SAVE DATE \n");
	pwr_disable_backup_domain_write_protect();
	setDateTime.hour = setDateTime.hour - 7;
	RTC_Sync(&setDateTime, +7);
	pwr_enable_backup_domain_write_protect();
}

void view_set_date_time(void)
{
	uint8_t buff[10];
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	HD44780_Puts(5, 1, buff);
	sprintf(buff, " %02d:%02d:%02d ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	HD44780_Puts(5, 2, buff);
}


void view_set_date(void)
{
	uint8_t buff[10];
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	HD44780_Puts(5, 1, buff);
}

void view_set_time(void)
{
	uint8_t buff[10];
	sprintf(buff, "  %02d:%02d:%02d   ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	HD44780_Puts(5, 1, buff);
}

void data_time_update(void)
{
	uint8_t buff[20];
	sprintf(buff, " %02d/%02d/%04d ", localTime.mday, localTime.month, localTime.year);
	HD44780_Puts(4,1, buff);
	sprintf(buff, "%02d:%02d:%02d", localTime.hour, localTime.min, localTime.sec);
	HD44780_Puts(6,2, buff);
	HD44780_Puts(0, 3, "Menu");
	HD44780_Puts(16, 3, "Exit");
}

void Screen_Info_Exec(void)
{
	uint8_t buff[20];
	HD44780_Clear();
	tick_wait_ms(100);
	// sprintf(buff, "BOTTLE: %d", sys.bot);
	HD44780_Puts(0, 0, buff);
	sprintf(buff, "TIME: %02d:%02d:%02d", localTime.hour, localTime.min, localTime.sec);
	HD44780_Puts(0, 1, buff);
	sprintf(buff, "DATE: %02d/%02d/%04d", localTime.mday, localTime.month, localTime.year);
	HD44780_Puts(0, 2, buff);
	HD44780_Puts(16, 3, "Exit");
}

bool display = 0;
void temp_update(void)
{
	uint8_t buff[20];
	HD44780_Puts(0,1, "    TEMPERATURE:    ");
	sprintf(buff, "      %.1f *C       ", tempGet/10);
	HD44780_Puts(0,2, buff);
	HD44780_Puts(0, 3, "Menu");
	HD44780_Puts(16, 3, "Exit");
}

void lcd_refresh(void)
{
	switch(State)
	{
		case Exe_Idle:
			if(!tick_checkMS(&displayTick, 5000))
			{
				display = !display;
			}
			if(display)
				data_time_update();
			else
				temp_update();
			break;
		case SET_DAY_UP: case SET_DAY_DOWN:
			set_day();
			break;
		case SET_MONTH_UP: case SET_MONTH_DOWN:
			set_month();
			break;
		case SET_YEAR_UP: case SET_YEAR_DOWN:
			set_year();
			break;
		case SET_HOUR_UP: case SET_HOUR_DOWN:
			set_hour();
			break;
		case SET_MIN_UP: case SET_MIN_DOWN:
			set_min();
			break;
		case SET_SEC_UP: case SET_SEC_DOWN:
			set_sec();
			break;
		case SET_DATE: case SET_SAVE_DATE:
			view_set_date();
			break;
		case SET_TIME: case SET_SAVE_TIME:
			view_set_time();
			break;
		case SET_TIME_MENU: case SET_DATE_MENU:
			view_set_date_time();
			break;
		case SET_BOT_UP: case SET_BOT_DOWN: case MANUAL_TRIGGER_MENU:
			view_bottle();
			break;
		default:
			break;
	}
}


void set_bottle_up(void)
{
	//INFO("SET BOTTLE UP\n");
	if(++bottle > 25) bottle = 1;
}

void set_bottle_down(void)
{
	//INFO("SET BOTTLE DOWN\n");
	if(--bottle == 0) bottle = 25;
}

void view_bottle(void)
{
	uint8_t buff[2];
	sprintf(buff, "%02d", bottle);
	HD44780_Puts(13, 2, buff);
	HD44780_Puts(0, 3, "Next");
}

void manual_moving(void)
{
	uint8_t buff[6];
	//INFO("moving to bottle \n");
	HD44780_Clear();
	tick_wait_ms(100);
	HD44780_Puts(0, 1, " MOVING TO BOTTLE...");
	HD44780_Puts(16, 3, "Back");
}

void move_to_home(void)
{
	INFO("MOVE TO HOME \n");
}

void move_to_current(void)
{
	INFO("MOVE TO CURRENT \n");
}

uint8_t buf[8];

void view_log_up(void)
{
	//INFO("VIEW LOG UP\n");
	uint8_t buff[10];
	if(++bot_view > 25) bot_view = 1;
	log_load(bot_view, buf);
	tick_wait_ms(100);
	sprintf(buff, "* BOTTLE: %02d", buf[0]);
	HD44780_Puts(0, 0, buff);
	sprintf(buff, "* TIME: %02d:%02d:%02d", buf[1], buf[2], buf[3]);
	HD44780_Puts(0, 1, buff);
	sprintf(buff, "* DATE: %02d/%02d/%04d", buf[4], buf[5], (uint16_t)(buf[6] << 8 | buf[7]));
	HD44780_Puts(0, 2, buff);
	HD44780_Puts(16, 3, "Back");
}

void view_log_down(void)
{
	//INFO("VIEW LOG DOWN \n");
	uint8_t buff[10];
	if(--bot_view == 0) bot_view = 25;
	log_load(bot_view, buf);
	tick_wait_ms(100);
	sprintf(buff, "* BOTTLE: %02d", buf[0]);
	HD44780_Puts(0, 0, buff);
	sprintf(buff, "* TIME: %02d:%02d:%02d", buf[1], buf[2], buf[3]);
	HD44780_Puts(0, 1, buff);
	sprintf(buff, "* DATE: %02d/%02d/%04d", buf[4], buf[5], (uint16_t)(buf[6] << 8 | buf[7]));
	HD44780_Puts(0, 2, buff);
	HD44780_Puts(16, 3, "Back");
}

void view_log(void)
{
	uint8_t buff[10];
	HD44780_Clear();
	tick_wait_ms(100);
	log_load(bot_view, buf);
	tick_wait_ms(100);
	sprintf(buff, "* BOTTLE: %02d", buf[0]);
	HD44780_Puts(0, 0, buff);
	sprintf(buff, "* TIME: %02d:%02d:%02d", buf[1], buf[2], buf[3]);
	HD44780_Puts(0, 1, buff);
	sprintf(buff, "* DATE: %02d/%02d/%04d", buf[4], buf[5], (uint16_t)(buf[6] << 8 | buf[7]));
	HD44780_Puts(0, 2, buff);
	HD44780_Puts(16, 3, "Back");
}

void menu_manager(void)
{	
    if(!tick_checkMS(&menuTick, 10))
    {	
      	MenuUpdate(KeyProcess());
      	lcd_refresh();
    }	
}

