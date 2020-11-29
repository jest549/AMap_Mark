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

signals:

public slots:
    void GetPointLatLngFromAMap(const QString lat,const QString lng);
    QString GetAMapAuthorName();
};

#endif // JSWEBBRIDGE_H
