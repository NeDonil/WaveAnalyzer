//
// Created by nedonil on 22.11.22.
//

#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QStatusBar>
#include <QMenuBar>
#include <QContextMenuEvent>
#include <QFileSystemModel>
#include <QPalette>
#include "MainWindow.h"
#include "WaveProcessor.h"

MainWindow::MainWindow() {
    resize(800, 600);
    setWindowTitle(tr("Wave analyzer"));
    setMinimumSize(160, 160);

    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    Wave wave(values);

    m_WaveViewWidget = new WaveViewWidget(wave);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::darkGray);
    setPalette(pal);

    setCentralWidget(m_WaveViewWidget);
    setContentsMargins(0, 0, 0, 0);
    createActions();
    createMenus();
}

void MainWindow::createActions() {
    m_OpenAct = new QAction("&Open", this);
    m_OpenAct->setStatusTip(tr("Open new file"));

    m_ProcessAct = new QAction("&Process", this);
    m_ProcessAct->setStatusTip(tr("Process current wave"));

    m_ResetAct = new QAction("&Reset", this);
    m_ResetAct->setStatusTip(tr("Reset period"));

    connect(m_OpenAct, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_ProcessAct, &QAction::triggered, this, &MainWindow::processWave);
    connect(m_ResetAct, &QAction::triggered, this, &MainWindow::resetPeriod);
}

void MainWindow::createMenus() {
    m_FileMenu = menuBar()->addMenu(tr("&File"));
    m_FileMenu->addAction(m_OpenAct);

    m_ActionMenu = menuBar()->addMenu(tr("&Action"));
    m_ActionMenu->addAction(m_ProcessAct);
    m_ActionMenu->addAction(m_ResetAct);
}

void MainWindow::openFile(){
    //auto fileinfo = QFileIQFileDialog::getOpenFileName(this, tr("Select wave file"), "assets/", "Wave(*.wave)")nfo();
    auto filename = QFileDialog::getOpenFileName(this, tr("Select wave file"), "assets/", "Wave(*.wave)");
    qDebug() << filename;

    m_WaveViewWidget->readFromFile(filename);
}

void MainWindow::processWave(){
    m_WaveViewWidget->processWave();
}

void MainWindow::resetPeriod(){
    m_WaveViewWidget->resetPeriod();
}

MainWindow::~MainWindow(){
    delete m_WaveViewWidget;
}
