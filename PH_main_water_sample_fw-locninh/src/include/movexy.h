#ifndef __MOVEXY__H__
#define __MOVEXY__H__

#include "motor.h"
#include "input.h"
#include "sensor.h"
#include "timerTick.h"
#include "debug.h"
#include "tick.h"
#include "encoder.h"

enum {
	X_Axis = 1,
	Y_Axis = 2,
	Z_Axis = 3
};

enum {
	MOVE_DIR_X = 1,
	MOV_DIR_Y = 2
};

enum {
	Axis_State_Ready,
	Axis_State_Moving,
	Axis_State_Homming
};

typedef struct {
	TIME tick_home;
	uint8_t x;
	uint8_t pos_x;
	uint8_t y;
	uint8_t pos_y;
	uint8_t x_dir;
	uint8_t y_dir;
	uint8_t stepx;
	uint8_t stepy;
	int32_t turnx;
	int32_t turny;
	uint8_t y_offset;
	uint8_t x_offset;
} MOVEXY_STRUCT;

extern MOVEXY_STRUCT movexy;
extern uint8_t currentState[2];

void movexy_init(void);
bool move_motor_home(uint8_t axis);
void move_motor_home_done(uint8_t axis);

bool move_step(uint8_t axis, uint8_t step, uint8_t dir);
void move_axis_done(uint8_t axis);

bool move_coord_xy(int8_t x, int8_t y);


#endif  /*__MOVEXY__H__*/