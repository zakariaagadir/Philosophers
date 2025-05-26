#include <pthread.h>


void *func(void *arr)
{
    return NULL;
}


int main(void)
{
    pthread_t t;


    pthread_create(&t, NULL, func, NULL);

    pthread_join(t, NULL);

}