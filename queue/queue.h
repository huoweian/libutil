#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint8_t* buffer_base;
	uint8_t* buffer_end;
	int buffer_size;
	uint8_t* data_ptr;
	int data_size;
}RingBufferContext;

pthread_mutex_t iolock;

int ring_buffer_write(RingBufferContext* ctx, uint8_t* buffer, int size);
int ring_buffer_read(RingBufferContext* ctx, uint8_t* buffer, int size);
int ring_buffer_init(RingBufferContext* ctx, int size);
void ring_buffer_free(RingBufferContext* ctx);
int ring_buffer_datasize(RingBufferContext* ctx);
int ring_buffer_buffersize(RingBufferContext* ctx);
int ring_buffer_freesize(RingBufferContext* ctx);


#endif //QUEUE_H_