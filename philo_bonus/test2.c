#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>



int main()
{
    int i;
    int *counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t   *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
    sem_init(sem, 1, 1);
    pid_t   pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    for(i = 0; i < 5; i++)
    {
        sem_wait(sem);
        (*counter)++;
        printf("Process %d: counter = %d\n", getpid(), *counter);
        sem_post(sem);
        // usleep(100000);
    }

    if (pid > 0) // parent
    {
        wait(NULL);
        sem_destroy(sem);
        munmap(counter, sizeof(int));
        munmap(sem, sizeof(sem_t));
    }


}



