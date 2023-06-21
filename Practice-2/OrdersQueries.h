#ifndef ORDERSQUERIES_H
#define ORDERSQUERIES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Consultas.h"  
#include "MYodbc.h"

int Open(MyDataBase *pMDB, char ***Data, int *size);
int Range(MyDataBase *pMDB, char *fecha1, char *fecha2, char ***Data, int *size);
int Exist_ordernumber(MyDataBase *pMDB, int ordernumber,  int *size);
int Details_date_status_price(MyDataBase *pMDB, int *size, int ordernumber, char* orderdate, char* status, float *totalprice);
int Details_products(MyDataBase *pMDB, int ordernumber, char ***Data, int *size);

#endif
