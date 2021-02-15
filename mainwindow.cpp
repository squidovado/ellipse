#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLineEdit* LEarr[12] = {ui->xcenter, ui->ycenter,
                            ui->tangent1a, ui->tangent1b, ui->tangent1c,
                            ui->tangent2a, ui->tangent2b, ui->tangent2c,
                            ui->point1x, ui->point1y,
                            ui->point2x, ui->point2y};
    new Panel(LEarr);
    scene = new Scene(this);
    ui->graphicsView->setScene(scene);
    placer = new Placer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

