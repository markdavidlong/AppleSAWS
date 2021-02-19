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

#include "rawdiskimage.h"

#include <QFileInfo>
#include <QFile>
#include <QCryptographicHash>
#include <QDataStream>

RawDiskImage::RawDiskImage(QString name)
{
    setCustomOrderMap0(dosOrderMap());
    setCustomOrderMap1(prodosOrderMap());

    // Set everything up assuming a 140k DOS image as a default for now

    m_track_count = 35;
    m_sector_size = 256;
    m_sectors_per_track = 16;
    m_sector_order = SectorOrder::Dos33;
    m_empty_image = true;

    if (!name.isEmpty())
    {
        read(name);
    }
}

RawDiskImage::~RawDiskImage()
{
    foreach (auto track, m_tracks)
    {
        while (track.count())
        {
            auto sectorblock = track.takeFirst();
            delete sectorblock;
        }
    }
}

bool RawDiskImage::read(QString filename)
{
    if (!isEmpty()) return false;  // Don't reread a disk!

    m_full_filename = filename;
    m_filename = QFileInfo(filename).fileName();

    if (m_filename.toUpper().contains(".D13"))
    {
        setSectorsPerTrack(13);
    }
    else
    {
        setSectorsPerTrack(16);
    }

    QFile infile(filename);
    QCryptographicHash hash(QCryptographicHash::Md5);


    if (infile.open(QIODevice::ReadOnly))
    {
        QByteArray contents = infile.readAll();


        int expectedsize = sectorsPerTrack() * numTracks() * 256;
        if (contents.size() != expectedsize)
        {
            if (contents.size() == 35*16*256) { m_sectors_per_track = 16; }
            else if (contents.size() == 35*13*256) { m_sectors_per_track = 13; }
            else qWarning() << QString("Size mismatch in file!  Expected %1, got %2")
                               .arg(expectedsize)
                               .arg(contents.size());
        }

        QDataStream qds(contents);
        for (int track = 0; track < numTracks(); track++)
        {
            TrackData td;
            for (int sector = 0; sector < m_sectors_per_track; sector++)
            {
                char buffer[256];
                if (qds.readRawData(buffer,sectorSize()) == sectorSize())
                {
                    TSPair tmpts(track,sector);
                    SectorData *newSec = new SectorData();
                    *newSec = QByteArray(buffer,sectorSize());
                    td.append(newSec);
                }
                else
                {
                    qDebug() << "Invalid sector read!";
                    return false;
                }
            }
            m_tracks.append(td);
        }
        hash.addData(contents);

        m_hash = hash.result();

        return true;
    }
    else
    {
        qDebug() << "Could not open file " << filename;
        return false;
    }
}

bool RawDiskImage::isEmpty() const { return m_empty_image; }

HashValue RawDiskImage::hash() const { return m_hash; }

QString RawDiskImage::diskImageName() const { return m_filename; }

QString RawDiskImage::fullDiskImageName() const { return m_full_filename; }

void RawDiskImage::setNumTracks(int count) { m_track_count = count; }

int RawDiskImage::numTracks() const { return m_track_count; }

bool RawDiskImage::hasSectors() const {return true; }

int  RawDiskImage::sectorSize() const { return m_sector_size; }

void RawDiskImage::setSectorSize(int size) { m_sector_size = size; }

void RawDiskImage::setSectorsPerTrack(int spt) { m_sectors_per_track = spt; }

int  RawDiskImage::sectorsPerTrack() const { return m_sectors_per_track; }

void RawDiskImage::setSectorOrder(SectorOrder order) { m_sector_order = order; }

SectorOrder RawDiskImage::sectorOrder() const { return m_sector_order; }

bool RawDiskImage::reorderSectorsTo(SectorOrder)
{
    //TODO: Handle Reordering
    return false;
}

SectorData *RawDiskImage::sectorAt(int track, int sector)
{
    if (track >= numTracks() || sector >= sectorsPerTrack())
    {
        return nullptr;
    }

    return m_tracks[track][sector];
}

SectorData *RawDiskImage::sectorAt(TSPair ts)
{
    return sectorAt(ts.track(),ts.sector());
}

SectorOrderMap RawDiskImage::dosOrderMap() const  {
    return QByteArrayLiteral("\x0\xD\xB\x9\x7\x5\x3\x1\xE\xC\xA\x8\x6\x4\x2\xF");
}

SectorOrderMap RawDiskImage::prodosOrderMap() const {
    return QByteArrayLiteral("\x0\x8\x1\x9\x2\xA\x3\xB\x4\xC\x5\xD\x6\xE\x7\xF");
}

SectorOrderMap RawDiskImage::pascalOrderMap() const {
    return QByteArrayLiteral("\x0\x2\x4\x6\x8\xA\xC\xE\x1\x3\x5\x7x9xBxDxF");
}

SectorOrderMap RawDiskImage::cpmOrderMap() const {
    return QByteArrayLiteral("\x0\x3\x6\x9\xC\xF\x2\x5\x8\xB\xE\x1\x4\x7\xA\xD");
}

SectorOrderMap RawDiskImage::customOrderMap0() const { return m_custom_order_map_0; }

SectorOrderMap RawDiskImage::customOrderMap1() const { return m_custom_order_map_0; }

bool RawDiskImage::setCustomOrderMap0(SectorOrderMap map)
{
    if (map.count() < m_sectors_per_track)
    {
        return false;
    }

    m_custom_order_map_0 = map.left(m_sectors_per_track);
    return true;
}

bool RawDiskImage::setCustomOrderMap1(SectorOrderMap map)
{
    if (map.count() < m_sectors_per_track)
    {
        return false;
    }

    m_custom_order_map_1 = map.left(m_sectors_per_track);
    return true;
}
