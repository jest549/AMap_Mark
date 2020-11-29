#ifndef AMAPMAINWINDOW_H
#define AMAPMAINWINDOW_H

#include <QMainWindow>
#include<QWebEngineView>
#include <QWebChannel>
#include <QDebug>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include<QWidget>
#include "jswebbridge.h"
#include <QLineEdit>
#include <QPushButton>
QT_BEGIN_NAMESPACE
namespace Ui { class AMapMainWindow; }
QT_END_NAMESPACE

class AMapMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AMapMainWindow(QWidget *parent = nullptr);
    ~AMapMainWindow();
    QWebEngineView *p_AmapView;
    QWebChannel *p_AmapChannel;
    JSWebBridge *p_AmapBridge;
    QMainWindow *p_AmapViewWedget;
    QHBoxLayout *p_AmapHLayout;
    QWidget *p_AmapHLayoutWidget;
    QPushButton *p_addOneMark;
    QPushButton *p_addMarkFromFile;
    QPushButton *p_removeAllMark;
    QPushButton *p_saveAllMarkToFile;
    QVBoxLayout *p_operaterLayout;
    QWidget *p_operaterLayoutWidget;
private:
    Ui::AMapMainWindow *ui;
    void resizeEvent(QResizeEvent* size);
};
#endif // AMAPMAINWINDOW_H
