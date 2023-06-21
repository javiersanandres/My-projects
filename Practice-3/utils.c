#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char *indexName)
{
    int namelength;
    namelength = (int)strlen(fileName);
    (void *)strcpy(indexName, fileName);
    (void *)strcpy(indexName + (namelength - 3), "idx");
    return;
}

void _readnode(Node *node, FILE *f)
{
    (void)fread(node->book_id, sizeof(node->book_id[0]), PK_SIZE, f);
    (void)fread(&(node->left), sizeof(node->left), 1, f);
    (void)fread(&(node->right), sizeof(node->right), 1, f);
    (void)fread(&(node->parent), sizeof(node->parent), 1, f);
    (void)fread(&(node->offset), sizeof(node->offset), 1, f);
}

bool createTable(const char *tableName)
{
    int cab = NO_DELETED_REGISTERS;
    char *indexname;
    FILE *f;
    bool ret;

    if (tableName == NULL)
        return false;

    if ((f = fopen(tableName, "r+")) == NULL)
    {
        if ((f = fopen(tableName, "w+")) == NULL)
            return false;

        (void)fwrite(&cab, sizeof(cab), 1, f);

        (void)fclose(f);

        if ((indexname = (char *)calloc(strlen(tableName) + 1, sizeof(char))) == NULL)
        {
            (void)fclose(f);
            return false;
        }

        replaceExtensionByIdx(tableName, indexname);

        ret = createIndex(indexname);
        free(indexname);
        return ret;
    }
    else
        (void)fclose(f);

    return true;
}

bool createIndex(const char *indexName)
{

    int cab[2] = {NO_DELETED_REGISTERS, NO_DELETED_REGISTERS};
    FILE *f;

    if (indexName == NULL)
        return false;

    if ((f = fopen(indexName, "r+")) == NULL)
    {
        if ((f = fopen(indexName, "w+")) == NULL)
            return false;

        (void)fwrite(&cab, sizeof(cab[0]), 2, f);

        (void)fclose(f);
    }
    else
        (void)fclose(f);

    return true;
}

void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side)
{
    int i;
    char aux[PK_SIZE + 1];
    Node node;

    if (_level > level || node_id < 0)
        return;

    (void)fseek(indexFileHandler, INDEX_HEADER_SIZE + node_id * (INDEX_REGISTER_SIZE), SEEK_SET);

    _readnode(&node, indexFileHandler);

    for (i = 0; i < 3 * (int)_level; i++)
        fprintf(stdout, " ");

    memcpy(aux, node.book_id, PK_SIZE);
    aux[PK_SIZE] = '\0';

    fprintf(stdout, "%c %s (%d): %d\n", side, aux, node_id, node.offset);

    printnode(_level + 1, level, indexFileHandler, node.left, 'l');
    printnode(_level + 1, level, indexFileHandler, node.right, 'r');

    return;
}

void printTree(size_t level, const char *indexName)
{

    int root;
    FILE *f;

    if (indexName == NULL)
        return;

    if ((f = fopen(indexName, "r+")) == NULL)
        return;

    (void)fread(&root, sizeof(root), 1, f);
    printnode(0, level, f, root, ' ');

    (void)fclose(f);
    return;
}

bool _findKey(const char *key_id, FILE *f, int node_id, int *nodeIDOrDataOffset)
{
    int cmp;
    Node node;

    (void)fseek(f, (long)( INDEX_HEADER_SIZE + node_id * (PK_SIZE + 4 * (sizeof(int))) ), SEEK_SET);

    _readnode(&node, f);

    cmp = memcmp(key_id, node.book_id, PK_SIZE);

    if (cmp == 0)
    {
        *nodeIDOrDataOffset = node.offset;
        return true;
    }
    else if (cmp < 0)
    {
        if (node.left < 0)
        {
            *nodeIDOrDataOffset = node_id;
            return false;
        }

        return _findKey(key_id, f, node.left, nodeIDOrDataOffset);
    }
    else
    {
        if (node.right < 0)
        {
            *nodeIDOrDataOffset = node_id;
            return false;
        }

        return _findKey(key_id, f, node.right, nodeIDOrDataOffset);
    }
}

bool findKey(const char *book_id, const char *indexName, int *nodeIDOrDataOffset)
{
    int root;
    bool ret;
    FILE *f;

    if (book_id == NULL || indexName == NULL || nodeIDOrDataOffset == NULL)
        return false;

    if ((f = fopen(indexName, "r+")) == NULL)
        return false;

    (void)fread(&root, sizeof(root), 1, f);

    if (root < 0)
    {
        *nodeIDOrDataOffset = -1;
        return false;
    }

    ret = _findKey(book_id, f, root, nodeIDOrDataOffset);
    (void)fclose(f);

    return ret;
}

