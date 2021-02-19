#ifndef RAWDISKIMAGE_H
#define RAWDISKIMAGE_H

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



#include "tspair.h"

#include <QString>
#include <QByteArray>
#include <QVector>

using SectorOrderMap = QByteArray;

using SectorData = QByteArray;
//PRODOS    using BlockData = QByteArray;
//PRODOS    using BlockPair = QPair<TSPair, TSPair>;
using TrackData = QList<SectorData*>;
using TrackList = QList<TrackData>;

using HashValue = QByteArray;


enum class SectorOrder
{
    Unknown = 0,
    Dos33 = 0,
    ProDos,
    Pascal,
    CPM,
    Custom1,
    Custom2
};


class RawDiskImage
{
public:

    RawDiskImage(QString filename = QString());
    ~RawDiskImage();
    bool read(QString filename);

    bool isEmpty() const;

    HashValue hash() const;

    QString diskImageName() const;
    QString fullDiskImageName() const;

    void setNumTracks(int  count);
    int  numTracks() const;

    bool hasSectors() const;
    int  sectorSize() const;
    void setSectorSize(int  size);
    void setSectorsPerTrack(int spt);
    int  sectorsPerTrack() const;

//PRODOS    bool hasBlocks();
//PRODOS    int blockSize();
//PRODOS    int blocksPerTrack();

    void setSectorOrder(SectorOrder order);
    SectorOrder sectorOrder() const;

    bool reorderSectorsTo(SectorOrder newOrder);

    SectorData *sectorAt(int track, int sector);
    SectorData *sectorAt(TSPair ts);

    inline SectorOrderMap dosOrderMap() const;;
    inline SectorOrderMap prodosOrderMap() const;
    inline SectorOrderMap pascalOrderMap() const;
    inline SectorOrderMap cpmOrderMap() const;

    SectorOrderMap customOrderMap0() const;
    SectorOrderMap customOrderMap1() const;

    bool setCustomOrderMap0(SectorOrderMap map);
    bool setCustomOrderMap1(SectorOrderMap map);

private:
//PRODOS    BlockPair mapBlockToTS(int blocknum);

private:
    TrackList m_tracks;

    bool m_empty_image;

    int m_sector_size;
    int m_sectors_per_track;
    int m_track_count;

    QString m_full_filename;
    QString m_filename;

    SectorOrder m_sector_order;

    SectorOrderMap m_custom_order_map_0;
    SectorOrderMap m_custom_order_map_1;

    HashValue m_hash;
};

#endif // RAWDISKIMAGE_H
