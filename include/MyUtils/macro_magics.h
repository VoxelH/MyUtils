#ifndef MYUTILS_MACRO_MAGICS_H
#define MYUTILS_MACRO_MAGICS_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MyUtils_Stringify(x) MyUtilsReserved_Stringify(x)
#define MyUtilsReserved_Stringify(x) #x

#define MyUtils_IsString(x)                                                                                            \
    __builtin_types_compatible_p(typeof(x), char *) | __builtin_types_compatible_p(typeof(x), char[])
#define MyUtils_CheckWhetherIsStringOrNot(x) static_assert(MyUtils_IsString(x), "传字符串谢谢喵~")

#define MyUtils_AsLoggingMessage(text)                                                                                 \
    ({                                                                                                                 \
        MyUtils_CheckWhetherIsStringOrNot(text);                                                                       \
        "[" __FILE_NAME__ ":" MyUtils_Stringify(__LINE__) "] " text;                                                   \
    })

/// 把栈上文本转化到堆上
/// 手动释放
#define MyUtils_MoveTextToHeap(text)                                                                                   \
    ({                                                                                                                 \
        MyUtils_CheckWhetherIsStringOrNot(text);                                                                       \
        void *heapText = malloc(sizeof(char) * (strlen(text) + 1));                                                    \
        assert(heapText != NULL);                                                                                      \
        strcpy(heapText, text);                                                                                        \
        heapText;                                                                                                      \
    })

#endif // MYUTILS_MACRO_MAGICS_H