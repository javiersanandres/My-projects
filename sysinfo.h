#ifndef SYSINFO_H
#define SYSINFO_H

#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

#include "types.h"
#include "block_wallet.h"

/**
  @brief sysinfo structure
*/
typedef struct _sysinfo
{
  int num_miners;
  int active_miners;
  int num_curvotes;
  pid_t pids[MAX_MINERS];
  Bool votes[MAX_MINERS];
  int coins[MAX_MINERS];
  Block oldblock;
  Block newblock;
} sysinfo;


void sysinfo_init(sysinfo *sysinfo);

Bool sysinfo_isfull(sysinfo *sysinfo);

int sysinfo_add_pid(sysinfo *sysinfo, pid_t pid);
int sysinfo_remove_pid(sysinfo *sysinfo, pid_t pid);

int sysinfo_add_coin(sysinfo *sysinfo, pid_t pid);

int sysinfo_set_vote(sysinfo *sysinfo, pid_t pid, Bool val);

void sysinfo_kill_all(sysinfo *sysinfo, int sig, int all_actv);

#endif