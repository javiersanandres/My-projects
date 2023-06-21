#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MYodbc.h"
#include "MenuProducts.h"
#include "MenuOrders.h"
#include "MenuCustomers.h"

static void MainMenu()
{
    printf("\nCompany Menu:\n");
    printf("  1.Products\n  2.Orders\n  3.Customers\n  4.Exit\n");
    printf("\nEnter a number that corresponds to your choice > ");
}

int main(int argc, char *argv[])
{
    int opcion, state, design_mode=0;
    MyDataBase *bd;

    if (argc>2)
        return EXIT_FAILURE;

    if (argc==2)
    {
        design_mode=atoi(argv[1]);
        if (design_mode!=1 && design_mode!=0)
            return EXIT_FAILURE;
    }

    if ((bd = init_struct_mydatabase()) == NULL)
        return EXIT_FAILURE;

    if (connect_to_my_database(bd) == EXIT_FAILURE)
    {
        printf("\n\nEXIT FAILURE\n");
        free_struct_mydatabase(bd);
        return EXIT_FAILURE;
    }


    printf("\n\n   ABRIENDO MENU\n\n");
    
    state=1;
    while(state==1)
    {
        MainMenu();

        if ((opcion = get_option(1, 4)) < 1)
        {
            state=0;
            break;
        }
        switch (opcion)
        {
            case 1:
                if (Productsmain(bd, design_mode) != EXIT_SUCCESS)
                    state=0;
                break;
            case 2:
                if (Ordersmain(bd, design_mode) != EXIT_SUCCESS)
                    state=0;
                break;
            case 3:
                if (Customersmain(bd, design_mode) != EXIT_SUCCESS)
                    state=0;
                break;
            case 4:
                printf("\n\n  CERRANDO MENU\n\n");
                state=2;
                break;
                
        }
    }


    if (disconnect_from_my_database(bd) == EXIT_FAILURE)
        state=0;

    free_struct_mydatabase(bd);
    if(state==2)
    {
        printf("\n\nEXIT SUCCESS\n");
        return EXIT_SUCCESS;
    }
    else
    {
        printf("\n\nEXIT FAILURE\n");
        return EXIT_FAILURE;
    }
}
