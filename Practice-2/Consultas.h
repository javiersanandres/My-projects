
#ifndef CONSULTAS_H
#define CONSULTAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "MYodbc.h"
#define MAX_SELECT 100
#define MAX_ROWS 1000


typedef enum{SQLINT_C=0, SQLCHAR_C=1, SQLDOUBLE_C=2, SQLDATE_C=3}Types;

typedef struct _Parameters{
    int integers[MAX_SELECT];
    double doubles[MAX_SELECT];
    char strings[MAX_SELECT][BufferLength];
    int num_integers;
    int num_doubles;
    int num_strings;
}Parameters;

typedef struct _Retornos{
    int integers[MAX_SELECT][MAX_ROWS];
    double doubles[MAX_SELECT][MAX_ROWS];
    char strings[MAX_SELECT][MAX_ROWS][BufferLength];
}Retornos;


int get_option(int min, int max);
void paginacion(char **Data, int size, int max_lines, int design_mode);
void Destroyer(char **Sol, int size, int type);
int Data_alloc(char ***Data, int size, int *count);
int Data_realloc(char ***Data, int size, int count);
void Data_adjust(char ***Data, int size, int max_length);
/*@null@*/
Retornos* General_query(MyDataBase *pMDB, char *query, int *size, int num_var, int num_rets, ...);


#endif