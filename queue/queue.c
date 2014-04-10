#include <queue.h>

int read_data(RingBufferContext* ctx, uint8_t* buffer, int buf_size)
{
	int ret = 0, datasize = 0, size = 0;
	datasize = ring_buffer_datasize(ctx);
	size = datasize;
	
	if(size <= 0)
		return 0;
	else if(size > buf_size) 
	{
		size = buf_size;
	}
	
	ret = ring_buffer_read(ctx, buffer, size);
	
	return ret;
}

int write_data(RingBufferContext* ctx, uint8_t* buffer, int buf_size)
{
	int ret = 0;
	int freesize = ring_buffer_freesize(ctx);
	int size = freesize;
	
	if(size <= 0)
		return 0;
	else if (size >= buf_size) 
		size = buf_size;
	else //freesize < buf_size
		return -1;
	
	ret = ring_buffer_write(ctx, buffer, buf_size);
	if(0 == ret)
		ret = size;
	else
		ret = 0;
	
	return ret;
}

int ring_buffer_write(RingBufferContext* ctx, uint8_t* buffer, int size)
{
	uint8_t* wptr = ctx->data_ptr + ctx->data_size;
	uint8_t* wptr2 = NULL;
	int size2 = 0;
	if(size <= (ctx->buffer_size - ctx->data_size))//space available
	{
		if(wptr >= ctx->buffer_end)//rrrrrwwwwwwrrrrrr
		{
			int skip = wptr - ctx->buffer_end;
			wptr = ctx->buffer_base + skip;
		}
		else if((wptr + size) > ctx->buffer_end)//wwwwwrrrrrrww
		{
			wptr2 = ctx->buffer_base;
			size2 = size - (ctx->buffer_end - wptr);
			size -= size2;
		}
		else//rrrrwwwww
		{
			//nothing
		}
		memcpy(wptr, buffer, size);
		if(wptr2)
		{
			memcpy(wptr2, buffer+size, size2);
		}
		ctx->data_size += (size+size2);
		return 0;
	}
	else
	{
		return -1;
	}
}

int ring_buffer_read(RingBufferContext* ctx, uint8_t* buffer, int size)
{
	uint8_t* rptr = ctx->data_ptr + size;
	uint8_t* rptr2 = ctx->data_ptr + ctx->data_size;
	int data_size = ctx->data_size;
	if(size <= ctx->data_size)
	{
		if(rptr > ctx->buffer_end)
		{
			int size0 = ctx->buffer_end - ctx->data_ptr;
			int size1 = rptr - ctx->buffer_end;
			memcpy(buffer, ctx->data_ptr, size0);
			memcpy(buffer+size0, ctx->buffer_base, size1);
			ctx->data_ptr = ctx->buffer_base + size1;
			ctx->data_size -= size;
		}
		else
		{
			memcpy(buffer, ctx->data_ptr, size);
			ctx->data_ptr = ctx->data_ptr + size;
			ctx->data_size -= size;
			if(ctx->data_ptr >= ctx->buffer_end)
			{
				ctx->data_ptr = ctx->buffer_base;
			}
		}
		return size;
	}
	else
	{
		if(rptr2 > ctx->buffer_end)
		{
			int size0 = ctx->buffer_end - ctx->data_ptr;
			int size1 = rptr2 - ctx->buffer_end; 
			memcpy(buffer, ctx->data_ptr, size0);
			memcpy(buffer+size0, ctx->buffer_base, size1);
			ctx->data_ptr = ctx->buffer_base + size1;
			ctx->data_size -= data_size;
		}
		else
		{
			memcpy(buffer, ctx->data_ptr, ctx->data_size);
			ctx->data_ptr = ctx->data_ptr + size;
			ctx->data_size -= data_size;
			if(ctx->data_ptr >= ctx->buffer_end)
			{
				ctx->data_ptr = ctx->buffer_base;
			}
		}
		return data_size;
		//return -1;
	}
}

int ring_buffer_init(RingBufferContext* ctx, int size)
{
	ctx->buffer_base = (uint8_t*)malloc(size);
	if(ctx->buffer_base)
	{
		memset(ctx->buffer_base, 0, size);
		ctx->buffer_end = ctx->buffer_base + size;
		ctx->buffer_size = size;
		ctx->data_ptr = ctx->buffer_base;
		ctx->data_size = 0;
	}
	return ctx->buffer_base ? 0 : -1;
}

void ring_buffer_free(RingBufferContext* ctx)
{
	free(ctx->buffer_base);
	ctx->buffer_base = NULL;
}

int ring_buffer_datasize(RingBufferContext* ctx)
{
	return ctx->data_size;
}

int ring_buffer_buffersize(RingBufferContext* ctx)
{
	return ctx->buffer_size;
}

int ring_buffer_freesize(RingBufferContext* ctx)
{
	return ctx->buffer_size - ctx->data_size;
}

