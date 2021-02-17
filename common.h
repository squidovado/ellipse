#ifndef COMMON_H
#define COMMON_H

#define isEqual(x, y) (abs(x-y)<1e-6)

struct InputData
{
    enum Mode {OneTangentTwoPoints, TwoTangentsOnePoint} mode;
    double xcenter, ycenter;
    double tangent1a, tangent1b, tangent1c, point1x, point1y,
           tangent2a, tangent2b, tangent2c, point2x, point2y;
};

struct GraphicsData
{

};

#endif // COMMON_H
