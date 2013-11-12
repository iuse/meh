#ifndef UDPADAPTER_H
#define UDPADAPTER_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QUdpSocket>

//#include "heatmap.h"

class UDPAdapter : public QObject
{
    Q_OBJECT

public:
    explicit UDPAdapter(QObject *parent = 0);

signals:
    void coordChanged(int x, int y);

public slots:
    void readyRead();

public slots:

private:
    QUdpSocket *socket;
};

#endif // UDPADAPTER_H
