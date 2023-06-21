#include "Consultas.h"    



int get_option(int min, int max)
{
    char op[3], garbage;

    if( min<1 || max > 9)
        return -1;

    do
        {
            
            if(fgets(op,3,stdin) !=NULL)
            {   
                if(strlen(op)>1 && op[1]!='\n')
                    while((garbage=(char)getchar())!='\n' && (int)garbage!=EOF);

                if(op[1]!='\n' || (atoi(op)<min || atoi(op)>max))
                    printf("\nOpcion Incorrecta, vuelve a introducir: ");
            }
            else
                return -1;
            
        }while(op[1]!='\n'|| (atoi(op)<min || atoi(op)>max));

        return atoi(op);
}

void paginacion(char **Data, int size, int max_lines, int design_mode)
{
    int i, npag, max_pages;
    char cpag[3], garbage;

    npag=0;

    if (size<=10 || design_mode==0)
    {
        printf("\n");
        for (i=0;i<size;i++)
            printf("%s\n", Data[i]);
        
        return;
    }

    if(size/(float)max_lines == 0)
        max_pages=size/(float)max_lines;
    else
        max_pages=size/max_lines +1;

    do
    {   
        printf("\n\n");
        for(i=max_lines*npag; (i<max_lines*(npag+1) && i<size); i++)
            printf("%s\n", Data[i]);

        printf("\n\n\t\t\t\t\t\tPage: %d/%d        [Previous Page: < ]    [Next page: > ]    [Exit: X ] ",npag+1,max_pages);
        
        
        do
        {
            if(fgets(cpag,3,stdin) !=NULL)
                {   
                    if(strlen(cpag)>1 && cpag[1]!='\n')
                        while((garbage=(char)getchar())!='\n' && (int)garbage!=EOF);

                    if(cpag[1]!='\n' || (cpag[0]!='X' && cpag[0]!='x' && cpag[0] !='<' && cpag[0] !='>'))
                        printf("\nOpcion Incorrecta, vuelve a introducir: ");
                    
                }
                else
                    return;
            
        }while(cpag[1]!='\n' || (cpag[0]!='X' && cpag[0]!='x' && cpag[0] !='<' && cpag[0] !='>'));
        

        if(cpag[0] == '<')
            npag=((npag-1)>0 ? (npag-1):0);
        else if(cpag[0]=='>')
            npag=((npag+1)<max_pages ? (npag+1):npag);
        else
            printf("\n\t\t\t\t\t\tSaliendo...\n\n");

    }while(cpag[0]!='X' && cpag[0]!='x');
}

void Destroyer(char **Data, int size, int type)
{
    int i;

    for(i=0; i<size; i++ )
        free((Data)[i]);
    
    
    if (type==1)
        free(Data);
        
}

int Data_alloc(char ***Data, int size, int *count)
{
    int i;

    if (Data==NULL || size<=0 || count==NULL)
        return EXIT_FAILURE;

    if(((*Data)=(char **)calloc((size_t)size,sizeof(char *)))==NULL)
        return EXIT_FAILURE;

    
    for(i=0; i<size; i++ )
    {
        if(((*Data)[i]=(char *)calloc((size_t)BufferLength,sizeof(char)))==NULL)
        {
            Destroyer((*Data), i, 1);
            return EXIT_FAILURE;
        }
        
        (*count)=1;
    }

    return EXIT_SUCCESS;
}

