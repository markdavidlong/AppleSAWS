#include "diskfile.h"

#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QDebug>

#include "tracksectorlist.h"
#include "applesoftfile.h"
#include "binaryfile.h"
#include "relocatablefile.h"
#include "textfile.h"

DiskFile::DiskFile(QString filename)
{
    if (!filename.isEmpty())
    {
        read(filename);
    }
}

DiskFile::~DiskFile()
{
    foreach (GenericFile *file, m_files)
    {
        delete file;
    }
}

bool DiskFile::read(QString filename)
{
    m_imageName = QFileInfo(filename).fileName();
    if (m_imageName.toUpper().contains(".D13"))
    {
        m_sectors_per_track = 13;
    }
    else
    {
        m_sectors_per_track = 16;
    }

    QFile infile(filename);
    QCryptographicHash hash(QCryptographicHash::Md5);

    if (infile.open(QIODevice::ReadOnly))
    {
        QByteArray contents = infile.readAll();
        QDataStream qds(contents);
        for (int track = 0; track < 35; track++)
        {
            for (int sector = 0; sector < m_sectors_per_track; sector++)
            {
                char buffer[256];
                if (qds.readRawData(buffer,256) == 256)
                {
                    Sector sec;
                    sec.setTrackSector(track,sector);
                    sec.setData(QByteArray(buffer,256));
                    m_contents[track][sector] = sec;
                }
                else
                {
                    qDebug() << "Invalid sector read!";
                    return false;
                }
            }
        }
        hash.addData(contents);

        m_hash = hash.result();
        qDebug() << "Hash: " << m_hash;

        return true;
    }
    else
        qDebug() << "Could not open file " << filename;
    return false;
}

VTOC DiskFile::getVTOC()
{
    return getSector(17,0).promoteToVTOC();
}

QList<CatalogSector> DiskFile::getCatalogSectors()
{
    QList<CatalogSector> retval;
    VTOC vtoc = getVTOC();
    TSPair ts = vtoc.firstCatalogSector();

    CatalogSector cs = getSector(ts).promoteToCatalogSector();
    retval.append(cs);
    while (cs.nextCatalogSector() != TSPair(0,0)) {
        ts = cs.nextCatalogSector();
        cs = getSector(ts).promoteToCatalogSector();
        retval.append(cs);
    }
    return retval;
}

GenericFile *DiskFile::getFile(FileDescriptiveEntry fde)
{
    GenericFile *retval = 0;
    if (m_files.contains(fde))
    {
        retval = m_files[fde];
    }
    else
    {
        TrackSectorList tsl = getSector(fde.firstTSListSector).promoteToTrackSectorList();
        QByteArray data = getDataFromTrackSectorList(tsl);

        if (fde.fileType() == "A")
        {
            retval = new ApplesoftFile(data);
        }
        else if (fde.fileType() == "B")
        {
            retval = new BinaryFile(data);
        }
        else if (fde.fileType() == "R")
        {
            retval = new RelocatableFile(data);
        }
        else if ((fde.fileType() == "T"))
        {
            retval = new TextFile(data);
        }
        else
        {
            retval = new GenericFile(data);
        }
        m_files[fde] = retval;
    }
    return retval;
}


QByteArray DiskFile::getDataFromTrackSectorList(TrackSectorList tsl)
{
    QByteArray retval;

    foreach(TSPair pair, tsl.getDataTSPairs())
    {
        Sector sec = getSector(pair);
        retval.append(sec.rawData());
    }

    if (tsl.getNextTSList() != TSPair(0,0)) {
        TrackSectorList nextTsl = getSector(tsl.getNextTSList()).promoteToTrackSectorList();
        retval.append(getDataFromTrackSectorList(nextTsl));
    }

    return retval;
}

QList<FileDescriptiveEntry> DiskFile::getAllFDEs() {
    QList<FileDescriptiveEntry> retval;

    QList<CatalogSector>  sectors = getCatalogSectors();

    foreach (CatalogSector cs, sectors)
    {
        QList<FileDescriptiveEntry> fdes = cs.getFDEs();
        retval.append(fdes);
    }
    return retval;
}

