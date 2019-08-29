#include <string.h>

#include "clock_rtc.h"
#include "HD44780.h"
#include "lcd_buf.h"
#include "menu.h"
#include "menu_callbacks.h"
#include "portio.h"
#include "debug.h"
#include "config_pool.h"

extern char lcd_buf[LCD_ROWS][LCD_COLS];
extern float temp_limit_H, temp_limit_L;

extern void (*key_next_func)(void);
extern void (*key_prev_func)(void);
extern void (*key_enter_func)(void);
extern void (*key_back_func)(void);

extern float temp_limit_H, temp_limit_L;

DATE_TIME setDateTime;

uint32_t brightness;

static void disp_brightness_refresh(void);

/* ------------ SET TEMP ---------------*/

float tempSet_H, tempSet_L;
bool saveTempSet = 0;
uint8_t bot;
uint8_t bot_sum = 1;

static void disp_temp_refresh(float temp)
{
	uint8_t buff[8];
	uint8_t center;
	center = (LCD_COLS - (10))>>1;
	memset(lcd_buf, '-', LCD_COLS);
	buf_str_XY(center,0, " SET-TEMP ");
	buf_clear_menu();
	sprintf(buff, "%0.1f *C", temp);
	buf_locate(center + 2, 1);
	buf_str(buff);
	buf_str_XY(6, 3, "+");
	buf_str_XY(13, 3, "-");
	buf_str_XY(0, 3, "Save");
	buf_str_XY(16, 3, "Back");
}

void get_current_temp_config(void)
{
	tempSet_H = Save_Cfg.temp_set_high;
	tempSet_L = Save_Cfg.temp_set_low;
}

static void disp_temp_h_next(void)
{
	if(tempSet_H < 0 && tempSet_H > 20) return;
	if(tempSet_H < 20) tempSet_H += 0.5;
	else tempSet_H = 0;
	disp_temp_refresh(tempSet_H);
}

static void disp_temp_h_prev(void)
{
	if(tempSet_H < 0 && tempSet_H > 20) return;
	if(tempSet_H > 0) tempSet_H -= 0.5;
	else tempSet_H = 20;
	disp_temp_refresh(tempSet_H);
}

static void disp_temp_l_next(void)
{
	if(tempSet_L < 0 && tempSet_L > 20) return;
	if(tempSet_L < 20) tempSet_L += 0.5;
	else tempSet_L = 0;
	disp_temp_refresh(tempSet_L);
}

static void disp_temp_l_prev(void)
{
	if(tempSet_L < 0 && tempSet_L > 20) return;
	if(tempSet_L > 0) tempSet_L -= 0.5;
	else tempSet_L = 20;
	disp_temp_refresh(tempSet_L);
}

static void disp_temp_h_enter(void)
{
	Save_Cfg.temp_set_high = tempSet_H;
	saveTempSet = 1;
	buf_str_XY(7, 2, "Saved");
	buf_str_XY(0, 3, "     ");
	temp_limit_H = Save_Cfg.temp_set_high;
	CFG_Save();
}

static void disp_temp_l_enter(void)
{
	Save_Cfg.temp_set_low = tempSet_L;
	saveTempSet = 1;
	buf_str_XY(7, 2, "Saved");
	buf_str_XY(0, 3, "     ");
	temp_limit_L = Save_Cfg.temp_set_low;
	CFG_Save();
}

static void disp_temp_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;
	menu_refresh();
}

void set_temperature_h_callback(void)
{
	key_next_func = disp_temp_h_next;
	key_prev_func = disp_temp_h_prev;
	key_enter_func = disp_temp_h_enter;
	key_back_func = disp_temp_back;
	disp_temp_refresh(tempSet_H);
}

void set_temperature_l_callback(void)
{
	key_next_func = disp_temp_l_next;
	key_prev_func = disp_temp_l_prev;
	key_enter_func = disp_temp_l_enter;
	key_back_func = disp_temp_back;
	disp_temp_refresh(tempSet_L);
}
/* ---------------------------------- */

