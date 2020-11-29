#include "amapmainwindow.h"
#include "ui_amapmainwindow.h"

AMapMainWindow::AMapMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AMapMainWindow)
{
    ui->setupUi(this);
}

AMapMainWindow::~AMapMainWindow()
{
    delete ui;
}

