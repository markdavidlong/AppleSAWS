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

#include "LocationInfoDialog.h"
#include "ui_LocationInfoDialog.h"

LocationInfoDialog::LocationInfoDialog(QWidget *parent) :
    QDialog(parent,  Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
    ui(new Ui::LocationInfoDialog)
{
    ui->setupUi(this);
    showSizeWidgets(false);
}

LocationInfoDialog::~LocationInfoDialog()
{
    delete ui;
}

void LocationInfoDialog::setInfoLabelString(QString label)
{
    ui->infoLabel->setText(label);
}

quint16 LocationInfoDialog::getAddress()
{
    return ui->addressEdit->text().toInt(Q_NULLPTR,16);
}

QString LocationInfoDialog::getInfo()
{
    return ui->infoEdit->text();
}

int LocationInfoDialog::getSymbolSize()
{
    return ui->sizeCombo->currentIndex();
}

void LocationInfoDialog::showSizeWidgets(bool show)
{
    ui->sizeCombo->setHidden(!show);
    ui->sizeLabel->setHidden(!show);
}
