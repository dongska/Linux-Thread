#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

/*线程属性*/

void* thread(void* arg)
{
	printf("thread 0x%x running...\n",(unsigned int)pthread_self());
	while(1)
		sleep(1);
	pthread_exit((void*)2);
}

int main()
{
	/*定义线程属性*/
	pthread_attr_t attr;
	/*初始化线程属性*/
	pthread_attr_init(&attr);
	
	int detachstate;
	/*获取线程属性分离状态*/
	pthread_attr_getdetachstate(&attr,&detachstate);

	if(detachstate == PTHREAD_CREATE_DETACHED)
	{
		printf("default exit state:PTHREAD_CREATE_DETACHED\n");
	}
	else
		printf("default exit state:PTHREAD_CREATE_JOINABLE\n");
	/*设置线程属性分离状态*/
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	
	pthread_t tid;
	pthread_create(&tid,&attr,thread,NULL);
	
	/*测试detach与join的互斥*/
	int err;
	if((err = pthread_join(tid,NULL)) > 0)
	{
		printf("master join 0x%x failed:%s",(unsigned int)tid,strerror(err));
		exit(0);
	}

	/*线程属性销毁*/
	pthread_attr_destroy(&attr);	

	return 0;
}
