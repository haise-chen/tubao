#include "tubao.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tubao w;
    w.setFixedSize(1330, 1060);
    w.show();
    return a.exec();
}
