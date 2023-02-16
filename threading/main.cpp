#include <iostream>
#include <pthread.h>
#include <unistd.h>

void *fun(void *arg)
{
        int ret = 7 + *(int*)arg, i = 0;
        printf("in thread, param: %d\n", *(int*)arg);
        while(true)
        {
                printf("in thread, i: %d\n", i++);
                ret += i;
                sleep(1);
        }

        pthread_exit(&ret); // ret becomes available by using pthread_join()
}

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
