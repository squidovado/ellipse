#define _USE_MATH_DEFINES
#include <cmath>
#include <QSharedPointer>
#include <QMessageBox>
#include "geometry.h"
#include "common.h"

bool solve3(const double (&matrix)[3][3], const double (&rhs)[3], double (&solution)[3])
{
    //Cramer's rule
    double det = matrix[0][0]*matrix[1][1]*matrix[2][2]+matrix[0][1]*matrix[1][2]*matrix[2][0]+
            matrix[0][2]*matrix[1][0]*matrix[2][1]-matrix[0][0]*matrix[1][2]*matrix[2][1]-
            matrix[0][1]*matrix[1][0]*matrix[2][2]-matrix[0][2]*matrix[1][1]*matrix[2][0];
    if (isEqual(det,0.0))
        return false;
    double det1 = rhs[0]*matrix[1][1]*matrix[2][2]+matrix[0][1]*matrix[1][2]*rhs[2]+
            matrix[0][2]*rhs[1]*matrix[2][1]-rhs[0]*matrix[1][2]*matrix[2][1]-
            matrix[0][1]*rhs[1]*matrix[2][2]-matrix[0][2]*matrix[1][1]*rhs[2];
    double det2 = matrix[0][0]*rhs[1]*matrix[2][2]+rhs[0]*matrix[1][2]*matrix[2][0]+
            matrix[0][2]*matrix[1][0]*rhs[2]-matrix[0][0]*matrix[1][2]*rhs[2]-
            rhs[0]*matrix[1][0]*matrix[2][2]-matrix[0][2]*rhs[1]*matrix[2][0];
    double det3 = matrix[0][0]*matrix[1][1]*rhs[2]+matrix[0][1]*rhs[1]*matrix[2][0]+
            rhs[0]*matrix[1][0]*matrix[2][1]-matrix[0][0]*rhs[1]*matrix[2][1]-
            matrix[0][1]*matrix[1][0]*rhs[2]-rhs[0]*matrix[1][1]*matrix[2][0];
    solution[0]=det1/det;
    solution[1]=det2/det;
    solution[2]=det3/det;
    return true;
}

double Line::angle() const
{
    return isEqual(_b,0.0) ? M_PI/2.0 : atan(_d/_b);
}

void Line::set(QPointF point, double B, double D)
{
    _tpoint = point;
    _b = B;
    _d = D;
}

QLineF Line::intersected(const QRectF& rect) const
{
    double t,x,y;
    QVector<QPointF> points;
    if(!isEqual(_b,0.0))
    {
        t = (rect.left()-_tpoint.x())/_b;
        y = _tpoint.y() + t * _d;
        if (y>rect.top() && y<rect.bottom())
            points.push_back(QPointF(rect.left(),y));
        t = (rect.right()-_tpoint.x())/_b;
        y = _tpoint.y() + t * _d;
        if (y>rect.top() && y<rect.bottom())
            points.push_back(QPointF(rect.right(),y));
    }
    if(!isEqual(_d,0.0))
    {
        t = (rect.top()-_tpoint.y())/_d;
        x = _tpoint.x() + t * _b;
        if (x>rect.left() && x<rect.right())
            points.push_back(QPointF(x,rect.top()));
        t = (rect.bottom()-_tpoint.y())/_d;
        x = _tpoint.x() + t * _b;
        if (x>rect.left() && x<rect.right())
            points.push_back(QPointF(x,rect.bottom()));
    }
    if (points.size() == 2)
        return QLineF(points[0], points[1]);
    else
        return QLineF();
}

void Ellipse::setParameters(double A11, double A12, double A22)
{
    _a11 = A11;
    _a12 = A12;
    _a22 = A22;
}

qreal Ellipse::angle() const
{
    return isEqual(_a11,_a22) ? M_PI/4.0 : atan(2*_a12/(_a22-_a11))/2.0;
}

QRectF Ellipse::boundingRect() const
{
    double angle_ = angle();
    double sin_ = sin(angle_);
    double cos_ = cos(angle_);
    double horizontalSemiAxes = 1.0/sqrt(_a11*cos_*cos_-2.0*_a12*sin_*cos_+_a22*sin_*sin_);
    double verticalSemiAxes = 1.0/sqrt(_a11*sin_*sin_+2.0*_a12*sin_*cos_+_a22*cos_*cos_);
    return QRectF(-horizontalSemiAxes,-verticalSemiAxes,2*horizontalSemiAxes,2*verticalSemiAxes);
}

