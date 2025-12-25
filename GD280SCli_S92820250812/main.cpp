#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    //w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);
    w.setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowTitleHint | Qt::WindowMaximizeButtonHint |Qt::WindowCloseButtonHint);
    //w.setWindowOpacity(1);
    //w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    //w.showMaximized();
    w.showNormal();
    //w.show();
    return a.exec();
}

