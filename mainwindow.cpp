#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene(this);
    ui->graphicsView->setScene(scene);
    placer = new Placer(this);
    connect(ui->panel, &Panel::newDataAdded, placer, &Placer::processNewData);
    connect(placer, &Placer::line2updated, ui->panel, &Panel::updateLine2);
    connect(placer, &Placer::tpoint2updated, ui->panel, &Panel::updateTPoint2);
    connect(placer, &Placer::elementsUpdated, scene, &Scene::updateScene);
    connect(scene, &Scene::sceneUpdated, ui->graphicsView, &View::updateView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

