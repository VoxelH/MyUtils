#ifndef MYUTILS_LIST_H
#define MYUTILS_LIST_H
#include <stdbool.h>
#include <stddef.h>

#define LIST_DEFAULT_CAPACITY ((size_t)16)
#define LIST_RETRY_CAPACITY ((size_t)16)

typedef struct {
    size_t _length;
    size_t _capacity;
    void** _mem;
} List;

typedef bool (*matcher)(size_t index, void* data);

bool List_Init(List** this);
bool List_InitPreserve(List** this, size_t capacity);
bool List_Preserve(List* this, size_t capacity);
void List_Destroy(List* this);
void List_DestroyFree(List* this);
void List_Clear(List* this);
void List_ClearFree(List* this);

size_t List_Length(const List* this);
size_t List_Capacity(const List* this);

bool List_Add(List* this, void* data);
bool List_Insert(List* this, size_t index, void* data);
bool List_Remove(List* this, size_t index);
bool List_RemoveFree(List* this, size_t index);

bool List_Get(const List* this, size_t index, void** data);
bool List_Set(const List* this, size_t index, void* data);

bool List_FindFirst(const List* this, matcher match, size_t* index);
bool List_FindLast(const List* this, matcher match, size_t* index);
bool List_Contains(const List* this, const void* data);
bool List_FindAll(const List* this, matcher match, size_t** indices, size_t* numIndices);

bool List_Copy(const List* dst, const List* src, size_t dstStart, size_t srcStart, size_t num);
bool List_CopyAdd(List* dst, const List* src, size_t dstStart, size_t num);
bool List_FromArray(List** this, void** array, size_t arraySize);
bool List_ToArray(const List* this, void*** array, size_t* arraySize);

#endif //MYUTILS_LIST_H