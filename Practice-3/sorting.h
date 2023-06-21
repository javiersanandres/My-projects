/**
 *
 * Descripcion: Header file for sorting functions 
 *
 * Fichero: sorting.h
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "permutations.h"

#ifndef SORTING_H
#define SORTING_H

/* constants */

#ifndef ERR
  #define ERR -1
  #define OK (!(ERR))
#endif

/* type definitions */
typedef int (* pfunc_sort)(int*, int, int);
typedef int (* pQS_pivotf)(int * , int, int, int *);

/* Functions */
int Array_is_Sorted(int* ip, int *iu);
int Array_is_SortedInv(int* ip, int *iu);

int SelectSort(int* array, int ip, int iu);
int SelectSortInv(int* array, int ip, int iu);
int min(int* array, int ip, int iu, int *ob);

int MergeSort(int* array, int ip, int iu);
int merge(int *array, int ip, int iu, int medio);


int QuickSort_median(int *array, int ip, int iu);
int QuickSort_median_avg(int *array, int ip, int iu);
int QuickSort_median_stat(int *array, int ip, int iu);

int QuickSort(int *array, int ip, int iu, pQS_pivotf funct);
int partition(int *array, int ip, int iu, int *M, pQS_pivotf);


int median(int *array, int ip, int iu, int *pos);
int median_avg(int *array, int ip, int iu, int *pos);
int median_stat(int *array, int ip, int iu, int *pos);

#endif
