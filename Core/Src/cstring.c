//
// Created by ubuntu on 2023/8/29.
//
#include "cstring.h"
// 初始化向量
void initVector(cstring *vec, int initialCapacity) {
    vec->data = (u_int8_t *) malloc(initialCapacity * sizeof(u_int8_t));
    vec->size = 0;
    vec->capacity = initialCapacity;
}


// 向向量中添加元素
void pushBack(cstring *vec, u_int8_t value) {
    if (vec->size == vec->capacity) {
        // 扩展容量        vec->capacity *= 2;
        vec->data = (u_int8_t *) realloc(vec->data, vec->capacity * sizeof(u_int8_t));
    }

    vec->data[vec->size] = value;
    vec->size++;
}

void pushArray(cstring *vec, u_int8_t *data, int size) {
    for (int i = 0; i < size; i++) {
        pushBack(vec, data[i]);
    }
}

void pushString(cstring *vec, u_int8_t *str) {
    pushArray(vec, str, strlen(str));
}

// 获取向量中的元素数量
int size(const cstring *vec) {
    return vec->size;
}

// 获取向量中的元素
u_int8_t at(const cstring *vec, int index) {
    if (index >= 0 && index < vec->size) {
        return vec->data[index];
    } else {
        // 可以根据需要返回特定的错误码        return -1;
    }
}

// 释放向量内存
void freeVector(cstring *vec) {
    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;
}