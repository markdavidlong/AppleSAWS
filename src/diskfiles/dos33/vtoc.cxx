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

#include "vtoc.h"

#include <QDebug>
#include <QString>

#include "sector.h"
#include "util.h"

VTOC::VTOC(QSharedPointer<const Sector> data)
{
    m_data = data;
    m_first_catalog_sector = TSPair(m_data->at(0x01), m_data->at(0x02));
    m_dos_version = m_data->at(0x03);
    m_volnum = m_data->at(0x06);
    m_max_ts_pairs = m_data->at(0x27);
    m_last_track_alloc = m_data->at(0x30);
    m_dir_of_alloc= m_data->at(0x31);
    m_tracks_per_disk = m_data->at(0x34);
    m_sec_per_track = m_data->at(0x35);
    m_byte_per_sec = makeWord(m_data->at(0x36),
                              m_data->at(0x37));

}

TSPair VTOC::firstCatalogSector() const {


  //  return TSPair(0x11,0x0f); // Force to look at the normal location

 //   return TSPair(m_data->at(0x01), m_data->at(0x02));
    return m_first_catalog_sector;
}

quint8 VTOC::dosVersion() {
    return m_dos_version;
    //return m_data->at(0x03);
}

quint8 VTOC::volumeNumber() {
    return m_volnum;
    //return m_data->at(0x06);
}

quint8 VTOC::maxTSPairs() {
    //return m_data->at(0x27);
    return m_max_ts_pairs;
}

quint8 VTOC::lastTrackAllocated() {
    return m_last_track_alloc;
    //return m_data->at(0x30);
}

qint8 VTOC::directionOfAllocation() {
    return m_dir_of_alloc;
    //return m_data->at(0x31);
}

quint8 VTOC::tracksPerDisk() {
    return m_data->at(0x34);
}

quint8 VTOC::sectorsPerTrack() {
    return m_sec_per_track;
    //return m_data->at(0x35);
}

qint16 VTOC::bytesPerSector() {
    return m_byte_per_sec;
//    return makeWord(m_data->at(0x36),
//                    m_data->at(0x37));
}

bool VTOC::isSectorInUse(TSPair ts) const {
    quint8 track = ts.track();
    quint8 sec = ts.sector();
    quint8 baseaddr = (track * 4) + 0x38;

    //quint16 word = (((quint16) m_data->rawData()[baseaddr]) *256) + (quint8) m_data->rawData()[baseaddr+1];
    quint16 word = makeWord(m_data->at(baseaddr+1),
                            m_data->at(baseaddr));
    quint16 bitpos = (quint16) 0x01 << (quint16) sec;

    return !(word & bitpos);
}

QList<TSPair> VTOC::sectorsInUse() const
{
    QList<TSPair> retval;

    for (int track = 0; track < 35; track++)
    {
        for (int sector = 0; sector < 16; sector++)
        {
            TSPair ts(track,sector);
            if (isSectorInUse(ts))
            {
                retval.append(ts);
            }
        }
    }
    return retval;
}

QList<TSPair> VTOC::sectorsNotInUse() const
{
    QList<TSPair> retval;

    for (int track = 0; track < 35; track++)
    {
        for (int sector = 0; sector < 16; sector++)
        {
            TSPair ts(track,sector);
            if (!isSectorInUse(ts))
            {
                retval.append(ts);
            }
        }
    }
    return retval;
}



void VTOC::dump()
{
    /*
    for (qint8 idx = 0x0f; idx >= 0; idx--) {
        quint8 shift;
        if (idx < 0x08) { shift = idx; } else { shift = idx-0x08; }
        qDebug() << "Idx: " << idx << "Shift: " << (quint8) shift << "Bitpos: " << (quint8) (0x01 << shift);
    }
*/
    qDebug() << "Dumping VTOC Track " << m_data->track() << "Sector " << m_data->sector() << " ...";
    qDebug() << "   Track number of first catalog sector: " << QString::number(firstCatalogSector().track());
    qDebug() << "   Sector number of first catalog sector: " << QString::number(firstCatalogSector().sector());
    qDebug() << "   Release number of DOS used to INIT disk: " << QString::number(dosVersion());
    qDebug() << "   Disk Volume Number: " << QString::number(volumeNumber());
    qDebug() << "   Max track/sector pairs that fit in t/s list sector (122=256): " << QString::number(maxTSPairs());
    qDebug() << "   Last track where sectors were allocated: " << QString::number(lastTrackAllocated());
    qDebug() << "   Direction of track allocations (+/- 1): " << QString::number(directionOfAllocation());
    qDebug() << "   Number tracks per disk: " << QString::number(tracksPerDisk());
    qDebug() << "   Number sectors per disk: " << QString::number(sectorsPerTrack());
    qDebug() << "   Number bytes/sector: " << QString::number(bytesPerSector());
    qDebug() << "   Track Usage (.=free, 0-F=used):";
    for (quint8 track = 0; track < m_data->at(0x34);track++)
    {
        qDebug() << "      " << QString("Track %1:").arg((int) track,2,10,QChar('0')) << buildUseString(track);
    }
 }

QString VTOC::buildUseString(quint8 track) {
    QString usestr;
    for (qint8 sec = 0x0f; sec >= 0; sec--)
    {
        usestr.append(isSectorInUse(TSPair(track,sec))?QString::number(sec,16).toUpper():"-");
    }
    return usestr;
}


