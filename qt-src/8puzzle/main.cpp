#include "eightpuzzle.h"
#include "head.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eightPuzzle w;
    w.show();

    return a.exec();
}
