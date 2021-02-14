#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene(this);
    ui->graphicsView->setScene(scene);
    placer = new Placer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

