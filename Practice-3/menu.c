#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "utils.h"

#define max_name 512

static void MainMenu();

static int get_option(int min, int max);

int main()
{
    int opcion, limit;
    char garbage;
    char table_name[max_name] = "\0";
    char index_name[max_name] = "\0";
    char book_key[max_name];
    char book_title[max_name];
    char aux[max_name];
    Book book;

    printf("\n\n   ABRIENDO MENU\n\n");

    while (true)
    {
        MainMenu();

        if ((opcion = get_option(1, 4)) < 1)
            return 0;

        switch (opcion)
        {
        case 1:

            printf("\nIntroduce el nombre de la tabla de datos (sin .dat)> ");
            (void*)fgets(table_name, max_name, stdin);
            table_name[strlen(table_name)-1] = '\0';
            strcat(table_name, ".dat");

            if (createTable(table_name) == false)
            {
                fprintf(stderr, "\nError: No se ha podido crear la tabla de datos");
                return 0;
            }

            replaceExtensionByIdx(table_name, index_name);
 
            break;

        case 2:

            if (strcmp(table_name, "\0") == 0)
            {
                printf("\nNo has seleccionado una tabla");
                break;
            }

            printf("\nIntroduce la clave del libro a insertar > ");
            (void*)fgets(book_key,max_name, stdin);
            if(strlen(book_key)!=PK_SIZE+1 || book_key[PK_SIZE]!='\n')
            {
                printf("\nClave de libro no valido");
                break;
            }
            if (strlen(book_key) > 4 && book_key[4] != '\n')
                while ((garbage = (char)getchar()) != '\n' && (int)garbage != EOF);

            printf("\nIntroduce el titulo del libro a insertar > ");
            (void*)fgets(book_title, max_name, stdin);
            book_title[strlen(book_title)-1] = '\0';
            

            memcpy(book.book_id, book_key, PK_SIZE);
            book.title_len = strlen(book_title);
            if((book.title=(char *)calloc(book.title_len,sizeof(char)))==NULL)
            {
                fprintf(stderr, "\nError, no se ha podido reservar memoria para book.title");
                return 0;
            }
            memcpy(book.title, book_title, book.title_len);
            

            if(addTableEntry(&book, table_name, index_name)==false) /*Si falla, la función ya muestra un mensaje de error*/
            {
                /*Se ha añadido este hueco por si en el futuro se quire cambiar las cabeceras de las funciones de bool a int y hacer un control de errores más profundo*/
                free(book.title);
                break;
            } 
            
            free(book.title);
            break;

        case 3:

            if (strcmp(table_name, "\0") == 0)
            {
                printf("\nNo has seleccionado una tabla");
                break;
            }

            printf("\nIndroduce el numero de niveles a imprimir (0 para el root, -1 para todos) > ");
            (void*)fgets(aux, max_name, stdin);
            aux[strlen(aux)-1]='\0';
            if((limit=atoi(aux))<-1)
            {
                printf("\nLimite incorrecto");
                break;
            }
            if(limit==-1)
                limit=INT_MAX;

            printf("\n");
            printTree((size_t)(limit-1), index_name);

            break;

        case 4:
            
            printf("\n\n  CERRANDO MENU\n\n");
            return 0;
        }
    }
}

static void MainMenu()
{
    printf("\n\nCompany Menu:\n");
    printf("  1.Use\n  2.Insert\n  3.Print\n  4.Exit\n");
    printf("\nEnter a number that corresponds to your choice > ");
}

int get_option(int min, int max)
{
    char op[3], garbage;

    do
    {

        if ((void *)fgets(op, 3, stdin) != NULL)
        {
            if (strlen(op) > 1 && op[1] != '\n')
                while ((garbage = (char)getchar()) != '\n' && (int)garbage != EOF);

            if (op[1] != '\n' || (atoi(op) < min || atoi(op) > max))
                printf("\nOpcion Incorrecta, vuelve a introducir: ");
        }
        else
        {
            fprintf(stderr, "\nError : fgets ha devuelto NULL");
            return -1;
        }

    } while (op[1] != '\n' || (atoi(op) < min || atoi(op) > max));

    return atoi(op);
}
