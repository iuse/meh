#include "mywebview.h"

MyWebView::MyWebView(UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent) :
    QWebView(parent),
    parent(parent),
    udp(udpAdapter),
    timer(qTimer)
{
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timeout()));
    this->resize(WIN_WIDTH, WIN_HEIGHT);
}

/**
 * Slot to receive timeout signal from a QTimer.
 * It is used to set the sampling rate.
 *
 * @brief Heatmap::timeout
 */
void MyWebView::timeout()
{
    QWebElement element;
    element = ((QWebHitTestResult)(this->page()->mainFrame()->hitTestContent(QPoint(this->x, this->y)))).element();

    if(element.localName() != "" && element != this->lastElement) {
        element.setStyleProperty("outline", "red solid thick");
        this->lastElement.setStyleProperty("outline", "");
        this->lastElement = element;
    }
}

/**
 * Slot to receive new coordinates and update the heatmap.
 * This can be used by other Qt objects emitting signals.
 *
 * @brief Heatmap::setCoord sets new coordinates through Qt signal.
 * @param x x-coordinate on the widget window.
 * @param y y-coordinate on the widget window.
 */
void MyWebView::openGazerCoordChanged(int x, int y)
{
    this->x = (x<WIN_WIDTH)?x:(WIN_WIDTH-1);
    this->y = (y<WIN_HEIGHT)?y-65:(WIN_HEIGHT-1);
}

void MyWebView::mouseCoordChanged(int x, int y)
{
    this->x = x;
    this->y = y;
}


void MyWebView::mouseMoveEvent(QMouseEvent *ev)
{
    emit setMouseCoordChanged(ev->x(), ev->y());
}
