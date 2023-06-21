#include "MenuCustomers.h"

static void CustomersMenu();
static int printFind_C(MyDataBase *pMDB, int design_mode );
static int printListProducts(MyDataBase *pMDB, int design_mode);
static int printBalance(MyDataBase *pMDB);

int Customersmain(MyDataBase *pMDB, int design_mode)
{

    int opcion;
    
    do
    {   
        CustomersMenu();

        if ((opcion = get_option(1, 4)) < 1)
            return EXIT_FAILURE;

        switch (opcion)
        {
            
            case 1:
                
                if(printFind_C(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;

            case 2:
                if(printListProducts(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;
            
            case 3:
                if(printBalance(pMDB)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break; 

            case 4:
                return EXIT_SUCCESS;
           

        }

    } while (1==1); /*PARA EL SPLINT, ES RIDICULO*/

    /*Para el splint, que no se queje, no debería de ser necesario*/
    return EXIT_FAILURE;
}

static void CustomersMenu()
{
    printf("\nCustomers Menu:\n");
    printf("  1.Find\n  2.List Products\n  3.Balance\n  4.Back\n");
    printf("\nEnter a number that corresponds to your choice > ");
}

static int printFind_C(MyDataBase *pMDB, int design_mode)
{
    int size=0, lastletter;
    char **Data=NULL;
    char contactname[BufferLength];

    printf("\n\nEnter customer name > ");
    if(fgets((contactname+1), BufferLength-2, stdin) != contactname+1)
        return EXIT_FAILURE;
    
    contactname[0]='%';
    lastletter = (int)strlen(contactname);
    contactname[lastletter - 1] = '%';

    if(C_Find(pMDB, contactname, &Data , &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    if(size !=0)
    {
        paginacion(Data,size,10, design_mode);            
    }
    else
        printf("There's no customer with that name\n\n");
    
    printf("\n\n");

    Destroyer(Data, size, 1);
    return EXIT_SUCCESS;
}

static int printListProducts(MyDataBase *pMDB, int design_mode)
{
    int size=0, customernumber;
    char **Data=NULL;

    printf("\n\nEnter customer number > ");
    (void)scanf("%d", &customernumber);
    (void)getchar();
        
    if (Exist_customernumber(pMDB, customernumber, &size)!= EXIT_SUCCESS)
        return EXIT_FAILURE;

    if (size==0)
    {
        printf("The client number does not exist\n\n");
        return EXIT_SUCCESS;
    }

    if(ListProducts(pMDB, customernumber, &Data , &size, design_mode) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    if(size !=0)
    {
        paginacion(Data,size,10, design_mode);
    }
    else
        printf("The customer has not made any purchase\n\n");

    printf("\n\n");

    Destroyer(Data, size, 1);
    return EXIT_SUCCESS;
}

static int printBalance(MyDataBase *pMDB )
{
    int size=0, customernumber;
    double balance=0;

    printf("\n--------------------------------------------------\n");
    printf("Enter customer number > ");
    (void)scanf("%d", &customernumber);
    (void)getchar();
        
    if (Exist_customernumber(pMDB, customernumber, &size)!= EXIT_SUCCESS)
        return EXIT_FAILURE;

    if (size==0)
    {
        printf("The customer number does not exist");
        printf("\n--------------------------------------------------\n\n");
        return EXIT_SUCCESS;
    }

    if(Balance(pMDB, &size, customernumber, &balance) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    if(size ==1)
    {
        printf("Balance = %0.2f", balance);
    }
    else
        printf("The customer has not made any purchase");
    printf("\n--------------------------------------------------\n\n");

    return EXIT_SUCCESS;
}