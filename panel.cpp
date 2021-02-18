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
    if (index==0)
    {
        ui->point2x->setEnabled(false);
        ui->point2y->setEnabled(false);
        ui->tangent2a->setEnabled(true);
        ui->tangent2b->setEnabled(true);
        ui->tangent2c->setEnabled(true);
    }
    else
    {
        ui->point2x->setEnabled(true);
        ui->point2y->setEnabled(true);
        ui->tangent2a->setEnabled(false);
        ui->tangent2b->setEnabled(false);
        ui->tangent2c->setEnabled(false);
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

Panel::~Panel()
{
    delete ui;
}
