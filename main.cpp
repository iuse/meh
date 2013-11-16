#include <QApplication>

#include "macros.h"
#include "heatmap.h"
#include "mywebview.h"
#include "udpadapter.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QUrl url;
    if (argc > 1)
        url = QUrl::fromUserInput(argv[1]);
    else
        url = QUrl("http://www.google.com/ncr");

    MainWindow *browser = new MainWindow(url);
    browser->show();

    return a.exec();
}
