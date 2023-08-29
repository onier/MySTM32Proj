//
// Created by ubuntu on 2023/8/29.
//

#ifndef MYSTM32PROJ_CSTRING_H
#define MYSTM32PROJ_CSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    u_int8_t *data;      // 存储数据的数组
    int size;       // 当前向量中的元素数量
    int capacity;   // 向量的容量（可容纳的最大元素数量）
} cstring;

typedef enum {
    OK = 0x00U,
    ERR = 0x01U,
    BUSY = 0x02U,
    TIMEOUT = 0x03U
} RET;

// 初始化向量
void initVector(cstring *vec, int initialCapacity);


// 向向量中添加元素
void pushBack(cstring *vec, u_int8_t value);

void pushArray(cstring *vec, u_int8_t *data, int size);

void pushString(cstring *vec, u_int8_t *str);

// 获取向量中的元素数量
int size(const cstring *vec);

// 获取向量中的元素
u_int8_t at(const cstring *vec, int index);
// 释放向量内存
void freeVector(cstring *vec);
//int main() {
//    Vector vec;
//    initVector(&vec, 3);
//
//    pushBack(&vec, 'A');
//    pushBack(&vec, 'B');
//    pushBack(&vec, 'C');
////    pushString(&vec, "12345");    char temp[] = "123456789";
//    pushArray(&vec,temp,6);
////    printf("Vector size: %d\n", size(&vec));//    printf("Vector elements: ");//    for (int i = 0; i < size(&vec); i++) {//        printf("%s ", at(&vec, i));//    }    printf("%s \n", vec.data);
//
//    freeVector(&vec);
//    return 0;
//}

#endif //MYSTM32PROJ_CSTRING_H
