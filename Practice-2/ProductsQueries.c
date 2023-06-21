#include "ProductsQueries.h"

#define MULT 10


int Stock(MyDataBase *pMDB, char* productcode, int *quantityinstock)  
{ 
    int size=0;
    Retornos *ret;
    char query[]="select quantityinstock from products where productcode = ?;";

    if(productcode==NULL || quantityinstock==NULL)
        return EXIT_FAILURE;

    if ((ret=General_query(pMDB, query, &size, 1, 1, SQLCHAR_C, productcode, SQLINT_C))==NULL)
        return EXIT_FAILURE;

    if (size>0)
        *quantityinstock=ret->integers[0][0];
    else
        *quantityinstock=-1;
    
    free(ret);
    return EXIT_SUCCESS;
}

int P_Find(MyDataBase *pMDB, char *likey, char ***Data, int *size)
{
    int count=0 ,i;
    Retornos *ret;
    char query[]="select p.productcode, p.productname from products p where productname like ?;";
    
    if(likey==NULL)
        return EXIT_FAILURE;


    if (Data_alloc(Data, MULT, &count)==EXIT_FAILURE)
       return EXIT_FAILURE;

    if ((ret=General_query(pMDB, query, size, 1, 2, SQLCHAR_C, likey, SQLCHAR_C, SQLCHAR_C))==NULL)
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

        strcat(ret->strings[0][i], " ");
        strcat(ret->strings[0][i], ret->strings[1][i]); 

        strcpy((*Data)[i],ret->strings[0][i]);
    }

    Destroyer((*Data+(*size)), count*MULT-(*size), 0);

    free(ret);

    return EXIT_SUCCESS;
}
