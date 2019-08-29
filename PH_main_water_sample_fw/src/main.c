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

#include "lcd_buf.h"
#include "menu_callbacks.h"

#include "pid.h"
#include "move.h"
#include "limitSwitch.h"

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

int main(void)
{
  uint8_t key;
  uint8_t stepx, stepy;
  TIME time;
  TIME tick;
  clock_init();
  tick_init();
  CFG_Load();
  timer_tick_init();
  sys_init();
  cdcacm_init();
  iwd_init();
  tick_initTime(&tick);
  tick_initTime(&time);
  while(1)
  {
    if(!tick_checkMS(&tick, 500))
    {
      lcd_refresh();
    }
    sys_manager();
    if(!tick_checkMS(&time, 1000))
    {

      INFO("CNTX: %d \n", cntx);
      INFO("CNTY: %d \n", cnty);
      INFO("CNTZ: %d \n", cntz);
      //get_samples();
    }
    cdcacm_manage();
    iwdg_reset();
  }
}
static void iwd_init(void)
{
  iwdg_reset();
  iwdg_set_period_ms(5000);
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
