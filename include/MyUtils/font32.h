#ifndef MYUTILS_FONT32_H
#define MYUTILS_FONT32_H
#include <stddef.h>

/// \remark Win32智慧的将每一个lfFaceName长度设置为32，所以字体名称最多只有32个wchar_t(包括 \0)
/// 不信你去看 Visual Studio Font Picker
void GetFonts(wchar_t*** fonts, size_t* num);

#endif // MYUTILS_FONT32_H
