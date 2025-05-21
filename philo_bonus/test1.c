#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>    // for mmap
#include <semaphore.h>   // for sem_t
#include <fcntl.h>       // for O_CREAT
#include <sys/wait.h>    // for wait

int main()
{
    // Shared integer counter
    int *counter = mmap(NULL, sizeof(int),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS,
                        -1, 0);

    // Shared unnamed semaphore (initial value 1 for mutual exclusion)
    sem_t *sem = mmap(NULL, sizeof(sem_t),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS,
                      -1, 0);

    sem_init(sem, 1, 1);  // 1 = shared between processes

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    for (int i = 0; i < 5; i++)
    {
        sem_wait(sem); // Lock
        (*counter)++;
        printf("Process %d: counter = %d\n", getpid(), *counter);
        sem_post(sem); // Unlock
        usleep(100000); // sleep 100ms
    }

    if (pid > 0) // parent
    {
        wait(NULL);
        sem_destroy(sem);
        munmap(counter, sizeof(int));
        munmap(sem, sizeof(sem_t));
    }

    return 0;
}
