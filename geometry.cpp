#include <QSharedPointer>
#include <QMessageBox>
#include "geometry.h"
#include "common.h"

double* solve3(const double (&matrix)[3][3], const double (&rhs)[3])
{
    //Cramer's rule
    double det = matrix[0][0]*matrix[1][1]*matrix[2][2]+matrix[0][1]*matrix[1][2]*matrix[2][0]+
            matrix[0][2]*matrix[1][0]*matrix[2][1]-matrix[0][0]*matrix[1][2]*matrix[2][1]-
            matrix[0][1]*matrix[1][0]*matrix[2][2]-matrix[0][2]*matrix[1][1]*matrix[2][0];
    if (isEqual(det,0.0))
        return nullptr;
    double det1 = rhs[0]*matrix[1][1]*matrix[2][2]+matrix[0][1]*matrix[1][2]*rhs[2]+
            matrix[0][2]*rhs[1]*matrix[2][1]-rhs[0]*matrix[1][2]*matrix[2][1]-
            matrix[0][1]*rhs[1]*matrix[2][2]-matrix[0][2]*matrix[1][1]*rhs[2];
    double det2 = matrix[0][0]*rhs[1]*matrix[2][2]+rhs[0]*matrix[1][2]*matrix[2][0]+
            matrix[0][2]*matrix[1][0]*rhs[2]-matrix[0][0]*matrix[1][2]*rhs[2]-
            rhs[0]*matrix[1][0]*matrix[2][2]-matrix[0][2]*rhs[1]*matrix[2][0];
    double det3 = matrix[0][0]*matrix[1][1]*rhs[2]+matrix[0][1]*rhs[1]*matrix[2][0]+
            rhs[0]*matrix[1][0]*matrix[2][1]-matrix[0][0]*rhs[1]*matrix[2][1]-
            matrix[0][1]*matrix[1][0]*rhs[2]-rhs[0]*matrix[1][1]*matrix[2][0];
    double* solution = new double[3];
    solution[0]=det1/det;
    solution[1]=det2/det;
    solution[2]=det3/det;
    return solution;
}

Line Line::translated(QPointF point) const
{
    return Line(); //Line(_a-point.x(),_b,_c-point.y(),_d);
}

Placer::Placer(QObject *parent) : QObject(parent), line1(), line2(),
                                    tpoint1(1.0, 0.0), tpoint2(0.0, -0.5), ellipse()
{
    //calculate();
}

void Placer::processNewData(QSharedPointer<const InputData> data)
{
    checkData(data);
}

bool Placer::checkData(QSharedPointer<const InputData> data) const
{
    if (isEqual(data->tangent1a,0) && isEqual(data->tangent1b,0))
    {
        QMessageBox::warning(nullptr, "Bad input", "First tangent must have at least one non-zero variable coefficient.");
        return false;
    }
    if (data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a,0) && isEqual(data->tangent2b,0))
    {
        QMessageBox::warning(nullptr, "Bad input", "Second tangent must have at least one non-zero variable coefficient.");
        return false;
    }
    if (isEqual(data->tangent1a*data->xcenter + data->tangent1b*data->ycenter, data->tangent1c))
    {
        QMessageBox::warning(nullptr, "Bad input", "Ellipse center shouldn't belong to first tangent.");
        return false;
    }
    if (data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a*data->xcenter + data->tangent2b*data->ycenter, data->tangent2c))
    {
        QMessageBox::warning(nullptr, "Bad input", "Ellipse center shouldn't belong to second tangent.");
        return false;
    }
    if (isEqual(data->xcenter,data->point1x) && isEqual(data->ycenter,data->point1y))
    {
        QMessageBox::warning(nullptr, "Bad input", "Ellipse center and first point of tangency shouldn't coincide.");
        return false;
    }
    if (data->mode == InputData::OneTangentTwoPoints && isEqual(data->xcenter,data->point2x) && isEqual(data->ycenter,data->point2y))
    {
        QMessageBox::warning(nullptr, "Bad input", "Ellipse center and second point of tangency shouldn't coincide.");
        return false;
    }
    if(!isEqual(data->tangent1a*data->point1x + data->tangent1b*data->point1y, data->tangent1c))
    {
        QMessageBox::warning(nullptr, "Bad input", "First tangent must pass through first point of tangency.");
        return false;
    }
    if(data->mode == InputData::TwoTangentsOnePoint && isEqual(data->tangent2a*data->point1x + data->tangent2b*data->point1y, data->tangent2c))
    {
        QMessageBox::warning(nullptr, "Bad input", "Second tangent shouldn't pass through first point of tangency.");
        return false;
    }
    if(data->mode == InputData::OneTangentTwoPoints && isEqual(data->tangent1a*data->point2x + data->tangent1b*data->point2y, data->tangent1c))
    {
        QMessageBox::warning(nullptr, "Bad input", "First tangent shouldn't pass through second point of tangency.");
        return false;
    }
    return true;
}

void Placer::calculate()
{
    //local coord system with the origin at the center of the ellipse
    QPointF origin = ellipse.center();
    QPointF p1local;// = p1 - origin;
    QPointF p2local;// = p2 - origin;
    Line l1local;// = l1.translated(origin);
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
    double* solution = solve3(matrix, rhs);
}
