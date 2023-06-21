/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.1
 * Fecha: 21-09-2019
 *
 */

#include "permutations.h"


/***************************************************/
/* Function: random_num                            */
/* Authors:  David Brenchley-Javier San Andres     */
/*                                                 */
/* Rutine that generates a random number           */
/* between two given numbers                       */
/*                                                 */
/* Comments: Check that sup >= inf >=0             */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/***************************************************/
int random_num(int inf, int sup)
{
  assert(0 <= inf);
  assert(inf <= sup);
  return inf + (int)(sup - inf + 1.0)*(rand() / (RAND_MAX+1.0));
}

/***************************************************/
/* Function: swap                                  */
/* Authors:  David Brenchley-Javier San Andres     */                                 
/*                                                 */
/* Rutine tha swaps the value of two elements      */
/*                                                 */
/* Comments: Check that both values are not NULL   */
/*           or the function won´t work            */
/*                                                 */
/* Input:                                          */
/* int *x: pointer to the first element            */
/* int *y: Pointer to the seconde element          */
/* Output:                                         */
/* Returns void                                    */
/***************************************************/
void swap(int* x, int* y)
{
  int aux;
  assert(x!=NULL);
  assert(y!=NULL);

  aux =*x;
  *x=*y;
  *y=aux;
}


/***************************************************/
/* Function: generate_perm                         */
/* Authors:  David Brenchley-Javier San Andres     */                                 
/*                                                 */
/* Rutine that generates a random permutation      */
/*                                                 */
/* Comments: Check that N>0                        */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: Pointer to  array                        */
/*        that contains the permutation            */
/* NULL in case of error                           */
/***************************************************/

/* Algortimo fisher-yates shuffle */

int *generate_perm(int N)
{

  int* perm, i;
  
  assert(N > 0);

  if ((perm = (int *) malloc(N * sizeof(perm[0]))) == NULL)
    return NULL;

  for (i = 0; i < N; i++)  
    perm[i] = i + 1;
  

  for (i = 0; i < N-1; i++)
    swap(&perm[i], &perm[random_num(i, N-1)]);

  return perm;
}

/***************************************************/
/* Function: generate_permutations                 */
/* Authors:  David Brenchley-Javier San Andres     */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Comments: n_perms and N must be larger than 0   */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/*                                                 */
/* Output:                                         */
/* int**: Pointer to array of pointers that point  */
/*        to each of the permutations              */
/* NULL in case of error                           */
/*                                                 */
/***************************************************/
int **generate_permutations(int n_perms, int N)
{
  int** perm_table, i,j; 

  assert (n_perms > 0);
  assert (N > 0);

  if((perm_table = (int **)malloc(n_perms*sizeof(perm_table[0])))==NULL)
    return NULL;

  for(i=0; i<n_perms; i++)
  {
    
    if((perm_table[i]=generate_perm(N))==NULL)
    {

      for(j=0; j<i; j++)
        free(perm_table[j]);

      free(perm_table);
      return NULL; 
    }
  }
    
  return perm_table;
}
