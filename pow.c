/**
 * @file pow.c
 * @author SOPER teaching team.
 * @brief Computation of the POW.
 * @version 1.5
 * @date 2023-02-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <pthread.h>

#include "pow.h"

#define PRIME POW_LIMIT
#define BIG_X 435679812
#define BIG_Y 100001819

extern volatile sig_atomic_t pgot_usr2;
extern long RES;

long int pow_hash(long int x)
{
  
  long int result = (x * BIG_X + BIG_Y) % PRIME;

  return result;
}



/*MINER FUNCTION*/

void *pow_solve(void *arg)
{
    long i, bot, top, obj;
    long *param;

    param = (long *)arg;
    bot = param[0];
    top = param[1];
    obj = param[2];

    for (i = bot; i < top; i++)
    {
        if (pow_hash(i) == obj)
        {
            RES = i;
        }

        if (RES != LONG_MIN || pgot_usr2 != 0) /*If a thread finds the result all of them stop automatically, except the main thread*/
        {
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}