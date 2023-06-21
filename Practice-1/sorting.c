/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */


#include "sorting.h"


/***********************************************************************/
/* Function: Array_is_Sorted                                           */
/*                                                                     */
/* Your comment:    Check that ip and iu are not NULL, otherwise the   */
/*                  function won´t work                                */
/*                  (It will NOT through an error if it happens)       */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/*  Checks if a numeric array is ordered in ascending order            */
/*  int *ip is a pointer to the first element                          */
/*  int *iu is a pointer to the last element                           */
/*  returns OK if array is ordered in ascending order, ERR otherwise   */
/*                                                                     */                                    
/***********************************************************************/                          
int Array_is_Sorted(int* ip, int *iu){

  assert(ip!=NULL);
  assert(iu!=NULL);

    while((ip) != iu)
    {
      if(ip[0] >= ip[1])
        return ERR;
      ip++;  
    }
  
  return OK;
}

/***********************************************************************/
/* Function: Array_is_SortedInv                                        */
/*                                                                     */
/* Your comment:    Check that ip and iu are not NULL, otherwise the   */
/*                  function won´t work                                */
/*                  (It will NOT through an error if it happens)       */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/*  Checks if a numeric array is ordered in descending order           */
/*  int *ip is a pointer to the first element                          */
/*  int *iu is a pointer to the last element                           */
/*  returns OK if array is ordered in descending order, ERR otherwise  */
/*                                                                     */                                    
/***********************************************************************/
int Array_is_SortedInv(int* ip, int *iu){

  assert(ip!=NULL);
  assert(iu!=NULL);

    while((ip) != iu)
    {
      if(ip[0] <= ip[1])
        return ERR;
      ip++;  
    }
  
  return OK;
}
/****************************************************************************/
/* Function: :  SelectSort                                                  */
/* Authors:  David Brenchley-Javier San Andres                              */                                 
/*                                                                          */
/* Rutine that orders a permutation in ascending order                      */
/*                                                                          */
/* Comments: Check that array is not Null                                   */
/*           and that iu>=ip>=0 or the function won't work                  */
/*           (It does NOT through an error if such happens)                 */
/*                                                                          */
/* Input:                                                                   */
/* int *array: pointer to table that holds the permutation                  */
/* int ip is the index of the first element                                 */
/* int iu is the index of the last element                                  */
/* Output:                                                                  */
/* returns number of ob (basic operations)                                  */
/****************************************************************************/
int SelectSort(int* array, int ip, int iu)
{
  int ob=0, i=ip, minimo;

  assert(array!=NULL);
  assert(ip>=0);
  assert(ip<=iu);

  while(i<iu)
  {
    minimo=min(array, i,iu, &ob);
    swap(&array[i],&array[minimo]);
    i++;    
  }
  
  assert(Array_is_Sorted(&array[ip],&array[iu])==OK);
  
  return ob;
}

/****************************************************************************/
/* Function: :  SelectSortInv                                               */
/* Authors:  David Brenchley-Javier San Andres                              */                                 
/*                                                                          */
/* Rutine that orders a permutation in descending order                     */
/*                                                                          */
/* Comments: Check that array is not Null                                   */
/*           and that iu>=ip>=0 or the function won't work                  */
/*           (It does NOT through an error if it happens)                   */
/*                                                                          */
/* Input:                                                                   */
/* int *array: pointer to table that holds the permutation                  */
/* int ip is the index of the first element                                 */
/* int iu is the index of the last element                                  */
/* Output:                                                                  */
/* returns number of ob (basic operations)                                  */
/****************************************************************************/
int SelectSortInv(int* array, int ip, int iu)
{
  int ob=0, i=iu, minimo;

  assert(array!=NULL);
  assert(ip>=0);
  assert(ip<=iu);

  while(i>ip)
  {
    minimo=min(array, ip,i, &ob); 
    swap(&array[i],&array[minimo]);
    i--;    
  }
  
  assert(Array_is_SortedInv(&array[ip],&array[iu])==OK);
  return ob;
  
}

/****************************************************************************/
/* Function: min                                                            */
/* Authors:  David Brenchley-Javier San Andres                              */                                 
/*                                                                          */
/* Rutine that returns the position of minimum element in an array          */
/* and saves in ob the number of vasic operation that are executed          */
/*                                                                          */
/*                                                                          */
/* Comments: Check that array and ob are not Null                           */
/*           and that iu>=ip>=0 or the function won't work                  */
/*           (It will NOT through an error if it happens)                   */
/*                                                                          */
/* Input:                                                                   */
/* int *array: pointer to table that holds the permutation                  */
/* int *ob: pointer to variable that holds the number of obs                */
/* int ip is the index of the first element                                 */
/* int iu is the index of the last element                                  */
/* or NULL in case of error                                                 */
/****************************************************************************/
int min(int* array, int ip, int iu, int* ob)
{
  
  int i, minimo=ip;

  assert(array!= NULL);
  assert(ob != NULL);

  for(i=ip+1; i<=iu; i++)
  {
    if(array[i] < array[minimo])
      minimo =i;
    
    (*ob)++;
  }
  
  return minimo;

}

