#include "heatmap.h"

/**
 * Constructs a Heatmap object.
 *
 * @brief Heatmap::Heatmap
 * @param parent defaults to 0.
 */
Heatmap::Heatmap(int mode, UDPAdapter *udpAdapter, QTimer *qTimer, MyWebView *myWebView, QWidget *parent) :
    QWidget(parent),
    mode(mode),
    udp(udpAdapter),
    timer(qTimer),
    webview(myWebView),
    x(-1),
    y(-1),
    oldX(0),
    oldY(0),
    colorImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_ARGB32),
    alphaImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_ARGB32),
    data{},
    width(WIN_WIDTH),
    height(WIN_HEIGHT),
    radius(RADIUS),
    max(1),
    opacity(OPACITY)
{
    // Append image to top left corner
    setAttribute(Qt::WA_StaticContents);

    // Create color spectrum palette
    initColorPalette();

    // Reset visible background color
    this->clearImages();
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    if(this->mode == MODE_OPENGAZER) {
        // Connect coordinate update signal to the slot
        connect(this->udp, SIGNAL(coordChanged(int,int)), this, SLOT(setCoord(int,int)));
    } else if(this->mode == MODE_MOUSE) {
        // Start mouse tracking
        connect(this->webview, SIGNAL(mouseCoordChanged(int,int)), this, SLOT(mouseCoordChanged(int,int)));
    }
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timeout()));

    this->resize(WIN_WIDTH, WIN_HEIGHT);
}

/**
 * Slot to receive new coordinates and update the heatmap.
 * This can be used by other Qt objects emitting signals.
 *
 * @brief Heatmap::setCoord sets new coordinates through Qt signal.
 * @param x x-coordinate on the widget window.
 * @param y y-coordinate on the widget window.
 */
void Heatmap::setCoord(int x, int y)
{
    this->x = (x<WIN_WIDTH)?x:(WIN_WIDTH-1);
    this->y = (y<WIN_HEIGHT)?y:(WIN_HEIGHT-1);
}

/**
 * Slot to receive timeout signal from a QTimer.
 * It is used in OpenGazer mode to set the sampling rate.
 *
 * @brief Heatmap::timeout
 */
void Heatmap::timeout()
{
    if((this->x == this->oldX && this->y == this->oldY) || (this->x < 0 || this->y < 0))
        return;
    this->addDataPoint(this->x, this->y);
    this->oldX = this->x;
    this->oldY = this->y;
}

/**
 * Slot to receive mouse move signal from MyWebView.
 *
 * @brief Heatmap::mouseCoordChanged
 * @param x
 * @param y
 */
void Heatmap::mouseCoordChanged(int x, int y)
{
//    this->addDataPoint(x, y);
    this->x = x;
    this->y = y;
}

/**
 * Overriden paintEvent. Draws the color spectrum representation of the heatmap.
 *
 * @brief Heatmap::paintEvent
 * @param ev
 */
void Heatmap::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QRect dirtyRect = ev->rect();
    painter.drawImage(dirtyRect.topLeft(), this->colorImage, dirtyRect);
}

/**
 * Updates the intensity level at the point (x,y) and then updates
 * the image.
 *
 * @brief Heatmap::addDataPoint
 * @param x
 * @param y
 */
void Heatmap::addDataPoint(int x, int y)
{
    this->data[x][y] += 1;
    int temp = this->data[x][y];

    if(this->max < temp) {
        this->max = temp;
        this->setDataSet();
    } else {
        this->drawAlphaImage(x, y, temp, true);
    }
}

/**
 * Runs through entire pixel matrix and calls drawAlphaImage() to
 * refresh each point.
 *
 * There is a minimal optimization and only ponits where intensity level
 * is bigger than zero is updated, but more optimizations could be done.
 *
 * @brief Heatmap::setDataSet
 */
void Heatmap::setDataSet()
{
    this->clearImages();
    for(int i = 0; i < WIN_WIDTH; i++)
        for(int j = 0; j < WIN_HEIGHT; j++)
            if(this->data[i][j] > 0)
                this->drawAlphaImage(i, j, this->data[i][j], false);
    this->refreshColorImage();
}

/**
 * Draws a circle at the given coordinate (x,y) and alpha intensity
 * depending on count parameter.
 *
 * This method is called by setDataSet() if a new maximum is registered,
 * it is called by addDataPoint() if otherwise.
 *
 * @brief Heatmap::drawAlphaImage
 * @param x x-coordinate of the center of the circle
 * @param y y-coordinate of the center of the circle
 * @param count intensity level at coordinate (x,y)
 * @param refresh is true if the image colors need to be refreshed
 */
void Heatmap::drawAlphaImage(int x, int y, int count, bool refresh)
{
    QPainter painter(&(this->alphaImage));
    QRadialGradient grad(x, y, this->radius);

    int intensity = this->opacity * ((float)count / (float)this->max);

    grad.setColorAt(0.0, QColor::fromRgb(0, 0, 0, (int) intensity));
    grad.setColorAt(0.1, QColor::fromRgb(0, 0, 0, (int) (intensity * 0.8)));
    grad.setColorAt(0.5, QColor::fromRgb(0, 0, 0, (int) (intensity * 0.2)));
    grad.setColorAt(1.0, QColor::fromRgb(0, 0, 0, 0));

    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawEllipse(x - this->radius, y - this->radius,
                        this->radius * 2, this->radius * 2);

    update();

    if(refresh)
        this->refreshColorImage();
}

/**
 * Renders the visible QImage colorImage with colors as a representation
 * of intensity level.
 *
 * @brief Heatmap::refreshColoImage
 * @param x
 * @param y
 */
void Heatmap::refreshColorImage()
{
    int alpha, index;
    uint color, alphaMask;
    for(int x = 0; x < WIN_WIDTH; x++)
        for(int y = 0; y < WIN_HEIGHT; y++) {
            alpha = this->alphaImage.pixel(x, y) >> 24;
            if(alpha > 0) {
                index = alpha;
                alpha = alpha * ((float) this->opacity / 255);  // scale back alpha relative to max opacity level
                alphaMask = (alpha << 24) | 16777215u;          // mask to add alpha level for the pixel 16777215 = 0xFFFFFF
                color = this->palette[index].rgb() & alphaMask; // final color value as 0xAARRGGBB unsigned integer
                this->colorImage.setPixel(x, y, color);         // assign calculated color to the pixel
            }
        }
}

void Heatmap::clearImages()
{
    this->alphaImage.fill(QColor(255, 255, 255, 0));
    this->colorImage.fill(QColor(255, 255, 255, 0));
    update();
}

void Heatmap::initColorPalette()
{
    QImage paletteImage(1, 256, QImage::Format_RGB32);
    QPainter painter(&paletteImage);

    QRect rect(0,0,1,256);
    QLinearGradient grad(0, 0, 0, 256);
    grad.setColorAt(0.45, QColor(0,0,255));
    grad.setColorAt(0.55, QColor(0,255,255));
    grad.setColorAt(0.65, QColor(0,255,0));
    grad.setColorAt(0.95, Qt::yellow);
    grad.setColorAt(1.0 , QColor(255,0,0));

    painter.setPen(Qt::NoPen); // very important, otherwise 1px width rectangle is just a black line
    painter.fillRect(rect, grad);
    painter.drawRect(rect);

    for(int i = 0; i < 256; i++)
        this->palette[i] = paletteImage.pixel(0, i);
}
