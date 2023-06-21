#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include "types.h"
#include "block_wallet.h"

#define MAX_BUFFER_SIZE 5

/**
 * @brief Circular buffer structure
 */
typedef struct _circularbuffer
{
    Block buffer[MAX_BUFFER_SIZE];    // Pointer to the buffer array
    int head;                         // Head index
    int tail;                         // Tail index
    int count;                        // Number of elements in the buffer
    
}circularbuffer;

// Function prototypes

/**
 * @brief Inititate circular buffer values
 *
 * @param circbuffer Pointer to the circular buffer
 */
void circularbuffer_init(circularbuffer *circbuffer);

/**
 * @brief Indicate if the buffer is empty
 *
 * @param circbuffer Pointer to the circular buffer
 * 
 * @return True if empty, False otherwise
 */
Bool circularbuffer_is_Empty(circularbuffer *circbuffer);

/**
 * @brief Indicate if the buffer is full
 *
 * @param circbuffer Pointer to the circular buffer
 * 
 * @return True if full, False otherwise
 */
Bool circularbuffer_is_Full(circularbuffer *circbuffer);

/**
 * @brief Adds a Block to the circular buffer
 *
 * @param circbuffer Pointer to the circular buffer
 * @param block Pointer to Block to be added (copied) on to the buffer
 * 
 * @return 0 if successful, -1 if error
 */
int additem(circularbuffer *circbuffer, Block *block);

/**
 * @brief Gets a Block from the circular buffer
 *
 * @param circbuffer Pointer to the circular buffer
 * 
 * @return Pointer to Block or NULL if error 
 *         (The Block is still in the buffer, it isn´t copied,so may be overwritten if not careful)
 */
Block *getitem(circularbuffer *circbuffer);

#endif
