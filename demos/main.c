#include "../include/MyUtils/win32/font.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    setlocale(LC_ALL, "zh-CN.utf-8");
    wchar_t **fonts;
    size_t numFonts;
    GetFonts(&fonts, &numFonts, true);
    for (size_t i = 0; i < numFonts; i++)
    {
        printf("%zu %ws\n", i, fonts[i]);
    }
}
