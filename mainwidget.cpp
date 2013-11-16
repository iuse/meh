#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    int mode = MODE_MOUSE;

    udp = new UDPAdapter(this);
    timer = new QTimer(this);
    webview = new MyWebView(mode, udp, timer, this);
    heatmap = new Heatmap(mode, udp, timer, webview, this);

    timer->start(50);

    heatmap->setStyleSheet("background: transparent;");
    heatmap->setAttribute(Qt::WA_TranslucentBackground);

    webview->stackUnder(heatmap);
    this->resize(WIN_WIDTH, WIN_HEIGHT);
}

void MainWidget::load(const QUrl &url)
{
    this->webview->load(url);
}

int MainWidget::getMode() const
{
    return mode;
}

void MainWidget::setMode(int value)
{
    mode = value;
}

MyWebView *MainWidget::getWebview() const
{
    return webview;
}

void MainWidget::setWebview(MyWebView *value)
{
    webview = value;
}


