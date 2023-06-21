/**
 * @file minero.c
 * @author David Brenchley Uriol-Javier San Andrés.
 * @brief Program in charge of obtaining the preimage of a hash function and analyse the monitor's return value
 * @version 1.5
 * @date 2023-03-09
 */
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
#include <signal.h>
#include <mqueue.h>
#include <semaphore.h>

#include "pow.h"
#include "handlers.h"
#include "funciones.h"
#include "types.h"
#include "sysinfo.h"

#define SHARED_MEMORY "/shared_system_memory"
#define SEMAPHORE_SINC "/semaphore_sinc"

volatile sig_atomic_t pgot_int = 0;
volatile sig_atomic_t pgot_usr1 = 0;
volatile sig_atomic_t pgot_usr2 = 0;
volatile sig_atomic_t pgot_alrm = 0;

volatile long RES = LONG_MIN;   /*Global variable that will hold the result of a mining round*/

/**
 * @brief Private freeing function
 *
 * @param param Pointer to an array of pointers to the paramenter
 * @param n Number of pointers to free in the array
 */
void _free_parameters(long **param, int n);

int _miner_init(sem_t **sem_mon, sem_t **sem_mutex, sem_t **sem_winner, sysinfo **sh_sysinfo, Bool *isfirst);

int _threads_memory_init(pthread_t **hilos, long n_threads, long ***param);

