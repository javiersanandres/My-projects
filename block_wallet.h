#ifndef BLOCK_WALLET_H
#define BLOCK_WALLET_H

#include <unistd.h>
#include <semaphore.h>

#include "types.h"

#pragma pack(1)

/**
  @brief Wallet structure
*/
typedef struct _Wallet
{
  pid_t miner_id;
  int coins;

}Wallet;

/**
  @brief Block structure
*/
typedef struct _Block
{
  int block_id;               /*Block id*/
  pid_t winner;               /*Winner's id*/
  long objective;             /*Mining objective*/
  long solution;              /*Mining solution*/
  Bool valid;                 /*Solution validation flag*/
  int total_votes;            /*Total votes used to validate*/
  int positive_votes;         /*Total positive votes*/
  Wallet wallets[MAX_MINERS]; /*Miners' wallets*/

} Block;



/*WALLET FUNCTIONS*/

void wallet_init(Wallet *wlt);

int wallet_copy(Wallet *dest_wlt, Wallet *org_wlt);

void dwallet_print(Wallet *wlt, int fd);



/*BLOCK FUNCTIONS*/

void block_init(Block *blk);

int block_copy(Block *dest_blk, Block *org_block);

void fblock_print(Block *blk, FILE *file);

void dblock_print(Block *blk, int fd);


#endif