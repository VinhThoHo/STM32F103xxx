#include "movexy.h"
#include "config_pool.h"
#include "stdio.h"
#include "string.h"
#include "portio.h"
#include "encoder.h"

#define X_MAX_STEP 5
#define Y_MAX_STEP 3

MOVEXY_STRUCT movexy;


uint8_t currentState[2];
static int currentPos[2];
uint32_t timeoutx;
uint32_t timeouty;

void movexy_init(void)
{
	motor_init();

	motor_setup_mode(MOTORX, STOP);
  motor_setup_speed(MOTORX, MEDIUM_SPEED);
  motor_fast_stop(MOTORX);

  motor_setup_mode(MOTORY, STOP);
  motor_setup_speed(MOTORY, MEDIUM_SPEED);
  motor_fast_stop(MOTORY);

  sensor_init();
  sensor_start();

	tick_initTime(&movexy.tick_home);
}

bool move_motor_home(uint8_t axis)
{
  if(currentState[axis] != Axis_State_Ready)
  {
    return 0;
  }
  currentState[axis] = Axis_State_Homming;
  if(axis == X_Axis)
  {
    INFO("MOVE X TO HOME\n");
    motor_run(MOTORX, FORWARD);
    cntx = 0;
    timeoutx = 0xFFFFFF;
  }
  if(axis == Y_Axis)
  {
    INFO("MOVE Y TO HOME\n");
    motor_run(MOTORY, FORWARD);
    cnty = 0;
    timeouty = 0xFFFFFF;
  }
  return 1;
}

