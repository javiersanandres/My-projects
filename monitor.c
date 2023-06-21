#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>
#include <stdarg.h>
#include <signal.h>

#include "types.h"
#include "circularbuffer.h"
#include "funciones.h"
#include "handlers.h"

volatile sig_atomic_t pgot_int = 0;
volatile sig_atomic_t pgot_usr1 = 0;
volatile sig_atomic_t pgot_usr2 = 0;
volatile sig_atomic_t pgot_alrm = 0;

volatile long RES = LONG_MIN; /*Global variable that will hold the result of a mining round*/

/**
 * @brief Monitor and checker main
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
int main(int argc, char *argv[])
{
    int sts_chk, sts_mon;
    struct sigaction sa_act;
    sem_t *sem_mon = NULL, *sh_sems = NULL, *sem_empty = NULL, *sem_fill = NULL, *sem_mutex = NULL;
    circularbuffer *sh_circbuff;
    pid_t pid;

    /*Create named sempahore for sincronizing checker with monitors*/
    if ((sem_mon = sem_open(SEMAPHORE_MONITOR_ALIVE, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        if ((sem_mon = sem_open(SEMAPHORE_MONITOR_ALIVE, 0)) == SEM_FAILED)
        {
            perror("sem_open: ");
            exit(EXIT_FAILURE);
        }
    }
    
    /*Map shared memory*/ /*Note that it's anonymous*/
    if ((sh_circbuff = (circularbuffer *)mmap(NULL, sizeof(circularbuffer) + 3 * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
    {
        perror("mmap");
        clean_up(2, semaphore_close, sem_mon, semaphore_unlink, SEMAPHORE_MONITOR_ALIVE);
        exit(EXIT_FAILURE);
    }

    /*Initiate buffer values*/
    circularbuffer_init(sh_circbuff);

    /*Unnamed semaphores*/
    sh_sems = (sem_t *)(sh_circbuff + 1);

    sem_empty = sh_sems;
    sem_fill = sh_sems + 1;
    sem_mutex = sh_sems + 2;

    /*Initiate unnamed semaphores for producer-consumer*/
    if (sem_init(sem_empty, 1, MAX_BUFFER_SIZE) == -1)
    {
        perror("sem_init");
        clean_up(2, semaphore_close, sem_mon, shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t));
        exit(EXIT_FAILURE);
    }

    if (sem_init(sem_fill, 1, 0) == -1)
    {
        perror("sem_init");
        clean_up(3, semaphore_close, sem_mon, shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t), semaphore_destroy, sem_empty);
        exit(EXIT_FAILURE);
    }

    if (sem_init(sem_mutex, 1, 1) == -1)
    {
        perror("sem_init");
        clean_up(4, semaphore_close, sem_mon, shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t), semaphore_destroy, sem_empty, semaphore_destroy, sem_fill);
        exit(EXIT_FAILURE);
    }


    /*Set SIGINT handler*/

    sa_act.sa_flags = 0;
    sigfillset(&sa_act.sa_mask);

    pid = fork();
    if (pid < 0)    /*Error*/
    {
        perror("fork monitor");
        clean_up(5, semaphore_close, sem_mon, shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t), semaphore_destroy, sem_empty, semaphore_destroy, sem_fill, semaphore_destroy, sem_mutex);
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)  /*Monitor*/
    {
        sa_act.sa_handler=SIG_IGN;
        sigaction(SIGINT, &sa_act, NULL);

        sts_mon=monitor(sh_circbuff, sh_sems);

        clean_up(2, semaphore_close, sem_mon, shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t));
        exit(sts_mon);
        
    }
    else    /*Checker*/
    {    
        sa_act.sa_handler=pall_handler;
        sigaction(SIGINT, &sa_act,NULL);
        
        sts_chk = checker(sh_circbuff, sh_sems, sem_mon);
        wait(&sts_mon);

        clean_up(3,semaphore_destroy, sem_empty, semaphore_destroy, sem_fill, semaphore_destroy, sem_mutex);
        clean_up(3,semaphore_close, sem_mon,  shared_munmap, (void *)sh_circbuff, sizeof(circularbuffer) + 3 * sizeof(sem_t));

        if(sts_mon==EXIT_FAILURE)
            fprintf(stderr, "\nMonitor EXIT_FAILURE");
        
        if(sts_chk == EXIT_FAILURE)
            fprintf(stderr, "\nChecker EXIT_FAILURE");

        exit(sts_chk);
    }
    
}
