#include "heatmap.h"

/**
 * Main constructor
 *
 * @brief Heatmap::Heatmap
 * @param parent
 */
Heatmap::Heatmap(QWidget *parent) :
    QWidget(parent),
    udp(this),
    colorImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_ARGB32),
    alphaImage(WIN_WIDTH, WIN_HEIGHT, QImage::Format_ARGB32),
    data{},
    width(WIN_WIDTH),
    height(WIN_HEIGHT),
    max(1),
    opacity(150),
    radius(RADIUS)
{
    // Append image to top left corner
    setAttribute(Qt::WA_StaticContents);

    // Create color spectrum palette
    initColorPalette();

    // Reset visible background color
    this->clearImages();

    // Connect coordinate update signal to the slot
//    connect(&udp, SIGNAL(coordChanged(int,int)), this, SLOT(setCoord(int,int)));

    // Start mouse tracking
//    this->setMouseTracking(true);

    // resize widget to fullscreen
    resize(WIN_WIDTH, WIN_HEIGHT);
}

/**
 * Slot to receive new coordinates and update the heatmap
 *
 * @brief Heatmap::setCoord
 * @param x
 * @param y
 */
void Heatmap::setCoord(int x, int y)
{
    this->addDataPoint((x<WIN_WIDTH)?x:(WIN_WIDTH-1),
                       (y<WIN_HEIGHT)?y:(WIN_HEIGHT-1));
}

void Heatmap::updateCoords(int x, int y)
{
    this->addDataPoint((x<WIN_WIDTH)?x:(WIN_WIDTH-1),
                       (y<WIN_HEIGHT)?y:(WIN_HEIGHT-1));
}

void Heatmap::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QRect dirtyRect = ev->rect();
    painter.drawImage(dirtyRect.topLeft(), this->colorImage, dirtyRect);
}

void Heatmap::mouseMoveEvent(QMouseEvent *ev)
{
//    this->addDataPoint(ev->x(), ev->y());
//    emit Mouse_Pos();
}

void Heatmap::mousePressEvent(QMouseEvent *ev)
{

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
        this->setDataSet(temp);
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
 * @param max
 */
void Heatmap::setDataSet(int max)
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
    QPainter painter(&this->alphaImage);
    QRadialGradient grad(x, y, this->radius);

    int intensity = this->opacity * ((float)count / (float)this->max);

    grad.setColorAt(0.0, QColor::fromRgb(0, 0, 0, (int) intensity));
    grad.setColorAt(0.3, QColor::fromRgb(0, 0, 0, (int) (intensity * 0.7)));
    grad.setColorAt(0.6, QColor::fromRgb(0, 0, 0, (int) (intensity * 0.2)));
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
            alpha = this->getAlphaLevel(x, y);
            if(alpha > 0) {
                index = alpha; //((float)alpha / this->opacity) * 255; // mapping alpha range to color spectrum
                alphaMask = (alpha << 24) | 16777215u; // mask to add alpha level for the pixel 16777215 = 0xFFFFFF
                color = this->palette[index].rgb() & alphaMask; // final color value as 0xAARRGGBB unsigned integer
//                qDebug() << "alphaImage(" << x << "," << y<<"):"<< QString::number(alphaImage.pixel(x,y), 16);
//                qDebug() << "     color(" << x << "," << y<<"):"<< QString::number(t, 16);
                this->colorImage.setPixel(x, y, color);
            }
        }
}

void Heatmap::clearImages()
{
    this->alphaImage.fill(QColor(255, 255, 255, 0)); //todo: aggiungere alpha channel?
    this->colorImage.fill(QColor(255, 255, 255, 0)); //todo: aggiungere alpha channel?
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

int Heatmap::getAlphaLevel(int x, int y)
{
    return this->alphaImage.pixel(x, y) >> 24;
}



