#include "MenuOrders.h"

static void OrdersMenu();
static int printOpen(MyDataBase *pMDB, int design_mode);
static int printRange(MyDataBase *pMDB, int design_mode );
static int printDetails(MyDataBase *pMDB, int design_mode);

int Ordersmain(MyDataBase *pMDB, int design_mode)
{

    int opcion;
    
    do
    {   
        OrdersMenu();

        if ((opcion = get_option(1, 4)) < 1)
            return EXIT_FAILURE;

        switch (opcion)
        {
            
            case 1:
                
                if(printOpen(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;

            case 2:
                if(printRange(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;
            
            case 3:
                if(printDetails(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break; 

            case 4:
                return EXIT_SUCCESS;


            

        }

    } while (1==1); /*PARA EL SPLINT, ES RIDICULO*/

    /*Para el splint, que no se queje, no debería de ser necesario*/
    return EXIT_FAILURE;

}



static void OrdersMenu()
{
    printf("\nOrders Menu:\n");
    printf("  1.Open\n  2.Range\n  3.Detail\n  4.Back\n");
    printf("\nEnter a number that corresponds to your choice > ");
}

static int printOpen(MyDataBase *pMDB, int design_mode)
{

    int size=0;
    char **Data=NULL;
    
                 
    if(Open(pMDB, &Data , &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    
    printf("\n\nOrdernumbers without an assigned shippeddate:");
    if(size !=0)
    {
        paginacion(Data,size,10, design_mode);
    }
    else
        printf("\nThere is no order without shipping date");

    Destroyer(Data, size, 1);
    return EXIT_SUCCESS;
}

static int printRange(MyDataBase *pMDB, int design_mode )
{

    int size=0;
    char fecha1[BufferLength], fecha2[BufferLength];
    char **Data=NULL;
    
    printf("\n\nEnter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    
    (void)scanf("%s - %s", fecha1, fecha2);
    (void)getchar();

    if(Range(pMDB, fecha1, fecha2, &Data , &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    
    if(size !=0)
    {
        printf("\nOrders made within those dates (Ordernumber | Orderdate | Shippeddate):");
        paginacion(Data,size,10, design_mode);  
    }
    else
        printf("There are not any orders within that range");

    printf("\n\n");

    Destroyer(Data, size, 1);
    return EXIT_SUCCESS;
}

static int printDetails(MyDataBase *pMDB, int design_mode)
{
    char orderdate[BufferLength]="\0", status[BufferLength]="\0";
    int ordernumber, size=0;
    float totalprice=0;
    char **Data=NULL;

    printf("\n\nEnter ordernumber > ");
    (void)scanf("%d", &ordernumber);
    (void)getchar();

    
    if(Exist_ordernumber(pMDB, ordernumber, &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    if (size==0)
    {
        printf("The order number does not exist\n\n");
        return EXIT_SUCCESS;
    }

    if(Details_date_status_price(pMDB, &size, ordernumber, orderdate, status, &totalprice) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    if (size!=0)
    {
        printf("\nOrderdate = %s, Status = %s", orderdate, status);
        printf("\nTotal Price: %.2f", totalprice); 
    } else 
    {
        printf("\n\n\nThere are not any products associated with that order\n");
        return EXIT_SUCCESS;
    }

    
    if(Details_products(pMDB, ordernumber, &Data, &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    if(size !=0) /*Siempre va a ser distinto que cero si ha llegado hasta aquí*/
    {
        printf("\n\n\nProducts associated with that order (productnumber | quantityordered | priceeach):\n");
        paginacion(Data,size,10, design_mode);  
    }

    Destroyer(Data, size, 1);

    printf("\n\n");

    return EXIT_SUCCESS;
}

