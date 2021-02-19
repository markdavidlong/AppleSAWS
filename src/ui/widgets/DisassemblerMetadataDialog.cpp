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

#include "DisassemblerMetadataDialog.h"
#include "ui_DisassemblerMetadataDialog.h"
#include "util.h"
#include <QDebug>


DisassemblerMetadataDialog::DisassemblerMetadataDialog(BinaryFileMetadata *bfm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisassemblerMetadataDialog)
{
    ui->setupUi(this);
    ui->entryTable->verticalHeader()->show();
    ui->removeEntryPointButton->setEnabled(false);
    ui->removeSymbolButton->setEnabled(false);

    m_bfm = bfm;

    m_epmodel = new EntryPointModel(this,m_bfm->entryPoints());
    m_asmodel = new AssemblerSymbolModel(this,m_bfm->assemblerSymbols());

    ui->entryTable->setModel(m_epmodel);
    ui->symbolTable->setModel(m_asmodel);

    connect(ui->exitButton, &QPushButton::clicked,
            this, &DisassemblerMetadataDialog::handleExitButton);
    connect(ui->processButton, &QPushButton::clicked,
            this, &DisassemblerMetadataDialog::handleProcessButton);

    connect(ui->addEntryPointButton, &QToolButton::clicked,
            this, &DisassemblerMetadataDialog::handleAddEntryPointButton);
    connect(ui->addSymbolButton, &QToolButton::clicked,
            this, &DisassemblerMetadataDialog::handleAddSymbolButton);

    connect(ui->removeEntryPointButton, &QToolButton::clicked,
            this, &DisassemblerMetadataDialog::handleRemoveEntryPointButton);
    connect(ui->removeSymbolButton, &QToolButton::clicked,
            this, &DisassemblerMetadataDialog::handleRemoveSymbolButton);

    connect(ui->entryTable->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &DisassemblerMetadataDialog::handleEntryPointSelectionChanged);
    connect(ui->symbolTable->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &DisassemblerMetadataDialog::handleSymbolSelectionChanged);
}

DisassemblerMetadataDialog::~DisassemblerMetadataDialog()
{
    delete ui;
}

void DisassemblerMetadataDialog::showEvent(QShowEvent *)
{
    ui->entryTable->resizeRowsToContents();
    ui->symbolTable->resizeRowsToContents();
}

void DisassemblerMetadataDialog::closeEvent(QCloseEvent *)
{
    handleExitButton();
}

void DisassemblerMetadataDialog::handleExitButton()
{
    m_bfm->requestDisassembly();
    m_bfm->save();
    this->close();
}

void DisassemblerMetadataDialog::handleProcessButton()
{
    m_bfm->requestDisassembly();
}

void DisassemblerMetadataDialog::handleAddEntryPointButton()
{
    LocationInfoDialog lid(this);
    lid.setWindowTitle("Add Entry Point");
    if (lid.exec() == Accepted)
    {
        EntryPoint ep;
        ep.address = lid.getAddress();
        ep.note = lid.getInfo();
        m_bfm->entryPoints()->addPoint(ep);
        ui->entryTable->resizeRowsToContents();
        ui->entryTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveEntryPointButton()
{
    QModelIndexList selection = ui->entryTable->selectionModel()->selectedRows(0);
    qDebug() << "Removing" << selection.count() << "row(s)";
    if (selection.count())
    {
 //       qDebug() << "Removing row" << selection[0].row();
        m_epmodel->removeRows(selection[0].row(),1);
//        qDebug() << "Removed row" << selection[0].row();
    }
}

void DisassemblerMetadataDialog::handleAddSymbolButton()
{
    LocationInfoDialog lid(this);
    lid.setInfoLabelString("Symbol Name");
    lid.setWindowTitle("Add Symbol");
    lid.showSizeWidgets(true);
    if (lid.exec() == Accepted)
    {
        AssemblerSymbol as;
        as.address = lid.getAddress();
        as.name = lid.getInfo();
        if (lid.getSymbolSize() == 0) // Byte
        {
            as.symbolsize = SizeByte;
        }
        else
        {
            as.symbolsize = SizeWord;
        }
        m_bfm->assemblerSymbols()->addSymbol(as);
        ui->symbolTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveSymbolButton()
{
    QModelIndexList selection = ui->symbolTable->selectionModel()->selectedRows(0);
    qDebug() << "Removing" << selection.count() << "row(s)";
    if (selection.count())
    {
        m_asmodel->removeRows(selection[0].row(),1);
    }
}

void DisassemblerMetadataDialog::handleEntryPointSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    int selectedcount = ui->entryTable->selectionModel()->selectedRows().count();
    ui->removeEntryPointButton->setEnabled(selectedcount);
}

void DisassemblerMetadataDialog::handleSymbolSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    int selectedcount = ui->symbolTable->selectionModel()->selectedRows().count();
    ui->removeSymbolButton->setEnabled(selectedcount);
}
