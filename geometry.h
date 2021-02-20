#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore/QObject>
#include <QtCore/QPointF>

class InputData;
class GraphicsData;

class Line
{
public:
    Line() : _a(0.0), _b(1.0), _c(0.0), _d(0.0) {}
    Line(double A, double B, double C, double D) : _a(A), _b(B), _c(C), _d(D) {}
    Line translated(QPointF) const;
    QLineF intersected(const QRectF& rect) const;
    void set(double, double, double, double);
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
    Ellipse() : _a11(1.0), _a12(0.0), _a22(4.0), _center(1.0,-0.5) {}
    QRectF boundingRect() const; //in canonical coordinates
    qreal angle() const; //between canonical coordinates and center origined coordinates
    void setCenter(QPointF point) {_center = point;}
    void setParameters(double, double, double);
    QPointF center() const {return _center;}
    double a11() const {return _a11;}
    double a12() const {return _a12;}
    double a22() const {return _a22;}
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
    void line2updated(double, double, double);
    void tpoint2updated(double, double);
    void elementsUpdated(QSharedPointer<const GraphicsData>) const;
private:
    bool checkData(QSharedPointer<const InputData>) const;
    bool calculateTwoLinesOnePoint();
    bool calculateTwoPointsOneLine();
    void calculateScene() const;
    void errorMessage(quint8) const;

    Line line1, line2;
    QPointF tpoint1, tpoint2;
    Ellipse ellipse;
};
#endif // GEOMETRY_H
