#include "water_pump.h"

WATER_STRUCT water;

void water_init(void) // khoi tao qua trinh lay mau nuoc
{
	motor_setup_mode(MOTORZ, STOP);
	motor_setup_speed(MOTORZ, MEDIUM_SPEED);
	motor_fast_stop(MOTORZ);
	water.flag_charge = 0;
	water.flag_balance = 0;
	water.flag_discharge = 0;
	water.flag_wait_discharge = 0;
	water.flag_complete = 0;
	water.flag_motor = 0;
	O_CTRL1(1); // off pump
	O_CTRL2(1); // off van hut
	O_CTRL3(1); // off van thoi
}

void water_charge_start(void) //bat dau kep ong xa nuoc
{
	motor_run(MOTORZ, FORWARD); // bat motor kep ong
	water.flag_motor = 1; // bat co bao motor kep da chay
}

void water_charge(void) // kep ong xa nuoc ok -> hut nuoc
{
	O_CTRL1(0); // on pump
	O_CTRL2(0); // on van huts
	O_CTRL3(1); // off van thoi
	water.flag_charge =  1; // co bao dang bom nuoc
	water.flag_motor = 1;
	tick_initTime(&water.tick_water_full);
}

void water_balance(void)
{
	//motor_fast_stop(MOTORZ); //stop motor kep ong xa
	//water.flag_motor = 0;
	O_CTRL1(0); // on bom hut 
	O_CTRL2(1); // off hut
	O_CTRL3(0); // on thoi
	water.flag_balance = 1;
	water.flag_charge = 0;
	tick_initTime(&water.tick_balance); // init thoi gian wait can bang
	tick_initTime(&water.tick_wait_off_motor);
}

void water_discharge(void)
{
	O_CTRL1(1); // off pump
	O_CTRL2(0); // on van thoi
	O_CTRL3(0); // on van hut
	// tick_wait_ms(100);
	motor_run(MOTORZ, REVERSE); // mo kep ong nuoc xa nuoc
	water.flag_discharge = 1; // co bao xa nuoc
	water.flag_balance = 0; // xoa co bao can bang
	tick_initTime(&water.tick_discharge); //init lai thoi gian wait xa xong
}

void loop_water(uint8_t input) // check loop process sample water
{

	if((input == IN2_ON) | (!tick_checkMS(&water.tick_water_full, 8000) && water.flag_charge))
	// if((!tick_checkMS(&water.tick_water_full, 8000) && water.flag_charge))
	{
		INFO("Water full \n");
		//motor_fast_stop(MOTORZ);
		tick_wait_ms(50);
		water_balance();
	}

	if(!tick_checkMS(&water.tick_balance, 8000) && water.flag_balance)
	{
		INFO("Wait balance water \n");
		water_discharge();
		water.flag_balance = 0;
	}

	if(!tick_checkMS(&water.tick_wait_off_motor, 2000) && water.flag_balance)
	{
		motor_fast_stop(MOTORZ);
		water.flag_motor = 0;
		tick_initTime(&water.tick_trigger);
	}

	if(!tick_checkMS(&water.tick_discharge, 2000) && water.flag_discharge)
	{
		INFO("Wait discharge water \n");
		motor_fast_stop(MOTORZ);
		water.flag_discharge = 0;
		tick_initTime(&water.tick_wait_discharge);
		water.flag_wait_discharge = 1;
	}

	if(!tick_checkMS(&water.tick_wait_discharge, 100) && water.flag_wait_discharge)
	{
			INFO("DIS_CHARGE OFF MOTOR\n");
			motor_fast_stop(MOTORZ);
			water.flag_wait_discharge = 0;
			water.flag_complete = 1;
			O_CTRL2(1); // on van thoi
			O_CTRL3(0); // on van hut
	}

	sensor_manager();

	if(sensor_get(SS_MT3) > 0.1)
	{
		if(water.flag_motor == 1)
		{
			INFO("KEP ONG OK, bat dau bom nuoc\n");
			water_charge();
			water.flag_motor = 0;
		}

		// if(water.flag_discharge == 1)
		// {
		// 	INFO("DIS_CHARGE OFF MOTOR\n");
		// 	//motor_fast_stop(MOTORZ);
		// 	water.flag_discharge = 0;
		// 	water.flag_complete = 1;
		// 	O_CTRL2(0); // on van thoi
		// 	O_CTRL7(1); // on van hut
		// }
	}

}