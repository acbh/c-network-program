#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *working(void *arg)
{
	printf("this is child_thread, thread id is %ld\n", pthread_self());
	for (int i = 0; i < 10; i++)
	{
		if (i == 6)
		{
			pthread_exit(NULL);
		}
		printf("child is running %d\n", i);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, working, NULL);
	printf("child thread created, thread id is %ld\n", tid);
	printf("main thread is running id is %ld\n", pthread_self());

	for (int i = 0; i < 3; i++)
	{
		printf("i = %d\n", i);
	}

	pthread_exit(NULL);
	return 0;
}