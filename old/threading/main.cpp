#include <iostream>
#include <pthread.h>
#include <unistd.h>

void cleanup_handler(void *arg);

void *fun(void *arg);

int main()
{
        pthread_t t;
        int ret, arg = 7;

        ret = pthread_create(&t, NULL, &fun, &arg);

        sleep(10);
        pthread_cancel(t); // effectively ends thread

        printf("cancelled thread, ret: %d\n", ret);

        return 0;
}

void cleanup_handler(void *arg)
{
        printf("in cleanup, %d\n", *(int*)arg);
}

void *fun(void *arg)
{
        int ret = 7 + *(int*)arg, i = 0;
        printf("in thread, param: %d\n", *(int*)arg);
        
        pthread_cleanup_push(&cleanup_handler, &ret);
        
        while(true)
        {
                printf("in thread, i: %d\n", i++);
                ret += i;
                sleep(1);
        }

        pthread_cleanup_pop(0);

        pthread_exit(&ret); // ret becomes available by using pthread_join()
}
