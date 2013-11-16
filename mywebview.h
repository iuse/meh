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
    explicit MyWebView(int mode, UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent = 0);

public slots:
    void setCoord(int x, int y);
    void update();

signals:
    void mouseCoordChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *ev);

private:
    int mode;

    UDPAdapter *udp;
    QTimer *timer;

    QWebElement lastElement;

    int x;
    int y;
};

#endif // MYWEBVIEW_H
