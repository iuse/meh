#include "heatmap.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Heatmap hm;
    hm.show();

    return a.exec();
}
