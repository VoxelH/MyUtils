#include "MyUtils/error.h"
#include <stdio.h>


#include <stdlib.h>

#include "MyUtils/macro_magics.h"

char *errorMessage = NULL;

void Error_SetToStackString(char *error)
{
#ifdef Error_PrintError
    fprintf(stderr, "%s\n", error);
#endif
    if (errorMessage != NULL)
        free(errorMessage);
    errorMessage = MyUtils_MoveTextToHeap(error);
}

void Error_SetToHeapString(char *error)
{
#ifdef Error_PrintError
    fprintf(stderr, "%s\n", error);
#endif
    if (errorMessage != NULL)
        free(errorMessage);
    errorMessage = error;
}

void Error_Clear()
{
    if (errorMessage != NULL)
        free(errorMessage);
    errorMessage = MyUtils_MoveTextToHeap("");
}

const char *Error_Get()
{
    return errorMessage;
}