#define Error_PrintError 1

#include "../../include/MyUtils/win32/font.h"
#include "MyUtils/list.h"

#include <windows.h>
#include <wingdi.h>

struct FontEnumConfig
{
    List *list;
    bool monospaceOnly;
};

int CALLBACK FontEnumProc(const LOGFONTW *logFont, const TEXTMETRICW *textMetric, DWORD discard2, LPARAM lparam)
{
    static wchar_t lastSeen[32] = {0};
    const wchar_t *name = logFont->lfFaceName;
    const struct FontEnumConfig config = *(struct FontEnumConfig *)lparam;
    if (wcscmp(lastSeen, name) != 0 && wcslen(name) >= 1 && name[0] != L'@')
    {
        wchar_t *nameOnHeap = malloc(sizeof(wchar_t) * 32);
        wcscpy(nameOnHeap, name);
        wcscpy(lastSeen, name);
        if (config.monospaceOnly)
        {
            if ((textMetric->tmPitchAndFamily & 1) == 0)
            {
                List_Add(config.list, nameOnHeap);
            }
        }
        else
        {
            List_Add(config.list, nameOnHeap);
        }
    }
    return 1;
}

void GetFonts(wchar_t ***fonts, size_t *num, bool monospaceOnly)
{
    HDC hdc = GetDC(NULL);
    LOGFONTW logFont = {0};
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfFaceName[0] = L'\0';
    logFont.lfPitchAndFamily = 0;

    List *list;
    List_Init(&list);

    struct FontEnumConfig config = {list, monospaceOnly};
    EnumFontFamiliesExW(hdc, &logFont, FontEnumProc, (LPARAM)&config, 0);

    List_ToArray(list, (void ***)fonts, num);
    List_Destroy(list);
}