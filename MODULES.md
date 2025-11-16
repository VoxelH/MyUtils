## Generic

---

### List
1. 头文件： `MyUtils/list.h`
2. 描述：可变长度列表的实现

### SemVer
1. 头文件: `MyUtils/semver.h`
2. 描述：`semver`解释器

## Win32

---

### Font
1. 头文件 `MyUtils/win32/font.h`
2. 描述：Windows平台上处理字体的功能模块
3. 示例
```c++
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "MyUtils/win32/font.h"

int main()
{
    setlocale(LC_ALL, "zh-CN.utf-8");  // 使中文能够正常输出
    wchar_t **fonts;
    size_t numFonts;
    GetFonts(&fonts, &numFonts);
    for (size_t i = 0; i < numFonts; i++)
    {
        printf("%zu %ws\n", i, fonts[i]);
        free(fonts[i]);  // 别忘了
    }
    return 0;
}
```
5. 注意事项：每个字体名称只有32个`wchar_t`，这归功于Win32的限制