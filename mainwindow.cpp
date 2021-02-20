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
    ui->graphicsView->scale(1.0,-1.0);
    placer = new Placer(this);
    connect(ui->dockWidgetContents, &Panel::newDataAdded, placer, &Placer::processNewData);
    connect(placer, &Placer::line2updated, ui->dockWidgetContents, &Panel::updateLine2);
    connect(placer, &Placer::tpoint2updated, ui->dockWidgetContents, &Panel::updateTPoint2);
    connect(placer, &Placer::elementsUpdated, scene, &Scene::updateScene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

