#include "MYodbc.h"

/*@null@*/
MyDataBase *init_struct_mydatabase()
{
    MyDataBase *pMDB;
    if((pMDB=(MyDataBase *)calloc(1,sizeof(MyDataBase)))==NULL)
        return NULL;

    return pMDB;
}

int connect_to_my_database(MyDataBase *pMDB)
{
    
    pMDB->ret = odbc_connect(&pMDB->env, &pMDB->dbc);
    if (!SQL_SUCCEEDED(pMDB->ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}


void alloc_statementhandle(MyDataBase *pMDB)
{
    
    /* Allocate a statement handle */
    pMDB->ret = SQLAllocHandle(SQL_HANDLE_STMT, pMDB->dbc, &(pMDB->stmt));

}


int free_statementhandle(MyDataBase *pMDB)
{
    /* free up statement handle */
    pMDB->ret2 = SQLFreeHandle(SQL_HANDLE_STMT, pMDB->stmt);
    if (!SQL_SUCCEEDED(pMDB->ret2)) {
        odbc_extract_error("", pMDB->stmt, SQL_HANDLE_STMT);
        return pMDB->ret;
    }
    return EXIT_SUCCESS;

    
}
int disconnect_from_my_database(MyDataBase *pMDB)
{


    /* DISCONNECT */
    pMDB->ret = odbc_disconnect(pMDB->env, pMDB->dbc);
    if (!SQL_SUCCEEDED(pMDB->ret)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void free_struct_mydatabase(MyDataBase *pMDB)
{
    free(pMDB);

}