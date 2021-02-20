#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtWidgets/QGraphicsScene>

class GraphicsData;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);
public slots:
    void updateScene(QSharedPointer<const GraphicsData>);
private:
    QGraphicsEllipseItem* ellipse = nullptr;
    QGraphicsLineItem *line1 = nullptr, *line2 = nullptr;
};

#endif // GRAPHICS_H
