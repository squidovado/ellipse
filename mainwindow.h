#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "geometry.h"
#include "graphics.h"
#include "panel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void resizeEvent(QResizeEvent *event) override;
    Ui::MainWindow *ui;
    Scene* scene = nullptr;
    Placer* placer = nullptr;
};
#endif // MAINWINDOW_H
