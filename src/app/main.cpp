#include "MainGUI.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainGUI w;
    w.restoreConfig();
    w.show();
    return a.exec();
}
