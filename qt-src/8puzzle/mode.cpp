
//
//  mode.c
//  8_puzzle
//
//  Created by Damcy on 13-11-24.
//  Copyright (c) 2013年 Damcy. All rights reserved.
//

#include "head.h"
#include "eightpuzzle.h"

//计算h_x 方法1：不在对应位置点的个数
int eightPuzzle::count_h_1(ItemPtr node)
{
    int mark = 0;
    int temp_i = 0, temp_j = 0;
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            if (node->matrix[temp_i][temp_j] != terminated_matrix[temp_i][temp_j]) {
                ++mark;
            }
        }
    }
    return mark;
}

//计算h_x 方法2：逆序数
//这个一般适用于
//1 2 3
//4 5 6
//7 8
int eightPuzzle::count_h_2(ItemPtr node)
{
    int mark = 0, temp_i = 0, temp_j = 0;
    int arr[9] = {0};
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            arr[temp_i*3 + temp_j] = node->matrix[temp_i][temp_j];
        }
    }
    for (temp_i = 0; temp_i < 9; ++temp_i) {
        for (temp_j = 0; temp_j < temp_i; ++temp_j) {
            if (arr[temp_j] > arr[temp_i]) {
                ++mark;
            }
        }
    }
    return mark;
}

//计算h_x 方法3：不在对应位置点的个数
int eightPuzzle::count_h_3(ItemPtr node)
{
    int mark = 0;
    return mark;
}
