#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "macros.h"
#include "heatmap.h"
#include "mywebview.h"
#include "udpadapter.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);

    void load(const QUrl &url);

    int getMode() const;
    void setMode(int value);

    MyWebView *getWebview() const;
    void setWebview(MyWebView *value);

signals:

public slots:

private:
    UDPAdapter *udp;
    QTimer *timer;
    Heatmap *heatmap;
    MyWebView *webview;
    int mode;
};

#endif // MAINWIDGET_H
