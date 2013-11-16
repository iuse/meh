#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QWidget>
#include <QtWebKitWidgets>
#include <QTimer>
#include <QDebug>

#include "macros.h"
#include "udpadapter.h"

class MyWebView : public QWebView
{
    Q_OBJECT

public:
    explicit MyWebView(UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent = 0);

signals:
    void setMouseCoordChanged(int x, int y);

public slots:
    void timeout();
    void openGazerCoordChanged(int x, int y);
    void mouseCoordChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *ev);

private:
    QWidget *parent;
    UDPAdapter *udp;
    QTimer *timer;

    QWebElement lastElement;

    int x;
    int y;
};

#endif // MYWEBVIEW_H
