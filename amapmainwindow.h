#ifndef AMAPMAINWINDOW_H
#define AMAPMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AMapMainWindow; }
QT_END_NAMESPACE

class AMapMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AMapMainWindow(QWidget *parent = nullptr);
    ~AMapMainWindow();

private:
    Ui::AMapMainWindow *ui;
};
#endif // AMAPMAINWINDOW_H
