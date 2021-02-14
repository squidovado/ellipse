#include "geometry.h"

bool isEqual(double x, double y)
{
    const double eps = 1e-6;
    return abs(x-y)<eps;
}

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
    return Line(_a-point.x(),_b,_c-point.y(),_d);
}

Placer::Placer(QObject *parent) : QObject(parent), line1(0.,1.,0.,0.), line2(0.,0.,0.,1.),
    tpoint1(1.0, 0.0), tpoint2(0.0, -0.5), ellipse(QPointF(1.0,-0.5))
{
    changeOrder = {FirstLine, SecondLine, FirstPointOfTangency, SecondPointOfTangency};
    calculate();
}

void Placer::calculate()
{
    ChangeableObject chObj = changeOrder[0];
    switch(chObj)
    {
    case FirstLine:
        calculateNewLine(tpoint2, tpoint1, line2, line1);
        break;
    case SecondLine:
        calculateNewLine(tpoint1, tpoint2, line1, line2);
        break;
    case FirstPointOfTangency:
        calculateNewPoint(line2, line1, tpoint2, tpoint1);
        break;
    case SecondPointOfTangency:
        calculateNewPoint(line1, line2, tpoint1, tpoint2);
        break;
    }
}

void Placer::calculateNewLine(const QPointF& p1, const QPointF& p2, const Line& l1, Line& l2)
{
    //local coord system with the origin at the center of the ellipse
    QPointF origin = ellipse.center();
    QPointF p1local = p1 - origin;
    QPointF p2local = p2 - origin;
    Line l1local = l1.translated(origin);
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

void Placer::calculateNewPoint(const Line& l1, const Line& l2, const QPointF& p1, QPointF& p2)
{

}
