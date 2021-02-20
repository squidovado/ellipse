#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include "common.h"
#include "graphics.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    line1 = new QGraphicsLineItem(0.0,0.0,100.0,0.0);
    line2 = new QGraphicsLineItem(0.0,0.0,0.0,100.0);
    ellipse = new QGraphicsEllipseItem(0,0,100,100);
    //ellipse->setRotation(180);
    addItem(line1);
    addItem(line2);
    addItem(ellipse);
}

void Scene::updateScene(QSharedPointer<const GraphicsData> data)
{
    line1->setLine(data->line1);
    line2->setLine(data->line2);
    ellipse->setRect(data->ellipseRect);
    ellipse->setPos(data->ellipseCenter);
    ellipse->setRotation(data->angle);
}
