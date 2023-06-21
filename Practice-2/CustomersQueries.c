#include "CustomersQueries.h"


#define MULT 10

int C_Find(MyDataBase *pMDB, char *contactname, char ***Data, int *size)
{
    int count=0, i;
    Retornos *ret=NULL;
    char query[]="select c.customernumber, c.customername, c.contactfirstname, c.contactlastname from customers c where c.contactfirstname like ? or c.contactlastname like ? order by c.customernumber;";
    char customernumber[BufferLength];
    
    if (contactname==NULL)
        return EXIT_FAILURE;

    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
        return EXIT_FAILURE;
    

    if ((ret=General_query(pMDB, query, size, 2, 4, SQLCHAR_C, contactname, SQLCHAR_C, contactname, SQLINT_C, SQLCHAR_C, SQLCHAR_C, SQLCHAR_C))==NULL)
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

        sprintf(customernumber ,"%d", ret->integers[0][i]);
        strcat(customernumber, " ");
        strcat(customernumber, ret->strings[0][i]);
        strcat(customernumber, " ");
        strcat(customernumber, ret->strings[1][i]);
        strcat(customernumber, " ");
        strcat(customernumber, ret->strings[2][i]);

        strcpy((*Data)[i], customernumber);
    }
    
    
    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;
}

int ListProducts(MyDataBase *pMDB, int customernumber, char ***Data, int *size, int design_mode)
{
    int count=0, i, max_length=0, length;
    Retornos *ret=NULL;
    char query[]="select p.productname, sum(o1.quantityordered) from orderdetails o1, orders o, products p where o.customernumber=? and o.ordernumber =o1.ordernumber and o1.productcode=p.productcode group by p.productcode order by p.productcode;";
    char quantity[11];


    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
       return EXIT_FAILURE;

    if ((ret=General_query(pMDB, query, size, 1, 2, SQLINT_C, customernumber, SQLCHAR_C, SQLINT_C))==NULL)
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

        sprintf(quantity ,"%d", ret->integers[0][i]);
        
        strcat(ret->strings[0][i], " ");
        strcat(ret->strings[0][i], quantity); 

        length=(int)strlen(ret->strings[0][i]);
        if(length>max_length)
            max_length=length;

        strcpy((*Data)[i],ret->strings[0][i]);
    }
    if (design_mode==1)
        Data_adjust(Data, *size, max_length);
    
    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;

}

int Exist_customernumber(MyDataBase *pMDB, int customernumber,  int *size)
{
    Retornos *ret;
    char query[]="select c.customernumber from customers c where c.customernumber = ?;";

    if ((ret=General_query(pMDB, query, size, 1, 0, SQLINT_C, customernumber))==NULL)
        return EXIT_FAILURE;
    
    free(ret);

    return EXIT_SUCCESS;
}

int Balance(MyDataBase *pMDB, int *size, int customernumber, double *balance)
{
    Retornos *ret;
    char query[]="with tabla1 as (select sum(pa.amount) as Positivo from payments pa where pa.customernumber =?), tabla2 as (select sum(o1.quantityordered*o1.priceeach) as Negativo from orderdetails o1, orders o where o1.ordernumber=o.ordernumber and o.customernumber=?)select (Positivo-Negativo) as Balance from tabla1, tabla2;";

    if(balance==NULL)
        return EXIT_FAILURE;

    if ((ret=General_query(pMDB, query, size, 2, 1, SQLINT_C, customernumber, SQLINT_C, customernumber, SQLDOUBLE_C))==NULL)
        return EXIT_FAILURE;
    if ((*size)>0)
        (*balance)=ret->doubles[0][0];
        
    free(ret);

    return EXIT_SUCCESS;
}


