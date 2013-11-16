#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "macros.h"
//#include "heatmap.h"
#include "mywebview.h"
//#include "udpadapter.h"
#include "mainwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl &url);

signals:
    void mouseCoordChanged(int x, int y);
    void openGazerCoordChanged(int x, int y);

public slots:
    void setMouseCoordChanged(int x, int y);
    void browserTrackMouse();
    void browserTrackOpenGazer();
    void browserTrackOff();
    void heatmapTrackMouse();
    void heatmapTrackOpenGazer();
    void heatmapTrackOff();

protected slots:
    void adjustLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void changeLocation();


private:
    int webviewMode;
    int heatmapMode;
    int progress;

    QLineEdit *locationEdit;

    UDPAdapter *udp;
    QTimer *timer;
    Heatmap *heatmap;
    MyWebView *webview;
};

#endif // MAINWINDOW_H
