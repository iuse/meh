#include "mywebview.h"

MyWebView::MyWebView(int mode, UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent) :
    QWebView(parent)
{
    this->udp = udpAdapter;
    this->timer = qTimer;

    this->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64,Lm1laC1xdC1zdHlsZSB7CiAgICBiYWNrZ3JvdW5kOiB5ZWxsb3cgIWltcG9ydGFudDsKICAgIG91dGxpbmU6IHJlZCBzb2xpZCB0aGljayAhaW1wb3J0YW50Owp9Cg=="));

    this->resize(WIN_WIDTH, WIN_HEIGHT);
    this->load(QUrl("http://www.youtube.com/watch?v=AY4ajbu_G3k"));

    if(mode == MODE_OPENGAZER) {
        // Connect coordinate update signal to the slot
        connect(udp, SIGNAL(coordChanged(int,int)), this, SLOT(setCoord(int,int)));
    } else if(mode == MODE_MOUSE) {
        // Start mouse tracking
        this->setMouseTracking(true);
    }
    connect(this->timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MyWebView::setCoord(int x, int y)
{
    this->highlightDOMElement(x, y);
}

void MyWebView::update()
{
    QWebElement element;
    element = ((QWebHitTestResult)(this->page()->mainFrame()->hitTestContent(QPoint(this->x, this->y)))).element();

    if(element.localName() != "" && element != this->lastElement) {
//        qDebug() << "different element, namespaceURI:"
//                 << element.namespaceUri()
//                 << "; localName:" << element.localName()
//                 << "; classes:" << element.classes();
        element.setStyleProperty("outline", "red solid thick");
        this->lastElement.setStyleProperty("outline", "");
        this->lastElement = element;
    }
}

void MyWebView::mouseMoveEvent(QMouseEvent *ev)
{
//    this->highlightDOMElement(ev->x(), ev->y());
    this->x = ev->x();
    this->y = ev->y();
}

void MyWebView::highlightDOMElement(int x, int y) {
//    qDebug() << "mouse moving x:" << x << ", y:" << y;
//    QString code = "document.elementFromPoint(" + QString::number(x) + "," + QString::number(y)
//            + ").style.border='double';";
    QString code =
            "document.currHighlightEl = document.elementFromPoint(" + QString::number(x) + "," + QString::number(y) + ");" +
            "if(document.lastHighlightEl != document.currHighlightEl){" +
//                "document.lastHighlightEl.style.outline = document.originalOutlineStyle;" +
//                "if(typeof document.lastHighlightEl != 'undefined'){document.lastHighlightEl.style.outline = '';};" +
//                "document.originalOutlineStyle = document.currHighlightEl.style.outline;" +
                "document.currHighlightEl.style.outline = 'red solid thick';"
                "document.lastHighlightEl = document.currHighlightEl;" +
            "};";
    this->page()->mainFrame()->evaluateJavaScript(code);
}
