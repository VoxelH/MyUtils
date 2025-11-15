#define Error_PrintError 1

#include "MyUtils/font32.h"
#include "MyUtils/list.h"

#include <windows.h>
#include <wingdi.h>

int CALLBACK FontEnumProc(
   const LOGFONTW    *logFont,
   const TEXTMETRICW *discard1,
         DWORD      discard2,
         LPARAM     list
)
{
    static wchar_t lastSeen[32] = {0};
    const wchar_t* name = logFont->lfFaceName;
    if (wcscmp(lastSeen, name) != 0 && wcslen(name) >= 1 && name[0] != L'@')
    {
        wchar_t* nameOnHeap = malloc(sizeof(wchar_t) * 32);
        wcscpy(nameOnHeap, name);
        wcscpy(lastSeen, name);
        List_Add((List*) list, nameOnHeap);
    }
    return 1;
}

void GetFonts(wchar_t ***fonts, size_t *num)
{
    HDC hdc = GetDC(NULL);
    LOGFONTW logFont = { 0 };
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfFaceName[0] = L'\0';
    logFont.lfPitchAndFamily = 0;

    List* list;
    List_Init(&list);

    EnumFontFamiliesExW(hdc, &logFont, FontEnumProc, (LPARAM) list, 0);

    List_ToArray(list, (void***) fonts, num);
    List_Destroy(list);
}