/**
 * @brief Mines for a number of solutions
 *
 * @param  < TARGET_INI > < ROUNDS > < N_THREADS >.
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
int main(int argc, char *argv[])
{

    int j, k, error, wstatus, status;
    int pfd[2], nbytes;
    unsigned int secs;
    long n_threads, obj, aux;
    long **param = NULL;

    struct sigaction sa_act;
    sigset_t new_set, old_set;

    pid_t pid;
    pthread_t *hilos = NULL;
    sem_t *sem_mon = NULL, *sem_mutex = NULL, *sem_winner = NULL;
    sysinfo *sh_sysinfo = NULL;
    Bool isfirst;
    Block aux_blk;
    mqd_t queue;

    if (argc != 3)
    {
        fprintf(stderr, "Error minero: Arguments should be < SECS > < N_THREADS >\n");
        exit(EXIT_FAILURE);
    }

    secs = atoi(argv[1]);
    n_threads = atol(argv[2]);

    /*Create 1 pipe to communicate with the accountant*/
    if (pipe(pfd) < 0)
    {
        perror(" pipe ");
        exit(EXIT_FAILURE);
    }


    sa_act.sa_flags = 0;
    sigfillset(&sa_act.sa_mask);

    pid = fork();
    if (pid < 0) /*Error*/
    {
        perror("fork miner");
        clean_up(2, file_close_fd, pfd[0], file_close_fd, pfd[1]);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) /*Accountant*/
    {
        sa_act.sa_handler=SIG_IGN;
        sigaction(SIGINT, &sa_act, NULL);

        close(pfd[1]);

        status = accountant(pfd[0], getppid());

        close(pfd[0]);

        exit(status);
    }
    else /*Miner*/
    {

        close(pfd[0]);
        status = EXIT_SUCCESS;

        /*Establish signal handlers and signal mask*/
        

        sa_act.sa_handler=pall_handler;

        sigaction(SIGINT, &sa_act, NULL);
        sigaction(SIGUSR1, &sa_act, NULL);
        sigaction(SIGUSR2, &sa_act, NULL);
        sigaction(SIGALRM, &sa_act, NULL);

        /*INITIALIZATION*/

        /*Initializing shared memory and semaphores*/
        if (_miner_init(&sem_mon, &sem_mutex, &sem_winner, &sh_sysinfo, &isfirst) == EXIT_FAILURE)
        {
            fprintf(stderr, "\n_miner_init failure");
            close(pfd[0]);
            wait(NULL);
            exit(EXIT_FAILURE);
        }


        /*Initializing thread memory*/
        if(_threads_memory_init(&hilos, n_threads, &param)==EXIT_FAILURE)
        {
            fprintf(stderr, "\n_pthreads_memory_init failure");
            clean_up(3, file_close_fd, pfd[0],  semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
            wait(NULL);
            exit(EXIT_FAILURE);
        }
       
        /*Adding yourself to the system*/
        sem_wait(sem_mutex);
            error = sysinfo_add_pid(sh_sysinfo, getpid());
        sem_post(sem_mutex);

        if (error == -1)
        {
            fprintf(stderr, "\nMiners full");
            clean_up(4, free_pointer, hilos, file_close_fd, pfd[0], semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
            _free_parameters(param, n_threads);
            wait(NULL);
            exit(EXIT_FAILURE);
        }

        /*EXECUTION*/
        alarm(secs);

        if (isfirst == True) /*First miner - First round*/
        {

            sem_wait(sem_mutex);

                sh_sysinfo->oldblock.solution = FIRST_OBJ; 
                sh_sysinfo->active_miners=1;

                sysinfo_kill_all(sh_sysinfo, SIGUSR1, 0);
                pgot_usr1=0;

            sem_post(sem_mutex);
            

        }
        else /*Another miner - First round*/
        {
            sigemptyset(&new_set);
            sigaddset(&new_set,SIGUSR1);
            sigprocmask(SIG_BLOCK, &new_set, &old_set);

            if(pgot_usr1==0)
                sigsuspend(&old_set);
            
            pgot_usr1=0;
            sigprocmask(SIG_SETMASK, &old_set, NULL);

            sem_wait(sem_mutex);
                sh_sysinfo->active_miners++;
            sem_post(sem_mutex);


        }

        aux = POW_LIMIT / n_threads;
        sem_wait(sem_mutex);
            obj=sh_sysinfo->oldblock.solution;
        sem_post(sem_mutex);
        RES = LONG_MIN;

        sigemptyset(&new_set);
        sigaddset(&new_set,SIGALRM);
        sigaddset(&new_set,SIGINT);
        sigaddset(&new_set,SIGPIPE);

        /*MINING SECTION*/
        while(pgot_alrm==0 && pgot_int==0)
        {

#ifdef DEBUG
    fprintf(stderr, "\n%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s", GREEN ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif
            sigprocmask(SIG_BLOCK, &new_set, NULL);

            for (j = 0; j < n_threads && RES == LONG_MIN  && pgot_usr2==0; j++) /*In case a thread finds the solution before other threads are created*/
            {
                /*Set parameters*/
                param[j][0] = aux * j;
                if (j == n_threads - 1)
                    param[j][1] = POW_LIMIT;
                else
                    param[j][1] = aux * (j + 1);
                param[j][2] = obj;

                error = pthread_create(&hilos[j], NULL, pow_solve, (void *)param[j]); /*Create threads*/
                if (error != 0)
                {
                    perror("pthread_create");
                    clean_up(4, free_pointer, hilos, file_close_fd, pfd[0], semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
                    _free_parameters(param, n_threads);
                    waitpid(pid, NULL, 0);
                    exit(EXIT_FAILURE);
                }
            }

            for (k = 0; k < j; k++)
            {
                error = pthread_join(hilos[k], NULL); /*Wait for created threads*/
                if (error != 0)
                {
                    perror("pthread_join");
                    clean_up(4, free_pointer, hilos, file_close_fd, pfd[0], semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
                    _free_parameters(param, n_threads);
                    waitpid(pid, NULL, 0);
                    exit(EXIT_FAILURE);
                }
            }
#ifdef DEBUG
    fprintf(stderr, "\n%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s", YELLOW ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif

            /*WINNER AND VOTING*/
            if(sem_trywait(sem_winner)==0)
            {
                winner(sh_sysinfo, sem_mutex, sem_mon, obj, RES, getpid());
                sem_post(sem_winner);
            }
            else
            {
                looser(sh_sysinfo, sem_mutex, getpid());
            }

            block_init(&aux_blk);
            sem_wait(sem_mutex);
                block_copy(&aux_blk, &(sh_sysinfo->oldblock));
            sem_post(sem_mutex);

#ifdef DEBUG
    fprintf(stderr, "\n%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s", BLUE ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif


            nbytes = write(pfd[1], (void *)&aux_blk, sizeof(Block));
            if (nbytes == -1)
            {
                perror(" miner write error ");
                clean_up(4, free_pointer, hilos, file_close_fd, pfd[0], semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
                _free_parameters(param, n_threads);
                wait(NULL);
                exit(EXIT_FAILURE);
            }

            obj = aux_blk.solution;
            RES = LONG_MIN;

            sigprocmask(SIG_UNBLOCK, &new_set, NULL);

#ifdef DEBUG
    fprintf(stderr, "\n%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s", MAGENTA ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif

        }
        /*Block to prevent a signal from interrupting the mutex*/
        sigprocmask(SIG_BLOCK, &new_set, NULL);

        /*Remove yourself from the system*/
        sem_wait(sem_mutex);
            
        sysinfo_remove_pid(sh_sysinfo, getpid());
        sh_sysinfo->active_miners--;

        /*Last miner*/
        if(sh_sysinfo->num_miners==0)
        {
            if(sem_trywait(sem_mon)==0)
            {
                if((queue = mq_open(MQ_NAME, O_WRONLY)) != -1)
                {   
                    
                    block_init(&aux_blk);
                    aux_blk.objective=LONG_MIN;
                    aux_blk.solution=LONG_MIN;

                    mq_send(queue, (char *)&(aux_blk), sizeof(Block),1);
                    mq_close(queue);
                }

                sem_post(sem_mon);
            }   

            clean_up(5, semaphore_unlink, SEMAPHORE_SINC,semaphore_unlink, SEMAPHORE_MONITOR_ALIVE, shared_unlink, SHARED_MEMORY, semaphore_destroy, sem_mutex, semaphore_destroy, sem_winner);

        }
        else /*Not the last one*/
        {
            sem_post(sem_mutex);
        }   


        /*Close pipe too signal accountant to finish*/
        close(pfd[1]);

        /*Wait for the monitor to end*/
        wait(&wstatus); 

        if (WIFEXITED(wstatus)==0)
            fprintf(stderr, "\nAssistant EXIT_FAILURE\n");

        if(pgot_int==1)
            fprintf(stdout, "\nFinishing by sigint...\n");
        else
            fprintf(stdout, "\nFinishing by alarm...\n");

        fflush(stdout);

        clean_up(3, free_pointer, hilos, semaphore_close, sem_mon, shared_munmap, (void *)sh_sysinfo, sizeof(sysinfo) + 2*sizeof(sem_t));
        _free_parameters(param, n_threads);

        exit(status);
    }
}

int _miner_init(sem_t **sem_mon, sem_t **sem_mutex, sem_t **sem_winner, sysinfo **sh_sysinfo, Bool *isfirst)
{
    int shm_fd;
    sem_t *sem_sinc = NULL;

    /*Create named sempahore for sincronizing monitors between themselves*/
    if ((sem_sinc = sem_open(SEMAPHORE_SINC, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        if ((sem_sinc = sem_open(SEMAPHORE_SINC, 0)) == SEM_FAILED)
        {
            perror("sem_open: ");
            return (EXIT_FAILURE);
        }
    }

    /*Create named sempahore for sincronizing checker with monitors*/
    if (((*sem_mon) = sem_open(SEMAPHORE_MONITOR_ALIVE, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        if (((*sem_mon) = sem_open(SEMAPHORE_MONITOR_ALIVE, 0)) == SEM_FAILED)
        {
            perror("sem_open: ");
            sem_close(sem_sinc);
            return (EXIT_FAILURE);
        }
    }

    /*Creates shared memory segment*/
    if ((shm_fd = shm_open(SHARED_MEMORY, O_CREAT | O_EXCL | O_RDWR, 0666)) == -1)
    {
        *isfirst = False;

        /*Wait for first miner and signal next miner*/
        sem_wait(sem_sinc);
        sem_post(sem_sinc);

        if ((shm_fd = shm_open(SHARED_MEMORY, O_RDWR, 0)) == -1) /*Opens shared memory segment*/
        {
            perror("shm_open");
            clean_up(2, semaphore_close, sem_sinc, semaphore_close, *sem_mon);
            return (EXIT_FAILURE);
        }

        /*Map shared memory*/
        if (((*sh_sysinfo) = (sysinfo *)mmap(NULL, sizeof(sysinfo) + 2*sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            clean_up(3, semaphore_close, sem_sinc, semaphore_close, *sem_mon, file_close_fd, shm_fd );
            return (EXIT_FAILURE);
        }
        close(shm_fd);

        (*sem_mutex) = (sem_t *)(*sh_sysinfo + 1);
        (*sem_winner) = (sem_t *)(*sem_mutex + 1);

    }
    else
    {

        *isfirst = True;
        /*Truncate (extend) shared memory file*/
        if (ftruncate(shm_fd, sizeof(sysinfo) + 2*sizeof(sem_t)) == -1)
        {
            perror("ftruncate");
            clean_up(4, shared_unlink, SHARED_MEMORY, semaphore_close, sem_sinc, semaphore_close, *sem_mon, file_close_fd, shm_fd );
            exit(EXIT_FAILURE);
        }

        /*Map shared memory*/
        if (((*sh_sysinfo) = (sysinfo *)mmap(NULL, sizeof(sysinfo) + 2*sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED)
        {
            perror("mmap");
            clean_up(4, shared_unlink, SHARED_MEMORY, semaphore_close, sem_sinc, semaphore_close, *sem_mon, file_close_fd, shm_fd );
            return (EXIT_FAILURE);
        }
        close(shm_fd);

        sysinfo_init(*sh_sysinfo);

        (*sem_mutex) = (sem_t *)(*sh_sysinfo + 1);

        if (sem_init((*sem_mutex), 1, 1) == -1)
        {
            perror("sem_init");
            clean_up(4, shared_unlink, SHARED_MEMORY, semaphore_close, sem_sinc, semaphore_close, *sem_mon, shared_munmap, (void *)(*sh_sysinfo), sizeof(sysinfo) + 2*sizeof(sem_t));
            return (EXIT_FAILURE);
        }

        (*sem_winner) = (sem_t *)(*sem_mutex + 1);

        if (sem_init((*sem_winner), 1, 1) == -1)
        {
            perror("sem_init");
            clean_up(5, shared_unlink, SHARED_MEMORY, semaphore_destroy, *sem_mutex, semaphore_close, sem_sinc, semaphore_close, *sem_mon, shared_munmap, (void *)(*sh_sysinfo), sizeof(sysinfo) + 2*sizeof(sem_t));
            return (EXIT_FAILURE);
        }

        /*Signal miners*/
        sem_post(sem_sinc);

    }

    sem_close(sem_sinc);

    return EXIT_SUCCESS;

}


int _threads_memory_init(pthread_t **hilos, long n_threads, long ***param)
{
    int i;

    if (((*hilos) = (pthread_t *)malloc(n_threads * sizeof(pthread_t))) == NULL) /*Create thread identifiers*/
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    if (((*param) = (long **)malloc(n_threads * sizeof(long *))) == NULL) /*Creates array of pointers for the thread parameters*/
    {
        perror("malloc");
        free(*hilos);
        return EXIT_FAILURE;
    }

    for (i = 0; i < n_threads; i++)
    {
        if(((*param)[i] = (long *)malloc(3 * sizeof(long))) == NULL) /*Reserve parameters for each specific thread*/
        {
            perror("malloc");
            free(*hilos);
            _free_parameters((*param), i);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void _free_parameters(long **param, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        free(param[i]);
    }
    free(param);
}