void move_motor_home_done(uint8_t axis)
{
  sensor_manager();
  if(currentState[axis] == Axis_State_Homming)
  {
    if(axis == X_Axis)
    {
        // if(sensor_get(SS_MT1) > sensor_get(SS_LM1))
      if((cntx > 10) && (sensor_get(SS_MT1) > 1.1))
      {
        movexy.x = 0;
        movexy.pos_x = 0;
        cntx = 0;
        INFO("AXIS X HOME DONE\n");
        motor_fast_stop(MOTORX);
        currentState[axis] = Axis_State_Ready;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
      else if((timeoutx) && (sensor_get(SS_MT1) > 1.1))
      {
        timeoutx--;
        movexy.x = 0;
        movexy.pos_x = 0;
        cntx = 0;
        INFO("AXIS X HOME DONE\n");
        motor_fast_stop(MOTORX);
        currentState[axis] = Axis_State_Ready;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
    }
    if(axis == Y_Axis)
    {
      // if(sensor_get(SS_MT2) > sensor_get(SS_LM2))
      if((cnty > 10) && (sensor_get(SS_MT2) > 1.3))
      {
        movexy.y = 0;
        movexy.pos_y = 0;
        cnty = 0;
        INFO("AXIS Y HOME DONE\n");
        motor_fast_stop(MOTORY);
        currentState[axis] = Axis_State_Ready;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
      else if((timeouty) && (sensor_get(SS_MT1) > 1.3))
      {
        timeouty--;
        movexy.y = 0;
        movexy.pos_y = 0;
        cnty = 0;
        INFO("AXIS Y HOME DONE\n");
        motor_fast_stop(MOTORY);
        currentState[axis] = Axis_State_Ready;
        BUZZER(1);
        tick_initTime(&port.tick_buzzer);
      }
    }
  }
}

void move_axis_eneded(uint8_t axis)
{
  if(axis < 3)
    currentState[axis] = Axis_State_Ready;
}

bool move_xy_ready(void)
{
  for(uint8_t i = 1; i < 3; i++)
  {
    if(currentState[i] != Axis_State_Ready)
      return 0;
  }
  return 1;
}

bool move_step(uint8_t axis, uint8_t step, uint8_t dir)
{
  if(currentState[axis] == Axis_State_Ready)
  {
    if(axis == X_Axis)
    {
     // INFO("X RUN\n");
      if(dir == 1)
      {
        //INFO("DIR +\n");
        motor_run(MOTORX, REVERSE);
        movexy.pos_x += step;
      }
      if(dir == 0)
      {
        //INFO("DIR -\n");
        motor_run(MOTORX, FORWARD);
        movexy.pos_x -= step;
      }
      INFO("CNTX: %d \n", cntx);
      movexy.x_dir = dir;
      movexy.turnx = cntx;
      movexy.stepx = step;
      currentState[axis] = Axis_State_Moving;
    }
    if(axis == Y_Axis)
    {
      if(dir == 1)
      {
        //INFO("RUN Y + %d STEP \n", step);
        motor_run(MOTORY, REVERSE);
        movexy.pos_y += step;
        if(step == 1) movexy.y_offset = 0;
        if(step == 2) movexy.y_offset = 4;
        if(step == 3) movexy.y_offset = 12;
      }
      if(dir == 0)
      {
       // INFO("RUN Y - %d STEP \n", step);
        motor_run(MOTORY, FORWARD);
        movexy.pos_y -= step;
        if(step == 1) movexy.y_offset = 0;
        if(step == 2) movexy.y_offset = 4;
        if(step == 3) movexy.y_offset = 16;
      }
      INFO("CNTY: %d \n", cnty);
      movexy.y_dir = dir;
      movexy.turny = cnty;
      movexy.stepy = step;
      currentState[axis] = Axis_State_Moving;
    }
  }
}

void move_axis_done(uint8_t axis)
{
  if(currentState[axis] == Axis_State_Moving)
  {
    if(axis == X_Axis)
    {
      if(movexy.x_dir == 1)
      {
        if(cntx > (movexy.turnx + 125 * movexy.stepx))
        {
          motor_fast_stop(MOTORX);
          currentState[axis] = Axis_State_Ready;
          movexy.turnx = 0;
          movexy.x = movexy.pos_x;
          cntx = 0;
          BUZZER(1);
          tick_initTime(&port.tick_buzzer);
        }
      }
      if(movexy.x_dir == 0)
      {
        if(cntx > (movexy.turnx + 90 * movexy.stepx))
        {
          motor_fast_stop(MOTORX);
          currentState[axis] = Axis_State_Ready;
          movexy.turnx = 0;
          movexy.x = movexy.pos_x;
          cntx = 0;
          BUZZER(1);
          tick_initTime(&port.tick_buzzer);
        }
      }
    }
    if(axis == Y_Axis)
    {
      if(movexy.y_dir == 1)
      {
        if(cnty > (movexy.turny + 125 * movexy.stepy))
        {
          motor_fast_stop(MOTORY);
          INFO("STOP STEP Y\n");
          currentState[axis] = Axis_State_Ready;
          movexy.turny = 0;
          movexy.y = movexy.pos_y;
          cnty = 0;
        }
      }
      if(movexy.y_dir == 0)
      {
        if(cnty > (movexy.turny + 95 * movexy.stepy))
        {
          motor_fast_stop(MOTORY);
          INFO("STOP STEP Y\n");
          currentState[axis] = Axis_State_Ready;
          movexy.turny = 0;
          movexy.y = movexy.pos_y;
          cnty = 0;
        }
      }
    }
  }
}




// void move_axis_done(uint8_t axis)
// {
//   if(currentState[axis] == Axis_State_Moving)
//   {
//     if(axis == X_Axis)
//     {
//       if(movexy.x_dir == 1)
//       {
//         if(cntx > (movexy.turnx + 255 * movexy.stepx))
//         // if(((cntx > movexy.turnx) && ((cntx - movexy.turnx) > (245 * movexy.stepx))) || 
//         //   ((cntx < movexy.turnx) && (((50000 - movexy.turnx) + (245 * movexy.stepx) + 1) > (245 * movexy.stepx))))
//         {
//           movexy.turnx = 0;
//           movexy.x = movexy.pos_x;
//           cntx = 0;
//           BUZZER(1);
//           tick_initTime(&port.tick_buzzer);
//           motor_fast_stop(MOTORX);
//           currentState[axis] = Axis_State_Ready;
//         }
//       }
//       if(movexy.x_dir == 0)
//       {
//         // if(((cntx > movexy.turnx) && ((cntx - movexy.turnx) > (248 * movexy.stepx))) || 
//         //   ((cntx < movexy.turnx) && (((50000 - movexy.turnx) + (248 * movexy.stepx) + 1) > (248 * movexy.stepx))))
//         if(cntx > (movexy.turnx + 250 * movexy.stepx))
//         {
//           motor_fast_stop(MOTORX);
//           currentState[axis] = Axis_State_Ready;
//           movexy.turnx = 0;
//           movexy.x = movexy.pos_x;
//           cntx = 0;
//           BUZZER(1);
//           tick_initTime(&port.tick_buzzer);
//         }
//       }
//     }
//     if(axis == Y_Axis)
//     {
//       INFO("turn Y: %d\n", movexy.turny);
//       INFO("cnt Y: %d \n", cnty);
//       if(movexy.y_dir == 1)
//       {
//         INFO("y_offset: %d \n", movexy.y_offset);
//        if(cnty > (movexy.turny + 220 * movexy.stepy))
//         // if(((cnty > movexy.turny) && ((cnty - movexy.turny) > (225 * movexy.stepy + movexy.y_offset))) || 
//         //   ((cnty < movexy.turny) && (((50000 - movexy.turny) + (225 * movexy.stepy) + 1) > (225 * movexy.stepy + movexy.y_offset))))
//         {
          
//           currentState[axis] = Axis_State_Ready;
//           movexy.turny = 0;
//           movexy.y = movexy.pos_y;
//           cnty = 0;
//           INFO("STOP STEP Y\n");
//           motor_fast_stop(MOTORY);
//         }
//       }
//       if(movexy.y_dir == 0)
//       {
//         if(cnty > (movexy.turny + 250 * movexy.stepy + movexy.y_offset))
//         // if(((cnty > movexy.turny) && ((cnty - movexy.turny + movexy.y_offset) > (250 * movexy.stepy))) || 
//         //   ((cnty < movexy.turny) && (((50000 - movexy.turny) + (250 * movexy.stepy) + 1 + movexy.y_offset) > (250 * movexy.stepy))))
//         {
//           currentState[axis] = Axis_State_Ready;
//           movexy.turny = 0;
//           movexy.y = movexy.pos_y;
//           motor_fast_stop(MOTORY);
//           INFO("STOP STEP Y\n");
//           cnty = 0;
//         }
//       }
//     }
//   }
// }

void coordinate_motor_pos(int coord[2], uint8_t motor[2])
{
  motor[0] = coord[X_Axis] + coord[Y_Axis];
  motor[1] = coord[X_Axis] - coord[Y_Axis];
}

bool move_coord_xy(int8_t x, int8_t y)
{
  int8_t x_coord, y_coord;
  if((x > 5) || (x < 0)) return 0;
  if((y > 3) || (y < 0)) return 0;
  x_coord = x - movexy.x;
  y_coord = y - movexy.y;
  if(x != movexy.x)
  {
    if(x_coord > 0) 
    {
      move_step(X_Axis, x_coord, 1);
      //INFO("X chay toi \n");
    }
    // if(x_coord == 0)
    // {
    //   move_motor_home(X_Axis);
    // }
    else
    {
      //move_step(X_Axis, -x_coord, 0);
      move_motor_home(X_Axis);
      //INFO("X chay lui \n");
    }
  }
  if(y != movexy.y)
  {
    if(y_coord > 0) 
    {
      move_step(Y_Axis, y_coord, 1);
    }
    else
    {
      //move_step(Y_Axis, -y_coord, 0);
      move_motor_home(Y_Axis);
    }
  }
  return 1;
}

// bool move_absolute(uint8_t x, uint8_t y)
// {
//   int tmp[2], delta[2], dir[2], motorPos[2];
//   if(!move_xy_ready())
//     return 0;
//   coordinate_motor_pos(xy, motorPos);
//   for(int i = 0; i < 2; ++i)
//   {
//     delta[i] = xy[i] - currentPos[i];
//     currentPos[i] = xy[i];
//   }
//   if(motorPos[X_Axis] < 0 || motorPos[X_Axis] > X_MAX_LIMIT)
//     return 0;
//   if(motorPos[Y_Axis] < 0 || motorPos[Y_Axis] > Y_MAX_LIMIT)
//     return 0;
//   for(uint8_t i = 0; i < 2; i++)
//   {

//   }
// }
