#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "MyUtils/font32.h"

int main()
{
    setlocale(LC_ALL, "zh-CN.utf-8");
    wchar_t **fonts;
    size_t numFonts;
    GetFonts(&fonts, &numFonts);
    for (size_t i = 0; i < numFonts; i++)
    {
        printf("%zu %ws\n", i, fonts[i]);
    }
}
