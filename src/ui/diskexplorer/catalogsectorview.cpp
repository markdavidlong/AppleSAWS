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

#include "catalogsectorview.h"
#include "ui_catalogsectorview.h"
#include "sector.h"

#include <QTableWidgetItem>

CatalogSectorView::CatalogSectorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogSectorView)
{
    ui->setupUi(this);
    for (int row = 0; row < 7; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            auto item = new QTableWidgetItem("");
            item->setForeground(Qt::black);
            item->setFont(QFont("Print Char 21",8));
            item->setFlags(Qt::NoItemFlags);
            ui->fdeTable->setItem(row,col,item);
        }
    }
    QHeaderView *verticalHeader = ui->fdeTable->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);
}

CatalogSectorView::~CatalogSectorView()
{
    delete ui;
}

void CatalogSectorView::setSector(QSharedPointer<Sector>sec)
{

    m_sector = sec;

    if (sec)
    {
        auto cs = sec->asCatalogSector();

        ui->csLabel->setText(QString("Catalog Sector at (%1/%2)")
                             .arg(sec->track()).arg(sec->sector()));

        auto table = ui->fdeTable;
        for (int row = 0; row < 7; row++)
        {
            auto tsCell = table->item(row,0);
            auto sizeCell = table->item(row,1);
            auto typeCell = table->item(row,2);
            auto nameCell = table->item(row,3);

            FileDescriptiveEntry fde = cs.makeFDE(row*0x23+0x0B);

            if (fde.deleted)
            {
                auto delstr = QString("Del (%1/%2)").arg((quint8) fde.filename[29])
                        .arg(fde.firstTSListSector().sector());
                tsCell->setText(delstr);
            }
            else
            {
                auto tsstr = QString("%1/%2").arg(fde.firstTSListSector().track())
                        .arg(fde.firstTSListSector().sector());
                tsCell->setText(tsstr);
            }

            auto type = fde.fileTypeIdentifier();
            if (fde.isLocked()) { type += " Locked"; }
            typeCell->setText(type);

            auto name = fde.filename;
            if (fde.deleted)
            {
                name.truncate(29);
                nameCell->setForeground(Qt::red);
                nameCell->setText(name.appleFontPrintable());
            }
            else
            {
                nameCell->setForeground(Qt::black);
                nameCell->setText(name.appleFontPrintable());
            }

            auto size = fde.lengthInSectors;
            sizeCell->setText(QString::number(size));
        }

        auto next = cs.nextCatalogSector();
        ui->nextCSLabel->setText(
                    QString("Next Catalog Sector: (%1/%2)")
                    .arg(next.track()).arg(next.sector()));

    }
    else
    {
        ui->csLabel->setText("Catalog Sector");
        auto table = ui->fdeTable;
        for (int row = 0; row < 7; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                auto cell = table->item(row,col);
                cell->setText("");

            }
        }
        ui->nextCSLabel->setText("");
    }
}
