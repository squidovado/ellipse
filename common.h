#ifndef COMMON_H
#define COMMON_H

#include <QLineF>
#include <QRectF>

#define isEqual(x, y) (abs(x-y)<1e-6)

struct InputData
{
    double xcenter, ycenter;
    double tangent1a, tangent1b, tangent1c, point1x, point1y,
           tangent2a, tangent2b, tangent2c, point2x, point2y;
};

struct GraphicsData
{
    QLineF line1, line2;
    QPointF ellipseCenter;
    qreal angle;
    QRectF ellipseRect;
};

#endif // COMMON_H
