#include "move.h"
#include "config_pool.h"
#include "stdio.h"
#include "string.h"
#include "portio.h"
#include "encoder.h"
#include "input.h"
#include "debug.h"
#include "pid.h"
#include "limitSwitch.h"

#define X_MAX_STEP 5
#define Y_MAX_STEP 3

MOVE_STRUCT movex;
MOVE_STRUCT movey;

PID_STRUCT pid_x;
double input_x = 0;
double output_x = 0;
double setpoint_x = 0;

PID_STRUCT pid_y;
double input_y = 0;
double output_y= 0;
double setpoint_y = 0;

void movexy_init(void)
{
  motor_init();
  encoder_init();
  input_init();
  port_ctrl_init();
  limit_switch_config();
  //O_CTRL5(1);
  motor_setup_mode(MOTORX, STOP);
  motor_setup_speed(MOTORX, MEDIUM_SPEED);
  motor_fast_stop(MOTORX);

  motor_setup_mode(MOTORY, STOP);
  motor_setup_speed(MOTORY, MEDIUM_SPEED);
  motor_fast_stop(MOTORY);

  sensor_init();
  sensor_start();
  movex.state = Axis_State_Ready;
  movex.cnt = 0;
  movex.pos = 0;
  movey.state = Axis_State_Ready;
  movey.cnt = 0;
  movey.pos = 0;

  tick_initTime(&movex.tick);
  pid_init(&pid_x, &input_x, &output_x, &setpoint_x, 1, 1, 0.3, DIRECT);
  pid_set_mode(&pid_x, AUTOMATIC);
  pid_set_sample_time(&pid_x, 100);
  pid_set_output_limits(&pid_x, 0, 50);

  tick_initTime(&movey.tick);
  pid_init(&pid_y, &input_y, &output_y, &setpoint_y, 1, 1, 0.3, DIRECT);
  pid_set_mode(&pid_y, AUTOMATIC);
  pid_set_sample_time(&pid_y, 100);
  pid_set_output_limits(&pid_y, 0, 50);

}

bool move_motor_home(uint8_t axis)
{
  if(axis == X_Axis)
  {
    if(movex.state != Axis_State_Ready)
    {
      return 0;
    }
    else
    {
      movex.state = Axis_State_Homming;
      motor_setup_speed(MOTORX, 45);
      motor_run(MOTORX, FORWARD);
      return 1;
    }
  }
  if(axis == Y_Axis)
  {
    if(movey.state != Axis_State_Ready)
    {
      return 0;
    }
    else
    {
      movey.state = Axis_State_Homming;
      motor_setup_speed(MOTORY, 45);
      motor_run(MOTORY, FORWARD);
      return 1;
    }
  }
}

void move_motor_home_done(uint8_t axis)
{

  limit_switch_task();
  if(axis == X_Axis)
  {
    if(movex.state == Axis_State_Homming)
    {
      if(bEventSend[0] == 1)
      {
        INFO("X Home done\n");
        //motor_stop(MOTORX);
        BUZZER(1);
        motor_setup_speed(MOTORX, 0);
        motor_run(MOTORX, FORWARD);
        movex.dir = 0;
        movex.cnt = 0;
        movex.state = Axis_State_Ready;
        cntx = 0;
        tick_initTime(&port.tick_buzzer);
        if(movex.pos == 1)
          move_motor_step(X_Axis, 1, 1, cntx + 5);
      }
    }
  }
  if(axis == Y_Axis)
  {
    if(movey.state == Axis_State_Homming)
    {
      if(bEventSend[1] == 1)
      {
        INFO("Y Home done\n");
        //motor_setup_speed(MOTORY, 0);
        //motor_run(MOTORY, FORWARD);
        motor_stop(MOTORY);
        movey.dir = 0;
        movey.cnt = 0;
        cnty = 0;
        movey.state = Axis_State_Ready;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
        // if(movey.pos == 1)
        //   move_motor_step(Y_Axis, 1, 1, cnty + 4);
      }
    }
  }
}


void move_motor_step(uint8_t axis, uint8_t step, uint8_t dir, int16_t turn)
{
  double turn_x, turn_y;
  if(axis == X_Axis)
  {
    movex.state = Axis_State_Moving;
    turn_x = step * turn;
    setpoint_x = turn_x;
    movex.dir = dir;
  }
  if(axis == Y_Axis)
  {
    movey.state = Axis_State_Moving;
    turn_y = step * turn;
    setpoint_y =  turn_y;
    movey.dir = dir;
  }
}

void move_motor_step_done(uint8_t axis)
{
  if(movex.state == Axis_State_Moving)
  {
    input_x = cntx;
    if(pid_compute(&pid_x))
    {
      INFO("PID output: %0.2f \n", *pid_x.output);
      motor_setup_speed(MOTORX, (uint8_t)(*pid_x.output));
      motor_run_reverse(MOTORX);
      if(*pid_x.output == 0)
      {
        movex.state = Axis_State_Ready;
        movex.cnt = cntx;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
      INFO("vi tri x: %d\n", movex.cnt);
    }
  }
  if(movey.state == Axis_State_Moving)
  {
    input_y = cnty;
    if(pid_compute(&pid_y))
    {
      INFO("PID output: %0.2f \n", *pid_y.output);
      motor_setup_speed(MOTORY, (uint8_t)(*pid_y.output));
      motor_run_reverse(MOTORY);
      if(*pid_y.output == 0)
      {
        movey.state = Axis_State_Ready;
        movey.cnt = cnty;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
      INFO("vi tri y: %d\n", movey.cnt);
    }
  }
}

bool move_coord_xy(int8_t x, int8_t y)
{
  int8_t x_diff, y_diff;
  if((x > 6) || (x < 0) || (y > 4) || (y < 0)) return 0;
  if(x != movex.pos)
  {
    if(x == 1)
    {
      move_motor_home(X_Axis);
      movex.pos = x;
    }
    else if(x == 0)
    {
      move_motor_home(X_Axis);
      movex.pos = 0;
    }
    else
    {
      x_diff = x - movex.pos;
      if(x_diff > 0)
      {
        INFO("Move next step \n");
        move_motor_step(X_Axis, x - 1, 1, 123);
        movex.pos = x;
      }
      // else
      // {
      //   INFO("Move X Home \n");
      //   move_motor_home(X_Axis);
      //   movex.pos = x;
      // }   
    }
    
  }
  if(y != movey.pos)
  {
    if(y == 1)
    {
      move_motor_home(Y_Axis);
      movey.pos = y;
    }
    else if(y == 0)
    {
      move_motor_home(Y_Axis);
      movey.pos = 0;
    }
    else
    {
      y_diff = y - movey.pos;
      if(y_diff > 0)
      {
        INFO("Move next step Y\n");
        move_motor_step(Y_Axis, y-1 , 1, 123);
        movey.pos = y;
      }
      // else
      // {
      //   INFO("Move Y Home \n");
      //   cnty = 0;
      //   movey.pos = 0;
      //   move_motor_home(Y_Axis);
      // }
    }
  }
  return 1;
}
