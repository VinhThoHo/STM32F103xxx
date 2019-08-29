#include "statemachine.h"
#include <string.h>
#include "key.h"
#include "input.h"
#include "lcd_buf.h"
#include "menu.h"
#include "debug.h"
#include "clock_rtc.h"
#include "tick.h"
#include "timerTick.h"

// STATE_T eCurrentState;
// // typedef void const (*pVectorFunc)(void);
// void (*pStateFunc)(void);


// void (*key_next_func)(void) = &menu_next;
// void (*key_prev_func)(void) = &menu_prev;
// void (*key_enter_func)(void) = &menu_enter;
// void (*key_back_func)(void) = &menu_back;

// extern menu_t *currentPointer;

// /*event node list*/
// EVENT_NODE_T ENL_IDLE[] = {
// 	{EVENT_MENU_PRESS, STATE_NUM1, ACTION_NUM1_REFRESH, menu_manager},
// 	{EVENT_UP_PRESS, STATE_NUM2, ACTION_NUM2_REFRESH, sampler_manager}
// };

// EVENT_NODE_T ENL_NUM1 = {
// 	{EVENT_EXIT_PRESS, STATE_IDLE, ACTION_IDLE_REFRESH, idle_manager}
// };

// EVENT_NODE_T ENL_NUM2 = {
// 	{EVENT_DOWN_PRESS, STATE_IDLE, ACTION_IDLE_REFRESH, idle_manager}
// };

// STATE_DESCRIPTOR_T StateTransitionMatrix[3] = {
// 	{STATE_IDLE, 2, &ENL_IDLE},
// 	{STATE_NUM1, 1, &ENL_NUM1},
// 	{STATE_NUM2, 1, &ENL_NUM2}
// };

// void StateMachine_Init(void)
// {
// 	eCurrentState = STATE_IDLE;
// }

// void StateMachine(void)
// {
// 	uint8_t bEvents;
// 	EVENT_NODE_T *pENL, *pen;
// 	//INFO("Events List: %d \n", StateTransitionMatrix[eCurrentState].bNumEvents);
// 	for(bEvents = 0; bEvents < StateTransitionMatrix[eCurrentState].bNumEvents; bEvents++)
// 	{
// 		pENL = StateTransitionMatrix[eCurrentState].pEventList;
// 		pen = &pENL[bEvents];
// 		if(event_check(pen->Event))
// 		{	
// 			INFO("Have event \n");
// 			INFO("Event: %d \n", pen->Event);
// 			eCurrentState = pen->NextState;
// 			action(pen->Action);
// 			// pen->pFunc();
// 			pStateFunc = pen->pFunc;
// 			break;
// 		}
// 	}
// 	if(pStateFunc)
// 		pStateFunc();
// }

// uint8_t event_check(EVENTS_T event)
// {	
// 	switch(event)
// 	{
// 		case EVENT_MENU_PRESS:
// 			if(KeyProcess() == MENU)
// 				return(1);
// 			break;
// 		case EVENT_UP_PRESS:
// 			if(input_process() == IN1_ON)
// 				return(1);
// 			break;
// 		case EVENT_EXIT_PRESS:
// 			if(KeyProcess() == EXIT)
// 				return(1);
// 			break;
// 		case EVENT_DOWN_PRESS:
// 			if(KeyProcess() == DOWN)
// 				return(1);
// 			break;
// 		default:
// 			return(0);
// 			break;
// 	}
// 	return(0);
// }

// void action(ACTIONS_T action)
// {
// 	switch(action)
// 	{
// 		case ACTION_IDLE_REFRESH:
// 			INFO("ACTION IDLE\r\n");
// 			break;
// 		case ACTION_NUM1_REFRESH:
// 			INFO("ACTION NUM1 \r\n");
// 			break;
// 		case ACTION_NUM2_REFRESH:
// 			INFO("ACTION NUM2 \r\n");
// 			break;
// 		default:
// 			break;
// 	}
// }

// void idle_manager(void)
// {
// 	INFO("IDLE MANAGER \n");
// }

// void menu_manager(void)
// {
// 	uint8_t key;
// 	INFO("MENU MANAGER \n");
// 	menu_refresh();
// 	if(!tick_checkMS(&tick, 5))
//     {
//   //   	key = KeyProcess();
// 		// if(key == MENU)
// 		// {
// 		// if(key_enter_func) (*key_enter_func)();
// 		// }
// 		// if(key == UP)
// 		// {
// 		// if(key_next_func) (*key_next_func)();
// 		// }
// 		// if(key == DOWN)
// 		// {
// 		// if(key_prev_func) (*key_prev_func)();
// 		// }
// 		// if(key == EXIT)
// 		// {
// 	 //        if(menu_get_level(currentPointer) == 0)
// 	 //        	eCurrentState = STATE_IDLE;
// 	 //        else
// 	 //        {
// 	 //            if(key_back_func) (*key_back_func)();
//   //       }
//   //     }
//     }
// }

// void sampler_manager(void)
// {
// 	INFO("SAMPLER MANAGER \n");
// }