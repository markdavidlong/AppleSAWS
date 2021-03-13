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

#include "dos33imagemodel.h"
#include "tspair.h"

#include <QIcon>

Dos33ImageModel::Dos33ImageModel(QObject *parent) : QStandardItemModel(parent)
{
    setHorizontalHeaderLabels({"Disk Images"});

    m_icon_A = QIcon(":/A_YELLOW.png");
    m_icon_a = QIcon(":/A_GREY.png");
    m_icon_B = QIcon(":/B_GREEN.png");
    m_icon_b = QIcon(":/B_GREY.png");
    m_icon_R = QIcon(":/R_MAGENTA.png");
    m_icon_S = QIcon(":/S_ORANGE.png");
    m_icon_T = QIcon(":/T_CYAN.png");
    m_icon_I = QIcon(":/I_RED.png");
    m_icon_disk = QIcon(":/disk.png");
}

Dos33ImageModel::~Dos33ImageModel()
{

}

bool Dos33ImageModel::addDiskImage(Dos33DiskImage *image, QString name)
{
    if (!image) { return false; }

    if (name.isEmpty()) { name = image->getDiskImageName(); }

    if (m_images.contains(name)) { return false; }

    auto diskImageItem = new Dos33TreeItem(name);
    invisibleRootItem()->appendRow(diskImageItem);


    foreach (auto sector, image->getCatalogSectors())
    {
        foreach (auto fde, sector.getFDEs())
        {
            if (!fde.deleted)
            {
                QString fn = AppleString(fde.filename).appleFontPrintable();
                QString type = fde.fileTypeIdentifier();

                QIcon icon;
                if (type == "A") { icon = m_icon_A; }
                else if (type == "B") { icon = m_icon_B; }
                else if (type == "T") { icon = m_icon_T; }
                else if (type == "R") { icon = m_icon_R; }
                else if (type == "I") { icon = m_icon_I; }
                else if (type == "a") { icon = m_icon_a; }
                else if (type == "b") { icon = m_icon_b; }
                else if (type == "S") { icon = m_icon_S; }

                auto item = new Dos33TreeItem(icon,fn);


                QVariant typevar = (int) Dos33ItemType::File;
                item->setData(typevar, (int) Dos33TreeRole::ItemType);

                QVariant datavar;
                datavar.setValue(fde.firstTSListSector());
                item->setData(datavar, (int) Dos33TreeRole::GetTSPair);

                QVariant fdevar;
                fdevar.setValue(fde);
                item->setData(fdevar, (int) Dos33TreeRole::GetFDE);


                item->setToolTip(QString("%1 Block(s)").arg(fde.lengthInSectors));
                diskImageItem->appendRow(item);
            }
        }
    }

    auto sectors = new Dos33TreeItem("Track/Sector");
    diskImageItem->appendRow(sectors);

    for (int track = 0; track < image->tracks(); track++)
    {
        auto trk = new Dos33TreeItem(m_icon_disk, QString("Track %1").arg(track));
        sectors->appendRow(trk);
        for (int sector = 0; sector < image->sectorsPerTrack(); sector++)
        {
            auto sec = new Dos33TreeItem(m_icon_disk, QString("Sector %1").arg(sector));
            trk->appendRow(sec);
        }
    }

    return true;
}

Dos33DiskImage *Dos33ImageModel::getDiskImage(QString name)
{
    return nullptr;
}

Dos33DiskImage *Dos33ImageModel::removeDiskImage(QString name)
{
    return nullptr;
}
