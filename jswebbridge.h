#ifndef JSWEBBRIDGE_H
#define JSWEBBRIDGE_H

#include <QObject>
#include <QJSValue>
#include <functional>
#include <QVariant>
#include <QString>
#include <QDebug>
class JSWebBridge : public QObject
{
    Q_OBJECT
public:
    explicit JSWebBridge(QObject *parent = nullptr);

    QString saveAllMarkPath;
signals:
public slots:
    void GetPointLatLngFromAMap(const QString lat,const QString lng,const QString name);
    QString GetAMapAuthorName();
    void  saveAllMarkToFileFromAMap(QString lat,QString lng,QString labelname);
};

#endif // JSWEBBRIDGE_H
