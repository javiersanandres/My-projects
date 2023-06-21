#ifndef PRODUCTSQUERIES_H
#define PRODUCTSQUERIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Consultas.h"  
#include "MYodbc.h"

int Stock(MyDataBase *pMDB, char* productcode, int *quantityinstock);
int P_Find(MyDataBase *pMDB, char *likey, char ***Data, int *size);


#endif