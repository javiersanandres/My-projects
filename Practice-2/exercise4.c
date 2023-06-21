/**************************************************/
/* Programa: ejercise4       Date:                */
/* Authors:                                       */
/*                                                */
/* Program that checks InsertSort                 */
/*                                                */
/* Input: Command Line                            */
/* -size: number of elements of each permutation  */
/* Output: 0: OK, -1: ERR                         */
/**************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "permutations.h"
#include "sorting.h"

int main(int argc, char** argv)
{
  int tamano, i, j, ret;
  int* perm = NULL;

  srand(time(NULL));

  if (argc != 3) {
	fprintf(stderr, "Error in input parameters:\n\n");
    fprintf(stderr, "%s -size <int>\n", argv[0]);
    fprintf(stderr, "Where:\n");
    fprintf(stderr, " -size : number of elements in the permutation.\n");
    return 0;
  }
  printf("Practice number 1, section 4\n");
  printf("Done by: your names\n");
  printf("Group: Your group\n");

  /* check command line */
  for(i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-size") == 0) {
      tamano = atoi(argv[++i]);
    } else {
      fprintf(stderr, "Wrong paramenter %s\n", argv[i]);
      return 0;
    }
  }

  perm = generate_perm(tamano);

  if (perm == NULL) { /* error */
    printf("Error: Out of memory\n");
    exit(-1);
  }

  printf("La perm desordenada es: ");
  for(i=0; i<tamano; i++)
    printf("%d ",perm[i]);

  ret =SelectSort(perm, 0, tamano-1);

  if (ret == ERR) {
    printf("Error: Error in BubbleSort\n");
    free(perm);
    exit(-1);
  }

  printf("\n\nAhora la perm ordenada es:\n");
  for(j = 0; j < tamano; j++) {
    printf("%d \t", perm[j]);
  }


  if((Array_is_Sorted(&perm[0], &perm[tamano -1]))==OK)
    printf("\nArray is indeed sorted, with an ob=%d",ret);
  else
    printf("\nArray is not sorted");
    
  printf("\n");

  free(perm);

  return 0;
}

