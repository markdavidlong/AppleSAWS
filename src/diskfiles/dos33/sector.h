#ifndef SECTOR_H
#define SECTOR_H

#include <QByteArray>

#include "rawdiskimage.h"

#include "vtoc.h"
#include "catalogsector.h"
#include "tracksectorlist.h"

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



class Sector
{
public:

    Sector(SectorData *data = nullptr) {
        setData(data);
        m_track = 255;
        m_sector = 255;
    }

    void setData(SectorData *data)
    {
        m_raw_data = data;
    }

    VTOC promoteToVTOC() {
        return VTOC(this);
    }

    CatalogSector asCatalogSector() {
        return CatalogSector(this);
    }

    TrackSectorList asTrackSectorList()  {
        return TrackSectorList(this);
    }

    int sector() { return m_sector; }
    int track() { return m_track; }

    void setTrackSector(int track, int sector) {
        setTrack(track);
        setSector(sector);
    }

    void setTrackSector(TSPair ts)
    {
        setTrackSector(ts.track(),ts.sector());
    }

    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    quint8 operator[](uint offset) const;
    quint8 at(int offset) {
        if (offset >= m_raw_data->size()) return 0;

        return m_raw_data->at(offset);
    }

    void dump();

    SectorData *rawData() { return m_raw_data; }

private:
  //  QByteArray m_data;
    int m_track;
    int m_sector;

    SectorData *m_raw_data;
};

#endif // SECTOR_H
