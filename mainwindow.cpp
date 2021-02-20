#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff ) ;
    ui->graphicsView->setEnabled(false);


    scene = new Scene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scale(1.0,-1.0);
    placer = new Placer(this);
    connect(ui->panel, &Panel::newDataAdded, placer, &Placer::processNewData);
    connect(placer, &Placer::line2updated, ui->panel, &Panel::updateLine2);
    connect(placer, &Placer::tpoint2updated, ui->panel, &Panel::updateTPoint2);
    connect(placer, &Placer::elementsUpdated, scene, &Scene::updateScene);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

