#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "include/MyUtils/list.h"

int main() {
    setlocale(LC_ALL, "zh-CN.utf-8");

    // 1. 初始化列表
    List* list = NULL;
    if (!List_Init(&list)) {
        printf("列表初始化失败\n");
        return 1;
    }

    // 2. 添加元素 (这里用整数示例)
    List_Add(list, (void*)1ULL);
    List_Add(list, (void*)2ULL);
    List_Add(list, (void*)3ULL);

    // 3. 转换为数组
    void** tempArr = NULL;
    size_t size = 0;
    if (!List_ToArray(list, &tempArr, &size)) {
        printf("List_ToArray失败\n");
        List_Destroy(list);
        return 1;
    }

    // 4. 打印结果
    printf("size: %zu\n", size);
    for (size_t i = 0; i < size; i++) {
        printf("%zu ", (size_t)tempArr[i]);
    }
    printf("\n");

    // 5. 释放内存
    free(tempArr);
    List_Destroy(list);

    return 0;
}
