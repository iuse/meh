#include "mainwindow.h"

MainWindow::MainWindow(const QUrl& url) :
    webviewMode(MODE_NONE),
    heatmapMode(MODE_NONE),
    progress(0)
{
    udp = new UDPAdapter(this);
    timer = new QTimer(this);
    webview = new MyWebView(udp, timer, this);
    heatmap = new Heatmap(udp, timer, this);

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // set sampling interval
    timer->start(INTERVAL);

    heatmap->setStyleSheet("background: transparent;");
    heatmap->setAttribute(Qt::WA_TranslucentBackground);

    webview->stackUnder(heatmap);
    connect(webview, SIGNAL(setMouseCoordChanged(int,int)), this, SLOT(setMouseCoordChanged(int,int)));

    webview->load(url);
    connect(webview, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(webview, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(webview, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(webview, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(webview->pageAction(QWebPage::Back));
    toolBar->addAction(webview->pageAction(QWebPage::Forward));
    toolBar->addAction(webview->pageAction(QWebPage::Reload));
    toolBar->addAction(webview->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);

//    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
//    QAction* viewSourceAction = new QAction("Page Source", this);
//    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
//    viewMenu->addAction(viewSourceAction);

    QMenu *effectMenu = menuBar()->addMenu(tr("&Browser tracking"));
    effectMenu->addAction("Mouse tracking", this, SLOT(browserTrackMouse()));
    effectMenu->addAction("OpenGazer tracking", this, SLOT(browserTrackOpenGazer()));
    effectMenu->addAction("Tracking off", this, SLOT(browserTrackOff()));

    QMenu *heatmapMenu = menuBar()->addMenu(tr("&Heatmap tracking"));
    heatmapMenu->addAction("Mouse tracking", this, SLOT(heatmapTrackMouse()));
    heatmapMenu->addAction("OpenGazer tracking", this, SLOT(heatmapTrackOpenGazer()));
    heatmapMenu->addAction("Tracking off", this, SLOT(heatmapTrackOff()));

    setMouseTracking(true);

    setCentralWidget(webview);
    setUnifiedTitleAndToolBarOnMac(true);
    webview->stackUnder(heatmap);
    webview->resize(WIN_WIDTH, WIN_HEIGHT);
    heatmap->resize(WIN_WIDTH, WIN_HEIGHT);
    resize(WIN_WIDTH, WIN_HEIGHT);
}

void MainWindow::setMouseCoordChanged(int x, int y)
{
    emit mouseCoordChanged(x, y);
}

void MainWindow::browserTrackMouse()
{
    disconnect(this->udp, SIGNAL(openGazerCoordChanged(int,int)), this->webview, SLOT(openGazerCoordChanged(int,int)));
    connect(this, SIGNAL(mouseCoordChanged(int,int)), this->webview, SLOT(mouseCoordChanged(int,int)));
    this->webviewMode = MODE_MOUSE;
}

void MainWindow::browserTrackOpenGazer()
{
    disconnect(this, SIGNAL(mouseCoordChanged(int,int)), this->webview, SLOT(mouseCoordChanged(int,int)));
    connect(this->udp, SIGNAL(openGazerCoordChanged(int,int)), this->webview, SLOT(openGazerCoordChanged(int,int)));
    this->webviewMode = MODE_OPENGAZER;
}

void MainWindow::browserTrackOff()
{
    disconnect(this, 0, this->webview, 0);
    disconnect(this->udp, 0, this->webview, 0);
    this->webviewMode = MODE_NONE;
}

void MainWindow::heatmapTrackMouse()
{
    disconnect(this->udp, SIGNAL(openGazerCoordChanged(int,int)), this->heatmap, SLOT(openGazerCoordChanged(int,int)));
    connect(this, SIGNAL(mouseCoordChanged(int,int)), this->heatmap, SLOT(mouseCoordChanged(int,int)));
    this->heatmapMode = MODE_MOUSE;
}

void MainWindow::heatmapTrackOpenGazer()
{
    disconnect(this, SIGNAL(mouseCoordChanged(int,int)), this->heatmap, SLOT(mouseCoordChanged(int,int)));
    connect(this->udp, SIGNAL(openGazerCoordChanged(int,int)), this->heatmap, SLOT(openGazerCoordChanged(int,int)));
    this->heatmapMode = MODE_OPENGAZER;
}

void MainWindow::heatmapTrackOff()
{
    disconnect(this, 0, this->heatmap, 0);
    disconnect(this->udp, 0, this->heatmap, 0);
    this->heatmapMode = MODE_NONE;
}

void MainWindow::adjustLocation()
{
    locationEdit->setText(webview->url().toString());
}

void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(webview->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(webview->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
}

void MainWindow::changeLocation()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    webview->load(url);
    webview->setFocus();
}
