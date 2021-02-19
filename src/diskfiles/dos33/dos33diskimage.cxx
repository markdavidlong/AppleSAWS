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

#include "dos33diskimage.h"

#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

#include "tracksectorlist.h"
#include "genericfile.h"
#include "applesoftfile.h"
#include "binaryfile.h"
#include "IntBasicFile.h"
#include "relocatablefile.h"
#include "textfile.h"
#include "sector.h"

Dos33DiskImage::Dos33DiskImage(QString filename)
{
    m_disk_image = new RawDiskImage(filename);

    if (!filename.isEmpty())
    {
        read(filename);
        //TODO: Cross reference & dbl. check sec/track with VTOC
    }
    auto dummy_data = new SectorData;
    dummy_data->resize(256);
    m_dummy_sector.setData(dummy_data);
}

Dos33DiskImage::Dos33DiskImage(RawDiskImage *rawImage)
{
    m_disk_image = rawImage;
    //TODO: Cross reference & dbl. check sec/track with VTOC

}

Dos33DiskImage::~Dos33DiskImage()
{
    //    foreach (GenericFile *file, m_files)
    //    {
    //        delete file;
    //    }
}

bool Dos33DiskImage::read(QString filename)
{
    bool retval = m_disk_image->read(filename);

    if (retval)
    {
        for (auto tracknum = 0; tracknum < m_disk_image->numTracks(); tracknum++)
        {
            for (auto secnum = 0; secnum < m_disk_image->sectorsPerTrack(); secnum++)
            {
                TSPair tmpts(tracknum,secnum);
                Sector newSec;
                SectorData * data = m_disk_image->sectorAt(tmpts);
                newSec.setData(data);
                newSec.setTrackSector(tmpts);
                m_contents[tmpts] = newSec;
            }
        }
    }

    return retval;

    //    m_fullImageName = filename;
    //    m_imageName = QFileInfo(filename).fileName();
    //    QFile infile(filename);
    //    QCryptographicHash hash(QCryptographicHash::Md5);

    //    if (infile.open(QIODevice::ReadOnly))
    //    {
    //        QByteArray contents = infile.readAll();
    //        int expectedsize = sectorsPerTrack() * tracks() * 256;
    //        if (contents.size() != expectedsize)
    //        {
    //            if (contents.size() == 35*16*256) { m_sectors_per_track = 16; }
    //            else if (contents.size() == 35*13*256) { m_sectors_per_track = 13; }
    //            else qWarning() << QString("Size mismatch in file!  Expected %1, got %2")
    //                               .arg(expectedsize)
    //                               .arg(contents.size());
    //        }

    //        QDataStream qds(contents);
    //        for (int track = 0; track < 35; track++)
    //        {
    //            for (int sector = 0; sector < m_sectors_per_track; sector++)
    //            {
    //                char buffer[256];
    //                if (qds.readRawData(buffer,256) == 256)
    //                {
    //                    TSPair tmpts(track,sector);
    //                    Sector newSec;
    //                    newSec.setTrackSector(tmpts);
    //                    newSec.setData(QByteArray(buffer,256));
    //                    m_contents[tmpts] = newSec;
    //                }
    //                else
    //                {
    //                    qDebug() << "Invalid sector read!";
    //                    return false;
    //                }
    //            }
    //        }
    //        hash.addData(contents);

    //        m_hash = hash.result();
    //        //     qDebug() << "Hash: " << m_hash;

    //        return true;
    //    }
    //    else
    //        qDebug() << "Could not open file " << filename;
    //    return false;
}

Sector &Dos33DiskImage::getSector(TSPair ts) {
    if (m_contents.contains(ts))
        return m_contents[ts];
    else
        return m_dummy_sector;
}

Sector &Dos33DiskImage::getSector(int track, int sector)  {
    return getSector(TSPair(track,sector));
}

VTOC Dos33DiskImage::getVTOC()
{
    return getSector(17,0).promoteToVTOC();
}

QList<CatalogSector> Dos33DiskImage::getCatalogSectors()
{
    QList<CatalogSector> retval;
    VTOC vtoc = getVTOC();
    TSPair ts = vtoc.firstCatalogSector();

    CatalogSector cs = getSector(ts).asCatalogSector();
    retval.append(cs);
    while (cs.nextCatalogSector() != TSPair(0,0)) {
        ts = cs.nextCatalogSector();
        cs = getSector(ts).asCatalogSector();
        retval.append(cs);
    }

    return retval;
}

GenericFile *Dos33DiskImage::getFile(FileDescriptiveEntry fde)
{
    GenericFile *retval = 0;
    if (m_files.contains(fde))
    {
        retval = m_files[fde];
    }
    else
    {
        if (!fde.firstTSListSector().isValid())
        {
            qWarning("  Not returning a file from invalid TSList!");
            return nullptr;
        }

        TrackSectorList tsl = getSector(fde.firstTSListSector()).asTrackSectorList();
        if (!fde.firstTSListSector().isValid())
        {
            qWarning("  Not returning a file from invalid TSList!");
            return nullptr;
        }

        auto type = fde.fileTypeIdentifier();

        if (type == "A")
        {
            retval = new ApplesoftFile(this,fde);
        }
        else if (type == "B")
        {
            retval = new BinaryFile(this,fde);
        }
        else if (type == "R")
        {
            retval = new RelocatableFile(this,fde);
        }
        else if ((type == "T"))
        {
            retval = new TextFile(this,fde);
        }
        else if ((type == "I"))
        {
            retval = new IntBasicFile(this,fde);
        }
        else
        {
            retval = new GenericFile(this,fde);
        }
        retval->updateFromFDE(fde);
        m_files[fde] = retval;
    }
    if (retval) { retval->setDiskFile(this); }
    return retval;
}


ChunkByteList Dos33DiskImage::getDataFromTrackSectorList(TrackSectorList tsl)
{
    ChunkByteList retval;


    foreach(TSPair pair, tsl.getDataTSPairs())
    {
        if (pair.isValid())
        {
            Sector sec = getSector(pair);
            retval.appendChunk(sec.rawData());
        }
        else
        {
            qWarning("Not adding data from invalid TSList");
        }
    }

    auto next = tsl.getNextTSList();
    if (next.isValid() && next != TSPair(0,0)) {
        TrackSectorList nextTsl = getSector(tsl.getNextTSList()).asTrackSectorList();
        retval.appendChunkList(getDataFromTrackSectorList(nextTsl));
    }

    return retval;
}

ChunkByteList Dos33DiskImage::getDataFromTSPairList(TSPairList list)
{
    ChunkByteList retval;
    foreach(TSPair pair, list)
    {
        if (pair.isValid())
        {
            Sector sec = getSector(pair);
            retval.appendChunk(sec.rawData());
        }
        else
        {
            qWarning("Not adding data from invalid TSPairList");
        }
    }
    return retval;
}

QList<FileDescriptiveEntry> Dos33DiskImage::getAllFDEs()
{
    QList<FileDescriptiveEntry> retval;

    QList<CatalogSector>  sectors = getCatalogSectors();

    foreach (CatalogSector cs, sectors)
    {
        QList<FileDescriptiveEntry> fdes = cs.getFDEs();
        retval.append(fdes);
    }
    return retval;
}

QString Dos33DiskImage::getMetaDataPath() const {
    QString path = QString("%1.metadata/").arg(getFullDiskImageName());

    QDir dir(path);
    dir.mkpath(".");

    return path;
}

FileList Dos33DiskImage::fileList()
{
    return { };
}

