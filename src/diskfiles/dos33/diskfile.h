#pragma once

#include "util.h"
#include "sector.h"
#include "vtoc.h"

#include <QtGlobal>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>

class GenericFile;

class DiskFile
{
public:
    DiskFile(QString filename = "");
    ~DiskFile();

    bool read(QString filename);

    Sector &getSector(TSPair ts) { return getSector(ts.track(), ts.sector()); }

    Sector &getSector(int track, int sector) {
        return m_contents[track][sector];
    }

    VTOC getVTOC();

    QList<CatalogSector> getCatalogSectors();

    GenericFile *getFile(FileDescriptiveEntry fde);

    QByteArray getDataFromTrackSectorList(TrackSectorList tsl);

    QList<FileDescriptiveEntry> getAllFDEs();

    QByteArray fileHash() const { return m_hash; }

    QString getDiskImageName() const { return m_imageName; }
    QString getFullDiskImageName() const { return m_fullImageName; }
    QString getMetaDataPath() const;

private:

    QMap< int, QMap< int, Sector> > m_contents;
    QMap<FileDescriptiveEntry,GenericFile *> m_files;
    QByteArray m_hash;

    QString m_imageName;
    QString m_fullImageName;

    quint8 m_sectors_per_track;

};

