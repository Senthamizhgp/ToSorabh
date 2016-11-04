#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <signal.h>

int32_t start_peoplecounter(void);


void thread_exit(int sig)
{
	stop_peoplecounter();
}

void * threadFunc(void * void_ptr)
{

	start_peoplecounter();

	return NULL;
}

int32_t main()
{
	pthread_t hWifi;

	signal(SIGINT, thread_exit);
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&hWifi, NULL, threadFunc, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	/* wait for the second thread to finish */
	if(pthread_join(hWifi, NULL)) {

		fprintf(stderr, "Error joining thread\n");
		return 2;

	}
	return 0;
}
