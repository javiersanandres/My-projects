#ifndef MYODBC_H
#define MYODBC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "odbc.h"

#define BufferLength 512


struct _MyDataBase
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    int ret; /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */

};

typedef struct _MyDataBase MyDataBase;

/*@null@*/
MyDataBase *init_struct_mydatabase();

int connect_to_my_database(MyDataBase *pMDB);

void alloc_statementhandle(MyDataBase *pMDB);
int free_statementhandle(MyDataBase *pMDB);

int disconnect_from_my_database(MyDataBase *pMDB);

void free_struct_mydatabase(MyDataBase *pMDB);



#endif