void Ellipse::rotate(double angle)
{
    double sin_sin = sin(angle)*sin(angle);
    double sin_cos = sin(angle)*cos(angle);
    double cos_cos = cos(angle)*cos(angle);
    double a11 = _a11*cos_cos-2*_a12*sin_cos+_a22*sin_sin;
    double a12 = (_a11-_a22)*sin_cos+_a12*(cos_cos-sin_sin);
    double a22 = _a11*sin_sin+2*_a12*sin_cos+_a22*cos_cos;
    setParameters(a11, a12, a22);
}

Placer::Placer(QObject *parent) : QObject(parent), line1(), line2(), ellipse() {}

void Placer::processNewData(QSharedPointer<const InputData> data)
{
    _update2ndTangent = false;
    if (!checkData(data))
        return;
    ellipse.setCenter(QPointF(data->xcenter, data->ycenter));
    line1.set(QPointF(data->point1x,data->point1y),data->tangent1b,-data->tangent1a);
    line2.set(QPointF(data->point2x,data->point2y),data->tangent2b,-data->tangent2a);
    if(!calculate())
        return;
    createOutput();
}

bool Placer::checkData(QSharedPointer<const InputData> data)
{
    if (isEqual(data->tangent1a,0) && isEqual(data->tangent1b,0))
    {
        errorMessage(0);
        return false;
    }
    if (isEqual(data->tangent2a,0) && isEqual(data->tangent2b,0))
    {
        errorMessage(1);
        return false;
    }
    if (isEqual(data->tangent1a*data->xcenter + data->tangent1b*data->ycenter, data->tangent1c))
    {
        errorMessage(2);
        return false;
    }
    if (isEqual(data->tangent2a*data->xcenter + data->tangent2b*data->ycenter, data->tangent2c))
    {
        errorMessage(3);
        return false;
    }
    if(!isEqual(data->tangent1a*data->point1x + data->tangent1b*data->point1y, data->tangent1c))
    {
        errorMessage(4);
        return false;
    }
    if(!isEqual(data->tangent2a*data->point2x + data->tangent2b*data->point2y, data->tangent2c))
    {
        QMessageBox::StandardButton button = QMessageBox::question(nullptr, "Bad input",
                                    "Second tangent must pass through second point of tangency. "
                                    "Do you want 2nd tangent updated?",QMessageBox::Yes | QMessageBox::No,	QMessageBox::Yes);
        if (button == QMessageBox::No)
            return false;
        _update2ndTangent = true;
    }
    if (isEqual(data->tangent1a*data->tangent2b,data->tangent2a*data->tangent1b) &&
        isEqual(data->tangent1a*data->tangent2c,data->tangent2a*data->tangent1c) &&
        isEqual(data->tangent1b*data->tangent2c,data->tangent2b*data->tangent1c))
    {
        errorMessage(6);
        return false;
    }
    if(isEqual(data->tangent2a*data->point1x + data->tangent2b*data->point1y, data->tangent2c))
    {
        errorMessage(7);
        return false;
    }
    if(isEqual(data->tangent1a*data->point2x + data->tangent1b*data->point2y, data->tangent1c))
    {
        errorMessage(8);
        return false;
    }
    return true;
}

