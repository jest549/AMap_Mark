#include "amapmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AMapMainWindow w;
    w.show();
    return a.exec();
}
