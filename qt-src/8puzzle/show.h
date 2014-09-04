#ifndef SHOW_H
#define SHOW_H

#include <QWidget>
#include "head.h"

namespace Ui {
class Show;
}

class Show : public QWidget
{
    Q_OBJECT

public:
    int recursion_output(ItemPtr node);
    int find_0(ItemPtr node);
    explicit Show(ItemPtr first, QWidget *parent = 0);
    ~Show();

private slots:
    void on_done_clicked();

private:
    Ui::Show *ui;
};

#endif // SHOW_H
