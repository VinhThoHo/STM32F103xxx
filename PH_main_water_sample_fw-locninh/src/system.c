#include "system.h"
#include "string.h"
#include "stdio.h"
#include "timerTick.h"
#include "menu.h"
#include "HD44780.h"
#include "debug.h"
#include "input.h"
#include "tick.h"
#include "sensor.h"
#include "config_pool.h"
#include "eeprom.h"
#include "ds18b20.h"
#include "portio.h"
#include "water_pump.h"
#include "movexy.h"
#include "encoder.h"

#define DS_PORT GPIOC
#define DS_PIN GPIO12



SYSTEM_STRUCT sys;
DS18B20_STRUCT ds;

TIME sysTick;
TIME sysDsTime;
float tempGet;
TIME tempTick;


void bot_to_xy(uint8_t bot)
{
  if(bot <= 0 || bot > 25) return;
  sys.x = (bot-1) % 6;
  sys.y = (bot-1) / 6;
}

enum { MACH_MODE_STANDBY, MACH_MODE_MOVEXY, MACH_MODE_SAMPLE };

uint8_t currentMode;
uint8_t bot;

void system_init(void)
{
	MenuInit();
	MenuDisplay();
	port_ctrl_init();
	KeyInit();
	movexy_init();
  	bot = Save_Cfg.bottle;
	encoder_init();
	water_init();
	O_CTRL4(1);
	O_CTRL5(1);
	O_CTRL6(1);
	input_init();
	currentMode = MACH_MODE_STANDBY;
	tick_initTime(&sysTick);
	tick_initTime(&sysDsTime);
	tick_initTime(&tempTick);
	tick_wait_ms(100);
}

uint8_t buff[20];
uint8_t sml_cnt = 0;

void system_manager(void)
{
	uint8_t key;
	uint8_t input;
	uint8_t buff[20];
	
	uint8_t flag_complete;
	
	switch(currentMode)
	{
		case MACH_MODE_STANDBY:
			menu_manager();
			if(!tick_checkMS(&sysTick, 10))
			{
				if((tempGet/10) < 2) O_CTRL4(0);
				if((tempGet/10) > 5) O_CTRL4(1);
				//key = KeyProcess();
				input = input_process();
				if(input !=0)
				{
					INFO("INPUT : %d \n", input);
				}
				
			}
			if(!tick_checkMS(&tempTick, 1000))
			{
				INFO("Nhiet do: %.2f \n", tempGet/10);
				// sprintf(buff, "%.1f", (tempGet/10));
				//get_samples();
				INFO("BOT :%d \n", bot);
			}

			if(input == IN1_ON)
			{
				BUZZER(1);
          		tick_initTime(&port.tick_buzzer);
 				INFO("BOT: %d \n", bot);
 				if(++bot > 24) bot = 1;
				currentMode = MACH_MODE_MOVEXY;
				//INFO("MODE TO (X, Y) : (%d, %d) \n", sys.x, sys.y);
				HD44780_Clear();
				tick_wait_ms(200);
				sprintf(buff, "      BOT: %02d          ", bot);
				HD44780_Puts(0, 0, " * WATER-SAMPLER *  ");
				HD44780_Puts(0, 1, "  MOVE TO BOTTLE:   ");
				HD44780_Puts(0, 2, buff);
			}

			move_motor_home_done(Y_Axis);
      		move_motor_home_done(X_Axis);
      		port_ctrl_manager();
      		ds18b20_manager();
			break;
		case MACH_MODE_MOVEXY:
			bot_to_xy(bot);
			move_coord_xy(sys.x, sys.y);
      		move_motor_home_done(Y_Axis);
      		move_motor_home_done(X_Axis);
      		move_axis_done(X_Axis);
      		move_axis_done(Y_Axis);
      		port_ctrl_manager();
			if((currentState[X_Axis] == Axis_State_Ready) && (currentState[Y_Axis] == Axis_State_Ready))
			{
				currentMode = MACH_MODE_SAMPLE;
				water_charge_start();
				INFO("KEP ONG\n");
				sml_cnt++;
				HD44780_Clear();
				tick_wait_ms(200);
				sprintf(buff, "      BOT: %02d          ", bot);
				HD44780_Puts(0, 0, " * WATER-SAMPLER *  ");
				HD44780_Puts(0, 1, "    SAMPLING...     ");
				HD44780_Puts(0, 2, buff);
			}
			// if(input == IN1_ON)
			// {
			// 	// currentMode = MACH_MODE_SAMPLE;
			// 	// water_charge_start();
			// 	// INFO("KEP ONG\n");
			// }
			break;
		case MACH_MODE_SAMPLE:
			//INFO("MACH MODE SAMPE \n");
			input = input_process();
			// if(input == 3) 
			// {
			// 	INFO("INPUT: %d \n", input);

			// }
			//get_samples();
			loop_water(input);
			sensor_manager();
			port_ctrl_manager();
			if(water.flag_complete == 1) 
			{
				INFO("COMPLETE\n");
				motor_fast_stop(MOTORZ);
				INFO("cnt: %d\n", sml_cnt);

				if(sml_cnt == 1)
				{	
					HD44780_Puts(0, 0, " * WATER-SAMPLER *  ");
					HD44780_Puts(0, 1, "    COMPLETE !!!    ");
					HD44780_Puts(0, 2, "     WAITTING...    ");
				}
				else if(sml_cnt == 2)
				{
					HD44780_Puts(0, 0, " * WATER-SAMPLER *  ");
					HD44780_Puts(0, 1, "    WATER FULL !!!  ");
					HD44780_Puts(0, 2, "     WAITTING...    ");
				}
				if(!tick_checkMS(&water.tick_trigger, 18000))
				{
					//currentMode = MACH_MODE_STANDBY;
					flag_complete = 1;
				}
				if(flag_complete == 1)
				{
					if(sml_cnt == 1)
					{
						currentMode = MACH_MODE_MOVEXY;
					}
					if(sml_cnt == 2)
					{
						Save_Cfg.bottle = bot;
						currentMode = MACH_MODE_STANDBY;
						// HD44780_Clear();
						// tick_wait_ms(200);
						// sprintf(buff, "        %02d          ", bot);
						// HD44780_Puts(0, 0, " * WATER-SAMPLER *  ");
						// HD44780_Puts(0, 1, "       FULL!!!      ");
						// HD44780_Puts(0, 2, buff);
						CFG_Save();
						sml_cnt = 0;
					}
					water.flag_complete = 0;
					flag_complete = 0;
				}
				
			}
			break;
	}
	
}


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
					tempGet = (uint16_t)(ds.temp*10);
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





