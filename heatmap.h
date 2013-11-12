#ifndef HEATMAP_H
#define HEATMAP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include "udpadapter.h"

#define MODE_OPENGAZER 0
#define MODE_MOUSE 1

#define WIN_WIDTH 1280
#define WIN_HEIGHT 800
#define RADIUS 100
#define OPACITY 150

/**
 * @brief The Heatmap class
 */
class Heatmap : public QWidget
{
    Q_OBJECT

public:
    explicit Heatmap(int mode, QWidget *parent = 0);

public slots:
    void setCoord(int x, int y);

protected:
    void paintEvent(QPaintEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    UDPAdapter udp;

    QImage colorImage;
    QImage alphaImage;

    // Bidimensional array [x][y]
    // counts number of passes on coordinate (x,y)
    int data[WIN_WIDTH][WIN_HEIGHT];

    // Holds 256 levels of color gradient spectrum
    // from cold colors to warm colors
    QColor palette[256];

    int width; // image width
    int height; // image height
    int radius; // unit circle radius
    int max; // current maximum count
    int opacity; // maximum value for alpha

    // Coordinates update
    void updateCoords(int x, int y);

    // Draw methods
    void addDataPoint(int x, int y);
    void setDataSet();
    void drawAlphaImage(int x, int y, int count, bool refresh);
    void refreshColorImage();
    void clearImages();

    void initColorPalette();

signals:
    void mouse_pos();
};

#endif // HEATMAP_H
