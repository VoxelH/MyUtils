#include "MyUtils/error.h"
#include "MyUtils/list.h"

#include <stdlib.h>

bool List_Init(List** this) {
    *this = malloc(sizeof(List));
    if (*this == NULL) {
        Error_SetTo("创建失败");
        return false;
    }
    (*this)->_length = 0;
    (*this)->_capacity = LIST_DEFAULT_CAPACITY;
    (*this)->_mem = malloc(sizeof(void*) * LIST_DEFAULT_CAPACITY);
    if ((*this)->_mem == NULL) {
        Error_SetTo("创建失败——内部内存区无法初始化");
        free(*this);
        *this = NULL;
        return false;
    }
    return true;
}

bool List_InitPreserve(List** this, size_t capacity) {
    *this = malloc(sizeof(List));
    if (*this == NULL) {
        Error_SetTo("创建失败");
        return false;
    }
    (*this)->_length = 0;
    void* newMemory = malloc(capacity * sizeof(void*));
    if (newMemory == NULL) {
        Error_SetTo("创建失败——内部内存区无法初始化");
        free(*this);
        *this = NULL;
        return false;
    }
    (*this)->_capacity = capacity;
    (*this)->_mem = newMemory;
    return true;
}

bool List_PreserveImpl(List* this, size_t capacity) {
    void* newMemory = realloc(this->_mem, capacity * sizeof(void*));
    if (newMemory == NULL) {
        Error_SetTo("扩容失败，列表内容和长度不变");
        return false;
    }
    this->_capacity = capacity;
    this->_mem = newMemory;
    return true;
}

bool List_Preserve(List* this, size_t capacity) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (this->_capacity >= capacity) {
        return true;
    }
    return List_PreserveImpl(this, capacity);
}

void List_Destroy(List* this) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return;
    }
    free(this->_mem);
    free(this);
}

void List_DestroyFree(List* this) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return;
    }
    for (size_t i = 0; i < this->_length; i++) {
        free(this->_mem[i]);
    }
    free(this->_mem);
    free(this);
}

void List_Clear(List* this) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return;
    }
    this->_length = 0;
}

void List_ClearFree(List* this) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return;
    }
    for (size_t i = 0; i < this->_length; i++) {
        free(this->_mem[i]);
    }
    this->_length = 0;
}

size_t List_Length(const List *this) { return this->_length; }
size_t List_Capacity(const List *this) { return this->_capacity; }

bool CheckBeforeAddNew(List* this) {
    if (this->_length == this->_capacity) {
        // 不会溢出，因为内存不够大
        if (!List_PreserveImpl(this, this->_capacity * 2)) {
            if (!List_PreserveImpl(this, this->_capacity + LIST_RETRY_CAPACITY)) {
                Error_SetTo("扩容失败，无法添加项目");
                return false;
            }
        }
    }
    return true;
}

bool List_Add(List* this, void* data) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (!CheckBeforeAddNew(this)) {
        return false;
    }
    this->_mem[this->_length] = data;
    this->_length++;
    return true;
}

bool List_Insert(List* this, size_t index, void* data) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (index > this->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    if (!CheckBeforeAddNew(this)) {
        return false;
    }
    memmove(this->_mem + index + 1, this->_mem + index, sizeof(void*) * (this->_length - index));
    this->_mem[index] = data;
    this->_length++;
    return true;
}

bool List_Remove(List* this, size_t index) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (index >= this->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    // // 理论上UB
    // memmove(this->_mem + index - 1, this->_mem + index, sizeof(void*) * (this->_length - index - 1));
    if (index < this->_length - 1) {
        memmove(this->_mem + index, this->_mem + index + 1,
                (this->_length - index - 1) * sizeof(void*));
    }
    this->_length--;
    return true;
}

bool List_RemoveFree(List* this, size_t index) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (index >= this->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    free(this->_mem[index]);
    if (index < this->_length - 1) {
        memmove(this->_mem + index, this->_mem + index + 1,
                (this->_length - index - 1) * sizeof(void*));
    }
    this->_length--;
    return true;
}

