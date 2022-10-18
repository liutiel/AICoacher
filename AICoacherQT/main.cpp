#include "AICoacherQT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AICoacherQT w;
    w.show();
    return a.exec();
}
