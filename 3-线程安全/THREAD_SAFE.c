#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

/*version 0.0.1 实现线程冲突*/
/*version 0.0.2 实现线程锁解决线程冲突*/

int num;
pthread_mutex_t lock; //定义线程锁

void* thread_job1(void * arg)
{
	pthread_detach(pthread_self());

	int tmp;
	//lock??? 此处进行lock会让词线程占用大量使用num的时间片
	for(int i = 0;i < 5000;i++)
	{
		pthread_mutex_lock(&lock); // 上锁
		tmp = num;
		printf("Thread [0x%x] Add num %d\n",(unsigned int)pthread_self(),++tmp);
		num = tmp;
		pthread_mutex_unlock(&lock); // 解锁
	}
	//unlock???
	pthread_exit(0);
}

void * thread_job2(void * arg)
{

	pthread_detach(pthread_self());
	int tmp;
	for(int i = 0;i < 5000;i++)
	{
		pthread_mutex_lock(&lock); // 上锁
		tmp = num;
		printf("Thread [0x%x] Add num %d\n",(unsigned int)pthread_self(),++tmp);
		num = tmp;
		pthread_mutex_unlock(&lock); // 解锁
	}
	pthread_exit(0);
}


int main()
{
	pthread_t tids[2];
	pthread_mutex_init(&lock,NULL); //初始化线程锁

	for(int i=0;i<2;i++)
	{
		if(i == 0)
		{
			pthread_create(&tids[0],NULL,thread_job1,NULL);
		}else
		{
			pthread_create(&tids[1],NULL,thread_job2,NULL);
		}
	}
	while(1)
		sleep(1);

	return 0;
}
