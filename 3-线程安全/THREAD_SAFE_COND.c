#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/mman.h>

/* 实现条件变量 A线程白天工作 B线程黑天工作*/
int TIME; // 1:白天 0:黑夜

pthread_cond_t cdA,cdB; // 定义条件变量
pthread_mutex_t lock; //定义锁

void* threadA(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		while(!TIME) //如果不是白天
		{
			pthread_cond_wait(&cdA,&lock); // 挂起并解锁
		}
		/*白天*/
		printf("thread A working,TIME = [%d]\n",TIME);
		TIME = 0; // 工作完毕切换为黑夜
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cdB); // 唤醒一个cdB中的线程
		usleep(50000);
	}
}
void* threadB(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		while(TIME) //如果不是黑夜
		{
			pthread_cond_wait(&cdB,&lock); // 挂起到cdB并解锁
		}
		/*黑夜*/
		printf("thread B working,TIME = [%d]\n",TIME);
		TIME = 1; // 工作完毕切换为白天
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cdA); // 唤醒一个cdA中的线程
		usleep(50000);
	}
}

int main()
{
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cdA,NULL);
	pthread_cond_init(&cdB,NULL);

	pthread_t tidA,tidB;
	pthread_create(&tidA,NULL,threadA,NULL);
	pthread_create(&tidB,NULL,threadB,NULL);

	pthread_join(tidA,NULL);
	pthread_join(tidB,NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cdA);
	pthread_cond_destroy(&cdB);
	return 0;
}
