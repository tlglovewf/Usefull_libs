#include "Test_Tree.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Test_Tree w;
    w.show();
    return a.exec();
}
