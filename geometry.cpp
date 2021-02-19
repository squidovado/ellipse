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

void Line::set(double a, double b, double c, double d)
{
    _a = a;
    _b = b;
    _c = c;
    _d = d;
}

Line Line::translated(QPointF point) const
{
    return Line(_a-point.x(),_b,_c-point.y(),_d);
}

void Ellipse::setParameters(double a11, double a12, double a22)
{
    _a11 = a11;
    _a12 = a12;
    _a22 = a22;
}

Placer::Placer(QObject *parent) : QObject(parent), line1(), line2(),
                                    tpoint1(1.0, 0.0), tpoint2(0.0, -0.5), ellipse()
{
    //calculate();
}

void Placer::processNewData(QSharedPointer<const InputData> data)
{
    if (!checkData(data))
        return;
    ellipse.setCenter(QPointF(data->xcenter, data->ycenter));
    tpoint1 = QPointF(data->point1x, data->point1y);
    tpoint2 = QPointF(data->point2x, data->point2y);
    line1.set(tpoint1.x(),data->tangent1b,tpoint1.y(),-data->tangent1a);
    if (data->mode == InputData::TwoTangentsOnePoint)
    {
        double a2,c2;
        if (isEqual(data->tangent2b,0.0))
        {
            a2 = data->tangent2c/data->tangent2a;
            c2 = 0.0;
        }
        else
        {
            a2 = 0.0;
            c2 = data->tangent2c/data->tangent2b;
        }
        line2.set(a2,data->tangent2b,c2,-data->tangent2a);
        calculateTwoLinesOnePoint();
    }
    else
        calculateTwoPointsOneLine();
}

bool Placer::checkData(QSharedPointer<const InputData> data) const
{
    if (isEqual(data->tangent1a,0) && isEqual(data->tangent1b,0))
    {
        errorMessage(0);
        return false;
    }
    if (data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a,0) && isEqual(data->tangent2b,0))
    {
        errorMessage(1);
        return false;
    }
    if (isEqual(data->tangent1a*data->xcenter + data->tangent1b*data->ycenter, data->tangent1c))
    {
        errorMessage(2);
        return false;
    }
    if (data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a*data->xcenter + data->tangent2b*data->ycenter, data->tangent2c))
    {
        errorMessage(3);
        return false;
    }
    if (data->mode == InputData::OneTangentTwoPoints && isEqual(data->xcenter,data->point2x) && isEqual(data->ycenter,data->point2y))
    {
        errorMessage(4);
        return false;
    }
    if(!isEqual(data->tangent1a*data->point1x + data->tangent1b*data->point1y, data->tangent1c))
    {
        errorMessage(5);
        return false;
    }
    if(data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a*data->point1x + data->tangent2b*data->point1y, data->tangent2c))
    {
        errorMessage(6);
        return false;
    }
    if(data->mode == InputData::OneTangentTwoPoints && isEqual(data->tangent1a*data->point2x + data->tangent1b*data->point2y, data->tangent1c))
    {
        errorMessage(7);
        return false;
    }
    return true;
}

void Placer::calculateTwoPointsOneLine()
{
    //local coord system with the origin at the center of the ellipse
    QPointF origin = ellipse.center();
    QPointF p1local = tpoint1 - origin;
    QPointF p2local = tpoint2 - origin;
    Line l1local = line1.translated(origin);
    double matrix[3][3], rhs[3] = {1., 1., 0.};
    matrix[0][0] = p1local.x()*p1local.x();
    matrix[0][1] = 2*p1local.x()*p1local.y();
    matrix[0][2] = p1local.y()*p1local.y();
    matrix[1][0] = p2local.x()*p2local.x();
    matrix[1][1] = 2*p2local.x()*p2local.y();
    matrix[1][2] = p2local.y()*p2local.y();
    matrix[2][0] = p1local.x()*l1local.b();
    matrix[2][1] = p1local.x()*l1local.d()+p1local.y()*l1local.b();
    matrix[2][2] = p1local.y()*l1local.d();
    double solution[3];
    if (!solve3(matrix, rhs, solution) || solution[0]<=0.0 || solution[2]<=0.0 || solution[0]*solution[2]-solution[1]*solution[1]<=0.0)
    {
        errorMessage(8);
        return;
    }
    ellipse.setParameters(solution[0], solution[1], solution[2]);
    line2.set(tpoint2.x(),solution[1]*p2local.x()+solution[2]*p2local.y(),
              tpoint2.y(),-solution[0]*p2local.x()-solution[1]*p2local.y());
}

void Placer::calculateTwoLinesOnePoint()
{

}

void Placer::errorMessage(quint8 error) const
{
    QString errors[9] = {"First tangent must have at least one non-zero variable coefficient.",
                        "Second tangent must have at least one non-zero variable coefficient.",
                        "Ellipse center shouldn't belong to first tangent.",
                        "Ellipse center shouldn't belong to second tangent.",
                        "Ellipse center and second point of tangency shouldn't coincide.",
                        "First tangent must pass through first point of tangency.",
                        "Second tangent shouldn't pass through first point of tangency.",
                        "First tangent shouldn't pass through second point of tangency.",
                        "Ellipse solution can't be found (might be hyperbola or imaginary ellipse)."};
    QMessageBox::warning(nullptr, "Bad input", errors[error]);
}
