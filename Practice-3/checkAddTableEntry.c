/*
// Created by roberto on 30/8/20.
*/
#include "tester.h"

void checkAddTableEntry(const char * tableName, const char *indexName, int stop)
/**
 *  check AddIndexEntry
 */
{
    bool result;
    int nodeIDOrDataOffset;
    Book newbook;
    char printbookId[5];                                                                /*DAVID*/
    int tmp=-1;
    FILE * fp;
    int offset;
    int deleted;

   
    createTestDataFile(tableName);
    createTestIndexFile(indexName);
    printf("\n-------------Checking AddTableEntry----------------\n");
    
    /* 1) add existing book should complain*/
    result = addTableEntry(&b[0], tableName, indexName);
    if (result){
        memcpy(printbookId, a[0].book_id, PK_SIZE); printbookId[4]='\0';                /*DAVID*/
        fprintf(stderr,
                "Error in addIndexEntry, inserted existing key %s",
                printbookId);   
        fprintf(stdout, "\nEXPECTED ERROR STATUS: ERROR\n");                               /*DAVID*/
        exit(EXIT_FAILURE);
    }
    else{
        fprintf(stdout, "EXPECTED ERROR STATUS: OK\n");
    }
    
    printf("-----Original tree-------\n");
    printTree(10, indexName); 


    /* add a new book using an existing deleted book */
    memcpy(newbook.book_id, "VAR2", 4);
    newbook.title="VAR2_new1";
    newbook.title_len=9;
    offset = 71; /*47 en HEX*/
    deleted = 139; /*8b en HEX*/
    result = addTableEntry(&newbook, tableName, indexName);
    findKey(newbook.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR2-------\n");
    printTree(10, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get deleted 139 (first deleted) */
    fp = fopen(tableName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addTableEntry, first int in file should be %d, that is %X in hex, but is %d", deleted, deleted, tmp);
        exit(EXIT_FAILURE);
    }
    /*get deleted -1 (second deleted) */
    fseek(fp,deleted,SEEK_SET);
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=-1){
        fprintf(stderr,
                "Error in addTableEntry, pointer to second deleted space in file should be %d"
                " but is %d", -1, tmp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);                                                                                                /* AÑADIDO DAVID */

    if(stop<=1)
    {
        printf("-------------------------------------------------");
        printf("\n*checking AddTableEntry: OK\n");
        return;
    }
    
    /* add another new book using an existing deleted book */
    memcpy(newbook.book_id, "SPAC", 4);
    newbook.title="E";
    newbook.title_len=1;
    offset = 139; /*8b en HEX*/
    deleted = 148; /*94 en HEX*/
    result = addTableEntry(&newbook, tableName, indexName);
    findKey(newbook.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding SPAC-------\n");
    printTree(10, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get deleted 148 (first deleted) */
    fp = fopen(tableName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addTableEntry, first int in file should be %d, that is %X in hex, but is %d", deleted, deleted, tmp);
        exit(EXIT_FAILURE);
    }
    /*get deleted -1 (second deleted) */
    fseek(fp,deleted,SEEK_SET);
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=-1){
        fprintf(stderr,
                    "Error in addTableEntry, pointer to second deleted space in file should be %d but is %d", -1, tmp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);                                                                                                /* AÑADIDO DAVID */

    if(stop==2)
    {
        printf("-------------------------------------------------");
        printf("\n*checking AddTableEntry: OK\n");
        return;
    }

    /* add last book at the end */
    memcpy(newbook.book_id, "LAST", 4);
    newbook.title="LAST_book";
    newbook.title_len=9;
    offset = 190; /*BE en HEX*/
    deleted = 148; /*94 en HEX --> Doesn't change*/
    result = addTableEntry(&newbook, tableName, indexName);
    findKey(newbook.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding LAST-------\n");
    printTree(10, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get deleted 148 (first deleted) */
    fp = fopen(tableName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addTableEntry, first int in file should be %d, that is %X in hex, but is %d", deleted, deleted, tmp);
        exit(EXIT_FAILURE);
    }
    /*get deleted -1 (second deleted) */
    fseek(fp,deleted,SEEK_SET);
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=-1){
        fprintf(stderr,
                "Error in addTableEntry, pointer to second deleted space in file should be %d"
                " but is %d", -1, tmp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);                                                                                                /* AÑADIDO DAVID */

    if(stop==3)
    {
        printf("-------------------------------------------------");
        printf("\n*checking AddTableEntry: OK\n");
        return;
    }
        

    /* FILL UP THE GAP SO THAT THERE ARE NONE LEFT, POINTER TO FIRST GAP = -1 */
    memcpy(newbook.book_id, "FILL", 4);
    newbook.title="S";
    newbook.title_len=1;
    offset = 148; /*94 en HEX*/
    deleted = -1; /*FF FF FF FF en HEX*/
    result = addTableEntry(&newbook, tableName, indexName);
    findKey(newbook.book_id, indexName, &nodeIDOrDataOffset);
    printf("-----after adding FILL-------\n");
    printTree(10, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addTableEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get deleted -1 (first deleted) */
    fp = fopen(tableName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addTableEntry, first int in file should be %d, that is %X in hex, but is %d", deleted, deleted, tmp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);                                                                                                /* AÑADIDO DAVID */

    printf("-------------------------------------------------");
    printf("\n*checking AddTableEntry: OK\n");

    return;
}

