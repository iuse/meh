#include "macros.h"
#include "heatmap.h"
#include "mywebview.h"
#include "udpadapter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    UDPAdapter udp(&w);
    QTimer timer(&w);
    int mode = 0;

    timer.start(50);

    if(argc < 2)
        mode = MODE_OPENGAZER;
    else if (argc == 2 && QString(argv[1]).toInt() == MODE_MOUSE)
        mode = MODE_MOUSE;

    MyWebView wv(mode, &udp, &timer, &w);
    Heatmap hm(mode, &udp, &timer, &wv, &w);
    wv.stackUnder(&hm);

    hm.setStyleSheet("background: transparent;");
    hm.setAttribute(Qt::WA_TranslucentBackground);

    w.resize(WIN_WIDTH, WIN_HEIGHT);
    w.show();

    return a.exec();
}
