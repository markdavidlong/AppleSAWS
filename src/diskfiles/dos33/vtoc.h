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
    VTOC(Sector *data);

    void   dump();
    TSPair firstCatalogSector();
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

    Sector *m_data;
};

#endif // VTOC_H
