#ifndef MENUORDERS_H
#define MENUORDERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "OrdersQueries.h"
#include "MYodbc.h"

int Ordersmain(MyDataBase *pMDB, int design_mode);

#endif