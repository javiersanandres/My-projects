#include "funciones.h"

extern volatile sig_atomic_t pgot_int;
extern volatile sig_atomic_t pgot_usr1;
extern volatile sig_atomic_t pgot_usr2;
extern volatile sig_atomic_t pgot_alrm;

extern long RES;

void _add_termination_block(sem_t *sem_empty, sem_t *sem_fill, sem_t *sem_mutex, circularbuffer *sh_circbuff);

/*CHECKER AND MONITOR FUNCTIONS*/

int checker(circularbuffer *sh_circbuff, sem_t *sh_sems, sem_t *sem_mon)
{
    mqd_t queue;
    Block blk;
    sigset_t new_set;
    sem_t *sem_empty, *sem_fill, *sem_mutex;
    struct mq_attr attributes;

    sem_empty = sh_sems;
    sem_fill = sh_sems + 1;
    sem_mutex = sh_sems + 2;

    attributes.mq_maxmsg = 7;
    attributes.mq_msgsize = sizeof(Block);

    if ((queue = mq_open(MQ_NAME, O_CREAT | O_RDONLY | O_EXCL, S_IRUSR | S_IWUSR, &attributes)) == (mqd_t)-1)
    {
        perror("mq_open");
        _add_termination_block(sem_empty, sem_fill, sem_mutex, sh_circbuff);
        return EXIT_FAILURE;
    }
    /*Signal miners that queue is ready*/
    sem_post(sem_mon);

    blk.objective = 0;
    blk.solution = 0;

    sigemptyset(&new_set);
    sigaddset(&new_set,SIGINT);

    while ((blk.objective != LONG_MIN || blk.solution != LONG_MIN) && pgot_int==0)
    {

#ifdef DEBUG
    fprintf(stderr, "%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s\n", CYAN ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif
        
        if (mq_receive(queue, (char *)&blk, sizeof(blk), NULL) == -1)
        {

            if(pgot_int==0) /*Check mq_receive didn't exit due to SIGINT*/
            {
                perror("mq_receive");
                _add_termination_block(sem_empty, sem_fill, sem_mutex, sh_circbuff); /*Signal consumer to finish */
                clean_up(2,queue_close, queue, queue_unlink, MQ_NAME);
                return EXIT_FAILURE;
            }
        }

#ifdef DEBUG
    fprintf(stderr, "%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s\n", CYAN ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif

        sigprocmask(SIG_BLOCK, &new_set, NULL);

        if(pgot_int==0)
        {
            sem_wait(sem_empty);
            sem_wait(sem_mutex);

            if (pow_hash(blk.solution) == blk.objective)
                blk.valid = True;
            else
                blk.valid = False;

            additem(sh_circbuff, &blk);

            sem_post(sem_mutex);
            sem_post(sem_fill);

        }
        
        sigprocmask(SIG_UNBLOCK, &new_set, NULL);

#ifdef DEBUG
    fprintf(stderr, "%s %s %s %d int=%d usr1=%d usr2=%d alrm=%d %s\n", CYAN ,__FILE__,__func__, __LINE__, pgot_int, pgot_usr1, pgot_usr2, pgot_alrm, WHITE);
#endif

    }

    /*Check for SIGINT*/
    if (pgot_int != 0)
    {
        _add_termination_block(sem_empty, sem_fill, sem_mutex, sh_circbuff);
    }

    /*Signal miners queue is down*/
    sem_wait(sem_mon);
    clean_up(2,queue_close, queue, queue_unlink, MQ_NAME);
    
    return EXIT_SUCCESS;
}

int monitor(circularbuffer *sh_circbuff, sem_t *sh_sems)
{

    Block *blk;
    sem_t *sem_empty, *sem_fill, *sem_mutex;
    Bool finish = False;

    sem_empty = sh_sems;
    sem_fill = sh_sems + 1;
    sem_mutex = sh_sems + 2;

    while (1)
    {

        sem_wait(sem_fill);
        sem_wait(sem_mutex);

        blk = getitem(sh_circbuff);

        if (blk->objective == LONG_MIN && blk->solution == LONG_MIN)
        {
            finish = True;
        }
        else
        {
            fblock_print(blk, stdout);
            fflush(stdout);
        }

        sem_post(sem_mutex);
        sem_post(sem_empty);
        
        if (finish == True)
            break;
    }

    return EXIT_SUCCESS;
}


int accountant(int fr, pid_t ppid)
{
    int  nbytes, fd;
    char pid_name[20];
    Block blk;

    sprintf(pid_name, "account%d.dat", ppid);

    if ((fd = open(pid_name, O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, 0666)) == -1)
    {
        return EXIT_FAILURE;
    }

    do
    {
        nbytes = read(fr, &blk, sizeof(Block));
        if (nbytes == -1)
        {
            perror(" accountant read");
            close(fd);
            return EXIT_FAILURE;
        }

        if (nbytes != 0) /*Check that monitor hasn´t closed its pipe end*/
        {

            dblock_print(&blk, fd);
            fsync(fd);
        }

    } while (nbytes != 0);

    close(fd);
    return EXIT_SUCCESS;
}

int winner(sysinfo *sysinfo, sem_t *sem_mutex, sem_t *sem_mon, long obj, long sol, pid_t pid)
{
    int i;
    mqd_t queue;
    struct timespec nano;

    if (sysinfo == NULL || sem_mutex == NULL)
        return EXIT_FAILURE;

    sem_wait(sem_mutex);

        block_init(&(sysinfo->newblock));
        sysinfo->newblock.block_id = sysinfo->oldblock.block_id + 1;
        sysinfo->newblock.objective = obj;
        sysinfo->newblock.solution = sol;
        sysinfo->newblock.winner = pid;
        sysinfo->newblock.total_votes = sysinfo->active_miners;

        sysinfo->num_curvotes=0;

        sysinfo_kill_all(sysinfo, SIGUSR2, 1);
        pgot_usr2=0;

        /*Vote*/
        if (pow_hash(sol) == obj)
            sysinfo_set_vote(sysinfo, pid, True);
        else
            sysinfo_set_vote(sysinfo, pid, False);

    sem_post(sem_mutex);

    nano.tv_sec = (time_t)0;
    nano.tv_nsec = (time_t)100;

    while (sysinfo->num_curvotes != sysinfo->active_miners)
    {
        nanosleep(&nano, NULL);
    }

    sem_wait(sem_mutex);

        for (i = 0; i < sysinfo->active_miners; i++)
        {
            if (sysinfo->votes[i] == True)
                sysinfo->newblock.positive_votes++;
        }

        if (sysinfo->newblock.positive_votes > sysinfo->newblock.total_votes / 2)
        {
            sysinfo->newblock.valid = True;
            sysinfo_add_coin(sysinfo, pid);
        }

        for (i = 0; i < sysinfo->active_miners; i++)
        {
            sysinfo->newblock.wallets[i].miner_id = sysinfo->pids[i];
            sysinfo->newblock.wallets[i].coins = sysinfo->coins[i];
        }
        
        if(sem_trywait(sem_mon)==0)
        {
            if((queue = mq_open(MQ_NAME, O_WRONLY)) != -1)
            {
                mq_send(queue, (char *)&(sysinfo->newblock), sizeof(Block),1);
                mq_close(queue);
            }

            sem_post(sem_mon);
        }   

        block_copy(&(sysinfo->oldblock), &(sysinfo->newblock));

        nano.tv_sec = (time_t)0;
        nano.tv_nsec = (time_t)500000000;
        nanosleep(&nano, NULL);

        sysinfo_kill_all(sysinfo, SIGUSR1, 0);
        pgot_usr1=0;

    sem_post(sem_mutex);

    return EXIT_SUCCESS;
}

int looser(sysinfo *sysinfo, sem_t *sem_mutex,pid_t pid)
{
    sigset_t new_set, old_set;

    sigemptyset(&new_set);
    sigaddset(&new_set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &new_set, &old_set);

    if (pgot_usr2 == 0)
        sigsuspend(&old_set);

    pgot_usr2=0;
    sigprocmask(SIG_SETMASK, &old_set, NULL);

    /*Vote*/
    sem_wait(sem_mutex);

        if (pow_hash(sysinfo->newblock.solution) == sysinfo->newblock.objective)
            sysinfo_set_vote(sysinfo, pid, True);
        else
            sysinfo_set_vote(sysinfo, pid, False);

    sem_post(sem_mutex);

    /*Wait for sigusr1*/
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_set, &old_set);

    if (pgot_usr1 == 0)
        sigsuspend(&old_set);

    pgot_usr1=0;
    sigprocmask(SIG_SETMASK, &old_set, NULL);

    return EXIT_SUCCESS;
}

void clean_up(int num_args, ...)
{
    va_list list;
    int clean_type;
    int i;

    if (num_args <= 0)
    {
        return;
    }

    va_start(list, num_args);
    for (i = 0; i < num_args; i++)
    {
        clean_type = va_arg(list, int);

        switch (clean_type)
        {
        case free_pointer:
            free(va_arg(list, void *));
            break;
        case file_close_fd:
            close(va_arg(list, int));
            break;
        case file_unlink:
            unlink(va_arg(list, char *));
            break;
        case shared_unlink:
            shm_unlink(va_arg(list, char *));
            break;
        case shared_munmap:
            munmap(va_arg(list, void *), va_arg(list, size_t));
            break;
        case semaphore_close:
            sem_close(va_arg(list, sem_t *));
            break;
        case semaphore_unlink:
            sem_unlink(va_arg(list, char *));
            break;
        case semaphore_destroy:
            sem_destroy(va_arg(list, sem_t *));
            break;
        case queue_close:
            mq_close(va_arg(list, mqd_t));
            break;
        case queue_unlink:
            mq_unlink(va_arg(list, char *));
            break;
        }
    }
    va_end(list);
}

void _add_termination_block(sem_t *sem_empty, sem_t *sem_fill, sem_t *sem_mutex, circularbuffer *sh_circbuff)
{
    Block blk;

    sem_wait(sem_empty);
    sem_wait(sem_mutex);

    blk.objective = LONG_MIN;
    blk.solution = LONG_MIN;

    additem(sh_circbuff, &blk);

    sem_post(sem_mutex);
    sem_post(sem_fill);
}
