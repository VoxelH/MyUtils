#ifndef MYUTILS_GLOBAL_H
#define MYUTILS_GLOBAL_H

#include "MyUtils/macro_magics.h"

// 通过以下指令来开启打印错误功能
// #define Error_PrintError 1

/// \remark 推荐使用 `macro_magics.h` 中的 MyUtils_AsLoggingMessage 来装饰字符串
void Error_SetToStackString(char* error);
/// \remark 推荐使用 `macro_magics.h` 中的 MyUtils_AsLoggingMessage 来装饰字符串
void Error_SetToHeapString(char* error);
void Error_Clear();
const char* Error_Get();
// TODO Error_SetTo 全局错误设计，对于多线程不安全，如果你的项目有并发，需要注意。
#define Error_SetTo(text) Error_SetToStackString(MyUtils_AsLoggingMessage(text))

#endif //MYUTILS_GLOBAL_H