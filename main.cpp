#include "macros.h"
#include "heatmap.h"
#include "mywebview.h"
#include "udpadapter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    UDPAdapter udp;

    Heatmap *hm;
    MyWebView *wv;

    if(argc < 2) {
        hm = new Heatmap(MODE_OPENGAZER, &udp, &w);
        wv = new MyWebView(MODE_OPENGAZER, &udp, &w);
    }
    else if (argc == 2 && QString(argv[1]).toInt() == MODE_MOUSE) {
        hm = new Heatmap(MODE_MOUSE, &udp, &w);
        wv = new MyWebView(MODE_MOUSE, &udp, &w);
    }

    QPalette pal();
    hm->setStyleSheet("background:transparent;");
    hm->setAttribute(Qt::WA_TranslucentBackground);
//    hm->setWindowFlags(Qt::FramelessWindowHint);
//    w->show();

    wv->stackUnder(hm);
    w.resize(WIN_WIDTH, WIN_HEIGHT);
    w.show();



    return a.exec();
}
