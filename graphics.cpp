#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QLabel>
#include "common.h"
#include "graphics.h"


View::View(QWidget* parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff ) ;
    setEnabled(false);
    setRenderHint(QPainter::Antialiasing);
    scale(1.0,-1.0);
    topleft = new QLabel(this);
    topright = new QLabel(this);
    bottomleft = new QLabel(this);
    bottomright = new QLabel(this);
    topleft->setStyleSheet("color: black");
    topright->setStyleSheet("color: black");
    bottomleft->setStyleSheet("color: black");
    bottomright->setStyleSheet("color: black");
}

void View::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateView();
}

void View::updateView()
{
    if(isEqual(sceneRect().height(),0.0))
        return;
    fitInView(scene()->itemsBoundingRect());
    topleft->setText("("+QString::number(mapToScene(0,0).x(),'f',2)+", "+QString::number(mapToScene(0,0).y(),'f',2)+")");
    topleft->setGeometry(0,0,topleft->sizeHint().width(),topleft->sizeHint().height());
    topright->setText("("+QString::number(mapToScene(width(),0).x(),'f',2)+", "+QString::number(mapToScene(width(),0).y(),'f',2)+")");
    topright->setGeometry(width()-topright->sizeHint().width(),0,topright->sizeHint().width(),topright->sizeHint().height());
    bottomleft->setText("("+QString::number(mapToScene(0,height()).x(),'f',2)+", "+QString::number(mapToScene(0,height()).y(),'f',2)+")");
    bottomleft->setGeometry(0,height()-bottomleft->height(),bottomleft->sizeHint().width(),bottomleft->sizeHint().height());
    bottomright->setText("("+QString::number(mapToScene(width(),height()).x(),'f',2)+", "+QString::number(mapToScene(width(),height()).y(),'f',2)+")");
    bottomright->setGeometry(width()-bottomright->sizeHint().width(),height()-bottomright->height(),bottomright->sizeHint().width(),bottomright->sizeHint().height());
}

void View::clear()
{
    topleft->setText("");
    topright->setText("");
    bottomleft->setText("");
    bottomright->setText("");
}

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

void Scene::clear()
{
    line1->setLine(QLine());
    line2->setLine(QLine());
    ellipse->setRect(QRect());
}

void Scene::updateScene(QSharedPointer<const GraphicsData> data)
{
    line1->setLine(data->line1);
    line2->setLine(data->line2);
    ellipse->setRect(data->ellipseRect);
    ellipse->setPos(data->ellipseCenter);
    ellipse->setRotation(data->angle);
    emit sceneUpdated();
}
