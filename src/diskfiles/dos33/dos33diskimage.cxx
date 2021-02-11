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

Dos33DiskImage::Dos33DiskImage(QString filename)
{
    if (!filename.isEmpty())
    {
        read(filename);
    }
}

Dos33DiskImage::~Dos33DiskImage()
{
    foreach (GenericFile *file, m_files)
    {
        delete file;
    }
}

bool Dos33DiskImage::read(QString filename)
{
    m_fullImageName = filename;
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
        int expectedsize = sectorsPerTrack() * tracks() * 256;
        if (contents.size() != expectedsize)
        {
            if (contents.size() == 35*16*256) { m_sectors_per_track = 16; }
            else if (contents.size() == 35*13*256) { m_sectors_per_track = 13; }
            else qWarning() << QString("Size mismatch in file!  Expected %1, got %2")
                               .arg(expectedsize)
                               .arg(contents.size());
        }

        QDataStream qds(contents);
        for (int track = 0; track < 35; track++)
        {
            for (int sector = 0; sector < m_sectors_per_track; sector++)
            {
                char buffer[256];
                if (qds.readRawData(buffer,256) == 256)
                {
                    TSPair tmpts(track,sector);
                    Sector newSec;
                    newSec.setTrackSector(tmpts);
                    newSec.setData(QByteArray(buffer,256));
                    m_contents[tmpts] = newSec;
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
        //     qDebug() << "Hash: " << m_hash;

        return true;
    }
    else
        qDebug() << "Could not open file " << filename;
    return false;
}

VTOC Dos33DiskImage::getVTOC()
{
    return getSector(17,0).promoteToVTOC();
}

QList<CatalogSector> Dos33DiskImage::getCatalogSectors()
{
    //  qDebug() << "### Start getCatalogSector";

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
    //  qDebug() << "### End getCatalogSector";

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
        QByteArray data = getDataFromTrackSectorList(tsl);
        setFileType(fde.fileType());

        if (fileType() == "A")
        {
            retval = new ApplesoftFile(data);
        }
        else if (fileType() == "B")
        {
            retval = new BinaryFile(data);
        }
        else if (fileType() == "R")
        {
            retval = new RelocatableFile(data);
        }
        else if ((fileType() == "T"))
        {
            retval = new TextFile(data);
        }
        else if ((fileType() == "I"))
        {
            retval = new IntBasicFile(data);
        }
        else
        {
            retval = new GenericFile(data);
        }

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
            Sector sec = getSector(pair);
            retval.append(sec.rawData());
        }
        else
        {
            qWarning("Not adding data from invalid TSList");
        }
    }

    auto next = tsl.getNextTSList();
    if (next.isValid() && next != TSPair(0,0)) {
        TrackSectorList nextTsl = getSector(tsl.getNextTSList()).asTrackSectorList();
        retval.append(getDataFromTrackSectorList(nextTsl));
    }

    return retval;
}

QList<FileDescriptiveEntry> Dos33DiskImage::getAllFDEs()
{
    //   qDebug() << "### Start getAllFDEs";
    QList<FileDescriptiveEntry> retval;

    QList<CatalogSector>  sectors = getCatalogSectors();

    foreach (CatalogSector cs, sectors)
    {
        QList<FileDescriptiveEntry> fdes = cs.getFDEs();
        retval.append(fdes);
    }
    //   qDebug() << "### End getAllFDEs";
    return retval;
}

QString Dos33DiskImage::getMetaDataPath() const {
    QString path = QString("%1.metadata/").arg(getFullDiskImageName());

    QDir dir(path);
    dir.mkpath(".");

    return path;
}

