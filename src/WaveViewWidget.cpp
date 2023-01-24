//
// Created by nedonil on 22.11.22.
//

#include <QFileDialog>
#include <QMessageBox>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <algorithm>

#include "MainWindow.h"
#include "WaveViewWidget.h"
#include "WaveProcessor.h"

WaveViewWidget::WaveViewWidget(){
    initInfo();
}

WaveViewWidget::WaveViewWidget(Wave& wave)
{
    m_Wave = wave;
    initInfo();
    recalculateInfo();
}

void WaveViewWidget::initInfo(){
    m_Info.titleHeight = 31;
    m_Info.lineWidth = 2;
    m_Info.border = 50;
    m_Info.bgColor = QColor(0, 0, 0, 100);
    m_Info.yScaleFactor = 0.45;
    m_Info.xOffset = 0;
    m_Info.betweenAmplitudes = 5 * 2;
    m_Info.totalWaveWidth = m_Wave.getAmpCount() * m_Info.betweenAmplitudes;
}

void WaveViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    drawBackground(painter);


    if(m_Wave.getPeriod() > 1){
        drawPeriod(painter);
    }

    painter.setPen(QPen(Qt::red, m_Info.lineWidth, Qt::SolidLine, Qt::RoundCap));
    int ampCount = m_Wave.getAmpCount();

    for(int i = 0; i < ampCount; i++){
        auto yValue = m_Wave.getAmp(i) * m_Info.yScale;
        painter.drawLine(m_Info.startX + i * m_Info.betweenAmplitudes, m_Info.middleYLine + m_Info.middleYOffset - yValue,
                         m_Info.startX + i * m_Info.betweenAmplitudes, m_Info.middleYLine + m_Info.middleYOffset + yValue);
    }
}

void WaveViewWidget::resizeEvent(QResizeEvent *event){
    auto resizeOffset =  event->size() - ((MainWindow*)parent())->m_LastSize;
    ((MainWindow*)parent())->m_LastSize = event->size();

    m_Info.widgetWidth += resizeOffset.width();
    m_Info.widgetHeight += resizeOffset.height();

    recalculateInfo();
}

void WaveViewWidget::wheelEvent(QWheelEvent *event){
    QPoint numDegrees = event->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;

    m_Info.yScaleFactor += numSteps.y() * 0.009;
    m_Info.yScaleFactor = std::max(m_Info.yScaleFactor, 0.1);
    m_Info.totalWaveWidth = m_Wave.getAmpCount() * m_Info.betweenAmplitudes;

    if(m_Info.totalWaveWidth > m_Info.widgetWidth){
        int allowOffset = m_Info.widgetWidth - m_Info.totalWaveWidth - m_Info.betweenAmplitudes;
        m_Info.xOffset = std::max(m_Info.xOffset, allowOffset);
    }

    recalculateInfo();
    repaint();
}

void WaveViewWidget::mousePressEvent(QMouseEvent * event){
    m_Info.lastMousePos = event->pos();
}

void WaveViewWidget::mouseMoveEvent(QMouseEvent * event){
    if(m_Info.totalWaveWidth < m_Info.widgetWidth)
        return;

    int allowOffset = m_Info.widgetWidth - m_Info.totalWaveWidth - m_Info.betweenAmplitudes;
    auto offset = m_Info.lastMousePos - event->pos();

    m_Info.xOffset -= offset.x();
    m_Info.xOffset = std::min(m_Info.xOffset, 0);
    m_Info.xOffset = std::max(m_Info.xOffset, allowOffset);

    m_Info.lastMousePos = event->pos();

    if(offset.x() != 0){
        recalculateInfo();
        repaint();
    }

}

