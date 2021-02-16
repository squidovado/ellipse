#include "panel.h"
#include "ui_panel.h"

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);
    ui->xcenter->setText(tr("1.0"));
    ui->ycenter->setText(tr("-0.5"));
    ui->tangent1a->setText(tr("1.0"));
    ui->tangent1b->setText(tr("0.0"));
    ui->tangent1c->setText(tr("0.0"));
    ui->point1x->setText(tr("0.0"));
    ui->point1x->setEnabled(false);
    ui->point1y->setText(tr("-0.5"));
    ui->tangent2a->setText(tr("0.0"));
    ui->tangent2b->setText(tr("1.0"));
    ui->tangent2c->setText(tr("0.0"));
    ui->point2x->setText(tr("1.0"));
    ui->point2y->setText(tr("0.0"));
    ui->point2y->setEnabled(false);
}

Panel::~Panel()
{
    delete ui;
}
