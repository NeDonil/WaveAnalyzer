//
// Created by nedonil on 22.11.22.
//

#ifndef SPLINE_MAINWINDOW_H
#define SPLINE_MAINWINDOW_H


#include <QMainWindow>
#include "WaveViewWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

    ~MainWindow();

private slots:
    void openFile();
    void processWave();
    void resetPeriod();
private:
    void createActions();
    void createMenus();
    void parseFile(QString&);
private:
    WaveViewWidget* m_WaveViewWidget;
    QAction* m_OpenAct;
    QAction* m_ProcessAct;
    QAction* m_ResetAct;
    QMenu* m_FileMenu;
    QMenu* m_ActionMenu;
};

#endif //SPLINE_MAINWINDOW_H
