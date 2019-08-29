#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h>
#include "cdcacm.h"
#include "tick.h"
#include "HD44780.h"
#include "key.h"
#include "menu.h"
#include "timerTick.h"
#include "motor.h"
#include "debug.h"
#include "sensor.h"
#include "portio.h"
#include "serial.h"
#include "ds18b20.h"
#include "input.h"
#include "clock_rtc.h"
#include "logo.h"
#include "system.h"
#include "config_pool.h"
#include "eeprom.h"
#include "clock_rtc.h"
#include "encoder.h"

static void clock_init(void);
static void iwd_init(void);

/* Output for printf */
int _write(int file, char *ptr, int len);
int _write(int file, char *ptr, int len)
{
  int i;
  if(file == STDOUT_FILENO || file == STDERR_FILENO)
  {
    for(i = 0; i < len; i++)
    {
      if(ptr[i] == '\n')
      {
        cdcacm_input('\r');
      }
      cdcacm_input(ptr[i]);
    }
    return i;
  }
  errno = EIO;
  return -1;
}

typedef struct
{
  uint8_t x;
  uint8_t y;
} xy_struct;

xy_struct xy;

void bottle_to_xy(uint8_t bot)
{
  if(bot <= 0 || bot > 25) return;
  xy.x = (bot-1) % 5;
  xy.y = (bot-1) / 5;
}

int main(void)
{
  uint8_t bottle = 1;
  uint8_t key;
  TIME time;
  TIME tick;
  clock_init();
  tick_init();
  //CFG_Load();
  timer_tick_init();
  // MenuInit();
  // MenuDisplay();
  HD44780_Init(20, 4);
  LCD_LIGHT(1);
  HD44780_Puts(0, 0, "TEST MOTOR");
  motor_init();
  motor_setup_mode(MOTORX, STOP);
  motor_setup_speed(MOTORX, MEDIUM_SPEED);
  motor_fast_stop(MOTORX);

  motor_setup_mode(MOTORY, STOP);
  motor_setup_speed(MOTORY, MEDIUM_SPEED);
  motor_fast_stop(MOTORY);

  motor_setup_mode(MOTORZ, STOP);
  motor_setup_speed(MOTORZ, MEDIUM_SPEED);
  motor_fast_stop(MOTORZ);

  motor_run(MOTORX, FORWARD);
  motor_run(MOTORY, FORWARD);
  motor_run(MOTORZ, FORWARD);
  motor_run(MOTORX, REVERSE);
  motor_run(MOTORY, REVERSE);
  motor_run(MOTORZ, REVERSE);


  //RTC_Init();
  //KeyInit();
  //movexy_init();
  //system_init();

  //input_init(); 
  //movexy_init();
  //encoder_init();
  cdcacm_init();
  //move_motor_home(X_Axis);
  //move_motor_home(Y_Axis);

  tick_initTime(&tick);
  tick_initTime(&time);
  // tick_wait_ms(100);
  while(1)
  {
    if(!tick_checkMS(&time, 1000))
    {
      //get_samples();
      //HD44780_Puts(1, 1, "ABC");
      INFO("CNTX: %d \n", cntx);
      INFO("CNTY: %d \n", cnty);
    }
    if(!tick_checkMS(&tick, 10))
    {
      //system_manager();
    }
    // if(!tick_checkMS(&time, 1000))
    // {
    //   get_samples();
    //   //INFO("Toa do hien tai (x, y): (%d, %d)\n", movexy.x, movexy.y);    
    // }
    if(!tick_checkMS(&tick, 10))
    {
      //system_manager();
      key = KeyProcess();
      if(key == MENU)
      { 
        HD44780_Puts(0, 1, "RUN FORWARD");
        motor_run(MOTORX, FORWARD);
        motor_run(MOTORY, FORWARD);
        motor_run(MOTORZ, FORWARD);
      }
      if(key == UP)
      { 
        HD44780_Puts(0, 1, "RUN REVERSE");
        motor_run(MOTORX, REVERSE);
        motor_run(MOTORY, REVERSE);
        motor_run(MOTORZ, REVERSE);
      }
      if(key == DOWN)
      {
        HD44780_Puts(0, 1, "STOP        ");
        motor_fast_stop(MOTORX);
        motor_fast_stop(MOTORY);
        motor_fast_stop(MOTORZ);
      }
      if(key == EXIT)
      {

      }
      // move_coord_xy(xy.x, xy.y);
      // move_motor_home_done(Y_Axis);
      // move_motor_home_done(X_Axis);
      // move_motor_home_done(Z_Axis);
      // move_axis_done(X_Axis);
      // move_axis_done(Y_Axis);
      // port_ctrl_manager();
    }
    cdcacm_manage();
    iwdg_reset();
  }
}
static void iwd_init(void)
{
  iwdg_reset();
  iwdg_set_period_ms(1000);
  iwdg_start();
}

static void clock_init(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_TIM3);
  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, 0);
}