void WaveViewWidget::drawPeriod(QPainter &painter) {

    int period = m_Wave.getPeriod();

    m_Info.periodWidth = (m_Info.betweenAmplitudes) * (period);
    painter.fillRect(m_Info.startX - m_Info.xOffset, m_Info.startY - m_Info.titleHeight,
                     m_Info.periodWidth + m_Info.xOffset, m_Info.startY + m_Info.widgetHeight,
                     m_Info.bgColor);

    painter.setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap));

    for(int i = 1; i <= m_Info.count; i++){
        int startXValue = m_Info.startX + (m_Info.betweenAmplitudes) * i * (period);

        painter.drawLine(startXValue, m_Info.startY - m_Info.titleHeight,
                         startXValue, m_Info.startY + m_Info.widgetHeight);
    }
}

void WaveViewWidget::drawBackground(QPainter& painter) {

    painter.setPen(QPen(Qt::gray, m_Info.lineWidth, Qt::DashLine, Qt::RoundCap));
    painter.drawLine(m_Info.startX - m_Info.xOffset, m_Info.middleYLine + m_Info.middleYOffset,
                     m_Info.widgetWidth, m_Info.middleYLine + m_Info.middleYOffset);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));

    int widthCellCount = m_Info.totalWaveWidth / m_Info.cellSize;
    int defaultCellCount = m_Info.widgetWidth / m_Info.cellSize;

    widthCellCount = std::max(widthCellCount, defaultCellCount);

    for(int i = 0; i <= widthCellCount; i++){
        painter.drawLine(m_Info.startX + i * m_Info.cellSize, m_Info.startY - m_Info.titleHeight,
                         m_Info.startX + i * m_Info.cellSize, m_Info.widgetHeight);
    }

    for(int i = 0; i <= m_Info.widgetHeight / m_Info.cellSize; i++){
        painter.drawLine(m_Info.startX , m_Info.startY + i * m_Info.cellSize - m_Info.titleHeight,
                         m_Info.widgetWidth, m_Info.startY + i * m_Info.cellSize - m_Info.titleHeight);
    }
}


void WaveViewWidget::processWave() {
    auto period = WaveProcessor::processWave(m_Wave);

    if(period.size() == 0){
        QMessageBox::information(this, "Information", "Wave has no period");
        return;
    }

    m_Wave.setPeriod(period.size());
    m_Info.periodWidth = (period.size() - 1) * m_Info.betweenAmplitudes;
    recalculateInfo();
    repaint();
}

void WaveViewWidget::resetPeriod() {
    m_Wave.setPeriod(1);
    recalculateInfo();
    repaint();
}

void WaveViewWidget::readFromFile(QString &filename){
    if(!m_Wave.fromFile(filename)){
        QMessageBox::critical(this, "File Open Error", "Could not to read/parse file: " + filename);
        return;
    }

    m_Info.totalWaveWidth = m_Wave.getAmpCount() * m_Info.betweenAmplitudes;
    m_Wave.setPeriod(1);
    recalculateInfo();
}

void WaveViewWidget::recalculateInfo(){
    m_Info.startX = frameGeometry().x() + m_Info.xOffset;
    m_Info.startY = frameGeometry().y();
    m_Info.widgetWidth = frameGeometry().width();
    m_Info.widgetHeight = frameGeometry().height();

    int period = m_Wave.getPeriod();
    int ampCount = m_Wave.getAmpCount();
    auto wBegin = m_Wave.cbegin();
    auto wEnd = m_Wave.cend();

    double scale = (m_Info.yScaleFactor + 0.55);

    m_Info.middleYLine = m_Info.startY + (m_Info.widgetHeight - m_Info.border) / 2 - m_Info.titleHeight;
    m_Info.cellSize = (m_Info.middleYLine - m_Info.startY) * scale / 10;
    m_Info.middleYOffset = (ceil((double)m_Info.middleYLine / m_Info.cellSize) * m_Info.cellSize) - m_Info.middleYLine;
    m_Info.count = ampCount / period;
    m_Info.betweenAmplitudes = 5 * 2 * scale;
    m_Info.periodWidth = (m_Info.betweenAmplitudes) * (period) * scale;
    m_Info.yScale = (m_Info.widgetHeight * m_Info.yScaleFactor) / * std::max_element(wBegin, wEnd);
}
