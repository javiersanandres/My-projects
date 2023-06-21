/**
 *
 * Descripcion: Header for functions for permutations generation
 *
 * Fichero: permutations.h
 * Autor: Carlos Aguirre 
 * Version: 1.1
 * Fecha: 16-09-2019
 *
 */

#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

/* constants */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <assert.h>  

#ifndef ERR
  #define ERR -1
  #define OK (!(ERR))
#endif


int random_num(int inf, int sup);
int* generate_perm(int N);
int** generate_permutations(int n_perms, int N);
void swap (int* x, int* y);

#endif
