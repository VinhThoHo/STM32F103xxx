#include "system.h"
#include "string.h"
#include "stdio.h"
#include "menu.h"
#include "HD44780.h"
#include "debug.h"
#include "input.h"
#include "sensor.h"
#include "config_pool.h"
#include "eeprom.h"
#include "ds18b20.h"
#include "portio.h"
#include "water_pump.h"
#include "move.h"
#include "encoder.h"

#include "lcd_buf.h"
#include "menu.h"
#include "menu_callbacks.h"
#include "move.h"

#define DS_PORT GPIOC
#define DS_PIN GPIO12

SYS_STRUCT sys;
DS18B20_STRUCT ds;
extern float tempSet;
void ds18b20_manager(void);
void bot_to_xy(uint8_t bot);

void (*key_next_func)(void) = &menu_next;
void (*key_prev_func)(void) = &menu_prev;
void (*key_enter_func)(void) = &menu_enter;
void (*key_back_func)(void) = &menu_back;

extern menu_t *currentPointer;
extern uint8_t menu_index;
extern uint8_t lcd_row_pos;
float temp_limit_H, temp_limit_L;

void date_time_display(void)
{
	uint8_t buff[20];
	sprintf(buff, " %02d/%02d/%04d ", localTime.mday, localTime.month, localTime.year);
	buf_str_XY(4, 1, buff);
	sprintf(buff, "%02d:%02d:%02d", localTime.hour, localTime.min, localTime.sec);
	buf_str_XY(6, 2, buff);
}

void temp_display(void)
{
	uint8_t buff[20];
	buf_str_XY(4, 1, "TEMPERATURE:");
	sprintf(buff, "      %.1f *C       ", sys.tempGet/10);
	buf_str_XY(0, 2, buff);
}

static void home_screen(void)
{
	uint8_t center, right;
	buf_clear_menu();
	memset(lcd_buf, '-', LCD_COLS);
	center = (LCD_COLS - 13)>>1;
	right = (LCD_COLS - 4);
	buf_str_XY(center-1, 0, " WATER-SAMPLER ");
	buf_str_XY(0, 3, "Menu");
	buf_str_XY(right, 3, "Info");
	if(!tick_checkMS(&sys.time_screen, 10000))
	{
		sys.disp = !sys.disp;
	}
	if(sys.disp)
		date_time_display();
	else
		temp_display();
}

static void sampler_screen(void)
{
	buf_clear_menu();
  	buf_str_XY(0, 0, "----- SAMPLING ----");
}

static void move_screen(void)
{
	buf_clear_menu();
	buf_str_XY(0, 0, "----- MOVING -----");
}

void bot_to_xy(uint8_t bot)
{
	if(bot == 0)
	{
		sys.x = sys.y = 0;
	}
	else
	{
		sys.x = (bot - 1) % 6 + 1;
 	 	sys.y = (bot - 1) / 6 + 1;
 	}
}

void sys_init(void)
{
	RTC_Init();
	port_ctrl_init();
	KeyInit();
	HD44780_Init(20, 4);
	LCD_LIGHT(1);
	sys.state = STATE_IDLE;
	sys.disp = 0;
	sys.bot = Save_Cfg.bottle_curr;
	sys.num_bot = Save_Cfg.bottle_sum;
	//sys.num_bot = 0;
	sys.flag_move = 0;
	temp_limit_H = Save_Cfg.temp_set_high;
	temp_limit_L = Save_Cfg.temp_set_low;
	movexy_init();
	water_init();
	O_CTRL4(0);
	O_CTRL5(0);
	home_screen();
	tick_initTime(&sys.tick);
	tick_initTime(&sys.time_screen);
	move_motor_home(X_Axis);
	move_motor_home(Y_Axis);
}

uint8_t flag_complete;
uint8_t sml_cnt;

