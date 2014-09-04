#ifndef EIGHTPUZZLE_H
#define EIGHTPUZZLE_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QElapsedTimer>
#include <iostream>
#include "head.h"

namespace Ui {
class eightPuzzle;
}

class eightPuzzle : public QWidget
{
    Q_OBJECT

public:
    explicit eightPuzzle(QWidget *parent = 0);
    ~eightPuzzle();

    //初始化puzzle problem状态
    ItemPtr init_puzzle();
    //init open list
    DListPtr initDList();
    //init closed list
    DListPtr initClosed();
    //compare two node
    //same 0 diff 1
    int compare_two_node(ItemPtr first, ItemPtr second);
    //check input and output valid
    //valid 1 invalid 0
    int check_valid(DListPtr list);
    //程序入口函数，维护open close表
    ItemPtr deal();
    //free function
    void free_list(DListPtr list);
    //取open表的第一个节点
    ItemPtr get_open_first(DListPtr open);
    //遍历close表查找该状态是否已经走过
    //走过1 没走过0
    int travel_close(DListPtr closed, ItemPtr expand_node);
    //找到0在那个格子
    int find_0(ItemPtr node);
    //将节点加到open表里
    void add_to_open(DListPtr open, ItemPtr node);
    //将to_expand节点加入closed表，结束该节点的拓展流程
    void add_to_closed(DListPtr closed, ItemPtr node);
    //根据mark = 1 2 3 4进行上 下 左 右填充
    void step(int x, int y, int mark, ItemPtr node, DListPtr open, DListPtr closed);
    //根据0的位置进行移动并进行试探
    //进行判重 如果已有则判断步数哪个更优 不存在则加入open表
    void have_a_try(int x, int y, ItemPtr node, DListPtr open, DListPtr closed);
    //拓展该节点的下一个状态
    void expand_next(DListPtr open, DListPtr closed, ItemPtr to_expand);
    //check now看看取得open表的第一个节点是否为目标
    //这个可以放在expand的时候判断，不过为了不中断整个拓展流程所以放在这里，初步考虑两者应该是相同的一位h_x是已走步数
    //1为目标节点 0则不是
    int check_now(ItemPtr node);
    //recursion output
    int recursion_output(ItemPtr node);
    //find QLabel
    QLabel *find_label(int i);

    //mode
    int count_h_1(ItemPtr node);
    int count_h_2(ItemPtr node);
    int count_h_3(ItemPtr node);

private slots:
    void on_Go_clicked();

    void on_Log_clicked();

    void on_About_clicked();

private:
    int terminated_matrix[Matrix_N][Matrix_N];
    DListPtr open = NULL, closed = NULL;
    ItemPtr first = NULL;
    Ui::eightPuzzle *ui;
};
#endif // EIGHTPUZZLE_H
