//
// Created by nedonil on 22.11.22.
//

#include <QFileDialog>
#include <QMessageBox>
#include "WaveViewWidget.h"
#include "WaveProcessor.h"

void WaveViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    drawPeriod(painter);
    drawBackground(painter);

    painter.setPen(QPen(Qt::red, m_Info.lineWidth, Qt::SolidLine, Qt::RoundCap));
    int ampCount = m_Wave.getAmpCount();

    for(int i = 0; i < ampCount; i++){
        auto yValue = m_Wave.getAmp(i) * m_Info.yScale;
        painter.drawLine(m_Info.startX + i * m_Info.betweenAmplitudes, m_Info.middleYLine + m_Info.middleYOffset - yValue,
                         m_Info.startX + i * m_Info.betweenAmplitudes, m_Info.middleYLine + m_Info.middleYOffset + yValue);
    }
}

void WaveViewWidget::resizeEvent(QResizeEvent *event){
    recalculateInfo();
}

void WaveViewWidget::drawPeriod(QPainter &painter) {

    painter.setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap));
    painter.fillRect(m_Info.startX, m_Info.startY - m_Info.titleHeight,
                     m_Info.periodWidth, m_Info.startY + m_Info.widgetHeight, m_Info.bgColor);

    int period = m_Wave.getPeriod();
    for(int i = 1; i <= m_Info.count; i++){
        int startXValue = m_Info.startX + (m_Info.betweenAmplitudes) * i * (period);

        painter.drawLine(startXValue, m_Info.startY - m_Info.titleHeight,
                         startXValue, m_Info.startY + m_Info.widgetHeight);
    }
}

void WaveViewWidget::drawBackground(QPainter& painter) {

    painter.setPen(QPen(Qt::gray, m_Info.lineWidth, Qt::DashLine, Qt::RoundCap));
    painter.drawLine(frameGeometry().x(), m_Info.middleYLine + m_Info.middleYOffset,
                     m_Info.widgetWidth, m_Info.middleYLine + m_Info.middleYOffset);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    for(int i = 0; i <= m_Info.widgetWidth / m_Info.cellSize; i++){
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

    m_Info.periodWidth = (period.size() - 1) * m_Info.betweenAmplitudes;
    repaint();
}

void WaveViewWidget::resetPeriod() {
    m_Info.periodWidth = m_Wave.getAmpCount();
    repaint();
}

void WaveViewWidget::readFromFile(QString &filename){
    m_Wave.fromFile(filename);
    recalculateInfo();
}

void WaveViewWidget::recalculateInfo(){
    m_Info.titleHeight = 31;
    m_Info.lineWidth = 2;
    m_Info.border = 50;
    m_Info.bgColor = QColor(0, 0, 0, 100);

    m_Info.startX = frameGeometry().x();
    m_Info.startY = frameGeometry().y();
    m_Info.widgetWidth = frameGeometry().width();
    m_Info.widgetHeight = frameGeometry().height();

    int period = m_Wave.getPeriod();
    int ampCount = m_Wave.getAmpCount();
    auto wBegin = m_Wave.cbegin();
    auto wEnd = m_Wave.cend();

    m_Info.middleYLine = m_Info.startY + (m_Info.widgetHeight - m_Info.border) / 2 - m_Info.titleHeight;
    m_Info.cellSize = (m_Info.middleYLine - m_Info.startY) / 10;
    m_Info.middleYOffset = (ceil((double)m_Info.middleYLine / m_Info.cellSize) * m_Info.cellSize) - m_Info.middleYLine;
    m_Info.count = ampCount / period;
    m_Info.betweenAmplitudes = 5 * 2;
    m_Info.periodWidth = (m_Info.betweenAmplitudes) * (period);
    m_Info.yScale = (m_Info.widgetHeight * 0.45) / * std::max_element(wBegin, wEnd);
}