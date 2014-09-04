//
//  head.h
//  8_puzzle
//
//  Created by Damcy on 13-11-23.
//  Copyright (c) 2013年 Damcy. All rights reserved.
//

#ifndef __puzzle_head_h
#define __puzzle_head_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define Matrix_N 3

//define a data struct to hold every state
//next pre为open close队列里的前后关系
//hide_next hide_pre为隐藏的队列，即到达当前的最短路径
typedef struct item {
    int matrix[Matrix_N][Matrix_N];
    int h_x;
    int g_x;
    int f_x;
    struct item *next;
    struct item *hide_pre;
}Item;
typedef Item* ItemPtr;

typedef struct {
    ItemPtr head;
    ItemPtr tail;
}DList;
typedef DList* DListPtr;

//计算h_x 方法一：不在对应位置点的个数
int count_h_1(ItemPtr node);

#endif
