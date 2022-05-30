#include "Tool_Curl.h"
#include <QtWidgets/QApplication>
#include "curl/curl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tool_Curl w;
    w.show();
    return a.exec();
}
