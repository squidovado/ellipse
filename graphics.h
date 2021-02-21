#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

class GraphicsData;
class QLabel;

class View: public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget* parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
public slots:
    void updateView();
    void clear();
private:
    QLabel *topleft, *topright, *bottomleft, *bottomright;
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject *parent = nullptr);
public slots:
    void updateScene(QSharedPointer<const GraphicsData>);
    void clear();
signals:
    void sceneUpdated();
private:
    QGraphicsEllipseItem* ellipse = nullptr;
    QGraphicsLineItem *line1 = nullptr, *line2 = nullptr;
};

#endif // GRAPHICS_H
