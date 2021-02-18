#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore/QObject>
#include <QtCore/QPointF>

class InputData;

class Line
{
public:
    Line() : _a(0.0), _b(1.0), _c(0.0), _d(0.0) {}
    Line translated(QPointF) const;
    double a() const {return _a;}
    double b() const {return _b;}
    double c() const {return _c;}
    double d() const {return _d;}
private:
    //x=a+b*t
    //y=c+d*t
    double _a,_b,_c,_d;
};

class Ellipse
{
public:
    Ellipse() : a11(1.0), a12(0.0), a22(4.0), _center(1.0,-0.5) {}
    QRectF boundingRect() const;
    void setCenter(QPointF point) {_center = point;}
    QPointF center() const {return _center;}
private:
    //the ellipse is stored in a coordinate system with the origin in its center
    //so it can be presented as a11*x^2+2*a12*x*y+a22*y^2=1, where a11>0, a22>0, a11*a22-a12^2>0
    double a11, a12, a22;
    QPointF _center;
};

class Placer : public QObject
{
    Q_OBJECT
public:
    Placer(QObject *parent = nullptr);
public slots:
    void processNewData(QSharedPointer<const InputData>);
private:
    bool checkData(QSharedPointer<const InputData>) const;
    void calculate();

    Line line1, line2;
    QPointF tpoint1, tpoint2;
    Ellipse ellipse;
};
#endif // GEOMETRY_H
