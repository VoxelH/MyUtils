#include "MyUtils/list.h"
#include "MyUtils/error.h"


#include <stdlib.h>

#define ReturnIfNull(obj, errorMessage)                                                                                \
    if ((obj) == NULL)                                                                                                 \
    {                                                                                                                  \
        Error_SetTo(errorMessage);                                                                                     \
        return false;                                                                                                  \
    }
#define ReturnIfNullVoid(obj, errorMessage)                                                                            \
    if ((obj) == NULL)                                                                                                 \
    {                                                                                                                  \
        Error_SetTo(errorMessage);                                                                                     \
        return;                                                                                                        \
    }

// 不检测this是否为Null，也不检测capacity
// 对._mem的行为取决于realloc()
bool PreserveImpl(List *this, size_t capacity);
// 不检测this是否为Null
// 检测 this->capacity 是否能容纳 length + 1 个 项目
// 如果不能，会尝试扩容
bool CheckBeforeAddNew(List *this);

bool List_Init(List **this)
{
    ReturnIfNull(*this = malloc(sizeof(List)), "List_Init: 初始化 List 对象失败");
    (*this)->_mem = malloc(sizeof(void *) * LIST_DEFAULT_CAPACITY);
    if ((*this)->_mem == NULL)
    {
        Error_SetTo("List_Init: (*this)->_mem 无法被初始化");
        free(*this);
        *this = NULL;
        return false;
    }
    (*this)->_length = 0;
    (*this)->_capacity = LIST_DEFAULT_CAPACITY;
    return true;
}

bool List_InitPreserve(List **this, size_t capacity)
{
    ReturnIfNull(*this = malloc(sizeof(List)), "List_InitPreserve: 初始化 List 对象失败");
    (*this)->_mem = malloc(sizeof(void *) * capacity);
    if ((*this)->_mem == NULL)
    {
        Error_SetTo("List_InitPreserve: (*this)->_mem 无法被初始化");
        free(*this);
        *this = NULL;
        return false;
    }
    (*this)->_length = 0;
    (*this)->_capacity = capacity;
    return true;
}

bool PreserveImpl(List *this, size_t capacity)
{
    void *newMemory = realloc(this->_mem, sizeof(void *) * capacity);
    ReturnIfNull(newMemory, "PreserveImpl: 扩容失败") this->_capacity = capacity;
    this->_mem = newMemory;
    return true;
}

bool List_Preserve(List *this, size_t capacity)
{
    ReturnIfNull(this, "List_Preserve: 操作的 List 对象为 NULL");
    if (this->_capacity >= capacity)
        return true;
    return PreserveImpl(this, capacity);
}

void List_Destroy(List *this)
{
    ReturnIfNullVoid(this, "List_Destroy: 操作的 List 对象为 NULL");
    free(this->_mem);
    free(this);
}

void List_DestroyFree(List *this)
{
    ReturnIfNullVoid(this, "List_DestroyFree: 操作的 List 对象为 NULL");
    for (size_t i = 0; i < this->_length; i++)
        free(this->_mem[i]);
    free(this->_mem);
    free(this);
}

void List_Clear(List *this)
{
    ReturnIfNullVoid(this, "List_Clear: 操作的 List 对象为 NULL");
    this->_length = 0;
}

void List_ClearFree(List *this)
{
    ReturnIfNullVoid(this, "List_ClearFree: 操作的 List 对象为 NULL");
    for (size_t i = 0; i < this->_length; i++)
        free(this->_mem[i]);
    this->_length = 0;
}

size_t List_Length(List *this)
{
    ReturnIfNull(this, "List_Length: 操作的 List 对象为 NULL");
    return this->_length;
}
size_t List_Capacity(List *this)
{
    ReturnIfNull(this, "List_Capacity: 操作的 List 对象为 NULL");
    return this->_capacity;
}

bool CheckBeforeAddNew(List *this)
{
    // 不会溢出，因为size_t太大了
    if (this->_length == this->_capacity && !PreserveImpl(this, this->_capacity * 2) &&
        !PreserveImpl(this, this->_capacity + LIST_RETRY_CAPACITY))
    {
        Error_SetTo("CheckBeforeAddNew: 扩容失败，无法添加新项");
        return false;
    }
    return true;
}

bool List_Add(List *this, void *data)
{
    ReturnIfNull(this, "List_Add: 操作的 List 对象为 NULL");
    if (!CheckBeforeAddNew(this))
        return false;
    this->_mem[this->_length++] = data;
    return true;
}

bool List_Insert(List *this, size_t index, void *data)
{
    ReturnIfNull(this, "List_Insert: 操作的 List 对象为 NULL");
    if (index > this->_length)
    {
        Error_SetTo("List_Insert: 索引超出范围");
        return false;
    }
    if (!CheckBeforeAddNew(this))
        return false;
    memmove(this->_mem + index + 1, this->_mem + index, sizeof(void *) * (this->_length - index));
    this->_mem[index] = data;
    this->_length++;
    return true;
}

bool List_Remove(List *this, size_t index)
{
    ReturnIfNull(this, "List_Remove: 操作的 List 对象为 NULL");
    if (index >= this->_length)
    {
        Error_SetTo("List_Remove: 索引超出范围");
        return false;
    }
    if (index < this->_length - 1)
        memmove(this->_mem + index, this->_mem + index + 1, sizeof(void *) * (this->_length - index - 1));
    this->_length--;
    return true;
}

