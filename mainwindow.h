#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Scene;
class Placer;

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
    Ui::MainWindow *ui;
    Scene* scene = nullptr;
    Placer* placer = nullptr;
};
#endif // MAINWINDOW_H
