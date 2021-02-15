#ifndef PANEL_H
#define PANEL_H
#include <QtCore/QObject>

class QLineEdit;

class Panel : public QObject
{
    Q_OBJECT
public:
    Panel(QLineEdit*[12]);
private:
    QLineEdit *_xcenter = nullptr, *_ycenter = nullptr,
              *_tangent1a = nullptr, *_tangent1b = nullptr, *_tangent1c = nullptr,
              *_tangent2a = nullptr, *_tangent2b = nullptr, *_tangent2c = nullptr,
              *_point1x = nullptr, *_point1y = nullptr,
              *_point2x = nullptr, *_point2y = nullptr;
};

#endif // PANEL_H
