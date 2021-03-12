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

#ifndef SECTOR_H
#define SECTOR_H

#include <QByteArray>
#include <QSharedPointer>
//#include "rawdiskimage.h"

#include "vtoc.h"
#include "catalogsector.h"
#include "tracksectorlist.h"

#include "asdiskdata.h"

class Sector;

using pSector = QSharedPointer<Sector>;

class Sector : public QEnableSharedFromThis<Sector>
{
public:

    Sector(QByteArray &data, int track, int sector )
    {
        qDebug() << "Sector ctor: Datasize: " << data.size() << " t/s: " << track << "," << sector;
         m_track = track;
         m_sector = sector;
         m_raw_data = data;
         m_raw_data.append(12);
         m_raw_data.chop(1);
         if (m_raw_data.size() != 256)
         {
             qWarning() << "Setting sector with invalid sized data! [Size = " << data.size() << "]";
         }
    }

    ~Sector() { qDebug() << "Sector dtor for " << m_track << "," << m_sector; }

    VTOC promoteToVTOC() const {
        return VTOC(sharedFromThis());
    }

    CatalogSector asCatalogSector() const {
        return CatalogSector(sharedFromThis());
    }

    TrackSectorList asTrackSectorList() const {
        return TrackSectorList(sharedFromThis());
    }

    int sector() const { return m_sector; }
    int track() const { return m_track; }

    quint8 at(int offset) const {
        if (offset >= m_raw_data.size()) {

            qWarning() << "sector::at()  offset = " << offset
                       << ". m_raw_data.size() == " << m_raw_data.size();
        }

        return m_raw_data.at(offset);
    }

    void dump() const;

    QByteArray rawData()  { return m_raw_data; }

private:
    int m_track;
    int m_sector;

    QByteArray m_raw_data;
};


#endif // SECTOR_H
