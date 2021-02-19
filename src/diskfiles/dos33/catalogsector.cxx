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

#include "catalogsector.h"
#include "sector.h"

CatalogSector::CatalogSector(Sector *data)
{
    m_data = data;

    m_next = TSPair(0,0);

    TSPair next(m_data->at(0x01),m_data->at(0x02));

    if (next.isValid() && next.track() == 17)
    {
 //       next.dump();
        m_next = next;
    }
    else
    {
     //   qWarning() << "Track sector " << next.track() << "," << next.sector() << "is invalid! Not adding to catalog.";
   //     m_next.dump();
    }

    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = makeFDE(idx*0x23+0x0B);
        if (fde.firstTSListSector() !=  TSPair(0,0)) {
       //     if (fde.firstTSListSector().isValid())
            {
                m_fdes.append(fde);
            }
       //     else qDebug() << "Not appending invalid TSPair.";
        }
    //    else { qWarning("fde.firstTSListSector() is 0,0"); }
    }
}

void CatalogSector::dumpFDEs() {
    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = m_fdes[idx];
        if (fde.firstTSListSector() != TSPair(0,0)) {
            qDebug() << "FDE #"<<idx;
            fde.dump();
        }
    }
}

FileDescriptiveEntry CatalogSector::makeFDE(int offset)
{
    FileDescriptiveEntry fde;
    TSPair first(m_data->at(offset + 0x00),m_data->at(offset + 0x01));
    fde.setFirstTSListSector(first);

    fde.lengthInSectors = makeWord(m_data->at(offset + 0x21),
                                   m_data->at(offset + 0x22));
    
    fde.fileTypeFlags = m_data->at(offset + 0x02);

    if (fde.lengthInSectors > 16*35)
    {
        fde.lengthInSectors = -1;
    }

    for (int idx = 0x03; idx <= 0x20; idx++) {
        fde.filename.append(m_data->at(idx+offset));
    }

    if (fde.firstTSListSector().track() == 0xFF)
    {
        qDebug() << "File" << fde.filename.printable() << "is deleted";
        fde.deleted = true;
        qDebug() << fde.filename;
//        TSPair first = fde.firstTSListSector();
//        first.setTrack(m_data->at(offset + 0x20));
//        fde.setFirstTSListSector(first);
//        qDebug() << "   New track: " <<  (quint8) fde.firstTSListSector().track();
//        qDebug() << "   Sector: " << fde.firstTSListSector().sector();
    }

    return fde;
}

TSPair CatalogSector::sectorLocation() const {
    return TSPair(m_data->track(),m_data->sector());
}
