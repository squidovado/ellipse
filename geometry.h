#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QtCore/QObject>

class Placer : public QObject
{
    Q_OBJECT
public:
    Placer(QObject *parent = nullptr);
};
#endif // GEOMETRY_H
