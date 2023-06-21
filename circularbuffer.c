#include <stdio.h>
#include <stdlib.h>

#include "circularbuffer.h"

void circularbuffer_init(circularbuffer *circbuffer)
{
    if (circbuffer == NULL)
        return;

    circbuffer->head = 0;
    circbuffer->tail = 0;
    circbuffer->count = 0;
}
Bool circularbuffer_is_Empty(circularbuffer *circbuffer)
{   
    if(circbuffer==NULL)
        return False;

    if (circbuffer->count == 0)
        return True;
    else
        return False;
}

Bool circularbuffer_is_Full(circularbuffer *circbuffer)
{
    if (circbuffer == NULL)
        return True;

    if (circbuffer->count == MAX_BUFFER_SIZE)
        return True;
    else
        return False;
}

int additem(circularbuffer *circbuffer, Block *block)
{
    if (circbuffer == NULL)
        return -1;

    if (circularbuffer_is_Full(circbuffer))
        return -1; 
    

    block_copy(&(circbuffer->buffer[circbuffer->tail]),block);

    circbuffer->tail = (circbuffer->tail + 1) % MAX_BUFFER_SIZE;
    circbuffer->count++;

    return 0; 
}

// Dequeue a value from the circular buffer
Block *getitem(circularbuffer *circbuffer)
{
    Block *aux;

    if (circbuffer == NULL)
        return NULL;

    if (circularbuffer_is_Empty(circbuffer))
        return NULL; 
    

    aux = &(circbuffer->buffer[circbuffer->head]);
    circbuffer->head = (circbuffer->head + 1) % MAX_BUFFER_SIZE;
    circbuffer->count--;

    return aux; 
}
