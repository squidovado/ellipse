#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore/QObject>
#include <QtCore/QPointF>

class InputData;
class GraphicsData;

class Line
{
public:
    Line() : _tpoint(), _b(1.0), _d(0.0) {}
    Line(QPointF point, double B, double D) : _tpoint(point), _b(B), _d(D) {}
    QLineF intersected(const QRectF& rect) const;
    void set(QPointF, double, double);
    double b() const {return _b;}
    double d() const {return _d;}
    QPointF tpoint() const {return _tpoint;}
    double angle() const;
private:
    //x=_tpoint.x()+b*t
    //y=_tpoint.y()+d*t - parametric line equations

    //point of tangency
    QPointF _tpoint;
    double _b,_d;
};

class Ellipse
{
public:
    Ellipse() : _a11(1.0), _a12(0.0), _a22(4.0), _center(1.0,-0.5) {}
    QRectF boundingRect() const; //in canonical coordinates
    qreal angle() const; //between canonical coordinates and center origined coordinates
    void setCenter(QPointF point) {_center = point;}
    void setParameters(double, double, double);
    QPointF center() const {return _center;}
    void rotate(double angle);
private:
    //the ellipse is stored in a coordinate system with the origin in its center
    //so it can be presented as a11*x^2+2*a12*x*y+a22*y^2=1, where a11>0, a22>0, a11*a22-a12^2>0
    double _a11, _a12, _a22;
    QPointF _center;
};

class Placer : public QObject
{
    Q_OBJECT
public:
    Placer(QObject *parent = nullptr);
public slots:
    void processNewData(QSharedPointer<const InputData>);
signals:
    void elementsUpdated(QSharedPointer<const GraphicsData>) const;
    void line2updated(double, double, double);
private:
    bool checkData(QSharedPointer<const InputData>);
    bool calculate();
    void createOutput() const;
    void errorMessage(quint8) const;

    bool _update2ndTangent;
    Line line1, line2;
    Ellipse ellipse;
};
#endif // GEOMETRY_H
