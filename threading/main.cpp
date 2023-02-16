#include <iostream>
#include <pthread.h>

void *fun(int param)
{
	printf("in thread, param: %d", param);
	return &param;
}

int main()
{
	pthread_t t;
}
