#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class InputData;

namespace Ui {
class Panel;
}

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = nullptr);
    ~Panel();
signals:
    void newDataAdded(QSharedPointer<const InputData> data) const;
private slots:
    void comboBoxIndexChanged(int index);
    void calculate() const;
private:
    Ui::Panel *ui;
};

#endif // PANEL_H
