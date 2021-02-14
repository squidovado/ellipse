#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include "graphics.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    line1 = new QGraphicsLineItem(0.0,0.0,100.0,0.0);
    line2 = new QGraphicsLineItem(0.0,0.0,0.0,100.0);
    ellipse = new QGraphicsEllipseItem(0,0,100,100);
    addItem(line1);
    addItem(line2);
    addItem(ellipse);
}
