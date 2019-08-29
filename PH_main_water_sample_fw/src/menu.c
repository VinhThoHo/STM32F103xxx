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
#include "move.h"
#include "system.h"
#include "menu_callbacks.h"

// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))
menu_t menu1 = { "SET DATETIME", &menu2, &menu5, &sub_menu1_1, NULL, &get_current_datetime_callback};
	menu_t sub_menu1_1 = { "SET DATE", &sub_menu1_2, &sub_menu1_2, NULL, &menu1, &set_date_callback};
	menu_t sub_menu1_2 = { "SET TIME", NULL, &sub_menu1_1, NULL, &menu1, &set_time_callback};

menu_t menu2 = { "SET TEMPERATURE", &menu3, &menu1, &sub_menu2_1, NULL, get_current_temp_config};
	menu_t sub_menu2_1 = { "SET TEMP HIGH", &sub_menu2_2, &sub_menu2_2, NULL, &menu2, set_temperature_h_callback};
	menu_t sub_menu2_2 = { "SET TEMP LOW", NULL, &sub_menu2_1, NULL, &menu2, set_temperature_l_callback};

menu_t menu3 = { "SET BOTTLE", &menu4, &menu2, &sub_menu3_1, NULL, get_current_bottle_callback};
	menu_t sub_menu3_1 = { "BOTTLE NUM", &sub_menu3_2, &sub_menu3_2, NULL, &menu3, set_bottle_sum_callback};
	menu_t sub_menu3_2 = { "RESET BOTTLE", NULL, &sub_menu3_1, NULL, &menu3, set_bottle_reset_callback};

menu_t menu4 = { "VIEW LOG", &menu5, &menu3, &sub_menu4_1, NULL, NULL};
	menu_t sub_menu4_1 = {"VIEW BOTTLE", &sub_menu4_2, &sub_menu4_2, NULL, &menu4, NULL};
	menu_t sub_menu4_2 = {"CLEAR ALL", NULL, &sub_menu4_1, NULL, &menu4, NULL};

menu_t menu5 = { "DISPLAY", NULL, &menu4, &sub_menu5_1, NULL, NULL };
	menu_t sub_menu5_1 = { "BRIGHTNESS", &sub_menu5_2, &sub_menu5_2, NULL, &menu5, disp_brightness_callback};
	menu_t sub_menu5_2 = { "BACKLIGHT", NULL, &sub_menu5_1, &sub_menu5_2_1, &menu5, NULL};
		menu_t sub_menu5_2_1 = {"ON", &sub_menu5_2_2, &sub_menu5_2_2, NULL, &sub_menu5_2, backlight_on_callback};
		menu_t sub_menu5_2_2 = {"OFF", NULL, &sub_menu5_2_1, NULL, &sub_menu5_2, backlight_off_callback};

// menu_t menu6 = { "RESTORE DEFAULTS", NULL, &menu5, NULL, NULL, NULL};

menu_t *currentPointer = &menu1;

uint8_t menu_index;
uint8_t lcd_row_pos;
uint8_t lcd_row_pos_level_1, lcd_row_pos_level_2;

extern char lcd_buf[LCD_ROWS][LCD_COLS];

void menu_refresh(void)
{
	menu_t *temp;
	uint8_t i;
	uint8_t center;

	memset(lcd_buf, '-', LCD_COLS);

	if(currentPointer->parent)
	{
		temp = (currentPointer->parent)->child;
		center = (LCD_COLS - (strlen((temp->parent)->name)))>>1;
		buf_locate(center-1, 0);
		buf_char(' ');
		buf_str((temp->parent)->name);
		buf_char(' ');
	}
	else 
	{
		temp = &menu1;
		center = (LCD_COLS - 4)>>1;
		buf_str_XY(center-1, 0, " MENU ");
	}

	for(i = 0; i != menu_index - lcd_row_pos; i++)
	{
		temp = temp -> next;
	}

	buf_clear_menu();
	for(i = 1; i < LCD_ROWS; i++)
	{
		buf_locate(0, i);
		if(temp == currentPointer)
			buf_char(62);
		else
			buf_char(' ');
		buf_locate(2, i);
		buf_str(temp->name);
		temp = temp->next;
		if(!temp) break;
	}

	lcd_refresh();
}

uint8_t menu_get_index(menu_t *q)
{
	menu_t *temp;
	uint8_t i = 0;
	if(q->parent) temp = (q->parent)->child;
	else temp = &menu1;
	while(temp != q)
	{
		temp = temp -> next;
		i++;
	}
	return i;
}

uint8_t menu_get_level(menu_t *q)
{
	menu_t *temp = q;
	uint8_t i = 0;
	if(!q->parent) return 0;
	while(temp->parent != NULL)
	{
		temp = temp->parent;
		i++;
	}
	return i;
}

void menu_next(void) {

	if (currentPointer->next)
	{

		currentPointer = currentPointer->next;
		menu_index++;
		if (++lcd_row_pos > LCD_ROWS - 2) lcd_row_pos = LCD_ROWS - 2;
	}
	else
	{
		menu_index = 0;
		lcd_row_pos = 0;

		if (currentPointer->parent) currentPointer = (currentPointer->parent)->child;
		else currentPointer = &menu1;
	}

	menu_refresh();

}

void menu_prev(void) {

	currentPointer = currentPointer->prev;

	if (menu_index)
	{
		menu_index--;
		if (lcd_row_pos > 0) lcd_row_pos--;
	}
	else
	{
		menu_index = menu_get_index(currentPointer);

		if (menu_index >= LCD_ROWS - 2) lcd_row_pos = LCD_ROWS - 2;
		else lcd_row_pos = menu_index;
	}

	menu_refresh();
}

void menu_enter(void) {

	if (currentPointer->menu_function) currentPointer->menu_function();

	if (currentPointer->child)
	{

		switch (menu_get_level(currentPointer)) {
			case 0:
				lcd_row_pos_level_1 = lcd_row_pos;
				break;

			case 1:
				lcd_row_pos_level_2 = lcd_row_pos;
				break;
		}

		// switch...case can be replaced by:
		// lcd_row_pos_level[ menu_get_level(currentPointer) ] = lcd_row_pos;

		menu_index = 0;
		lcd_row_pos = 0;

		currentPointer = currentPointer->child;

		menu_refresh();
	}
}

void menu_back(void) {

	if (currentPointer->parent) {

		switch (menu_get_level(currentPointer)) {
			case 1:
				lcd_row_pos = lcd_row_pos_level_1;
				break;

			case 2:
				lcd_row_pos = lcd_row_pos_level_2;
				break;
			}

		currentPointer = currentPointer->parent;
		menu_index = menu_get_index(currentPointer);

		menu_refresh();

	}
}


