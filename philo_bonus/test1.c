#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>      // For O_CREAT, O_EXCL
#include <sys/wait.h>

#define SEM_NAME 
#define NUM_PROCESSES 10

int main(void)
{
    // Create the named semaphore with initial value 1
    sem_t *forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (forks == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            sem_wait(forks);
            printf("Philosopher %d (PID %d): eating 🍝\n", i + 1, getpid());
            usleep(500000); // simulate eating
            printf("Philosopher %d (PID %d): done eating\n", i + 1, getpid());
            sem_post(forks);
            exit(0);
        }
    }

    // Parent waits for all children
    for (int i = 0; i < NUM_PROCESSES; i++)
        wait(NULL);

    // Cleanup semaphore
    sem_close(forks);
    sem_unlink(SEM_NAME);

    return 0;
}
