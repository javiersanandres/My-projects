#include "OrdersQueries.h"


#define MULT 10


#include <stdio.h>

int Open(MyDataBase *pMDB, char ***Data, int *size)
{
    int count=0, i;
    Retornos *ret=NULL;
    char query[]="select ordernumber from orders where shippeddate is null order by ordernumber";
    char ordernumber[11];
    
    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if ((ret=General_query(pMDB, query, size, 0, 1, SQLINT_C))==NULL)
    {
        Destroyer(*Data, MULT, 1);
        return EXIT_FAILURE;
    }

    for (i=0;i<(*size);i++)
    {
        if (i==count*MULT)
        {
            count++;
            if (Data_realloc(Data, MULT, count)==EXIT_FAILURE)
            {
                free(ret);
                return EXIT_FAILURE;
            }
        }

        sprintf(ordernumber ,"%d", ret->integers[0][i]);
        strcpy((*Data)[i], ordernumber);
    }
    
    
    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;
}

int Range(MyDataBase *pMDB, char *fecha1, char *fecha2, char ***Data, int *size)
{
    int count=0 ,i;
    Retornos *ret=NULL;
    char query[]="select o.ordernumber, o.orderdate, coalesce(cast(o.shippeddate as varchar(10)), 'NULL') from orders o where orderdate >= ? and orderdate <= ? order by ordernumber;";
    char ordernumber[BufferLength];

    
    if(fecha1==NULL || fecha2==NULL)
        return EXIT_FAILURE;

    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if ((ret=General_query(pMDB, query, size, 2, 3, SQLCHAR_C, fecha1, SQLCHAR_C, fecha2, SQLINT_C, SQLCHAR_C, SQLCHAR_C))==NULL)
    {
        Destroyer(*Data, MULT, 1);
        return EXIT_FAILURE;
    }

    for (i=0;i<(*size);i++)
    {
        if (i==count*MULT)
        {
            count++;
            if (Data_realloc(Data, MULT, count)==EXIT_FAILURE)
            {
                free(ret);
                return EXIT_FAILURE;
            }
        }

        sprintf(ordernumber, "%d", ret->integers[0][i]);
        strcat(ordernumber, " ");
        strcat(ordernumber, ret->strings[0][i]);
        strcat(ordernumber, " ");
        strcat(ordernumber, ret->strings[1][i]);

        strcpy((*Data)[i], ordernumber);
    }
    
    
    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;
    
}

int Exist_ordernumber(MyDataBase *pMDB, int ordernumber,  int *size)
{
    Retornos *ret;
    char query[]="select o.ordernumber from orders o where o.ordernumber = ?;";

    if ((ret=General_query(pMDB, query, size, 1, 0, SQLINT_C, ordernumber))==NULL)
        return EXIT_FAILURE;
    
    free(ret);

    return EXIT_SUCCESS;
}



int Details_date_status_price(MyDataBase *pMDB, int *size, int ordernumber, char* orderdate, char* status, float *totalprice)  
{ 
    Retornos *ret;
    char query[]="WITH date_status AS (select o.orderdate, o.status, o.ordernumber from orders o where ordernumber = ?), price as (select sum(o.quantityordered*o.priceeach) as price, o.ordernumber from orderdetails o where ordernumber = ? GROUP BY o.ordernumber) SELECT orderdate, status, price from date_status natural join price;";
    
    
    if(orderdate==NULL || status==NULL)
        return EXIT_FAILURE;

    if ((ret=General_query(pMDB, query, size, 2, 3, SQLINT_C, ordernumber, SQLINT_C, ordernumber,SQLCHAR_C, SQLCHAR_C, SQLDOUBLE_C))==NULL)
        return EXIT_FAILURE;
    
    if (size!=0)
    {
        strcpy(orderdate, ret->strings[0][0]);
        strcpy(status, ret->strings[1][0]);
        *totalprice=(float)ret->doubles[0][0];
    }
    
    free(ret);

    return EXIT_SUCCESS;
}

int Details_products(MyDataBase *pMDB, int ordernumber, char ***Data, int *size)
{
    int count=0 , i;
    Retornos *ret;
    char query[]="select o.productcode, o.quantityordered, o.priceeach from orderdetails o  where o.ordernumber = ? order by orderlinenumber;";
    char quantityordered[11], priceeach[14];
    
    if(Data == NULL)
        return EXIT_FAILURE;

    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
        return EXIT_FAILURE;
    
    if ((ret=General_query(pMDB, query, size, 1, 3, SQLINT_C, ordernumber, SQLCHAR_C, SQLINT_C, SQLDOUBLE_C))==NULL)
    {
        Destroyer(*Data, MULT, 1);
        return EXIT_FAILURE;
    }

    
    for (i=0;i<(*size);i++)
    {
        if (i==count*MULT)
        {
            count++;
            if (Data_realloc(Data, MULT, count)==EXIT_FAILURE)
            {
                free(ret);
                return EXIT_FAILURE;
            }
        }

        sprintf(quantityordered,"%d", ret->integers[0][i]);
        sprintf(priceeach ,"%.2f", ret->doubles[0][i]);
        strcat(ret->strings[0][i], " ");
        strcat(ret->strings[0][i], quantityordered);
        strcat(ret->strings[0][i], " ");
        strcat(ret->strings[0][i], priceeach);

        strcpy((*Data)[i], ret->strings[0][i]);
    }
    
    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;
}

