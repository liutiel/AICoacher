#include "AIcoacherGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AIcoacherGUI w;
    w.show();
    return a.exec();
}
