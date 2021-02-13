#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtWidgets/QGraphicsScene>

class QChart;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);
private:
    QChart* chart = nullptr;
    QGraphicsEllipseItem* ellipse = nullptr;
    QGraphicsLineItem *line1 = nullptr, *line2 = nullptr;
};

#endif // GRAPHICS_H
