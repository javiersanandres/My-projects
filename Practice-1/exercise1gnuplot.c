/***********************************************/
/* Program: exercise1gnuplot     Date:         */
/* Authors:                                    */
/*                                             */
/* Program that simulates the generation       */
/* of two random nunmbers                      */
/* between two given numbers                   */
/*                                             */
/* Input: Command Line                         */
/* -limInf: lower limit                        */
/* -limSup: upper limit                        */
/* -numN: ammount of numbers                   */
/* Output: 0: OK, -1: ERR                      */
/***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "permutations.h"

int main(int argc, char** argv)
{
    int* frecuency_table, i;
    unsigned int inf, sup, num, j;

    FILE *pf;
    char file_name[256];

    srand(time(NULL));

    if (argc != 9) 
    {
        fprintf(stderr, "Input parameter error:\n\n");
        fprintf(stderr, "%s -limInf <int> -limSup <int> -numN <int>\n", argv[0]);
        fprintf(stderr, "Where:\n");
        fprintf(stderr, " -limInf : Lower limit.\n");
        fprintf(stderr, " -limSup : Upper limit.\n");
        fprintf(stderr, " -numN : ammout of mumbers to generate.\n");
        exit(-1);
    }

    /* check command line */
    for(i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-limInf") == 0) {
        inf = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-limSup") == 0) {
        sup = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-numN") == 0) {
        num = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-outputFile") == 0) {
            strcpy(file_name, argv[++i]);
        } else {
        fprintf(stderr, "Wrong Parameter %s \n", argv[i]);
        }
    }

    if((frecuency_table=(int *)calloc((sup-inf +1),sizeof(frecuency_table[0])))==NULL)
        return ERR;
    
    /*Save frecuency data in table*/
    for(j = 0; j < num; j++) 
        frecuency_table[random_num(inf, sup)-inf]++;

    /*Open file*/
    if((pf=fopen(file_name, "w"))==NULL)
        return ERR;

    /*Print in file */
    for(j = 0; j < sup-inf+1; j++) 
        fprintf(pf,"%d %d\n",inf +j, frecuency_table[j]);

    fclose(pf);
    free(frecuency_table);

    printf("Correct output\n");

    return OK;
}
