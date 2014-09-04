
//
//  deal.c
//  8_puzzle
//
//  Created by Damcy on 13-11-23.
//  Copyright (c) 2013年 Damcy. All rights reserved.
//

#include "head.h"
#include "eightpuzzle.h"
#include "ui_eightpuzzle.h"
extern int terminated_matrix[Matrix_N][Matrix_N];

//初始化puzzle problem状态
ItemPtr eightPuzzle::init_puzzle()
{
    int temp_i = 0, temp_j = 0, mark = 1, signal[9]={0};
    ItemPtr head = (ItemPtr)malloc(sizeof(Item));
    if (head != NULL) {
        mark = 0;
        for (temp_i = 0; temp_i < 9; ++temp_i) {
            signal[temp_i] = 0;
        }
        //scanf data
        head->matrix[0][0] = ui->init1->text().toInt();
        head->matrix[0][1] = ui->init2->text().toInt();
        head->matrix[0][2] = ui->init3->text().toInt();
        head->matrix[1][0] = ui->init4->text().toInt();
        head->matrix[1][1] = ui->init5->text().toInt();
        head->matrix[1][2] = ui->init6->text().toInt();
        head->matrix[2][0] = ui->init7->text().toInt();
        head->matrix[2][1] = ui->init8->text().toInt();
        head->matrix[2][2] = ui->init9->text().toInt();
        //check input data available?
        for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
            for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
                signal[head->matrix[temp_i][temp_j]] = 1;
            }
        }
        for (temp_i = 0; temp_i < 9; ++temp_i) {
            if (!signal[temp_i]) {
                mark = 1;
            }
        }
        if (mark) {
            //
            QMessageBox::information(NULL, "Waring", "初始状态有误~", QMessageBox::Yes, QMessageBox::Yes);
            return NULL;
        }

        head->g_x = 0;
        head->h_x = count_h_1(head);
        head->f_x = head->h_x + head->g_x;
        head->next = NULL;
        head->hide_pre = NULL;
    }
    else {
        //printf("cannt malloc ItemPtr\n");
        QMessageBox::warning(NULL, "warning", "Cannt malloc ItemPtr", QMessageBox::Yes, QMessageBox::Yes);
        return NULL;
    }
    return head;
}

//check input and output valid
//valid 1 invalid 0
int eightPuzzle::check_valid(DListPtr list)
{
    int sum1 = 0, sum2 = 0, signal[9]={0}, mark = 0;
    int temp_i = 0, temp_j = 0;
    int arr1[9] = {0}, arr2[9] = {0};
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            arr1[temp_i*3 + temp_j] = terminated_matrix[temp_i][temp_j];
            arr2[temp_i*3 + temp_j] = list->head->matrix[temp_i][temp_j];
        }
    }

    for (temp_i = 0; temp_i < 9; ++temp_i) {
        for (temp_j = 0; temp_j < temp_i; ++temp_j) {
            if (arr1[temp_i] && (arr1[temp_i] < arr1[temp_j])) {
                ++sum1;
            }
            if (arr2[temp_i] && (arr2[temp_i] < arr2[temp_j])) {
                ++sum2;
            }
        }
    }
    //check terminated data available?
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            ++signal[terminated_matrix[temp_i][temp_j]];
        }
    }
    for (temp_i = 0; temp_i < 9; ++temp_i) {
        if (!signal[temp_i]||signal[temp_i]>1) {
            ++mark;
        }
    }
    if (mark > 1) {
        QMessageBox::information(NULL, "Waring", "目标状态有误~", QMessageBox::Yes, QMessageBox::Yes);
        return -1;
    }
    if (((sum1%2) == (sum2%2))) {
        return 1;
    }
    return 0;
}

//init open dlist
DListPtr eightPuzzle::initDList()
{
    DListPtr plist = (DListPtr)malloc(sizeof(DList));
    ItemPtr head = init_puzzle();
    if (plist != NULL) {
        if (head != NULL) {
            plist->head = head;
            plist->tail = head;
        }
        else {
            free(plist);
            return NULL;
        }
    }
    else {
        printf("cannt not malloc DList\n");
        exit(0);
    }
    return plist;
}

//init closed dlist
DListPtr eightPuzzle::initClosed()
{
    DListPtr closed = (DListPtr)malloc(sizeof(DList));
    if (closed != NULL) {
        closed->head = NULL;
        closed->tail = NULL;
    }
    else {
        printf("cannt not malloc DList\n");
        exit(0);
    }
    return closed;
}

//free resource
void eightPuzzle::free_list(DListPtr list)
{
    ItemPtr node = NULL;
    if (list != NULL) {
        while (list->head != NULL) {
            node = list->head;
            list->head = list->head->next;
            free(node);
            node = NULL;
        }
        free(list);
        list = NULL;
    }
}

