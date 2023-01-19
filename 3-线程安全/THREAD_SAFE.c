#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int num;

void* thread_job1(void * arg)
{
	pthread_detach(pthread_self());
	int tmp;
	for(int i = 0;i < 5000;i++)
	{
		tmp = num;
		printf("Thread [0x%x] Add num %d\n",(unsigned int)pthread_self(),++tmp);
		num = tmp;
	}
	pthread_exit(0);
}

void * thread_job2(void * arg)
{

	pthread_detach(pthread_self());
	int tmp;
	for(int i = 0;i < 5000;i++)
	{
		tmp = num;
		printf("Thread [0x%x] Add num %d\n",(unsigned int)pthread_self(),++tmp);
		num = tmp;
	}
	while(1)
		sleep(1);
	pthread_exit(0);
}


int main()
{
	pthread_t tids[2];

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