bool List_RemoveFree(List *this, size_t index)
{
    ReturnIfNull(this, "List_RemoveFree: 操作的 List 对象为 NULL");
    if (index >= this->_length)
    {
        Error_SetTo("List_RemoveFree: 索引超出范围");
        return false;
    }
    free(this->_mem[index]);
    if (index < this->_length - 1)
        memmove(this->_mem + index, this->_mem + index + 1, sizeof(void *) * (this->_length - index - 1));
    this->_length--;
    return true;
}

bool List_Get(List *this, size_t index, void **data)
{
    *data = NULL;
    ReturnIfNull(this, "List_Get: 操作的 List 对象为 NULL");
    if (index >= this->_length)
    {
        Error_SetTo("List_Get: 索引超出范围");
        return false;
    }
    *data = this->_mem[index];
    return true;
}

bool List_Set(List *this, size_t index, void *data)
{
    ReturnIfNull(this, "List_Set: 操作的 List 对象为 NULL");
    if (index >= this->_length)
    {
        Error_SetTo("List_Set: 索引超出范围");
        return false;
    }
    this->_mem[index] = data;
    return true;
}

bool List_FindFirst(List *this, matcher match, size_t *index)
{
    ReturnIfNull(this, "List_FindFirst: 操作的 List 对象为 NULL") for (size_t i = 0; i < this->_length; i++)
    {
        if (match(i, this->_mem[i]))
        {
            *index = i;
            return true;
        }
    }
    return false;
}

bool List_FindLast(List *this, matcher match, size_t *index)
{
    ReturnIfNull(this, "List_FindLast: 操作的 List 对象为 NULL") for (size_t i = this->_length; i-- > 0;)
    {
        if (match(i, this->_mem[i]))
        {
            *index = i;
            return true;
        }
    }
    return false;
}

bool List_Contains(List *this, void *data)
{
    ReturnIfNull(this, "List_Contains: 操作的 List 对象为 NULL") for (size_t i = 0; i < this->_length;
                                                                      i++) if (this->_mem[i] == data) return true;
    return false;
}

bool List_FindAll(List *this, matcher match, size_t **indices, size_t *numIndices)
{
    static_assert(sizeof(size_t) == sizeof(void *), "黑魔法这一块/.");
    ReturnIfNull(this, "List_FindAll: 操作的 List 对象为 NULL") List *theIndices;
    if (!List_Init(&theIndices))
        return false;
    for (size_t i = 0; i < this->_length; i++)
    {
        if (match(i, this->_mem[i]))
        {
            // 是的，在64位系统上，size_t和void*长度相同，所以说List可以直接存储size_t，地址就是值
            // 不能使用 List_DestroyFree() List_ClearFree() List_RemoveFree()
            if (!List_Add(theIndices, (void *)i))
            {
                Error_SetTo("List_FindAll: 无法将匹配的索引添加到返回值");
                List_Destroy(theIndices);
                return false;
            }
        }
    }
    List_ToArray(theIndices, (void ***)&indices, numIndices);
    List_Destroy(theIndices);
    return true;
}

bool List_Copy(List *dst, List *src, size_t dstStart, size_t srcStart, size_t num)
{
    ReturnIfNull(dst, "List_Copy: 操作的 List 对象 dst 为 NULL")
        ReturnIfNull(src, "List_Copy: 操作的 List 对象 src 为 NULL") if (num == 0) return true;
    if (srcStart >= src->_length || srcStart + num > src->_length || dstStart >= dst->_length ||
        dstStart + num > dst->_length)
    {
        Error_SetTo("List_Copy: 索引超出范围");
        return false;
    }
    memmove(dst->_mem + dstStart, src->_mem + srcStart, sizeof(void *) * num);
    return true;
}

bool List_CopyAdd(List *dst, List *src, size_t srcStart, size_t num)
{
    ReturnIfNull(dst, "List_CopyAdd: 操作的 List 对象 dst 为 NULL")
        ReturnIfNull(src, "List_CopyAdd: 操作的 List 对象 src 为 NULL") if (num == 0) return true;
    if (srcStart >= src->_length || srcStart + num > src->_length)
    {
        Error_SetTo("List_CopyAdd: 索引超出范围");
        return false;
    }
    if (dst->_capacity - dst->_length < num)
    {
        if (dst->_capacity * 2 >= dst->_length + num)
        {
            if (!PreserveImpl(dst, dst->_capacity * 2) && !PreserveImpl(dst, dst->_capacity + num))
            {
                Error_SetTo("List_CopyAdd: 扩容失败，无法添加新项");
                return false;
            }
        }
        else if (!PreserveImpl(dst, dst->_capacity + num))
        {
            Error_SetTo("List_CopyAdd: 扩容失败，无法添加新项");
            return false;
        }
    }
    memmove(dst->_mem + dst->_length, src->_mem + srcStart, sizeof(void *) * num);
    dst->_length += num;
    return true;
}

bool List_FromArray(List **this, void **array, size_t arraySize)
{
    if (!List_InitPreserve(this, arraySize))
        return false;
    memcpy((*this)->_mem, array, arraySize * sizeof(void *));
    (*this)->_length = arraySize;
    return true;
}

bool List_ToArray(List *this, void ***array, size_t *arraySize)
{
    *array = NULL;
    if (arraySize != NULL)
        *arraySize = 0;
    ReturnIfNull(this, "List_ToArray: 操作的 List 对象为 NULL")
        ReturnIfNull(*array = malloc(this->_length * sizeof(void *)), "List_ToArray: 无法创建array对象");
    memcpy(*array, this->_mem, this->_length * sizeof(void *));
    if (arraySize != NULL)
        *arraySize = this->_length;
    return true;
}