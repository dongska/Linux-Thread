#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/mman.h>

/* 创建一个映射内存,共享映射,包含两种数据,两个进程访问 ,上锁保护 */
/* version 0.0.1 没有加进程锁 双进程++ */
/* version 0.0.2 增加进程锁 */
typedef struct{
	int num;
	pthread_mutex_t lock;
}shared_t;

int main()
{
	/*创建映射*/
	shared_t * ptr = NULL;
	int fd = open("MFile",O_RDWR); // 打开文件描述符
	ftruncate(fd,sizeof(shared_t)); // 拓展文件大小至结构体大小 
	ptr = mmap(NULL,sizeof(shared_t),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0); // 创建磁盘文件的硬盘映射
	close(fd);
	
	/*初始化进程锁*/
	pthread_mutexattr_t attr; // 定义锁属性
	pthread_mutexattr_init(&attr); // 初始化锁属性
	pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED); // 设置锁属性为进程锁
	pthread_mutex_init(&ptr->lock, &attr); // 使用锁属性初始化锁

	ptr->num = 0;

	pid_t pid;
	pid = fork();
	if(pid>0) 
	{
		for(int i=0;i<5000;i++)
		{
			pthread_mutex_lock(&ptr->lock); // 上锁
			printf("parent process [%d] ++unm = %d\n",getpid(),++(ptr->num));
			pthread_mutex_unlock(&ptr->lock); // 解锁
		}
		wait(NULL);
	}
	else if(pid == 0)
	{
		for(int i=0;i<5000;i++)
		{
			pthread_mutex_lock(&ptr->lock); // 上锁
			printf("child process [%d] ++unm = %d\n",getpid(),++(ptr->num));
			pthread_mutex_unlock(&ptr->lock); // 解锁
		}
		exit(0);

	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	pthread_mutex_destroy(&ptr->lock);
	pthread_mutexattr_destroy(&attr); // 销毁锁属性
	munmap(ptr,sizeof(shared_t)); // 关闭磁盘映射
	return 0;
}

