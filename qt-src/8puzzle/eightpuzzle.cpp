#include "head.h"
#include "eightpuzzle.h"
#include "ui_eightpuzzle.h"
#include "Show.h"
#include "about.h"

eightPuzzle::eightPuzzle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eightPuzzle)
{
    ui->setupUi(this);
    QRegExp regx("[0-9]$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->init1->setValidator( validator );
    ui->init2->setValidator( validator );
    ui->init3->setValidator( validator );
    ui->init4->setValidator( validator );
    ui->init5->setValidator( validator );
    ui->init6->setValidator( validator );
    ui->init7->setValidator( validator );
    ui->init8->setValidator( validator );
    ui->init9->setValidator( validator );
    ui->final1->setValidator( validator );
    ui->final2->setValidator( validator );
    ui->final3->setValidator( validator );
    ui->final4->setValidator( validator );
    ui->final5->setValidator( validator );
    ui->final6->setValidator( validator );
    ui->final7->setValidator( validator );
    ui->final8->setValidator( validator );
    ui->final9->setValidator( validator );
}

eightPuzzle::~eightPuzzle()
{
    free(first);
    free_list(open);
    free_list(closed);
    delete ui;
}

void eightPuzzle::on_Go_clicked()
{
    if (first) {
        free(first);
        first = NULL;
    }
    if (open) {
        free_list(open);
        open = NULL;
    }
    if (closed) {
        free_list(closed);
        closed = NULL;
    }

    open = initDList();
    closed = initClosed();
    terminated_matrix[0][0] = ui->final1->text().toInt();
    terminated_matrix[0][1] = ui->final2->text().toInt();
    terminated_matrix[0][2] = ui->final3->text().toInt();
    terminated_matrix[1][0] = ui->final4->text().toInt();
    terminated_matrix[1][1] = ui->final5->text().toInt();
    terminated_matrix[1][2] = ui->final6->text().toInt();
    terminated_matrix[2][0] = ui->final7->text().toInt();
    terminated_matrix[2][1] = ui->final8->text().toInt();
    terminated_matrix[2][2] = ui->final9->text().toInt();
    //qDebug() << terminated_matrix[0][0] << endl;
    if(open && closed) {//所有条件都ok
        //数据ok
        int vali = check_valid(open);
        if(vali == 1) {
            first = deal();
            int non = recursion_output(first);
        }
        else if(vali == 0){
            QMessageBox::critical(this, "无解", "此问题状态下无解~换一组数据试一试吧~", QMessageBox::Yes, QMessageBox::Yes);
        }
    }
}

void eightPuzzle::on_Log_clicked()
{
    Show* my_show = new Show(first);
    my_show->show();
}

void eightPuzzle::on_About_clicked()
{
    about* my_about = new about();
    my_about->show();
}
