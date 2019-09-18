#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdint.h>

typedef struct
{
	uint32_t buffSize;
	volatile uint8_t* buff;
	volatile uint32_t buffRdPtr;
	uint32_t buffWrPtr;
	volatile uint8_t reStart: 1, buffWrPtrOvf: 1, buffEmpty: 1;
} RingBuffer_t;

void RB_Init(RingBuffer_t* buf, uint32_t size);
void RB_InitStatic(RingBuffer_t* buf, uint8_t* buffer, uint32_t size);
void RB_Push(RingBuffer_t* buf, uint8_t value);
uint8_t RB_Pop(RingBuffer_t* buf);

#endif
