/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "hexconverter.h"
#include "ui_hexconverter.h"
#include <QString>
#include <QIntValidator>

HexConverter::HexConverter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HexConverter)
{
    ui->setupUi(this);

    ui->uint8LineEdit->setValidator(new QIntValidator(0,255,this));
    ui->int8LineEdit->setValidator(new QIntValidator(-128,127,this));
    ui->uint16LineEdit->setValidator(new QIntValidator(0,65535,this));
    ui->int16LineEdit->setValidator(new QIntValidator(-32768,32767,this));

    connect(ui->hexLineEdit, &QLineEdit::textEdited, this, &HexConverter::calcFromNewHex);
    connect(ui->uint8LineEdit, &QLineEdit::textEdited, this, &HexConverter::calcFromNewUint8);
    connect(ui->int8LineEdit, &QLineEdit::textEdited, this, &HexConverter::calcFromNewInt8);
    connect(ui->uint16LineEdit, &QLineEdit::textEdited, this, &HexConverter::calcFromNewUint16);
    connect(ui->int16LineEdit, &QLineEdit::textEdited, this, &HexConverter::calcFromNewInt16);
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
