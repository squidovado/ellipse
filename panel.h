#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

namespace Ui {
class Panel;
}

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);
    ~Panel();

private:
    Ui::Panel *ui;
};

#endif // PANEL_H