//取open表的第一个节点
ItemPtr eightPuzzle::get_open_first(DListPtr open)
{
    ItemPtr first = NULL;
    first = open->head;
    if (open->head == open->tail) {
        open->head = NULL;
        open->tail = NULL;
    }
    else {
        open->head = open->head->next;
    }
    return first;
}

//compare two node
//same 0 diff 1
int eightPuzzle::compare_two_node(ItemPtr first, ItemPtr second)
{
    int temp_i = 0, temp_j = 0;
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            if (first->matrix[temp_i][temp_j] != second->matrix[temp_i][temp_j]) {
                return 1;
            }
        }
    }
    return 0;
}

//遍历close表查找该状态是否已经走过
//走过1 没走过0
int eightPuzzle::travel_close(DListPtr closed, ItemPtr expand_node)
{
    ItemPtr node = NULL;
    if (closed->head == NULL) {
        return 0;
    }
    node = closed->head;
    while (node != NULL) {
        if (!compare_two_node(node, expand_node)) {
            if (node->g_x > expand_node->g_x) {
                node->g_x = expand_node->g_x;
                node->f_x = node->h_x + node->g_x;
                node->hide_pre = expand_node->hide_pre;
            }
            break;
        }
        node = node->next;
    }
    if (node == NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

//找到0在那个格子
int eightPuzzle::find_0(ItemPtr node)
{
    int temp_i = 0, temp_j = 0, result = 0;
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            if (node->matrix[temp_i][temp_j] == 0) {
                result = (temp_i+1)*10 + temp_j;
            }
        }
    }
    return result;
}

//将节点加到open表里
void eightPuzzle::add_to_open(DListPtr open, ItemPtr node)
{
    ItemPtr ptr = NULL, prePtr = NULL;
    if (open->head == NULL) {
        open->head = node;
        open->tail = node;
    }
    else {
        ptr = open->head;
        while (ptr != NULL && ptr->f_x <= node->f_x) {
            prePtr = ptr;
            ptr = ptr->next;
        }
        if (ptr == NULL) {
            open->tail->next = node;
            open->tail = node;
        }
        else {              //插入到prePtr 和 ptr 中间
            if (prePtr == NULL) {
                node->next = open->head;
                open->head = node;
            }
            else {
                prePtr->next = node;
                node->next = ptr;
            }
        }
    }
    ptr = NULL;
    prePtr = NULL;
}

//将to_expand节点加入closed表，结束该节点的拓展流程
void eightPuzzle::add_to_closed(DListPtr closed, ItemPtr node)
{
    if (closed->head == NULL) {
        closed->head = node;
        closed->tail = node;
    }
    else {
        node->next = NULL;
        closed->tail->next = node;
        closed->tail = node;
    }
}

//根据mark = 1 2 3 4进行上 下 左 右填充
void eightPuzzle::step(int x, int y, int mark, ItemPtr node, DListPtr open, DListPtr closed)
{
    ItemPtr new_node = NULL;
    int temp_i = 0, temp_j = 0;
    new_node = (ItemPtr)malloc(sizeof(Item));
    if (new_node) {
        for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
            for (temp_j = 0; temp_j <Matrix_N; ++temp_j) {
                new_node->matrix[temp_i][temp_j] = node->matrix[temp_i][temp_j];
            }
        }
        switch (mark) {
            case 1:
                new_node->matrix[x][y] = new_node->matrix[x+1][y];
                new_node->matrix[x+1][y] = 0;
                break;
            case 2:
                new_node->matrix[x][y] = new_node->matrix[x-1][y];
                new_node->matrix[x-1][y] = 0;
                break;
            case 3:
                new_node->matrix[x][y] = new_node->matrix[x][y+1];
                new_node->matrix[x][y+1] = 0;
                break;
            case 4:
                new_node->matrix[x][y] = new_node->matrix[x][y-1];
                new_node->matrix[x][y-1] = 0;
                break;
            default:
                break;
        }
        new_node->g_x = node->g_x + 1;
        new_node->hide_pre = node;
        new_node->next = NULL;
        new_node->h_x = count_h_1(new_node);
        new_node->f_x = new_node->h_x + new_node->g_x;
        if (!travel_close(closed, new_node)) {      //没走过
            add_to_open(open, new_node);
            new_node = NULL;
        }
        else {              //走过
            free(new_node);
            new_node = NULL;
        }
    }
    else {
        printf("cannt malloc new_node\n");
        exit(0);
    }
}

