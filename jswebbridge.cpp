#pragma execution_character_set("utf-8")//防止中文乱码
#include "jswebbridge.h"
#include <QMutex>
#include <QFile>
static QMutex fileMutex;
JSWebBridge::JSWebBridge(QObject *parent) : QObject(parent)
{
    saveAllMarkPath="D:/";
}
QString JSWebBridge::GetAMapAuthorName()
{
    qDebug()<<"AuthorName:Jest";
    return "AuthorName:Jest";
}
void JSWebBridge::GetPointLatLngFromAMap(const QString lat,const QString lng,const QString name)
{
   qDebug()<<"来自web标记点输出" <<"维度"<<lat<<","<<"经度"<<lng<<",名字"<<name;
}
void  JSWebBridge::saveAllMarkToFileFromAMap(QString lat,QString lng,QString labelname)
{
    fileMutex.lock();
    QFile file(saveAllMarkPath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
    {
        QString tmp_str=lng+","+lat+","+labelname;
        QTextStream out(&file);
        out<<tmp_str<< endl;
        file.close();

    }
    fileMutex.unlock();
}
