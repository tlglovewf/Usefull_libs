#include "Tool_BinMerge.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tool_BinMerge w;
    w.show();
    return a.exec();
}
