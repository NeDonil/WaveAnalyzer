//
// Created by nedonil on 22.11.22.
//

#ifndef SPLINE_WAVEVIEWWIDGET_H
#define SPLINE_WAVEVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include "Wave.h"

struct WaveViewWidgetInfo{
    int startX;
    int startY;

    int periodWidth;
    int lineWidth;
    int widgetWidth;

    int widgetHeight;
    int titleHeight;

    int xOffset;
    int middleYLine;
    int middleYOffset;

    int yScale;

    int count;
    int border;
    int cellSize;
    int betweenAmplitudes;

    double yScaleFactor;

    QColor bgColor;

    QPoint lastMousePos;
};

class WaveViewWidget : public QWidget{
public:
    WaveViewWidget();
    WaveViewWidget(Wave& wave);

    void setWave(const Wave& wave) { m_Wave = wave; recalculateInfo();}
    void readFromFile(QString& filename);
    void processWave();
    void resetPeriod();
protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void wheelEvent(QWheelEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
private:
    void drawBackground(QPainter& painter);
    void drawPeriod(QPainter& painter);
private:
    void initInfo();
    void recalculateInfo();
private:
    Wave m_Wave;
    WaveViewWidgetInfo m_Info;
};

#endif //SPLINE_WAVEVIEWWIDGET_H
