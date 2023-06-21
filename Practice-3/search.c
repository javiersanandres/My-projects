/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */

#include "search.h"

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "permutations.h"
#include "sorting.h"

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be
 *				 allocated externally to the function.
 */

/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
    keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
  {
    keys[i] = .5 + max / (1 + (max) * ((double)rand() / (RAND_MAX)));
  }

  return;
}

PDICT init_dictionary(int size, char order)
{
  PDICT dict;
  assert(size > 0);
  assert((int)order == SORTED || (int)order == NOT_SORTED);

  if ((dict = (PDICT)malloc(sizeof(DICT))) == NULL)
    return NULL;

  dict->size = size;
  dict->n_data = 0;
  dict->order = order;

  if ((dict->table = (int *)malloc(size * sizeof(int))) == NULL)
  {
    free(dict);
    return NULL;
  }

  return dict;
}

void free_dictionary(PDICT pdict)
{
  free(pdict->table);
  free(pdict);
}

int insert_dictionary(PDICT pdict, int key)
{

  int i;

  assert(pdict != NULL);
  assert(key >= 0);

  if (pdict->n_data == pdict->size)
    return ERR;

  i = pdict->n_data;
  pdict->table[i] = key;
  pdict->n_data++;

  if ((int)pdict->order == SORTED)
  {
    while (i >= 1 && pdict->table[i-1] > key)
    {
      swap(&(pdict->table[i]), &(pdict->table[i - 1]));
      i--;
    }

    assert(Array_is_Sorted(&pdict->table[0], &(pdict->table[pdict->n_data - 1])) == OK);
  }

  return OK;
}

int massive_insertion_dictionary(PDICT pdict, int *keys, int n_keys)
{
  int i;
  assert(pdict != NULL);
  assert(keys != NULL);
  assert(n_keys > 0);
  assert(pdict->size - pdict->n_data - n_keys >= 0);

  for (i = 0; i < n_keys; i++)
    insert_dictionary(pdict, keys[i]);

  return OK;
}

int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method)
{
  int ob;

  assert(pdict != NULL);
  assert(key > 0);
  assert(ppos != NULL);
  assert(method != NULL);

  ob = method(pdict->table, 0, pdict->n_data - 1, key, ppos);

  return ob;
}

/* Search functions of the Dictionary ADT */
int bin_search(int *table, int F, int L, int key, int *ppos)
{
  int M, ob;

  assert(table != NULL);
  assert(F >= 0);
  assert(L >= F);
  assert(key >= 0);
  assert(ppos != NULL);

  ob = 0;
  while (F <= L)
  {
    M = (F + L) / 2;

    if (key == table[M])
    {
      *ppos = M;
      return ob+1;
    }
    else if (key < table[M])
      L = M - 1;
    else
      F = M + 1;

    ob++;
  }

  *ppos=NOT_FOUND;

  return ob;
}

int lin_search(int *table, int F, int L, int key, int *ppos)
{
  int i, ob;

  assert(table != NULL);
  assert(F >= 0);
  assert(L >= F);
  assert(key >= 0);
  assert(ppos != NULL);

  i = F;
  ob = 0;
  while (i <= L)
  {
    ob++;
    if (table[i] == key)
      break;

    i++;
  }

  if (i > L)
    *ppos=NOT_FOUND;
  else
    *ppos = i;

  return ob;
}

int lin_auto_search(int *table, int F, int L, int key, int *ppos)
{
  int ob;

  assert(table != NULL);
  assert(F >= 0);
  assert(L >= F);
  assert(key >= 0);
  assert(ppos != NULL);

  ob = lin_search(table, F, L, key, ppos);

  if (*ppos != NOT_FOUND && *ppos != F)
  {
    swap(&table[*ppos], &table[*ppos - 1]);
    (*ppos)--;
  }

  return ob;
}
