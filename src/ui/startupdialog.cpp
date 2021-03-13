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

#include "startupdialog.h"
#include "ui_startupdialog.h"

#include "diskexplorer.h"

#include <QFileDialog>
#include "centralappwindow.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    m_notes = new NotesDialog(this);
    ui->setupUi(this);
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::on_exploreContentsBtn_clicked()
{
    DiskExplorer *w = new DiskExplorer(this);
    w->showLoadDialog(false);
}


void StartupDialog::on_newStuffButton_clicked()
{
    CentralAppWindow *w = new CentralAppWindow(this);
    w->showMaximized();
}

void StartupDialog::on_exploreDiskImageBtn_clicked()
{

}

void StartupDialog::on_bootSectorCompareBtn_clicked()
{

}

void StartupDialog::on_infoBtn_clicked()
{
    m_notes->show();
}
