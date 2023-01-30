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

/* 读写锁的实现 三个线程写 五个线程读 */

int num;
pthread_rwlock_t lock; // 定义读写锁

void* thread_write(void* arg)
{
	while(1)
	{
		pthread_rwlock_wrlock(&lock); // 上写锁
		printf("write thread [0x%x] add num %d \n",(unsigned int)pthread_self(),++num);
		pthread_rwlock_unlock(&lock); // 解锁
		sleep(3);
	}
}

void* thread_read(void* arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&lock); // 上读锁
		printf("read thread [0x%x] read num %d \n",(unsigned int)pthread_self(),num);
		pthread_rwlock_unlock(&lock); // 解锁
		sleep(3);
	}
}

int main()
{
	pthread_t tids[8];
	int i = 0;
	num = 0;

	pthread_rwlock_init(&lock,NULL); // 初始化读写锁

	for(i;i<3;i++)
	{
		pthread_create(&tids[i],NULL,thread_write,NULL);
	}
	for(i;i<8;i++)
	{
		pthread_create(&tids[i],NULL,thread_read,NULL);
	}
	while(i--)
	{
		pthread_join(tids[i],NULL);
	}
	pthread_rwlock_destroy(&lock); // 销毁读写锁

	return 0;
}
