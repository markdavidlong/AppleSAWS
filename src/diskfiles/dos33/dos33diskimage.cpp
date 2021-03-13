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
   m_disk_image = new ASDiskData();
    if (!filename.isEmpty())
    {
        read(filename);
    }
}

Dos33DiskImage::~Dos33DiskImage()
{
    delete m_disk_image;
}

bool Dos33DiskImage::read(QString filename)
{
    ASDiskImporter importer;
    bool retval = importer.importImage(filename, *m_disk_image);

    // TODO: Cache sectors!!

    return retval;
}

pSector Dos33DiskImage::getSector(TSPair ts) const {
    return getSector(ts.track(),ts.sector());
}

pSector Dos33DiskImage::getSector(int track, int sector)  const {
    auto sectorData
            = m_disk_image->getSector(track,sector);
    qDebug() << "Dos33DiskImage::getSector("<<track<<","<<sector<<"); [SIZE = " << sectorData.size() << "]";
    auto val = QSharedPointer<Sector>(new Sector(sectorData, track, sector));
    val->dump();
    return val;
}

VTOC Dos33DiskImage::getVTOC() const
{
    return getSector(17,0)->promoteToVTOC();
}

QList<CatalogSector> Dos33DiskImage::getCatalogSectors()
{
    QList<CatalogSector> retval;
    VTOC vtoc = getVTOC();
    TSPair ts = vtoc.firstCatalogSector();
    qDebug() << "First TSPair for catalogsectors is " << ts.track() << "," << ts.sector();

    CatalogSector cs = getSector(ts)->asCatalogSector();
    retval.append(cs);
    while (cs.nextCatalogSector() != TSPair(0,0)) {
        ts = cs.nextCatalogSector();
        qDebug() << "Next TSPair for catalogsectors is " << ts.track() << "," << ts.sector();

        cs = getSector(ts)->asCatalogSector();
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

        TrackSectorList tsl = getSector(fde.firstTSListSector())->asTrackSectorList();
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


QByteArray Dos33DiskImage::getDataFromTrackSectorList(TrackSectorList tsl)
{
    QByteArray retval;


    foreach(TSPair pair, tsl.getDataTSPairs())
    {
        if (pair.isValid())
        {
            pSector sec = getSector(pair);
            retval.append(sec->rawData());
        }
        else
        {
            qWarning("Not adding data from invalid TSList");
        }
    }

    auto next = tsl.getNextTSList();
    if (next.isValid() && next != TSPair(0,0)) {
        TrackSectorList nextTsl = getSector(tsl.getNextTSList())->asTrackSectorList();
        retval.append(getDataFromTrackSectorList(nextTsl));
    }

    return retval;
}

QByteArray Dos33DiskImage::getDataFromTSPairList(TSPairList list)
{
    QByteArray retval;
    foreach(TSPair pair, list)
    {
        if (pair.isValid())
        {
            pSector sec = getSector(pair);
            retval.append(sec->rawData());
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

