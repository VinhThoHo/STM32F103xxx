#ifndef __PID__H__
#define __PID__H__

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h>
#include "tick.h"

#define AUTOMATIC 1
#define MANUAL 0
#define DIRECT 0
#define REVERSE 1
#define P_ON_M 0
#define P_ON_E 1

typedef struct
{
	double Kp;
	double Ki;
	double Kd;
	double dispKp;
	double dispKi;
	double dispKd;
	int direct;
	int pOn;
	unsigned long lastTime;
	double outputSum, lastInput;
	unsigned long sampleTime;
	double outMin, outMax;
	bool inAuto, pOnE;
	double* input;
	double* output;
	double* setPoint;
}PID_STRUCT;


void pid_init(PID_STRUCT* pid, double* input, double* output, double* setPoint, double Kp, double Ki, double Kd, int direct);
void pid_set_output_limits(PID_STRUCT *pid, double min, double max);
void pid_set_control_direction(PID_STRUCT *pid, int dir);
void pid_set_turnings(PID_STRUCT *pid, float Kp, float Ki, float Kd, int pOn);
bool pid_compute(PID_STRUCT *pid);
void pid_initialize(PID_STRUCT *pid);
void pid_set_mode(PID_STRUCT *pid, int mode);
void pid_set_sample_time(PID_STRUCT *pid, int newSampleTime);

// void pid_set_output_limits(double min, double max);
// void pid_set_control_direction(int dir);
// void pid_set_turnings(float Kp, float Ki, float Kd, int pOn);
// bool pid_compute(void);
// void pid_initialize(void);
// void pid_set_mode(int mode);
// void pid_set_sample_time(int newSampleTime);

#endif