int Data_realloc(char ***Data, int size, int count)
{
    int i;

    if (Data==NULL || size<=0)
        return EXIT_FAILURE;

    if(((*Data)=(char **)realloc((*Data), (size_t)count*size*sizeof(char *)))==NULL)
    {
        Destroyer(*Data, (count-1)*size, 1);
        return EXIT_FAILURE;
    }
    for(i=(count-1)*size; i<count*size; i++)
    {
        if(((*Data)[i]=(char *)calloc((size_t)BufferLength,sizeof(char)))==NULL)
        {
            Destroyer((*Data), i, 1);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void Data_adjust(char ***Data, int size, int max_length)
{
    int i, j, k, length;
    char aux[BufferLength];

    for (i=0; i<size; i++)
    {
        length=(int)strlen((*Data)[i]);
        if (length!=max_length)
        {
            for (j=length-1, k=0;(*Data)[i][j]!=' ';j--, k++)
                aux[k]=(*Data)[i][j];

            aux[k]='\0';

            for (j+=1;j<max_length-k;j++)
                (*Data)[i][j]=' ';

            for (k-=1;j<max_length;j++, k--)
                (*Data)[i][j]=aux[k];
            
            (*Data)[i][j]='\0';
        }
    }
}

/*@null@*/
Retornos* General_query(MyDataBase *pMDB, char *query, int *size, int num_var, int num_rets, ...){
    va_list list;
    int i, type;
    Parameters *vars, *rets;
    Retornos *ret;


    if(pMDB==NULL || query==NULL || size==NULL || num_var<0 || num_rets<0 || num_var>MAX_SELECT || num_rets>MAX_SELECT)
        return NULL;
    
      
    alloc_statementhandle(pMDB);

    pMDB->ret = SQLPrepare(pMDB->stmt, (SQLCHAR *) query, SQL_NTS);

    if (!SQL_SUCCEEDED(pMDB->ret))
    {
        odbc_extract_error("", pMDB->stmt, SQL_HANDLE_ENV);
        return NULL;
    }

    if ((vars=(Parameters *)calloc((size_t)1, sizeof(Parameters)))==NULL)
        return NULL;
    
    if ((rets=(Parameters *)calloc((size_t)1, sizeof(Parameters)))==NULL)
    {
        free(vars);
        return NULL;
    }

    va_start(list, num_rets);

    for(i=0;i<num_var;i++)
    {
        type=va_arg(list, int);

        switch(type)
        {
            case 0:
                vars->integers[vars->num_integers]=va_arg(list, int);
                vars->num_integers++;
                (void)SQLBindParameter(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_PARAM_INPUT,
                            SQL_INTEGER, SQL_INTEGER,
                            0, 0, &(vars->integers[vars->num_integers-1]), (SQLLEN)sizeof(int), NULL);
                break;

            case 1:
                strcpy(vars->strings[vars->num_strings], va_arg(list, char*));
                vars->num_strings++;
                (void)SQLBindParameter(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_PARAM_INPUT,
                            SQL_CHAR, SQL_CHAR,
                            0, 0, vars->strings[vars->num_strings-1], (SQLLEN)BufferLength, NULL);
                break;

            case 2:
                vars->doubles[vars->num_doubles]=va_arg(list, double);
                vars->num_doubles++;
                (void)SQLBindParameter(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_PARAM_INPUT,
                            SQL_DOUBLE, SQL_DOUBLE,
                            0, 0, &(vars->doubles[vars->num_doubles-1]), (SQLLEN)sizeof(double), NULL);
                break;

            case 3:
                strcpy(vars->strings[vars->num_strings], va_arg(list, char*));
                vars->num_strings++;
                (void)SQLBindParameter(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_PARAM_INPUT,
                            SQL_DATE, SQL_DATE,
                            0, 0, vars->strings[vars->num_strings-1], (SQLLEN)BufferLength, NULL);
                break;
        }
    }

    (void)SQLExecute(pMDB->stmt);

    for(i=0;i<num_rets && rets->num_integers<MAX_SELECT && rets->num_strings<MAX_SELECT && rets->num_strings<MAX_SELECT;i++)
    {
        type=va_arg(list, int);
        switch(type)
        {
            case 0:
                (void)SQLBindCol(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_INTEGER,(SQLINTEGER *) &(rets->integers[rets->num_integers]), (SQLLEN)sizeof(int), NULL);
                rets->num_integers++;
                break;

            case 1:
                (void)SQLBindCol(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_C_CHAR,(SQLCHAR*) rets->strings[rets->num_strings], (SQLLEN)BufferLength, NULL);
                rets->num_strings++;
                break;

            case 2:
                (void)SQLBindCol(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_DOUBLE,(SQLDOUBLE*) &(rets->doubles[rets->num_doubles]), (SQLLEN)sizeof(double), NULL);
                rets->num_doubles++;
                break;

            case 3:
                (void)SQLBindCol(pMDB->stmt, (SQLUSMALLINT)(i+1), SQL_DATE, (SQLDATE *) rets->strings[rets->num_strings], (SQLLEN)BufferLength, NULL);
                rets->num_strings++;
                break;
        }
    }
    va_end(list);

    if (rets->num_integers==MAX_SELECT || rets->num_strings==MAX_SELECT || rets->num_strings==MAX_SELECT)
    {
        free(vars);
        free(rets);
        return NULL;
    }

    if ((ret=(Retornos *)calloc((size_t)1, sizeof(Retornos)))==NULL)
    {
        free(vars);
        free(rets);
        return NULL;
    }
    
    (*size)=0;
    while(SQL_SUCCEEDED(pMDB->ret = SQLFetch(pMDB->stmt))){
        if ((*size)>MAX_ROWS)
        {
            free(vars);
            free(rets);
            free(ret);
            return NULL;
        }

        for (i=0;i<rets->num_integers;i++)
            ret->integers[i][*size]=rets->integers[i];

        for (i=0;i<rets->num_strings;i++)
            strcpy(ret->strings[i][*size], rets->strings[i]);

        for (i=0;i<rets->num_doubles;i++)
            ret->doubles[i][*size]=rets->doubles[i];

        (*size)++;
    }

    (void)SQLCloseCursor(pMDB->stmt);
    
    free(vars);
    free(rets);
    
    if (free_statementhandle(pMDB)!=EXIT_SUCCESS)
    {
        free(ret);
        return NULL;
    }

    return ret;
}