bool addIndexEntry(char *book_id, int bookOffset, char const *indexName)
{

    int newnode_id, org_bor, bor, new_bor, nodeIDOrDataOffset=0;
    int aux = -1;
    char parent_id[PK_SIZE];
    FILE *f;

    if (book_id == NULL || bookOffset < 0 || indexName == NULL)
        return false;

    if (findKey(book_id, indexName, &nodeIDOrDataOffset) == true)
        return false;

    if ((f = fopen(indexName, "r+")) == NULL)
        return false;

    org_bor = INDEX_HEADER_SIZE / 2;
    (void)fseek(f, INDEX_HEADER_SIZE / 2, SEEK_SET);
    (void)fread(&bor, sizeof(bor), 1, f);

    /*INSERTO EL NODO*/
    if (bor != -1)
    {

        (void)fseek(f, INDEX_HEADER_SIZE + bor * INDEX_REGISTER_SIZE + PK_SIZE, SEEK_SET);
        (void)fread(&new_bor, sizeof(new_bor), 1, f);
        (void)fseek(f, org_bor, SEEK_SET);
        (void)fwrite(&new_bor, sizeof(new_bor), 1, f);

        (void)fseek(f, INDEX_HEADER_SIZE + bor * INDEX_REGISTER_SIZE, SEEK_SET);
        newnode_id = (int)(ftell(f) - INDEX_HEADER_SIZE) / INDEX_REGISTER_SIZE;
    }
    else
    {
        (void)fseek(f, 0, SEEK_END);
        newnode_id = (int)(ftell(f) - INDEX_HEADER_SIZE) / INDEX_REGISTER_SIZE;
    }

    (void)fwrite(book_id, sizeof(book_id[0]), PK_SIZE, f);
    (void)fwrite(&aux, sizeof(aux), 1, f);
    (void)fwrite(&aux, sizeof(aux), 1, f);
    (void)fwrite(&nodeIDOrDataOffset, sizeof(nodeIDOrDataOffset), 1, f);
    (void)fwrite(&bookOffset, sizeof(bookOffset), 1, f);

    /*ACTUALIZO EL NODO PADRE*/
    if (nodeIDOrDataOffset == -1) /*Es el root*/
    {
        (void)fseek(f, 0, SEEK_SET);
        (void)fwrite(&newnode_id, sizeof(newnode_id), 1, f);
    }
    else
    {
        (void)fseek(f, INDEX_HEADER_SIZE + nodeIDOrDataOffset * INDEX_REGISTER_SIZE, SEEK_SET);
        (void)fread(parent_id, sizeof(parent_id[0]), PK_SIZE, f);

        if (memcmp(book_id, parent_id, PK_SIZE) < 0)
        {
            (void)fwrite(&newnode_id, sizeof(newnode_id), 1, f);
        }
        else
        {
            (void)fseek(f, (long)sizeof(int), SEEK_CUR);
            (void)fwrite(&newnode_id, sizeof(newnode_id), 1, f);
        }
    }

    (void)fclose(f);
    return true;
}

bool addTableEntry(Book *book, const char *tableName, const char *indexName)
{

    int offset, nodeIDOrDataOffset=0, org_bor, bor, new_bor, bor_len, aux, aux_len;
    FILE *f;

    if (book == NULL || tableName == NULL || indexName == NULL)
        return false;

    if (findKey(book->book_id, indexName, &nodeIDOrDataOffset) == true)
    {
        fprintf(stderr, "\nError: Book with id=%.4s already in DB", book->book_id); /*Ese .4 es muy importante para que no acceda a memoria innecesaria*/
        return false;
    }
    else
    {

        if ((f = fopen(tableName, "r+")) == NULL)
            return false;

        org_bor = 0;
        (void)fread(&bor, sizeof(bor), 1, f);
        bor_len = 0;

        while (bor != -1)
        {
            (void)fseek(f, bor, SEEK_SET);
            (void)fread(&new_bor, sizeof(new_bor), 1, f); /*Siguiente borrado*/
            (void)fread(&bor_len, sizeof(bor_len), 1, f); /*Longitud borrado actual*/

            if (bor_len >= (int)book->title_len) /* Actualizas el anterior borrado*/
            {

                if ((bor_len - (int)book->title_len) > (PK_SIZE + (int)sizeof(int)))
                {
                    aux = bor + PK_SIZE + (int)sizeof(int) + (int)book->title_len;
                    (void)fseek(f, org_bor, SEEK_SET);
                    (void)fwrite(&aux, sizeof(aux), 1, f);

                    aux_len = bor_len - (int)book->title_len - PK_SIZE - (int)sizeof(int);
                    (void)fseek(f, aux, SEEK_SET);
                    (void)fwrite(&new_bor, sizeof(new_bor), 1, f);
                    (void)fwrite(&aux_len, sizeof(aux_len), 1, f);
                }
                else
                {
                    (void)fseek(f, org_bor, SEEK_SET);
                    (void)fwrite(&new_bor, sizeof(new_bor), 1, f);
                }

                break;
            }
            else /*Avanzas por la lista de borrados*/
            {
                org_bor = bor;
                bor = new_bor;
            }
        }

        if (bor_len >= (int)book->title_len) /*Igual que poner bor!=-1 debido a nuestra implementación*/
        {
            (void)fseek(f, bor, SEEK_SET);
            offset = (int)ftell(f);
        }
        else
        {
            (void)fseek(f, 0, SEEK_END);
            offset = (int)ftell(f);
        }

        (void)fwrite(book->book_id, sizeof(book->book_id[0]), PK_SIZE, f);
        (void)fwrite(&(book->title_len), sizeof((int)book->title_len), 1, f);
        (void)fwrite(book->title, sizeof(book->title[0]), book->title_len, f);

        (void)fclose(f);

        return addIndexEntry(book->book_id, offset, indexName);
    }
}
