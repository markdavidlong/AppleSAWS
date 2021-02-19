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

#include "vtocview.h"
#include "ui_vtocview.h"
#include "sector.h"

VTOCView::VTOCView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VTOCView)
{
    ui->setupUi(this);
}

VTOCView::~VTOCView()
{
    delete ui;
}
void VTOCView::setSector(Sector *sec)
{
    m_sector = sec;

    if (sec)
    {
        ui->m_mainLabel->setText(
                    QString("VTOC interpretation of T/S (%1/%2)")
                    .arg(sec->track()).arg(sec->sector()));
        auto v = sec->promoteToVTOC();
        auto ts = v.firstCatalogSector();
        ui->m_1stCatLabel->setText(QString("%1/%2").arg(ts.track()).arg(ts.sector()));

        ui->m_dosVersionLabel->setText(QString("%1").arg(v.dosVersion()));

        ui->m_volumeLabel->setText(QString("%1").arg(v.volumeNumber()));

        ui->m_tsPairCountLabel->setText(QString("%1").arg(v.maxTSPairs()));

        ui->m_lastAllocTrackLabel->setText(QString("%1").arg(v.lastTrackAllocated()));

        ui->m_allocDirLabel->setText(QString("%1").arg(v.directionOfAllocation()));

        ui->m_tracksPerDiskLabel->setText(QString("%1").arg(v.tracksPerDisk()));

        ui->m_sectorsPerTrackLabel->setText(QString("%1").arg(v.sectorsPerTrack()));

        ui->m_bytesPerSectorLabel->setText(QString("%1").arg(v.bytesPerSector()));

        QString usage;
        usage += "     ";
        for (int x = 0; x < v.tracksPerDisk(); x++)
        {
            usage += QString::number(x/10);
            usage += " ";
        }
        usage += "\n";
        usage += "     ";

        for (int x = 0; x < v.tracksPerDisk(); x++)
        {
            usage += QString::number(x%10);
            usage += " ";
        }
        usage += "\n\n"   ;
        for (int y = 0; y<v.sectorsPerTrack(); y++)
        {
            usage += QString("%1   ").arg(y,2,10);
            for (int x = 0; x < v.tracksPerDisk(); x++)
            {
                usage += v.isSectorInUse(TSPair(x,y))?"X":".";
                usage += " ";
            }
            usage += "\n";
        }

        ui->m_usageTextBrowser->setText("");
        ui->m_usageTextBrowser->setWordWrapMode(QTextOption::NoWrap);
        auto format = ui->m_usageTextBrowser->currentCharFormat();
        format.setFont(QFont("courier new",6));
        ui->m_usageTextBrowser->setCurrentCharFormat(format);
        ui->m_usageTextBrowser->insertPlainText(usage);

    }
    else
    {
        ui->m_mainLabel->setText("VTOC");
        ui->m_1stCatLabel->setText("--");
        ui->m_bytesPerSectorLabel->setText("--");
        ui->m_dosVersionLabel->setText("--");
        ui->m_lastAllocTrackLabel->setText("--");;
        ui->m_sectorsPerTrackLabel->setText("--");;
        ui->m_tracksPerDiskLabel->setText("--");;
        ui->m_tsPairCountLabel->setText("--");;
        ui->m_volumeLabel->setText("--");
        ui->m_allocDirLabel->setText("--");

        ui->m_usageTextBrowser->setText("   No Sector is Selected");

    }



}

QString VTOCView::makeUsage()
{
return QString();
}
