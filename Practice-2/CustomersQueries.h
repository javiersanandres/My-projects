#ifndef CUSTOMERSQUERIES_H
#define CUSTOMERSQUERIES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Consultas.h"  
#include "MYodbc.h"

int C_Find(MyDataBase *pMDB, char *contactname, char ***Data, int *size);
int ListProducts(MyDataBase *pMDB, int customernumber, char ***Data, int *size, int design_mode);
int Exist_customernumber(MyDataBase *pMDB, int cusrtomernumber,  int *size);
int Balance(MyDataBase *pMDB, int *size, int customernumber, double *balance);

#endif