uint8_t pos_col_time, pos_col_date;

void get_current_datetime_callback(void)
{
	setDateTime = localTime;
	pos_col_time = 0;
	pos_col_date = 0;
}


/* -------- SET TIME ----------------*/

static void disp_time_refresh(void)
{
	uint8_t buff[10];
	uint8_t center;
	center = (LCD_COLS - (10))>>1;
	memset(lcd_buf, '-', LCD_COLS);
	buf_str_XY(center,0, " SET-TIME ");
	buf_clear_menu();
	sprintf(buff, " %02d/%02d/%02d ", setDateTime.hour, setDateTime.min, setDateTime.sec);
	buf_str_XY(center, 1, buff);
	switch(pos_col_time)
	{	
		case 0:
			break;
		case 1:
			buf_str_XY(center + 2, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 2:
			buf_str_XY(center + 5, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 3:
			buf_str_XY(center + 8, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 4:
			buf_str_XY(0, 3, "Save");
			buf_str_XY(16, 3, "Back");
			break;
		case 5:
			buf_str_XY(center + 2, 2, "Saved");
			break;
		default:
			break;
	}
}

static void disp_time_enter(void)
{
	if(pos_col_time < 0 || pos_col_time > 5) return;
	if(++pos_col_time > 5) pos_col_time = 0;
	if(5 == pos_col_time)
	{
		pwr_disable_backup_domain_write_protect();
		setDateTime.hour = setDateTime.hour - 7;
		RTC_Sync(&setDateTime, +7);
		pwr_enable_backup_domain_write_protect();
	}
	disp_time_refresh();
}

static void disp_time_next(void)
{
	switch(pos_col_time)
	{
		case 1:
			if(setDateTime.hour < 24) if(++setDateTime.hour > 23) setDateTime.hour = 0;
			break;
		case 2:
			if(setDateTime.min < 60) if(++setDateTime.min > 59) setDateTime.min = 0;
			break;
		case 3:
			if(setDateTime.sec < 60) if(++setDateTime.sec > 59) setDateTime.sec = 0;
			break;
		default:
			break;
	}
	disp_time_refresh();
}

static void disp_time_prev(void)
{
	switch(pos_col_time)
	{
		case 1:
			if(setDateTime.hour > -1) if(--setDateTime.hour < 0) setDateTime.hour = 23;
			break;
		case 2:
			if(setDateTime.min > -1) if(--setDateTime.min < 0) setDateTime.min = 59;
			break;
		case 3:
			if(setDateTime.sec > -1) if(--setDateTime.sec < 0) setDateTime.sec = 59;
			break;
		default:
			break;
	}
	disp_time_refresh();
}

static void disp_time_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;
	menu_refresh();
}

void set_time_callback(void)
{
	key_enter_func = disp_time_enter;
	key_next_func = disp_time_next;
	key_prev_func = disp_time_prev;
	key_back_func = disp_time_back;
	disp_time_refresh();
}

/* --------------------------------- */

/* ------------- SET DATE ------------ */

static void disp_date_refresh(void)
{
	uint8_t buff[12];
	uint8_t center;
	center = (LCD_COLS - (10))>>1;
	memset(lcd_buf, '-', LCD_COLS);
	buf_str_XY(center,0, " SET-DATE ");
	buf_clear_menu();
	sprintf(buff, "%02d/%02d/%04d", setDateTime.mday, setDateTime.month, setDateTime.year);
	buf_str_XY(center, 1, buff);
	switch(pos_col_date)
	{	
		case 0:
			break;
		case 1:
			buf_str_XY(center + 1, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 2:
			buf_str_XY(center + 4, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 3:
			buf_str_XY(center + 9, 2, "^");
			buf_str_XY(center + 1, 3, "+");
			buf_str_XY(center + 8, 3, "-");
			break;
		case 4:
			buf_str_XY(0, 3, "Save");
			buf_str_XY(16, 3, "Back");
			break;
		case 5:
			buf_str_XY(center + 2, 2, "Saved");
			break;
		default:
			break;
	}
}

static void disp_date_enter(void)
{
	if(pos_col_date < 0 || pos_col_date > 5) return;
	if(++pos_col_date > 5) pos_col_date = 0;
	if(5 == pos_col_date)
	{
		pwr_disable_backup_domain_write_protect();
		setDateTime.hour = setDateTime.hour - 7;
		RTC_Sync(&setDateTime, +7);
		pwr_enable_backup_domain_write_protect();
	}
	disp_date_refresh();
}

static void disp_date_next(void)
{
	uint8_t temp;
	switch(pos_col_date)
	{
		case 1:
			if(setDateTime.mday < (31 + 1))
			{
				if(2 == setDateTime.month) temp = 28;
				if(2 == setDateTime.month && (setDateTime.year % 4 == 0)) temp = 29;
				if(4 == setDateTime.month || 6 == setDateTime.month || 9 == setDateTime.month || 11 == setDateTime.month) temp = 30;
				if(1 == setDateTime.month || 3 == setDateTime.month || 5 == setDateTime.month || 7 == setDateTime.month || 8 == setDateTime.month || 10 == setDateTime.month || 12 == setDateTime.month) temp = 31;
				if(++setDateTime.mday > temp) setDateTime.mday = 1;
			}
			break;
		case 2:
			if(setDateTime.month < (12 + 1)) if(++setDateTime.month > 12) setDateTime.month = 1;
			break;
		case 3:
			if(setDateTime.year < (2050 + 1)) if(++setDateTime.year > 2050) setDateTime.year = 2018;
			break;
		default:
			break;
	}
	disp_date_refresh();
}

static void disp_date_prev(void)
{
	uint8_t temp;
	switch(pos_col_date)
	{
		case 1:
			if(2 == setDateTime.month) temp = 28;
			if(2 == setDateTime.month && (setDateTime.year % 4 == 0)) temp = 29;
			if(4 == setDateTime.month || 6 == setDateTime.month || 9 == setDateTime.month || 11 == setDateTime.month) temp = 30;
			if(1 == setDateTime.month || 3 == setDateTime.month || 5 == setDateTime.month || 7 == setDateTime.month || 8 == setDateTime.month || 10 == setDateTime.month || 12 == setDateTime.month) temp = 31;
			if(setDateTime.mday > 0) if(--setDateTime.mday < 1) setDateTime.mday = temp;
			break;
		case 2:
			if(setDateTime.month > 0) if(--setDateTime.month < 1) setDateTime.month = 12;
			break;
		case 3:
			if(setDateTime.year > (2018 - 1)) if(--setDateTime.year < 2018) setDateTime.year = 2050;
			break;
		default:
			break;
	}
	disp_date_refresh();
}

static void disp_date_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;
	menu_refresh();
}

void set_date_callback(void)
{
	key_enter_func = disp_date_enter;
	key_next_func = disp_date_next;
	key_prev_func = disp_date_prev;
	key_back_func = disp_date_back;
	disp_date_refresh();
}

/* --------------------------------- */

/* -------------BACKLIGHT---------------*/
void backlight_on_callback(void)
{
	LCD_LIGHT(1);
}

void backlight_off_callback(void)
{
	LCD_LIGHT(0);
}
/* -------------------------------------*/

static void disp_brightness_next(void)
{

	//if(TIM4->CCR1 < 55000) TIM4->CCR1 += 5500;
	if(brightness < 55000) brightness += 5500;
	//INFO("brightness: %d \n", brightness);
	disp_brightness_refresh();

}

static void disp_brightness_prev(void)
{

	//if(TIM4->CCR1 >= 5500) TIM4->CCR1 -= 5500;
	if(brightness >= 5500) brightness -= 5500;
	//INFO("brightness: %d \n", brightness);
	disp_brightness_refresh();
}

static void disp_brightness_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;

	menu_refresh();
}

static void disp_brightness_refresh(void)
{

	// uint8_t percent = (TIM4->CCR1)/550;
	uint8_t percent =  brightness/550;

	buf_str_XY(0,0, "---- BRIGHTNESS ----");
	buf_clear_menu();

	memset(&lcd_buf[2][8],0xff, percent/10);
	memset(&lcd_buf[2][8+percent/10], '-', 10-percent/10);

	buf_locate(2,2);
	buf_int(percent);
	buf_locate(5,2);
	buf_char('%');
}

void disp_brightness_callback(void)
{

	key_next_func = disp_brightness_next;
	key_prev_func = disp_brightness_prev;
	key_enter_func = NULL;
	key_back_func = disp_brightness_back;

	disp_brightness_refresh();
}

void disp_bottle_sum_refresh(uint8_t bot)
{
	uint8_t buff[8];
	uint8_t center;
	center = (LCD_COLS - (10))>>1;
	memset(lcd_buf, '-', LCD_COLS);
	buf_str_XY(center,0, " SET-BOT-SUM ");
	buf_clear_menu();
	sprintf(buff, "%d", bot);
	buf_locate(center + 5, 1);
	buf_str(buff);
	buf_str_XY(6, 3, "+");
	buf_str_XY(13, 3, "-");
	buf_str_XY(0, 3, "Save");
	buf_str_XY(16, 3, "Back");
}

void get_current_bottle_callback(void)
{
	bot = Save_Cfg.bottle_curr;
	bot_sum = Save_Cfg.bottle_sum;
}

static void disp_bottle_sum_next(void)
{
	if(bot_sum < 1 && bot_sum > 2) return;
	if(bot_sum < 2) bot_sum += 1;
	else bot_sum = 1;
	disp_bottle_sum_refresh(bot_sum);
}

static void disp_bottle_sum_prev(void)
{
	if(bot_sum < 1 && bot_sum > 2) return;
	if(bot_sum > 1) bot_sum -= 1;
	else bot_sum = 2;
	disp_bottle_sum_refresh(bot_sum);
}

static void disp_bottle_sum_enter(void)
{
	Save_Cfg.bottle_sum = bot_sum;
	buf_str_XY(7, 2, "Saved");
	buf_str_XY(0, 3, "     ");
	//bot_sum = Save_Cfg.bot_sum;
	CFG_Save();
}

static void disp_bottle_sum_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;
	menu_refresh();
}

void set_bottle_sum_callback(void)
{
	key_next_func = disp_bottle_sum_next;
	key_prev_func = disp_bottle_sum_prev;
	key_enter_func = disp_bottle_sum_enter;
	key_back_func = disp_bottle_sum_back;
	disp_bottle_sum_refresh(bot_sum);
}

static void disp_bottle_reset_refresh(uint8_t bot)
{
	uint8_t buff[8];
	uint8_t center;
	center = (LCD_COLS - (10))>>1;
	memset(lcd_buf, '-', LCD_COLS);
	buf_str_XY(center,0, " RESET-BOT ");
	buf_clear_menu();
	sprintf(buff, "%d", bot);
	buf_locate(center + 5, 1);
	buf_str(buff);
	buf_str_XY(0, 3, "Reset");
	buf_str_XY(16, 3, "Back");
}

static void disp_bottle_reset_enter(void)
{
	bot = 0;
	Save_Cfg.bottle_curr = bot;
	disp_bottle_reset_refresh(bot);
	CFG_Save();
}

static void disp_bottle_reset_back(void)
{
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;
	menu_refresh();
}

void set_bottle_reset_callback(void)
{
	key_enter_func = disp_bottle_reset_enter;
	key_back_func = disp_bottle_reset_back;
	disp_bottle_reset_refresh(bot);
}


void restore_default_callback(void)
{
	INFO("RESTORE CONFIG\n");
}
