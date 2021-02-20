#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QGraphicsView>
#include "common.h"
#include "graphics.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    line1 = new QGraphicsLineItem();
    line2 = new QGraphicsLineItem();
    ellipse = new QGraphicsEllipseItem();
    QPen pen;
    pen.setCosmetic(true);
    pen.setWidth(2);
    line1->setPen(pen);
    line2->setPen(pen);
    ellipse->setPen(pen);
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
    views().at(0)->fitInView(itemsBoundingRect(), Qt::KeepAspectRatio);
}
