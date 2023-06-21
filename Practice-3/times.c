/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "times.h"

/*********************************************************************************************************/
/*
  Vamos a utilizar para medir el tiempo la función clock() en vez de clock_get_time().
  El motivo para esta elección es que que no estamos mediendo el tiempo
  exacto promedio ya que no medimos el tiempo que tarda en ordenar cada array de permutaciones.
  Por el contrario, se ha optado por medir solo dos tiempos y, dado que ahí ya acumulamos algo de error,
  no consideramos necesario ajustar nuestra precisión con la otra función.
*/
/*********************************************************************************************************/

/****************************************************************************/
/* Function: average_sorting_time                                           */
/* Authors:  David Brenchley-Javier San Andres                              */
/*                                                                          */
/* Rutine that saves to a time_aa structure, and the average, max           */
/* and min basic operation for a certain ordering algorithm                 */
/* that orders an N size permuation                                         */
/*                                                                          */
/*                                                                          */
/* Comments: Check that method and ptime are not Null                       */
/*           and that n_perms >0 and N>0  or the function won't work        */
/*           (It will NOT through an error if it happens)                   */
/*                                                                          */
/* Input:                                                                   */
/* pfunc_sort method: pointer to ordering function                          */
/* int N: size of the permutations                                          */
/* int n_perms: Number of diferent permutations of size N that will be      */
/* ordered to calculate different executing values                          */
/* Return OK or ERR in case of error                                        */
/****************************************************************************/
short average_sorting_time(pfunc_sort metodo, int n_perms, int N, PTIME_AA ptime)
{
  int **permutaciones, i, temp, ret = OK;
  double ini, fin;

  assert(metodo != NULL);
  assert(ptime != NULL);
  assert(N > 0);
  assert(n_perms > 0);

  if ((permutaciones = generate_permutations(n_perms, N)) == NULL)
    return ERR;

  ptime->N = N;
  ptime->n_elems = n_perms;
  ptime->average_ob = 0;
  ptime->max_ob = 0;
  ptime->min_ob = INT_MAX;

  ini = clock();
  for (i = 0; i < ptime->n_elems; i++)
  {
    if ((temp = metodo(permutaciones[i], 0, N - 1)) == ERR)
    {
      ret = ERR;
      break;
    }

    if (temp < ptime->min_ob)
      ptime->min_ob = temp;

    if (temp > ptime->max_ob)
      ptime->max_ob = temp;

    ptime->average_ob += temp;
  }
  fin = clock();

  if (ret == OK)
  {
    ptime->time = (double)(fin - ini) / CLOCKS_PER_SEC / n_perms;
    ptime->average_ob /= n_perms;
  }

  for (i = 0; i < n_perms; i++)
    free(permutaciones[i]);

  free(permutaciones);
  return ret;
}

/*****************************************************************************/
/* Function: generate_sorting_times                                         */
/* Authors:  David Brenchley-Javier San Andres                              */
/*                                                                          */
/* Rutine that generates a file with the avarage execution time, and the    */
/* average, max and min basic operation for a certain ordering algorithm    */
/* that orders random permutation of different sizes                        */
/*                                                                          */
/*                                                                          */
/* Comments: Check that method and file are not Null                        */
/*           and that num_max>=num_min>0 and incr>0 and n_perms>0          */
/*           or the function won't work                                     */
/*           (It will NOT through an error if it happens)                   */
/*                                                                          */
/* Input:                                                                   */
/* pfunc_sort method: pointer to ordering function                          */
/* char* file: pointer to array with the file name that will store the data */
/* int num_min: number of elements in the first set of permutations         */
/* int num_max: number of elements in the last set of permutations          */
/* int incr: Number of elements to be incremented for each set permutation  */
/* int n_perms: Number of reps to be executed for each set of permutation   */
/* Return OK or ERR in case of error                                        */
/****************************************************************************/
short generate_sorting_times(pfunc_sort method, char *file, int num_min, int num_max, int incr, int n_perms)
{

  PTIME_AA pt;
  int i, j, n_sizes = ((num_max - num_min) / incr) + 1;

  assert(method != NULL);
  assert(file != NULL);
  assert(num_min > 0);
  assert(num_max >= num_min);
  assert(incr > 0);
  assert(n_perms > 0);

  if ((pt = (PTIME_AA)malloc(n_sizes * sizeof(TIME_AA))) == NULL)
    return ERR;

  for (i = num_min, j = 0; i <= num_max; i += incr, j++)
  {

    if (average_sorting_time(method, n_perms, i, &pt[j]) == ERR)
    {
      free(pt);
      return ERR;
    }
  }

  if (save_time_table(file, pt, n_sizes) == ERR)
  {
    free(pt);
    return ERR;
  }

  free(pt);

  return OK;
}

