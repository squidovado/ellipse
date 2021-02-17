#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene(this);
    ui->graphicsView->setScene(scene);
    placer = new Placer(this);
    connect(ui->dockWidgetContents, &Panel::newDataAdded, placer, &Placer::processNewData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

