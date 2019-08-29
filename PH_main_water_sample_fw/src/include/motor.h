#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h>
#include "tick.h"
#include "key.h"

#define PWM_FREQ 20000


typedef enum {
	STOP,
	FORWARD,
	REVERSE,
}MOTOR_MODE;

typedef enum {
	LOW_SPEED = 40,
	MEDIUM_SPEED = 50, 
	HIGH_SPEED = 80,
}MOTOR_SPEED;

typedef enum {
	MOTORX = 0,
	MOTORY = 1,
	MOTORZ = 2
}MOTOR_ID;

typedef struct {
	uint32_t portPWM;
	uint16_t pinPWM;
	uint32_t portDIR;
	uint16_t pinDIR;
	uint16_t turn;
	bool direct;
	MOTOR_MODE mode;
	MOTOR_SPEED speed;
} MOTOR_STRUCT;

extern MOTOR_STRUCT motor[3];

#define portDIR_MT GPIOB
#define portPWM_MT GPIOA

#define pinPWM_MT1 GPIO8
#define pinPWM_MT2 GPIO9
#define pinPWM_MT3 GPIO10

#define pinDIR_MT1 GPIO13
#define pinDIR_MT2 GPIO14
#define pinDIR_MT3 GPIO15

void tim1_setup(void);
void motor_init(void);
void motor_setup_mode(MOTOR_ID motor_id, MOTOR_MODE mode);
void motor_setup_speed(MOTOR_ID motor_id, MOTOR_SPEED speed);
void motor_run_forward(MOTOR_ID motor_id);
void motor_run_reverse(MOTOR_ID motor_id);
void motor_stop(MOTOR_ID motor_id);
void motor_fast_stop(MOTOR_ID motor_id);
void motor_run(MOTOR_ID motor_id, MOTOR_MODE mode);

#endif /* __MOTOR_H__ */