void sys_manager(void)
{	
	uint8_t key, input;
	switch(sys.state)
	{
		case STATE_IDLE:
            key = KeyProcess();
            input = input_process();
            if(key == MENU)
            {
              menu_refresh();
              sys.state = STATE_CONFIG;
              tick_initTime(&sys.timeout);
            }
            else if(input == IN1_ON)
            {
              tick_initTime(&sys.timeout);
              if(++sys.bot > 24) sys.bot = 1; 
              bot_to_xy(sys.bot);
              INFO("X, Y : %d, %d \n", sys.x, sys.y);
              sys.state = STATE_MOVEXY;
              sys.flag_move = 1;
              sys.num_bot = Save_Cfg.bottle_sum;
              INFO("So chai lay mau: %d \n", sys.num_bot);
            }
            else
              home_screen();
          	if(!tick_checkMS(&sys.tick, 100))
          	{
          		//INFO("sys.tempGet: %.1f \n", sys.tempGet/10);
          		// INFO("temp_limit_H: %f \n", temp_limit_H);
          		// INFO("temp_limit_L: %f \n", temp_limit_L);
          		if(sys.tempGet/10 < temp_limit_L) O_CTRL4(1);
          		if(sys.tempGet/10 > temp_limit_H) O_CTRL4(0);
          	}
            move_motor_home_done(X_Axis);
            //move_motor_step_done(X_Axis);
    		move_motor_home_done(Y_Axis);
    		//move_motor_step_done(Y_Axis);
    		sensor_manager();
    		port_ctrl_manager();
    		ds18b20_manager();
            break;
        case STATE_CONFIG:
            key =  KeyProcess();
            if(key == MENU)
            {
              if(key_enter_func) (*key_enter_func)();
              tick_initTime(&sys.timeout);
            }
            if(key == UP)
            {
              if(key_next_func) (*key_next_func)();
              tick_initTime(&sys.timeout);
            }
            if(key == DOWN)
            {
              if(key_prev_func) (*key_prev_func)();
              tick_initTime(&sys.timeout);
            }
            if(key == EXIT)
            {
              if(menu_get_level(currentPointer) == 0)
              {
              	lcd_row_pos = 0;
              	menu_index = 0;
              	currentPointer = &menu1;
                sys.state = STATE_IDLE;
                key_next_func = menu_next;
				key_prev_func = menu_prev;
				key_enter_func = menu_enter;
				key_back_func = menu_back;
              }
              else
              {
                if(key_back_func) (*key_back_func)();
                tick_initTime(&sys.timeout);
              }
            }
            if(!tick_checkMS(&sys.timeout, 10000))
            {
            	lcd_row_pos = 0;
              	menu_index = 0;
            	currentPointer = &menu1;
              	sys.state = STATE_IDLE;
              	key_next_func = menu_next;
				key_prev_func = menu_prev;
				key_enter_func = menu_enter;
				key_back_func = menu_back;
            }
            break;
        case STATE_MOVEXY:
            move_screen();
            if(!tick_checkMS(&sys.timeout, 60000) && sys.flag_move)
            {
              sys.state = STATE_IDLE;
              sys.flag_move = 0;
            }
            move_coord_xy(sys.x, sys.y);
            move_motor_step_done(X_Axis);
            move_motor_home_done(X_Axis);
            move_motor_step_done(Y_Axis);
            move_motor_home_done(Y_Axis);
            port_ctrl_manager();
            if((movex.state == Axis_State_Ready) && (movey.state == Axis_State_Ready))
            {
            	//INFO("SWITCH STATE SAMPLE WATER \n");
            	sys.state = STATE_SAMPLER;
            	//sys.state = STATE_IDLE;
            	water_charge_start();
            	sml_cnt++;
            	tick_initTime(&sys.wait_discharge_time);
            }
            break;
        case STATE_SAMPLER:
        	sampler_screen();
        	input = input_process();
        	loop_water(input);
			port_ctrl_manager();
			if(!tick_checkMS(&sys.wait_discharge_time, 15000) && water.flag_complete)
			{
				motor_fast_stop(MOTORZ);
				// sys.state = STATE_IDLE;
				if(sml_cnt > 2)
				{
					if((--sys.num_bot) == 0)
					{
						Save_Cfg.bottle_curr = sys.bot;
						sys.state = STATE_IDLE;
						water.flag_complete = 0;
						// sml_cnt = 1;
						CFG_Save();
					}
					else
					{
						sys.bot++;
						if(sys.bot > 24) sys.bot = 1;
						bot_to_xy(sys.bot);
						sys.state = STATE_MOVEXY;
						sys.flag_move = 0;
						water.flag_complete = 0;
						// sml_cnt = 1;
					}
					sml_cnt = 0;
				}
				else
				{
					sys.state = STATE_MOVEXY;
					water.flag_complete = 0;
				}
				
			}
        	break;
        default:
            break;
	}
}

TIME sysDsTime;

void ds18b20_manager(void)
{
	uint8_t sensor;
	switch(ds.status)
	{
		case DS_INIT:
			tick_initTime(&sysDsTime);
			DS18B20_Init(&ds, DS_PORT, DS_PIN);
			ds.status = DS_SEARCH;
			break;
		case DS_SEARCH:
			//asm volatile("cpsid i");
			sensor = DS18B20_ScanBus(&ds);
			INFO("sensor: %d \n", sensor);
			//asm volatile("cpsie i");
			if(sensor != 0)
			{
				DS18B20_GetROM(&ds, 1, &ds.id[0]);
				if((ds.id[0] == 0xFF)&&(ds.id[1] == 0xFF)&&(ds.id[2] == 0xFF)){
					DS18B20_Init(&ds, DS_PORT, DS_PIN);
					//INFO("\r\nDS18B20 id: %02x %02x %02x %02x %02x %02x %02x %02x", ds.id[0],ds.id[1],ds.id[2],ds.id[3],ds.id[4],ds.id[5],ds.id[6],ds.id[7]);
					//INFO("\r\nDS18B20 error id 0xff reinit...");
				}
				else{
					ds.status = DS_WAIT;
					DS18B20_StartConversion(&ds, 1);
					tick_initTime(&sysDsTime);
				}
			}	
			else
			{
				if(!tick_checkMS(&sysDsTime, 5000)){
					DS18B20_Init(&ds, DS_PORT, DS_PIN);
					//INFO("\r\nDS18B20 error scanbus reinit...");
				}
			}
			break;
		case DS_READ:
			if(!tick_checkMS(&sysDsTime, 750))
			{
				asm volatile("cpsid i");
				DS18B20_GetTemp(&ds, 0);
				asm volatile("cpsie i");
				ds.status = DS_WAIT;
				if(ds.state == DS_OK){
					//tempGet = (uint16_t)(ds.temp*10);
					sys.tempGet = (uint16_t)(ds.temp*10);
					//INFO("\r\nDS18B20 temp: %d", tempGet);
				}
				else{
					ds.status = DS_WAIT;
				}
			}
			break;
		case DS_WAIT:
			if(!tick_checkMS(&sysDsTime, 2000))
			{
				ds.status = DS_READ;
				DS18B20_StartConversion(&ds, 0);
			}
			break;
	}
}

