#include "MenuProducts.h"


static void ProductsMenu();
static int printStock(MyDataBase *pMDB);
static int printFind(MyDataBase *pMDB, int design_mode);


int Productsmain(MyDataBase *pMDB, int design_mode)
{   

    int opcion;
    
    do
    {   
        ProductsMenu();
        
        if ((opcion = get_option(1, 3)) < 1)
            return EXIT_FAILURE;

        switch (opcion)
        {
            
            case 1:
                
                if(printStock(pMDB)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;

            case 2:

                if(printFind(pMDB, design_mode)!=EXIT_SUCCESS)
                    return EXIT_FAILURE;
                
                break;

            case 3:
                return EXIT_SUCCESS;
        }

    }while (1==1); /*PARA EL SPLINT, ES RIDICULO*/

    /*Para el splint, que no se queje, no debería de ser necesario*/
    return EXIT_FAILURE;
}


static void ProductsMenu()
{
    printf("\nProducts Menu:\n");
    printf("  1.Stock\n  2.Find\n  3.Back\n");
    printf("\nEnter a number that corresponds to your choice > ");
}

static int printStock(MyDataBase *pMDB)
{
    char productcode[BufferLength];
    int quantityinstock=0, lastletter;
    printf("\n--------------------------------------------------\n");
    printf("Enter productcode > ");

    
    if(fgets(productcode, BufferLength, stdin) != productcode)
        return EXIT_FAILURE;

    lastletter = (int)strlen(productcode);
    productcode[lastletter - 1] = '\0';

    if(Stock(pMDB, productcode, &quantityinstock) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    
    if(quantityinstock>=0)
        printf("Quantityinstock = %d", quantityinstock);
    else
        printf("There are not any products with that productname");
    printf("\n--------------------------------------------------\n\n");

    return EXIT_SUCCESS;
}

static int printFind(MyDataBase *pMDB, int design_mode)
{

    int lastletter, size=0;
    char likey[BufferLength];
    char **Data=NULL;
    
    printf("\n\nEnter productname > ");

                
    if(fgets((likey+1), BufferLength-2, stdin) != likey+1)
        return EXIT_FAILURE;
    
    likey[0]='%';
    lastletter = (int)strlen(likey);
    likey[lastletter - 1] = '%';

    if(P_Find(pMDB, likey, &Data , &size) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    
    if(size !=0)
    {
        printf("\n\nProducts with that name in between:\n");
        paginacion(Data, size, 10, design_mode);  
    }
    else
        printf("\nThere are not any products containing that name\n");

    printf("\n\n");

    Destroyer(Data, size, 1);
    return EXIT_SUCCESS;
}
