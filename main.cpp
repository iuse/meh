#include "heatmap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Heatmap hm;
    if(argc < 2)
        hm = new Heatmap(MODE_MOUSE);
    else if (argc == 2 && QString(argv[1]).toInt() == MODE_OPENGAZER)
        hm = new Heatmap(MODE_OPENGAZER);

    QPalette pal();
    hm.setStyleSheet("background:transparent;");
    hm.setAttribute(Qt::WA_TranslucentBackground);
//    hm.setWindowFlags(Qt::FramelessWindowHint);
    hm.show();

    return a.exec();
}
