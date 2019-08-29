#include "pid.h"
#include "debug.h"

void pid_init(PID_STRUCT *pid, double* input, double* output, double* setPoint, double Kp, double Ki, double Kd, int direct)
{
	pid->output = output;
	pid->input = input;
	pid->setPoint = setPoint;
	pid->inAuto = 0;
	pid_set_output_limits(pid, 0, 50);
	pid->sampleTime = 100;
	pid_set_control_direction(pid, direct);
	pid_set_turnings(pid, Kp, Ki, Kd, P_ON_E);
	pid->lastTime = tick_get() - pid->sampleTime;
}

// void pid_init(PID_STRUCT *pid, double Kp, double Ki, double Kd, int dir)
// {
// 	pid->input = 0;
// 	pid->output = 0;
// 	pid->setPoint = 0;
// 	pid->inAuto = 0;
// 	pid_set_output_limits(pid, 0, 50);
// 	pid->sampleTime = 100;
// 	pid_set_control_direction(pid, dir);
// 	pid_set_turnings(pid, Kp, Ki, Kd, P_ON_E);
// 	pid->lastTime = tick_get() - pid->sampleTime;
// }

void pid_set_output_limits(PID_STRUCT *pid, double min, double max)
{
	if(min >= max) return;
	pid->outMin = min;
	pid->outMax = max;

	if(pid->inAuto)
	{
		if(*pid->output > pid->outMax) *pid->output = pid->outMax;
		else if(*pid->output < pid->outMin) *pid->output = pid->outMin;

		if(pid->outputSum > pid->outMax) pid->outputSum = pid->outMax;
		else if(pid->outputSum < pid->outMin) pid->outputSum = pid->outMin;
	}
}

void pid_set_control_direction(PID_STRUCT *pid, int dir)
{
	if(pid->inAuto && dir != pid->direct)
	{
		pid->Kp = (0 - pid->Kp);
		pid->Ki = (0 - pid->Ki);
		pid->Kd = (0 - pid->Kd);
	}
	pid->direct = dir;
}

void pid_set_turnings(PID_STRUCT *pid, float Kp, float Ki, float Kd, int pOn)
{
	if(pid->Kp < 0 || pid->Ki < 0 || pid->Kd < 0) return;
	pid->pOn = pOn;
	pid->pOnE = pOn == P_ON_E;
	pid->dispKp = Kp;
	pid->dispKi = Ki;
	pid->dispKd = Kd;
	float sampleTimeInSec = ((float)pid->sampleTime/1000);
	pid->Kp = Kp;
	pid->Ki = Ki * sampleTimeInSec;
	pid->Kd = Kd / sampleTimeInSec;
	if(pid->direct == REVERSE)
	{
		pid->Kp = (0 - pid->Kp);
		pid->Ki = (0 - pid->Ki);
		pid->Kd = (0 - pid->Kd);
	}
}

bool pid_compute(PID_STRUCT *pid)
{
	if(!pid->inAuto) return 0;
	uint32_t now = tick_get();
	uint32_t timeChange = (now - pid->lastTime);
	if(timeChange >=  pid->sampleTime)
	{
		double input = *pid->input;
		double error = *pid->setPoint - input;
		double dInput = (input - pid->lastInput);
		pid->outputSum+= (pid->Ki * error);
		if(!pid->pOnE) pid->outputSum-= pid->Kp * dInput;
		INFO("outputsum: %0.2f \n", pid->outputSum);
		INFO("input: %0.2f \n", pid->input);
		//INFO("setPoint: %0.2f \n", pid.setPoint);
		if(pid->outputSum > pid->outMax) 
		{ 
			//INFO("outputsum > outputMax\n"); 
			pid->outputSum = pid->outMax;
		}
		else if(pid->outputSum < pid->outMin) 
		{ 	
			//INFO("outputsum <  outputmax \n"); 
			pid->outputSum = pid->outMin;
		}

		double output;
		if(pid->pOnE) output = pid->Kp * error;
		else output = 0;
		output += pid->outputSum - pid->Kd * dInput;
		if(output > pid->outMax) output = pid->outMax;
		else if(output < pid->outMin) output = pid->outMin;
		//INFO("output: %.3f\n", output);
		*pid->output = output;
		pid->lastInput = input;
		pid->lastTime = now;
		return 1;
	}
	else
		return 0;
}

void pid_initialize(PID_STRUCT *pid)
{
	pid->outputSum = *pid->output;
	pid->lastInput = *pid->input;
	if(pid->outputSum > pid->outMax) pid->outputSum = pid->outMax;
	else if(pid->outputSum < pid->outMin) pid->outputSum = pid->outMin;
}

void pid_set_mode(PID_STRUCT *pid, int mode)
{
	bool newAuto = (mode == AUTOMATIC);
	if(newAuto && !pid->inAuto)
		pid_initialize(pid);
	pid->inAuto = newAuto;
}

void pid_set_sample_time(PID_STRUCT *pid, int newSampleTime)
{
	if(newSampleTime > 0)
	{
		uint32_t ratio = (uint32_t)newSampleTime/(uint32_t)pid->sampleTime;
		pid->Ki *= ratio;
		pid->Kd /= ratio;
		pid->sampleTime = (uint32_t)newSampleTime;
	}
}
