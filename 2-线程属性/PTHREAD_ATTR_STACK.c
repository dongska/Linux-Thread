#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

/*线程属性*/

void* thread(void* arg)
{
	while(1)
		sleep(1);
	pthread_exit((void*)2);
}

int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	void* stackaddr;
	size_t stacksize;
	/*初始化线程属性后,地址为NULL大小0*/
	pthread_attr_getstack(&attr,&stackaddr,&stacksize);
	printf("默认attr中,addr:[%p],size:[%ld]\n",stackaddr,stacksize);
	
	pthread_t tid;
	int err;
	int num = 0;
	while(1)
	{
		/*修改线程大小,需要自行malloc*/
		if((stackaddr = malloc(0x100000)) == NULL)
		{
			perror("malloc call failed:");
			exit(0);
		}
		stacksize = 0x100000;
		pthread_attr_setstack(&attr,stackaddr,stacksize);
		if((err = pthread_create(&tid,&attr,thread,NULL)) > 0)
		{
			printf("pthread_create call failed:%s\n",strerror(err));
			exit(0);
		}
		printf("thread num:%d\n",++num);
	}
	pthread_attr_destroy(&attr);
	return 0;
}
