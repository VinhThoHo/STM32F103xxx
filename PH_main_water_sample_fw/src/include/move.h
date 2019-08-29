#ifndef __MOVE__H__
#define __MOVE__H__

#include "motor.h"
#include "input.h"
#include "sensor.h"
#include "timerTick.h"
#include "debug.h"
#include "tick.h"
#include "encoder.h"
#include "pid.h"

typedef enum {
	X_Axis = 1,
	Y_Axis = 2,
	Z_Axis = 3
}AXIS;

typedef enum {
	MOVE_DIR_X = 1,
	MOV_DIR_Y = 2
}DIR;

typedef enum {
	Axis_State_Ready,
	Axis_State_Moving,
	Axis_State_Homming,
	Axis_State_Move_Done,
	Axis_State_Home_Done
}AXIS_STATE;

typedef struct {
	TIME tick;
	AXIS_STATE state;
	uint8_t pos;
	bool dir;
	int32_t cnt;
} MOVE_STRUCT;

extern MOVE_STRUCT movex;
extern MOVE_STRUCT movey;
extern PID_STRUCT pid_x, pid_y;

void movexy_init(void);
bool move_motor_home(uint8_t axis);
void move_motor_home_done(uint8_t axis);
void move_motor_step(uint8_t axis, uint8_t step, uint8_t dir, int16_t turn);
void move_motor_step_done(uint8_t axis);
bool move_coord_xy(int8_t x, int8_t y);

#endif  /*__MOVEXY__H__*/