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
int Array_is_Sorted(int *ip, int *iu)
{

  assert(ip != NULL);
  assert(iu != NULL);

  while ((ip) != iu)
  {
    if (ip[0] >= ip[1])
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
int Array_is_SortedInv(int *ip, int *iu)
{

  assert(ip != NULL);
  assert(iu != NULL);

  while ((ip) != iu)
  {
    if (ip[0] <= ip[1])
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
int SelectSort(int *array, int ip, int iu)
{
  int ob = 0, i = ip, minimo;

  assert(array != NULL);
  assert(ip >= 0);
  assert(ip <= iu);

  while (i < iu)
  {
    minimo = min(array, i, iu, &ob);
    swap(&array[i], &array[minimo]);
    i++;
  }

  assert(Array_is_Sorted(&array[ip], &array[iu]) == OK);

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
int SelectSortInv(int *array, int ip, int iu)
{
  int ob = 0, i = iu, minimo;

  assert(array != NULL);
  assert(ip >= 0);
  assert(ip <= iu);

  while (i > ip)
  {
    minimo = min(array, ip, i, &ob);
    swap(&array[i], &array[minimo]);
    i--;
  }

  assert(Array_is_SortedInv(&array[ip], &array[iu]) == OK);
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
int min(int *array, int ip, int iu, int *ob)
{

  int i, minimo = ip;

  assert(array != NULL);
  assert(ob != NULL);

  for (i = ip + 1; i <= iu; i++)
  {
    if (array[i] < array[minimo])
      minimo = i;

    (*ob)++;
  }

  return minimo;
}


/****************************************************************************/
/* Function: MergeSort                                                      */
/* Authors:  David Brenchley-Javier San Andres                              */
/*                                                                          */
/* Rutine that orders a permutation using Divide and Conquer methodology    */
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
/* returns number of ob (basic operations) or ERR otherwise                 */
/****************************************************************************/
int MergeSort(int *array, int ip, int iu)
{
  int M, aux, ob = 0;


  assert(array != NULL);
  assert(ip >= 0);
  assert(iu >= ip);

  if (ip == iu)
    return 0;

  
  else
  {
    
    M = (ip + iu) / 2;

    if((aux=MergeSort(array, ip, M))==ERR)
      return ERR;
    ob+=aux;

    if((aux=MergeSort(array, M + 1, iu))==ERR)
      return ERR;
    ob+=aux;

    if((aux=merge(array, ip, iu, M))==ERR)
      return ERR;
    ob += aux;

  }

  assert(Array_is_Sorted(&array[ip], &array[iu]) == OK);

  return ob;
}

/******************************************************************************/
/* Function: Merge                                                            */
/* Authors:  David Brenchley-Javier San Andres                                */
/*                                                                            */
/* Rutine merges (joins together in order) two consequtive parts of an array  */
/*                                                                            */
/* Comments: Check that array is not Null                                     */
/*           and that iu>=ip>=0 or the function won't work                    */
/*           (It does NOT through an error if it happens)                     */
/*                                                                            */
/* Input:                                                                     */
/* int *array: pointer to table that holds the permutation                    */
/* int ip is the index of the first element                                   */
/* int iu is the index of the last element                                    */
/* int medio is the index of the last element in the first part before        */
/*            the first element of the second part (the middle element)       */
/* Output:                                                                    */
/* returns number of ob (basic operations) or Err in case of error            */
/******************************************************************************/

int merge(int *array, int ip, int iu, int medio)
{
  int *array_aux, ob=0, i = ip, j = medio + 1, k = 0;

  if ((array_aux = (int *)malloc((iu - ip + 1) * sizeof(array_aux[0]))) == NULL)
    return ERR;

  while (i <= medio && j <= iu)
  {
    if (array[i] <= array[j])
    {
      array_aux[k] = array[i];
      i++;
    }
    else
    {
      array_aux[k] = array[j];
      j++;
    }
    k++;
    ob++;
  }

  while (i <= medio)
  {
    array_aux[k] = array[i];
    k++;
    i++;
    ob++;
  }

  while (j <= iu)
  {
    array_aux[k] = array[j];
    k++;
    j++;
    ob++;
  }

  array_aux -= ip;
  for (i = ip; i <= iu; i++)
  {
    array[i] = array_aux[i];
  }
  array_aux += ip;

  free(array_aux);

  return ob;

  /*Numericamente se podria devolver ob=iu-ip+1 directamente*/
}




int QuickSort_median(int *array, int ip, int iu)
{
  return QuickSort(array, ip, iu, median);
}

int QuickSort_median_avg(int *array, int ip, int iu)
{
  return QuickSort(array, ip, iu, median_avg);
}

int QuickSort_median_stat(int *array, int ip, int iu)
{
  return QuickSort(array, ip, iu, median_stat);
}


/******************************************************************************/
/* Function: QuickSort                                                        */
/* Authors:  David Brenchley-Javier San Andres                                */
/*                                                                            */
/* Rutine QuickSort orders an array using Divide and COnquer methodology      */
/*                                                                            */
/* Comments: Check that array is not Null                                     */
/*           and that iu>=ip>=0 or the function won't work                    */
/*           (It does NOT through an error if it happens)                     */
/*                                                                            */
/* Input:                                                                     */
/* int *array: pointer to table that holds the permutation                    */
/* int ip is the index of the first element                                   */
/* int iu is the index of the last element                                    */
/* int funct is the function that chooses the pivot during each iteration     */
/*                                                                            */
/* Output:                                                                    */
/* returns number of ob (basic operations) or Err in case of error            */
/******************************************************************************/
int QuickSort(int *array, int ip, int iu, pQS_pivotf funct)
{
  int M, ob = 0;
  assert(array != NULL);
  assert(ip >= 0);
  assert(iu >= ip);
  assert(funct!=NULL);

  if(ip>iu)
    return ERR;

  if(ip==iu)
    return 0;
  
  ob+=partition(array, ip, iu, &M, funct);

  if(ip<M-1)
    ob+=QuickSort(array, ip, M-1, funct);
  
  if(M+1<iu)
    ob+=QuickSort(array, M+1, iu, funct);

  assert(Array_is_Sorted(&array[ip], &array[iu]) == OK);

  return ob;
    
}


int partition(int *array, int ip, int iu, int *M, pQS_pivotf funct)
{
  int i, k, ob=0;

  assert(M != NULL);
  assert(funct != NULL);


  ob+=funct(array,ip,iu, M);

  k=array[(*M)];
  
  swap(&array[ip], &array[(*M)]);

  (*M)=ip;
  
  for (i=ip+1; i<=iu; i++)
  {
    if(array[i]<k)
    {
      (*M)++;
      swap(&array[i], &array[(*M)]);
    }
    ob++;
  }

  swap(&array[ip], &array[(*M)]);

  return ob;
  
}

int median(int *array, int ip, int iu, int *pos)
{
   
  assert(pos!=NULL);

  *pos=ip;
  
  return 0;
  
}

int median_avg(int *array, int ip, int iu, int *pos)
{
  
  assert(pos!=NULL);

  *pos=(ip+iu)/2;

  return 0;
    
}

int median_stat(int *array, int ip, int iu, int *pos)
{

  int ob=0;
  
  assert(pos!=NULL);

  if (array[ip]<array[(ip+iu)/2])
  {
    if (array[(ip+iu)/2]<array[iu])
      *pos=(ip+iu)/2;
    else
    {
      if (array[iu]<array[ip])
        *pos=ip;
      else
        *pos=iu;

      ob++;
    }
    ob++;
  }
  else
  {
    if(array[ip]<array[iu])
      *pos=ip;
    else
    {
      if(array[(ip+iu)/2]<array[iu])
        *pos=iu;
      else
        *pos=(ip+iu)/2;
      ob++;
    }
    ob++;  
  }
  
  ob++;

  return ob;
}

