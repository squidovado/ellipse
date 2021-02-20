#include <QKeyEvent>
#include "panel.h"
#include "ui_panel.h"
#include "common.h"

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
    ui->point1y->setText(tr("-0.5"));
    ui->tangent2a->setText(tr("0.0"));
    ui->tangent2b->setText(tr("1.0"));
    ui->tangent2c->setText(tr("0.0"));
    ui->point2x->setText(tr("1.0"));
    ui->point2y->setText(tr("0.0"));
    connect(ui->comboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&Panel::comboBoxIndexChanged);
    connect(ui->pushButton,&QPushButton::clicked,this,&Panel::calculate);
    comboBoxIndexChanged(0);

    QDoubleValidator* validator = new QDoubleValidator();
    validator->setLocale(QLocale::English);
    QVector<QLineEdit*> LEvector = { ui->xcenter, ui->ycenter,
                                    ui->tangent1a, ui->tangent1b, ui->tangent1c, ui->point1x, ui->point1y,
                                    ui->tangent2a, ui->tangent2b, ui->tangent2c, ui->point2x, ui->point2y };
    foreach (auto& lineEdit, LEvector)
        lineEdit->setValidator(validator);
}

void Panel::comboBoxIndexChanged(int index)
{
    if (index==1)
    {
        ui->point2x->setReadOnly(false);
        ui->point2y->setReadOnly(false);
        ui->tangent2a->setReadOnly(true);
        ui->tangent2b->setReadOnly(true);
        ui->tangent2c->setReadOnly(true);
    }
    else
    {
        ui->point2x->setReadOnly(true);
        ui->point2y->setReadOnly(true);
        ui->tangent2a->setReadOnly(false);
        ui->tangent2b->setReadOnly(false);
        ui->tangent2c->setReadOnly(false);
    }
}

void Panel::calculate() const
{
    QSharedPointer<InputData> data = QSharedPointer<InputData>::create();
    data->mode = ui->comboBox->currentIndex()==0 ? InputData::TwoTangentsOnePoint : InputData::OneTangentTwoPoints;
    data->xcenter = ui->xcenter->text().toDouble();
    data->ycenter = ui->ycenter->text().toDouble();
    data->point1x = ui->point1x->text().toDouble();
    data->point1y = ui->point1y->text().toDouble();
    data->point2x = ui->point2x->text().toDouble();
    data->point2y = ui->point2y->text().toDouble();
    data->tangent1a = ui->tangent1a->text().toDouble();
    data->tangent1b = ui->tangent1b->text().toDouble();
    data->tangent1c = ui->tangent1c->text().toDouble();
    data->tangent2a = ui->tangent2a->text().toDouble();
    data->tangent2b = ui->tangent2b->text().toDouble();
    data->tangent2c = ui->tangent2c->text().toDouble();
    emit newDataAdded(data);
}

void Panel::updateLine2(double a, double b, double c)
{
    ui->tangent2a->setText(QString::number(a,'g',2));
    ui->tangent2b->setText(QString::number(b,'g',2));
    ui->tangent2c->setText(QString::number(c,'g',2));
}

void Panel::updateTPoint2(double x, double y)
{
    ui->point2x->setText(QString::number(x,'g',2));
    ui->point2y->setText(QString::number(y,'g',2));
}

void Panel::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter)
        calculate();
}

Panel::~Panel()
{
    delete ui;
}