/************************************************************************************/
/* Function: save_time_table                                                        */
/* Authors:  David Brenchley-Javier San Andres                                      */
/*                                                                                  */
/* Rutine that generates a file with the data held in the time_aa structs           */
/*                                                                                  */
/*                                                                                  */
/* Comments: Check that ptime and file are not Null                                 */
/*           and that n_times>0 or the function won't work                          */
/*           (It will NOT through an error if it happens)                           */
/*                                                                                  */
/* Input:                                                                           */
/* char* file: pointer array that holds the file name that will store the data      */
/* PTIME_AA ptime: Pointer to array of time_aa structs that hold execution data     */
/* int n_times: number of time_aa structs                                           */
/* Return OK or Err if an error occurs                                              */
/************************************************************************************/
short save_time_table(char *file, PTIME_AA ptime, int n_times)
{
  int i;
  FILE *f;

  if ((f = fopen(file, "w")) == NULL)
    return ERR;

  for (i = 0; i < n_times; i++)
  {
    fprintf(f, "     %010d        %011.8f          %013.2f           %010d          %010d      \n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
  }

  fclose(f);
  return OK;
}

short average_search_time(pfunc_search metodo, pfunc_key_generator generator, char order, int N, int n_times, PTIME_AA ptime)
{
  int *keys, *dictkeys, i, pos, temp;
  double ini, fin;
  PDICT dict;

  assert(metodo != NULL);
  assert(generator != NULL);
  assert(ptime != NULL);
  assert((int)order == SORTED || (int)order == NOT_SORTED);
  assert(N > 0);
  assert(n_times > 0);

  if ((dict = init_dictionary(N, order)) == NULL)
    return ERR;

  if ((dictkeys = generate_perm(N)) == NULL)
  {
    free_dictionary(dict);
    return ERR;
  }

  if (massive_insertion_dictionary(dict, dictkeys, N) == ERR)
  {
    free(dictkeys);
    free_dictionary(dict);
    return ERR;
  }

  ptime->N = N;
  ptime->n_elems = N * n_times; /*ptime->n_elem represents the total number of searches*/
  ptime->average_ob = 0;
  ptime->max_ob = 0;
  ptime->min_ob = INT_MAX;

  if ((keys = (int *)malloc(ptime->n_elems * sizeof(int))) == NULL)
  {
    free(dictkeys);
    free_dictionary(dict);
    return ERR;
  }

  generator(keys, ptime->n_elems, N); /*N reprsents the max key in this function*/

  ini = clock();
  for (i = 0; i < ptime->n_elems; i++)
  {
    temp=search_dictionary(dict, keys[i], &pos, metodo);
    
    if (temp < ptime->min_ob)
      ptime->min_ob = temp;

    if (temp > ptime->max_ob)
      ptime->max_ob = temp;

    ptime->average_ob += temp;
  }
  fin = clock();

  ptime->time = (double)(fin - ini) / CLOCKS_PER_SEC / ptime->n_elems;
  ptime->average_ob /= ptime->n_elems;
  

  free(dictkeys);
  free_dictionary(dict);
  free(keys);

  return OK;
}

short generate_search_times(pfunc_search method, pfunc_key_generator generator, char order, char *file, int num_min, int num_max, int incr, int n_times)
{

  PTIME_AA pt;
  int i, j, n_sizes = ((num_max - num_min) / incr) + 1;

  assert(method != NULL);
  assert(generator != NULL);
  assert(file != NULL);
  assert(num_min > 0);
  assert(num_max >= num_min);
  assert(incr > 0);
  assert(n_times > 0);

  if ((pt = (PTIME_AA)malloc(n_sizes * sizeof(TIME_AA))) == NULL)
    return ERR;

  for (i = num_min, j = 0; i <= num_max; i += incr, j++)
  {

    if (average_search_time(method, generator, order, i, n_times, &pt[j]) == ERR)
    {
      free(pt);
      return ERR;
    }
  }

  if (save_time_table(file, pt, n_sizes) == ERR)
  {
    free(pt);
    return ERR;
  }

  free(pt);

  return OK;
}
