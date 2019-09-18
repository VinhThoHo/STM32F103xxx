#include <stdint.h>
#include <stdlib.h>
#include "ring_buffer.h"

void RB_Init(RingBuffer_t* buf, uint32_t size)
{
	buf->buffSize = 0;
	buf->buff = NULL;
	if(size != 0)
		buf->buff = malloc(size);
	if(buf->buff != NULL)
		buf->buffSize = size;
	buf->buffRdPtr = 0;
	buf->buffWrPtr = 0;
	buf->reStart = 1;
	buf->buffWrPtrOvf = 0;
	buf->buffEmpty = 1;
}

void RB_InitStatic(RingBuffer_t* buf, uint8_t* buffer, uint32_t size)
{
	buf->buff = buffer;
	buf->buffSize = size;
	buf->buffRdPtr = 0;
	buf->buffWrPtr = 0;
	buf->reStart = 1;
	buf->buffWrPtrOvf = 0;
	buf->buffEmpty = 1;
}

void RB_Push(RingBuffer_t* buf, uint8_t value)
{
	uint32_t index;
	if(buf->buffSize == 0) return;
	if(buf->buff == NULL) return;
	index = buf->buffWrPtr;
	buf->buff[index] = value;
	++index;
	if(index >= buf->buffSize) index = 0;
	buf->buffWrPtr = index;
	if(index == buf->buffRdPtr) buf->buffWrPtrOvf = 1;
}

uint8_t RB_Pop(RingBuffer_t* buf)
{
	uint32_t index;
	uint8_t value;
	if(buf->buffSize == 0) return 0;
	if(buf->buff == NULL) return 0;
	index = buff->buffRdPtr;
	if(index == buf->buffWrPtr) return 0;
	value = buf->buff[index];
	++index;
	if(index >= buf->buffSize) index = 0;
	buf->buffRdPtr = index;
	if(index == buf->buffWrPtr)
		buf->buffEmpty = 1;
	return value;
}
