#ifndef VTOC_H
#define VTOC_H

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



#include <Qt>
#include "util.h"
#include "tspair.h"

class Sector;



class QString;

class VTOC
{
public:
    VTOC(QSharedPointer<const Sector> data);

    void   dump();
    TSPair firstCatalogSector() const;
    quint8 dosVersion();
    quint8 volumeNumber();
    quint8 maxTSPairs();
    quint8 lastTrackAllocated();
    qint8  directionOfAllocation();
    quint8 tracksPerDisk();
    quint8 sectorsPerTrack();
    qint16 bytesPerSector();

    bool   isSectorInUse(TSPair ts) const;

    QList<TSPair> sectorsInUse() const;
    QList<TSPair> sectorsNotInUse() const;


private:
    QString buildUseString(quint8 track);

    QSharedPointer<const Sector> m_data;

    TSPair m_first_catalog_sector;
    quint8 m_dos_version;
    quint8 m_volnum;
    quint8 m_max_ts_pairs;
    quint8 m_last_track_alloc;
    qint8 m_dir_of_alloc;
    quint8 m_tracks_per_disk;
    quint8 m_sec_per_track;
    quint16 m_byte_per_sec;

};

#endif // VTOC_H