bool List_Get(const List* this, size_t index, void** data) {
    *data = NULL;
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (index >= this->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    *data = this->_mem[index];
    return true;
}

bool List_Set(const List* this, size_t index, void* data) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (index >= this->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    this->_mem[index] = data;
    return true;
}

bool List_FindFirst(const List* this, matcher match, size_t* index) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    for (size_t i = 0; i < this->_length; i++) {
        if (match(i, this->_mem[i])) {
            *index = i;
            return true;
        }
    }
    return false;
}

bool List_FindLast(const List* this, matcher match, size_t* index) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    for (size_t i = this->_length; i-- > 0; ) {
        if (match(i, this->_mem[i])) {
            *index = i;
            return true;
        }
    }
    return false;
}

bool List_Contains(const List* this, const void* data) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    for (size_t i = this->_length; i-- > 0; ) {
        if (this->_mem[i] == data) {
            return true;
        }
    }
    return false;
}

bool List_FindAll(const List* this, matcher match, size_t** indices, size_t* numIndices) {
    static_assert(sizeof(size_t) == sizeof(void*), "黑魔法这一块/.");
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    List* theIndices;
    if (!List_Init(&theIndices)) {
        return false;
    }
    for (size_t i = 0; i < this->_length; i++) {
        if (match(i, this->_mem[i])) {
            // 是的，在64位系统上，size_t和void*长度相同，所以说List可以直接存储size_t
            // 地址就是值
            // 所以说可以直接传递，但是不能使用 List_DestroyFree List_ClearFree List_RemoveFree
            List_Add(theIndices, (void*) i);
        }
    }
    *numIndices = theIndices->_length;
    if (!*numIndices) {
        *indices = NULL;
        return false;
    }
    List_ToArray(theIndices, (void***) &indices, NULL);
    List_Destroy(theIndices);
    return true;
}

bool List_Copy(const List* dst, const List* src, size_t dstStart, size_t srcStart, size_t num) {
    if (src == NULL || dst == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    // size为0怎么办，只有天知道。
    if (num == 0) return true;
    // 现在不是星际时代，内存没那么大，不可能溢出
    if (srcStart >= src->_length || dstStart >= dst->_length || dstStart + num - 1 >= dst->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    memcpy(dst->_mem + dstStart, src->_mem + srcStart, num * sizeof(void*));
    return true;
}

bool List_CopyAdd(List* dst, const List* src, size_t srcStart, size_t num) {
    if (src == NULL || dst == NULL) {
        Error_SetTo("List对象为NULL");
        return false;
    }
    if (num == 0) return true;
    if (srcStart >= src->_length || srcStart + num - 1 >= src->_length) {
        Error_SetTo("索引超出范围");
        return false;
    }
    if (dst->_capacity - dst->_length < num) {
        if (!List_PreserveImpl(dst, dst->_capacity + num * 2)) {
            Error_SetTo("扩容失败，无法添加项目");
            return false;
        }
    }
    memcpy(dst->_mem + dst->_length, src->_mem + srcStart, num * sizeof(void*));
    dst->_length += num;
    return true;
}

bool List_FromArray(List** this, void** array, size_t arraySize) {
    if (!List_InitPreserve(this, arraySize)) return false;
    memcpy((*this)->_mem, array, arraySize * sizeof(void*));
    return true;
}

bool List_ToArray(const List* this, void*** array, size_t* arraySize) {
    if (this == NULL) {
        Error_SetTo("List对象为NULL");
        *array = NULL;
        if (arraySize != NULL) *arraySize = 0;
        return false;
    }
    *array = malloc(this->_length * sizeof(void*));
    if (*array == NULL) {
        Error_SetTo("无法创建返回对象");
        return false;
    }
    memcpy(*array, this->_mem, this->_length * sizeof(void*));
    if (arraySize != NULL) *arraySize = this->_length;
    return true;
}