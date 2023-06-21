
#include <stdio.h>
#include <stdlib.h>

#include "block_wallet.h"

/*WALLET FUNCTIONS*/

void wallet_init(Wallet *wlt)
{
    if (wlt == NULL)
        return;

    wlt->miner_id = 0;
    wlt->coins = 0;
}

int wallet_copy(Wallet *dest_wlt, Wallet *org_wlt)
{
    if(dest_wlt==NULL || org_wlt==NULL)
        return -1;
    
    dest_wlt->miner_id=org_wlt->miner_id;
    dest_wlt->coins=org_wlt->coins;

    return 0;
}

void dwallet_print(Wallet *wlt, int fd)
{
    if(wlt==NULL || fd<0)
        return;
    
    dprintf(fd, "%d:%d ", wlt->miner_id, wlt->coins);

}

/*BLOCK FUNCTIONS*/

void block_init(Block *blk)
{
    int i;

    if (blk == NULL)
        return;
        
    blk->block_id = 0;
    blk->winner = 0;
    blk->objective = 0;
    blk->solution = 0;
    blk->valid = False;
    blk->total_votes = 0;
    blk->positive_votes = 0;

    for (i = 0; i < MAX_MINERS; i++)
    {
        wallet_init(&(blk->wallets[i]));
    }
}

int block_copy(Block *dest_blk, Block *org_blk)
{
    int i;

    if (dest_blk == NULL || org_blk == NULL)
        return -1;

    dest_blk->block_id = org_blk->block_id;
    dest_blk->winner = org_blk->winner;
    dest_blk->valid = org_blk->valid;
    dest_blk->objective = org_blk->objective;
    dest_blk->solution = org_blk->solution;
    dest_blk->positive_votes = org_blk->positive_votes;
    dest_blk->total_votes = org_blk->total_votes;

    for (i = 0; i < MAX_MINERS; i++)
    {
        wallet_copy(&(dest_blk->wallets[i]),&(org_blk->wallets[i]));
    }

    return 0;
}

void fblock_print(Block *blk, FILE *file)
{
    if (blk == NULL || file == NULL)
        return;

    dblock_print(blk, fileno(file));
}

void dblock_print(Block *blk, int fd)
{
    int i;

    if (blk == NULL || fd < 0)
        return;

    dprintf(fd, "-Id:\t\t %04d\n Winner:\t %d\n Target:\t %08ld\n", blk->block_id, blk->winner, blk->objective);
    if (blk->valid == True)
    {
        dprintf(fd, " Solution:\t %08ld (validated)\n", blk->solution);
    }
    else
    {
        dprintf(fd, " Solution:\t %08ld (rejected)\n", blk->solution);
    }
    dprintf(fd, " Votes:\t\t %d/%d\n Wallets:\t ", blk->positive_votes, blk->total_votes);

    for (i = 0; i < blk->total_votes; i++)
    {
        dwallet_print(&(blk->wallets[i]), fd);
    }
    dprintf(fd, "\n\n");
}