//根据0的位置进行移动并进行试探
//进行判重 如果已有则判断步数哪个更优 不存在则加入open表
void eightPuzzle::have_a_try(int x, int y, ItemPtr node, DListPtr open, DListPtr closed)
{
    int count = 0;
    if (x == 1 && y == 1) {             //[1][1]    上下左右
        count = 0;
        while (++count < 5) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 3:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                case 4:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }

                default:
                    break;
            }
        }
    }
    else if (x == 1 && y == 0) {        //[1][0]    上下左
        count = 0;
        while (++count < 4) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 3:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }
    }
    else if (x == 1 && y == 2) {        //[1][2]    上下右
        count = 0;
        while (++count < 4) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 3:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }    }
    else if (x == 0 && y == 1) {        //[0][1]    上左右
        count = 0;
        while (++count < 4) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                case 3:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }
    }
    else if (x == 2 && y == 1) {        //[2][1]    下左右
        count = 0;
        while (++count < 4) {
            switch (count) {
                case 1:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                case 3:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }    }
    else if (x == 0 && y == 0) {        //[0][0]    上左
        count = 0;
        while (++count < 3) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }    }
    else if (x == 0 && y == 2) {        //[0][2]    上右
        count = 0;
        while (++count < 3) {
            switch (count) {
                case 1:
                {
                    step(x, y, 1, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }    }
    else if (x == 2 && y == 0) {        //[2][0]    下左
        count = 0;
        while (++count < 3) {
            switch (count) {
                case 1:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 3, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }
    }
    else {                              //[2][2]    下右
        count = 0;
        while (++count < 3) {
            switch (count) {
                case 1:
                {
                    step(x, y, 2, node, open, closed);
                    break;
                }
                case 2:
                {
                    step(x, y, 4, node, open, closed);
                    break;
                }
                default:
                    break;
            }
        }
    }
}


//拓展该节点的下一个状态
void eightPuzzle::expand_next(DListPtr open, DListPtr closed, ItemPtr to_expand)
{
    int position = 0, position_x = 0, position_y = 0;
    position = find_0(to_expand);
    position_x = position / 10 - 1;
    position_y = position % 10;
    have_a_try(position_x, position_y, to_expand, open, closed);

}

//check now看看取得open表的第一个节点是否为目标
//这个可以放在expand的时候判断，不过为了不中断整个拓展流程所以放在这里，初步考虑两者应该是相同的一位h_x是已走步数
//1为目标节点 0则不是
int eightPuzzle::check_now(ItemPtr node)
{
    int temp_i = 0, temp_j = 0;
    for (temp_i = 0; temp_i < Matrix_N; ++temp_i) {
        for (temp_j = 0; temp_j < Matrix_N; ++temp_j) {
            if (node->matrix[temp_i][temp_j] != terminated_matrix[temp_i][temp_j]) {
                return 0;
            }
        }
    }
    return 1;
}

//find QLabel
QLabel* eightPuzzle::find_label(int i)
{
    int a = i/10 - 1;
    int b = i%10;
    switch(a*3 + b + 1) {
    case 1:
        return ui->show1;
        break;
    case 2:
        return ui->show2;
        break;
    case 3:
        return ui->show3;
        break;
    case 4:
        return ui->show4;
        break;
    case 5:
        return ui->show5;
        break;
    case 6:
        return ui->show6;
        break;
    case 7:
        return ui->show7;
        break;
    case 8:
        return ui->show8;
        break;
    case 9:
        return ui->show9;
        break;
    }
}

//recursion output
int eightPuzzle::recursion_output(ItemPtr node)
{
    if (node != NULL) {
        int i = recursion_output(node->hide_pre);

        if (i == 0) {
            ui->show1->setText(QString::number(node->matrix[0][0]));
            ui->show2->setText(QString::number(node->matrix[0][1]));
            ui->show3->setText(QString::number(node->matrix[0][2]));
            ui->show4->setText(QString::number(node->matrix[1][0]));
            ui->show5->setText(QString::number(node->matrix[1][1]));
            ui->show6->setText(QString::number(node->matrix[1][2]));
            ui->show7->setText(QString::number(node->matrix[2][0]));
            ui->show8->setText(QString::number(node->matrix[2][1]));
            ui->show9->setText(QString::number(node->matrix[2][2]));
            int result = find_0(node);
            find_label(result)->setText("");
            return result;
        }
        else {
            int temp = find_0(node);
            int row_now = i/10 - 1;
            int column_now = i%10;
            int a = node->matrix[row_now][column_now];
            QLabel* label_i = find_label(i);
            QLabel* label_temp = find_label(temp);

            QElapsedTimer t;
            t.start();
            while(t.elapsed() < 1000)
                QCoreApplication::processEvents();

            label_i->setText(QString::number(a));
            label_temp->setText("");

            return temp;
        }
    }
    else {
        return 0;
    }
}

//deal with the problem
ItemPtr eightPuzzle::deal()
{
    int end = 1;
    ItemPtr first = NULL;
    while (end && open->head!=NULL) {
        first = get_open_first(open);
        if (check_now(first)) {
            end = 0;
            break;
        }
        add_to_closed(closed, first);
        expand_next(open, closed, first);
        first = NULL;
    }
    if (end) {
        QMessageBox::critical(this, "无解", "为什么会无解呢= =", QMessageBox::Yes, QMessageBox::Yes);
        return NULL;
    }
    return first;
}
