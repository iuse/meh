#include "mywebview.h"

MyWebView::MyWebView(int mode, UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent) :
    QWebView(parent),
    mode(mode),
    udp(udpAdapter),
    timer(qTimer)
{
//    this->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64,Lm1laC1xdC1zdHlsZSB7CiAgICBiYWNrZ3JvdW5kOiB5ZWxsb3cgIWltcG9ydGFudDsKICAgIG91dGxpbmU6IHJlZCBzb2xpZCB0aGljayAhaW1wb3J0YW50Owp9Cg=="));

    this->load(QUrl("http://www.youtube.com/watch?v=AY4ajbu_G3k"));

    if(this->mode == MODE_OPENGAZER) {
        // Connect coordinate update signal to the slot
        connect(udp, SIGNAL(coordChanged(int,int)), this, SLOT(setCoord(int,int)));
    } else if(this->mode == MODE_MOUSE) {
        // Start mouse tracking
        this->setMouseTracking(true);
    }
    connect(this->timer, SIGNAL(timeout()), this, SLOT(update()));

    this->resize(WIN_WIDTH, WIN_HEIGHT);
}

void MyWebView::setCoord(int x, int y)
{
    this->x = x;
    this->y = y;
}

void MyWebView::update()
{
    QWebElement element;
    element = ((QWebHitTestResult)(this->page()->mainFrame()->hitTestContent(QPoint(this->x, this->y)))).element();

    if(element.localName() != "" && element != this->lastElement) {
        element.setStyleProperty("outline", "red solid thick");
        this->lastElement.setStyleProperty("outline", "");
        this->lastElement = element;
    }
}

void MyWebView::mouseMoveEvent(QMouseEvent *ev)
{
    if(this->mode == MODE_MOUSE) {
        this->x = ev->x();
        this->y = ev->y();
    }
    emit mouseCoordChanged(ev->x(), ev->y());
}

