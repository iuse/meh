#ifndef HEATMAP_H
#define HEATMAP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QDebug>

#include "macros.h"
#include "udpadapter.h"

/**
 * @brief The Heatmap class
 */
class Heatmap : public QWidget
{
    Q_OBJECT

public:
    explicit Heatmap(UDPAdapter *udpAdapter, QTimer *qTimer, QWidget *parent = 0);

public slots:
    void timeout();
    void openGazerCoordChanged(int x, int y);
    void mouseCoordChanged(int x, int y);

protected:
    void paintEvent(QPaintEvent *ev);

private:
    QWidget *parent;
    UDPAdapter *udp;
    QTimer *timer;

    int x;
    int y;
    int oldX;
    int oldY;

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

    // Draw methods
    void addDataPoint(int x, int y);
    void setDataSet();
    void drawAlphaImage(int x, int y, int count, bool refresh);
    void refreshColorImage();
    void clearImages();

    void initColorPalette();
};

#endif // HEATMAP_H
