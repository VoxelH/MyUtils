#ifndef MYUTILS_LIST_H
#define MYUTILS_LIST_H
#include <stdbool.h>
#include <stddef.h>

/// 使用List_Init()和List_InitPreserve()时默认使用的容量
#define LIST_DEFAULT_CAPACITY ((size_t)16)
/// 内部扩容失败时会使用此值再尝试进行扩容
#define LIST_RETRY_CAPACITY ((size_t)16)

typedef struct
{
    size_t _length;
    size_t _capacity;
    void **_mem;
} List;

typedef bool (*matcher)(size_t index, void *data);

/// 在堆上初始化List对象
/// \param this 待初始化的对象。如果初始化失败，*this会被设置为NULL
/// \return 成功与否
bool List_Init(List **this);
/// 在堆上初始化List对象
/// \param this 待初始化的对象。如果初始化失败，*this会被设置为NULL
/// \param capacity 初始化对象时申请的容量
/// \return 成功与否
bool List_InitPreserve(List **this, size_t capacity);
bool List_Preserve(List *this, size_t capacity);
void List_Destroy(List *this);
void List_DestroyFree(List *this);
void List_Clear(List *this);
void List_ClearFree(List *this);

size_t List_Length(List *this);
size_t List_Capacity(List *this);

bool List_Add(List *this, void *data);
bool List_Insert(List *this, size_t index, void *data);
bool List_Remove(List *this, size_t index);
bool List_RemoveFree(List *this, size_t index);

bool List_Get(List *this, size_t index, void **data);
bool List_Set(List *this, size_t index, void *data);

bool List_FindFirst(List *this, matcher match, size_t *index);
bool List_FindLast(List *this, matcher match, size_t *index);
bool List_Contains(List *this, void *data);
bool List_FindAll(List *this, matcher match, size_t **indices, size_t *numIndices);

bool List_Copy(List *dst, List *src, size_t dstStart, size_t srcStart, size_t num);
bool List_CopyAdd(List *dst, List *src, size_t dstStart, size_t num);
bool List_FromArray(List **this, void **array, size_t arraySize);
bool List_ToArray(List *this, void ***array, size_t *arraySize);

#endif // MYUTILS_LIST_H