#include "graphics.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(Qt::blue);
}