bool Placer::calculate()
{
    //local coord system with the origin at the center of the ellipse
    QPointF origin = ellipse.center();
    QPointF p1local = line1.tpoint() - origin;
    QPointF p2local = line2.tpoint() - origin;
    QPointF middle = (p1local + p2local)/2.0;
    if(isEqual(middle.x(),0.0) && isEqual(middle.y(),0.0) && isEqual(line1.b()*line2.d(),line2.b()*line1.d()))
    {
        double angle = line1.angle();
        QTransform transform;
        transform.rotateRadians(-angle);
        QPointF p1rotated = transform.map(p1local);
        ellipse.setParameters(1.0,-p1rotated.x()/p1rotated.y(),(p1rotated.x()*p1rotated.x()+1.0)/p1rotated.y()/p1rotated.y());
        ellipse.rotate(angle);
        QMessageBox::warning(nullptr, "Infinite solutions", "There is infinity of solutions. One of them is shown.");
        return true;
    }
    double matrix[3][3], rhs[3] = {1., 1., 0.};
    matrix[0][0] = p1local.x()*p1local.x();
    matrix[0][1] = 2*p1local.x()*p1local.y();
    matrix[0][2] = p1local.y()*p1local.y();
    matrix[1][0] = p2local.x()*p2local.x();
    matrix[1][1] = 2*p2local.x()*p2local.y();
    matrix[1][2] = p2local.y()*p2local.y();
    matrix[2][0] = p1local.x()*line1.b();
    matrix[2][1] = p1local.x()*line1.d()+p1local.y()*line1.b();
    matrix[2][2] = p1local.y()*line1.d();
    double solution[3];

    if (!solve3(matrix, rhs, solution) || solution[0]<=0.0 || solution[2]<=0.0 || solution[0]*solution[2]-solution[1]*solution[1]<=0.0)
    {
        errorMessage(9);
        return false;
    }
    if (!isEqual(solution[0]*p2local.x()*line2.b()+solution[1]*(p2local.x()*line2.d()+p2local.y()*line2.b())
                 +solution[2]*p2local.y()*line2.d(),0.0) || _update2ndTangent)
    {
        double A = solution[0]*p2local.x()+solution[1]*p2local.y();
        double B = solution[1]*p2local.x()+solution[2]*p2local.y();
        double C = A*line2.tpoint().x()+B*line2.tpoint().y();
        if(!_update2ndTangent)
        {
            QMessageBox::StandardButton button = QMessageBox::question(nullptr, "Bad input",
                              "Excess data. Solution can't be found. Check input for correctness. "
                              "Change 2nd tangent to<br>"+QString::number(A) +
                              " * x + " + QString::number(B) + " * y = " + QString::number(C)+" ?",QMessageBox::Yes | QMessageBox::No,	QMessageBox::Yes);
            if (button == QMessageBox::No)
                return false;
        }
        line2.set(line2.tpoint(),B,-A);
        emit line2updated(A, B, C);
    }
    ellipse.setParameters(solution[0], solution[1], solution[2]);
    return true;
}

void Placer::createOutput() const
{
    QSharedPointer<GraphicsData> data = QSharedPointer<GraphicsData>::create();
    data->angle = -ellipse.angle()*180/M_PI;
    data->ellipseCenter = ellipse.center();
    QRectF ellipseRect = ellipse.boundingRect();
    data->ellipseRect = ellipseRect;
    QTransform transform;
    transform.translate(data->ellipseCenter.x(),data->ellipseCenter.y());
    transform.rotate(-data->angle);
    QPolygonF poly = transform.map(ellipseRect);
    double polyleft = poly[0].x();
    double polyright = poly[0].x();
    double polytop = poly[0].y();
    double polybottom = poly[0].y();
    foreach (auto point, poly)
    {
        if (point.x()<polyleft)
            polyleft = point.x();
        if (point.x()>polyright)
            polyright = point.x();
        if (point.y()<polybottom)
            polybottom = point.y();
        if (point.y()>polytop)
            polytop = point.y();
    }
    double polywidth = polyright - polyleft;
    double polyheight = polytop - polybottom;
    QRectF viewRect = QRectF(polyleft-polywidth/4.0, polybottom-polyheight/4.0, polywidth*1.5, polyheight*1.5);
    data->line1 = line1.intersected(viewRect);
    data->line2 = line2.intersected(viewRect);
    emit elementsUpdated(data);
}

void Placer::errorMessage(quint8 error) const
{
    const QString errors[] = {"First tangent must have at least one non-zero variable coefficient.",
                        "Second tangent must have at least one non-zero variable coefficient.",
                        "Ellipse center shouldn't belong to first tangent.",
                        "Ellipse center shouldn't belong to second tangent.",
                        "First tangent must pass through first point of tangency.",
                        "Second tangent must pass through second point of tangency.",
                        "Tangents shouldn't coincide",
                        "Second tangent shouldn't pass through first point of tangency.",
                        "First tangent shouldn't pass through second point of tangency.",
                        "Ellipse solution can't be found (might be hyperbola or imaginary ellipse)." };
    QMessageBox::warning(nullptr, "Bad input", errors[error]);
}
