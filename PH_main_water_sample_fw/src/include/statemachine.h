#ifndef __STATEMACHINE__H__
#define __STATEMACHINE__H__

#include "HD44780.h"
#include "debug.h"
#include "key.h"

// typedef void const (*pVectorFunc)(void);

// typedef enum 
// {
// 	STATE_IDLE,
// 	STATE_NUM1,
// 	STATE_NUM2,
// } STATE_T;

// typedef enum
// {
// 	EVENT_MENU_PRESS,
// 	EVENT_UP_PRESS,
// 	EVENT_TIMEOUT,
// 	EVENT_EXIT_PRESS,
// 	EVENT_DOWN_PRESS,
// } EVENTS_T;

// typedef enum
// {
// 	ACTION_IDLE_REFRESH,
// 	ACTION_NUM1_REFRESH,
// 	ACTION_NUM2_REFRESH,
// } ACTIONS_T;

// typedef const struct {
// 	EVENTS_T Event;
// 	STATE_T NextState;
// 	ACTIONS_T Action;
// 	void (*pFunc)(void);
// } EVENT_NODE_T; 

// typedef const struct {
// 	STATE_T eCurrState;
// 	uint8_t bNumEvents;
// 	EVENT_NODE_T *pEventList;
// } STATE_DESCRIPTOR_T;

// void StateMachine_Init(void);
// void StateMachine(void);
// uint8_t event_check(EVENTS_T event);
// void action(ACTIONS_T action);
// void SF_PRINTLCD(void);

// void idle_manager(void);
// void menu_manager(void);
// void sampler_manager(void);


#endif