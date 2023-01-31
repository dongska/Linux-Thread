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

/* 实现文件锁 */

int main()
{
	int fd = open("MFile",O_RDWR); // 获取文件描述符
	
	/*定义文件锁结构体*/
	struct flock DEF_flock;
	struct flock MY_flock;
	
	/*获取原文件锁属性*/
	fcntl(fd,F_GETLK,&DEF_flock);
	/*修改文件锁属性为写锁*/
	MY_flock.l_type = F_WRLCK;
	MY_flock.l_whence = SEEK_SET;
	MY_flock.l_start = 0;
	MY_flock.l_len = 0;
	/*Process A 上锁*/
	fcntl(fd,F_SETLKW,&MY_flock);

	printf("Process A lock success\n");
	/*Process B */
	pid_t pid = fork();
	if(pid == 0)
	{
		fcntl(fd,F_SETLKW,&MY_flock);
		sleep(1);
		printf("Process B lock success\n");
		exit(0);
	}

	sleep(10);

	MY_flock.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&MY_flock);
	printf("Process A unlock\n");

	sleep(3);
	close(fd);
	wait(NULL);

	return 0;
}
