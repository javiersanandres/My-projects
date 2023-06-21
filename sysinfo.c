#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"

void sysinfo_init(sysinfo *sysinfo)
{
    int i;

    if (sysinfo == NULL)
        return;

    sysinfo->num_miners = 0;
    sysinfo->active_miners = 0;
    sysinfo->num_curvotes = 0;

    for (i = 0; i < MAX_MINERS; i++)
    {
        sysinfo->pids[i] = 0;
        sysinfo->coins[i] = 0;
        sysinfo->votes[i] = False;
    }

    block_init(&(sysinfo->oldblock));
    block_init(&(sysinfo->newblock));
}

Bool sysinfo_isfull(sysinfo *sysinfo)
{
    if (sysinfo == NULL)
        return True;

    if (sysinfo->num_miners == MAX_MINERS)
        return True;
    else
        return False;
}

int sysinfo_add_pid(sysinfo *sysinfo, pid_t pid)
{
    if (sysinfo == NULL || pid < 0)
        return -1;

    if (sysinfo_isfull(sysinfo) == True)
        return -1;

    sysinfo->pids[sysinfo->num_miners] = pid;
    sysinfo->coins[sysinfo->num_miners] = 0;
    sysinfo->num_miners++;

    return 0;
}

int sysinfo_remove_pid(sysinfo *sysinfo, pid_t pid)
{
    int i;

    if (sysinfo == NULL || pid < 0)
        return -1;

    for (i = 0; i < sysinfo->num_miners; i++)
    {
        if (sysinfo->pids[i] == pid)
        {
            sysinfo->pids[i] = sysinfo->pids[sysinfo->num_miners - 1];
            sysinfo->coins[i] = sysinfo->coins[sysinfo->num_miners - 1];
            sysinfo->votes[i] = sysinfo->votes[sysinfo->num_miners - 1];

            sysinfo->pids[sysinfo->num_miners - 1] = 0;
            sysinfo->coins[sysinfo->num_miners - 1] = 0;
            sysinfo->votes[i] = 0;

            sysinfo->num_miners--;

            return 0;
        }
    }

    return -1;
}

int sysinfo_add_coin(sysinfo *sysinfo, pid_t pid)
{
    int i;

    if (sysinfo == NULL || pid < 0)
        return -1;

    for (i = 0; i < sysinfo->num_miners; i++)
    {
        if (sysinfo->pids[i] == pid)
        {
            sysinfo->coins[i]++;
            return 0;
        }
    }

    return -1;
}

int sysinfo_set_vote(sysinfo *sysinfo, pid_t pid, Bool val)
{
    int i;

    if (sysinfo == NULL || pid < 0)
        return -1;

    for (i = 0; i < sysinfo->num_miners; i++)
    {
        if (sysinfo->pids[i] == pid)
        {
            sysinfo->votes[i] = val;
            sysinfo->num_curvotes++;
            return 0;
        }
    }

    return -1;
}

void sysinfo_kill_all(sysinfo *sysinfo, int sig, int all_actv)
{
    int i, max;

    if (sysinfo == NULL || sig < 1)
        return;

    if (all_actv == 0)
        max = sysinfo->num_miners;
    else
        max = sysinfo->active_miners;
    for (i = 0; i < max; i++)
    {
        kill(sysinfo->pids[i], sig);
    }
}
