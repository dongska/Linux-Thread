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

	ptr->num = 0;

	pid_t pid;
	pid = fork();
	if(pid>0)
	{
		for(int i=0;i<5000;i++)
		{
			printf("parent process [%d] ++unm = %d\n",getpid(),++(ptr->num));
		}
		wait(NULL);
	}
	else if(pid == 0)
	{
		for(int i=0;i<5000;i++)
		{
			printf("child process [%d] ++unm = %d\n",getpid(),++(ptr->num));
		}
		exit(0);

	}
	else
	{
		perror("fork call failed");
		exit(0);
	}
	pthread_mutex_destroy(&ptr->lock);
	munmap(ptr,sizeof(shared_t)); // 关闭磁盘映射
	return 0;
}

