#include <QLineEdit>
#include "panel.h"

Panel::Panel(QLineEdit* (LEarr[12])) : _xcenter(LEarr[0]), _ycenter(LEarr[1]),
                                         _tangent1a(LEarr[2]), _tangent1b(LEarr[3]), _tangent1c(LEarr[4]),
                                         _tangent2a(LEarr[5]), _tangent2b(LEarr[6]), _tangent2c(LEarr[7]),
                                         _point1x(LEarr[8]), _point1y(LEarr[9]),
                                         _point2x(LEarr[10]), _point2y(LEarr[11])
{
    _xcenter->setText(tr("1.0"));
    _ycenter->setText(tr("-0.5"));
    _tangent1a->setText(tr("1.0"));
    _tangent1b->setText(tr("0.0"));
    _tangent1c->setText(tr("0.0"));
    _point1x->setText(tr("0.0"));
    _point1x->setEnabled(false);
    _point1y->setText(tr("-0.5"));
    _tangent2a->setText(tr("0.0"));
    _tangent2b->setText(tr("1.0"));
    _tangent2c->setText(tr("0.0"));
    _point2x->setText(tr("1.0"));
    _point2y->setText(tr("0.0"));
    _point2y->setEnabled(false);
}

