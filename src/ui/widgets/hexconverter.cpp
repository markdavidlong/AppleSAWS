#include "hexconverter.h"
#include "ui_hexconverter.h"
#include <QString>

HexConverter::HexConverter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HexConverter)
{
    ui->setupUi(this);

    ui->uint8LineEdit->setValidator(new QIntValidator(0,255));
    ui->int8LineEdit->setValidator(new QIntValidator(-128,127));
    ui->uint16LineEdit->setValidator(new QIntValidator(0,65535));
    ui->int16LineEdit->setValidator(new QIntValidator(-32768,32767));

    connect(ui->hexLineEdit, SIGNAL(textEdited(QString)), SLOT(calcFromNewHex(QString)));
    connect(ui->uint8LineEdit, SIGNAL(textEdited(QString)), SLOT(calcFromNewUint8(QString)));
    connect(ui->int8LineEdit, SIGNAL(textEdited(QString)), SLOT(calcFromNewInt8(QString)));
    connect(ui->uint16LineEdit, SIGNAL(textEdited(QString)), SLOT(calcFromNewUint16(QString)));
    connect(ui->int16LineEdit, SIGNAL(textEdited(QString)), SLOT(calcFromNewInt16(QString)));
}

HexConverter::~HexConverter()
{
    delete ui;
}

void HexConverter::calcFromNewHex(QString value)
{
    bool ok = true;
    quint16 ui16 = value.toInt(&ok,16);
    qint16 i16 = ui16;
    quint8 ui8 = ui16;
    qint8 i8 = ui16;

    ui->uint16LineEdit->setText(QString::number(ui16));
    ui->int16LineEdit->setText(QString::number(i16));
    ui->uint8LineEdit->setText(QString::number(ui8));
    ui->int8LineEdit->setText(QString::number(i8));
}

void HexConverter::calcFromNewUint16(QString value)
{
    bool ok = true;
    quint16 ui16 = value.toInt(&ok);
    qint16 i16 = ui16;
    quint8 ui8 = ui16;
    qint8 i8 = ui16;
    QString hex = QString::number(ui16,16);

    ui->int16LineEdit->setText(QString::number(i16));
    ui->uint8LineEdit->setText(QString::number(ui8));
    ui->int8LineEdit->setText(QString::number(i8));
    ui->hexLineEdit->setText(hex);
}

void HexConverter::calcFromNewInt16(QString value)
{
    bool ok = true;
    qint16 i16 = value.toInt(&ok);
    quint16 ui16 = i16;
    quint8 ui8 = i16;
    qint8 i8 = i16;
    QString hex = QString::number(ui16,16);

    ui->uint16LineEdit->setText(QString::number(ui16));
    ui->uint8LineEdit->setText(QString::number(ui8));
    ui->int8LineEdit->setText(QString::number(i8));
    ui->hexLineEdit->setText(hex);
}

void HexConverter::calcFromNewUint8(QString value)
{
    bool ok = true;
    quint8 ui8 = value.toInt(&ok);
    quint16 ui16 = ui8;
    qint16 i16 = ui16;
    qint8 i8 = ui16;
    QString hex = QString::number(ui16,16);

    ui->uint16LineEdit->setText(QString::number(ui16));
    ui->int16LineEdit->setText(QString::number(i16));
    ui->int8LineEdit->setText(QString::number(i8));
    ui->hexLineEdit->setText(hex);
}

void HexConverter::calcFromNewInt8(QString value)
{
    bool ok = true;
    qint8 i8 = value.toInt(&ok);
    quint8 ui8 = i8;
    quint16 ui16 = ui8;
    qint16 i16 = ui16;
    QString hex = QString::number(ui16,16);

    ui->uint16LineEdit->setText(QString::number(ui16));
    ui->int16LineEdit->setText(QString::number(i16));
    ui->uint8LineEdit->setText(QString::number(ui8));
    ui->hexLineEdit->setText(hex);
}
