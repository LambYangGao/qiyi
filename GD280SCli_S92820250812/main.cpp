#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint |Qt::WindowCloseButtonHint);
    w.showNormal();
    return a.exec();
}

