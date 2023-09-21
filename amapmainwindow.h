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
#include <QComboBox>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class AMapMainWindow; }
QT_END_NAMESPACE


class AMapMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    constexpr static int GPSFormat_DD_DDDDDD= 0;
    constexpr static int GPSFormat_DDMM_MMMMMM= 1;
    AMapMainWindow(QWidget *parent = nullptr);

    ~AMapMainWindow();
    void addOneMark(double lat,double lng,QString labelname);
    void getMarkerIconpath(QString path);
    void removeAllMark();
    void saveAllMarkToFile();
private:
    QWebEngineView *p_AmapView;
    QWebChannel *p_AmapChannel;
    JSWebBridge *p_AmapBridge;
    QMainWindow *p_AmapViewWedget;
    QHBoxLayout *p_AmapHLayout;
    QWidget *p_AmapHLayoutWidget;

    QWebEngineView *p_BaiduView;
    QWebChannel *p_BaiduChannel;
    JSWebBridge *p_BaiduBridge;
    QMainWindow *p_BaiduViewWedget;
    QWidget *p_BaiduHLayoutWidget;
    QHBoxLayout *p_BaiduHLayout;

    QPushButton *p_addOneMark;
    QPushButton *p_addMarkFromFile;
    QPushButton *p_removeAllMark;
    QPushButton *p_saveAllMarkToFile;
    QVBoxLayout *p_operaterLayout;
    QWidget *p_operaterLayoutWidget;
    QLineEdit *latLineEdit;
    QLineEdit *lngLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *logLngColumnEdit;//经度所在列
    QLineEdit *logLatColumnEdit;//维度所在列
    QLineEdit *logNameColumnEdit;//打点名所在列
    QLineEdit *logStartRowEdit;//开始打点的行
    QComboBox *selectLogGPSFormat;//选择GPS地图数据格式
    QString loadMarkFilePath;
private:
    Ui::AMapMainWindow *ui;
    void resizeEvent(QResizeEvent* size);

public slots:
    void addOneMarkBtnClickEvent();
    void removeAllMarkBtnClickEvent();
    void addMarkFromFileBtnClickEvent();
    void saveAllMarkToFileBtnClickEvent();
};
#endif // AMAPMAINWINDOW_H
