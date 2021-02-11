#ifndef DOS33DISKIMAGE_H
#define DOS33DISKIMAGE_H

#include <Qt>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>

#include "util.h"
#include "sector.h"
#include "vtoc.h"

class GenericFile;

class Dos33DiskImage
{
public:
    Dos33DiskImage(QString filename = "");
    ~Dos33DiskImage();

    bool read(QString filename);

    Sector &getSector(TSPair ts) { return m_contents[ts]; }

    Sector &getSector(int track, int sector) {
        TSPair ts(track,sector);
        return getSector(ts);
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

    QString fileType() const { return m_fileType; }
    void setFileType(QString type) { m_fileType = type; }

    quint8 sectorsPerTrack() const { return m_sectors_per_track; }
    quint8 tracks() const { return 35; }

private:

    QMap<TSPair, Sector> m_contents;
    QMap<FileDescriptiveEntry, GenericFile *> m_files;
    QByteArray m_hash;

    QString m_imageName;
    QString m_fullImageName;

    QString m_fileType;

    quint8 m_sectors_per_track;

};

#endif // DOS33DISKIMAGE_H
