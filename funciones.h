#ifndef FUNCIONES_H
#define FUNCIONES_H

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
#include <signal.h>
#include <stdarg.h>

#include "circularbuffer.h"
#include "pow.h"
#include "types.h"
#include "sysinfo.h"
#include "block_wallet.h"


/**
 * @brief Checker function 
 *
 * @param sh_circbuff Pointer to shared memory with circular buffer
 * @param sh_sems Pointer to the shared memory with consumer-producer semaphores
 * @param sem_mon Pointer to named semaphore to signal miners that monitor exists
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
int checker(circularbuffer *sh_circbuff, sem_t *sh_sems, sem_t* sem_mon);

/**
 * @brief Monitor function 
 *
 * @param sh_circbuff Pointer to shared memory with circular buffer
 * @param sh_sems Pointer to the shared memory with consumer-producer semaphores
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
int monitor(circularbuffer *sh_circbuff, sem_t *sh_sems);


/**
 * @brief Accountant function 
 *
 * @param fr Pipe reading file description
 * @param ppid Parents pid
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
int accountant(int fr, pid_t ppid);


int winner(sysinfo *sysinfo, sem_t *sem_mutex, sem_t *sem_mon, long obj, long sol, pid_t pid);

int looser(sysinfo *sysinfo, sem_t *sem_mutex,pid_t pid);

/**
 * @brief Mines for the objective in a given interval
 *
 * @param  arg Pointer to array of arguments (lower bound | upper bound | objective).
 *
 * @return Returns NULL
 */
void *pow_solve(void *arg);

/**
 * @brief Private clean_up function
 *
 * @param  num_args Number of arguments
 * @param ... Variables arguments, must preceed each pointer with a Clean_up type (types.h)
 *
 * @return Returns EXIT_SUCCESS or EXIT_FAILURE in case of error
 */
void clean_up(int num_args, ...);




#endif
