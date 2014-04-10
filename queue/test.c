#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "queue.h"

RingBufferContext ring_buffer;
pthread_mutex_t iolock;
//char *file = "/opt/nfsroot/cache/db/test";
//char *file = "/opt/nfsroot/cache/db/1.doc";
char *file = "/opt/nfsroot/cache/db/10003196.mkv";
char buffer1[32768], buffer2[32768];
int fd = 0, fd2 = 0;
FILE *fp = NULL;
FILE *fp_r = NULL;
pthread_t input_id, output_id;
int flags = 0;

void* thrd_input()
{
	int ret = 0;
	
	fd = open(file, O_RDONLY);
	//fp_r = fopen(file, "r")
	if(fd == -1){
		printf("Cannot open file.\n");
		flags = 1;
		pthread_exit(NULL);
		//return (void*)(-1);
	}
	while(1){
		memset(buffer1, 0, sizeof(buffer1));
		
		ret = read(fd, buffer1, sizeof(buffer1));
		printf("read ret = %d\n", ret);
		if(ret < 0) {
			printf("Cannot read from file.\n");
			flags = 1;
			pthread_exit(NULL);
			//return (void *)(-1);
		} else if (ret == 0) {
			printf("read ok.\n");
			break;
		} else {
		redo:
			pthread_mutex_lock(&iolock);
			if(ring_buffer_datasize(&ring_buffer) == ring_buffer_buffersize(&ring_buffer))
			{
				pthread_mutex_unlock(&iolock);
				usleep(1);
				goto redo;
			}
			ring_buffer_write(&ring_buffer,  (uint8_t *)buffer1, ret);
			pthread_mutex_unlock(&iolock);
		}
	}
	
	flags = 1;
	pthread_exit(NULL);
	//return (void *)(0);
}

void* thrd_output()
{
    int ret = 0;
	fd2 = open("./1.mkv", O_WRONLY | O_CREAT | O_TRUNC);
	if(fd < 0){
		printf("Cannot open file.\n");
		pthread_exit(NULL);
		//return (void*)(-1);
	}
	
	while(1){
		memset(buffer2, 0, sizeof(buffer2));
		pthread_mutex_lock(&iolock);
		ret = ring_buffer_datasize(&ring_buffer);
		if(ret == 0)
		{
			if(flags == 1) {
				pthread_mutex_unlock(&iolock);
				printf("write ok.\n");
				break;
			} else {		
				pthread_mutex_unlock(&iolock);
				continue;
			}
		} else if (ret > 0) {
			ret = ring_buffer_read(&ring_buffer,  (uint8_t *)buffer2, sizeof(buffer2));
			pthread_mutex_unlock(&iolock);
			printf("write ret = %d\n", ret);
			write(fd2, buffer2, ret);
		} else {
			//error
			pthread_mutex_unlock(&iolock);
		}
		
	}
	pthread_exit(NULL);
	//return (void *)(0);
}

int main(void)
{
	//int ret = 0;
	ring_buffer_init(&ring_buffer, 1024*1024);
	pthread_mutex_init(&iolock, NULL);
	
	if(pthread_create(&input_id, NULL, thrd_input, NULL) != 0) {
		printf("Create input_thread error!\n");
		return -1;
	}
	if(pthread_create(&output_id, NULL, thrd_output, NULL) != 0) {
		printf("Create output_thread error!\n");
		return -1;
	}
	
	// 等待线程input_id退出
	if (pthread_join(input_id,NULL) != 0) {
		printf("Join input_thread error!\n");
		return -1;
	}else
		printf("input_thread Joined!\n");
		
	// 等待线程output_id退出
	if (pthread_join(output_id,NULL) != 0) {
		printf("Join output_thread error!\n");
		return -1;
	}else
		printf("output_thread Joined!\n");
	
	pthread_mutex_destroy(&iolock);
	ring_buffer_free(&ring_buffer);
	printf("ok.\n");
	
	return 0;
}