#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore/QObject>
#include <QtCore/QPointF>

class InputData;
enum ChangeableObject
{
    FirstLine, SecondLine, FirstPointOfTangency, SecondPointOfTangency
};

class Line
{
public:
    Line(double A, double B, double C, double D) : _a(A), _b(B), _c(C), _d(D) {}
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
    Ellipse(QPointF center) : _center(center){}
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
    void processNewData(QSharedPointer<InputData>);
private:
    void calculate();
    void calculateNewPoint(const Line&, const Line&, const QPointF&, QPointF&);
    void calculateNewLine(const QPointF&, const QPointF&, const Line&, Line&);

    //The ellipse is totally defined with its center and three out of four Changeable Objects.
    //The fourth Object must be calculated. The program chooses the most early changed by user Object to calculate it.
    //The earliest changed object is in the front.
    QVector<ChangeableObject> changeOrder;
    Line line1, line2;
    QPointF tpoint1, tpoint2;
    Ellipse ellipse;
};
#endif // GEOMETRY_H
