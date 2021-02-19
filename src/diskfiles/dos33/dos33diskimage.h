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
#include "tracksectorlist.h"

#include "rawdiskimage.h"
#include "chunkbytelist.h"


class GenericFile;

using FileList = QList<GenericFile *>;

class Dos33DiskImage
{
public:
    Dos33DiskImage(QString filename = "");
    Dos33DiskImage(RawDiskImage *rawImage);
    ~Dos33DiskImage();

    bool read(QString filename);

    Sector &getSector(TSPair ts);
    Sector &getSector(int track, int sector);

    VTOC getVTOC();

    QList<CatalogSector> getCatalogSectors();

    GenericFile *getFile(FileDescriptiveEntry fde);

    ChunkByteList getDataFromTrackSectorList(TrackSectorList tsl);
    ChunkByteList getDataFromTSPairList(TSPairList list);

    QList<FileDescriptiveEntry> getAllFDEs();

    QByteArray fileHash() const { return m_hash; }

    QString getDiskImageName() const { return m_disk_image->diskImageName(); }
    QString getFullDiskImageName() const { return m_disk_image->fullDiskImageName(); }
    QString getMetaDataPath() const;
    RawDiskImage *rawImage() const { return m_disk_image; }

    int sectorsPerTrack() const { return m_disk_image->sectorsPerTrack(); }
    int tracks() const { return m_disk_image->numTracks(); }

    QList<GenericFile *> fileList();

private:

    RawDiskImage *m_disk_image;

    QMap<TSPair, Sector> m_contents;
    QMap<FileDescriptiveEntry, GenericFile *> m_files;
    QByteArray m_hash;

    Sector m_dummy_sector; // Returned for non-existant sectors on disk


};

#endif // DOS33DISKIMAGE_H
