#ifndef DISKFILE_H
#define DISKFILE_H

#include <Qt>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>

#include "util.h"
#include "sector.h"
#include "vtoc.h"

#include "genericfile.h"

class DiskFile
{
public:
    DiskFile(QString filename = "");
    ~DiskFile();

    bool read(QString filename);

    Sector &getSector(TSPair ts) { return getSector(ts.track, ts.sector); }

    Sector &getSector(int track, int sector) {
        return m_contents[track][sector];
    }

    VTOC getVTOC();

    QList<CatalogSector> getCatalogSectors();

    GenericFile *getFile(FileDescriptiveEntry fde);

    QByteArray getDataFromTrackSectorList(TrackSectorList tsl);

    QList<FileDescriptiveEntry> getAllFDEs();

    QByteArray fileHash() const { return m_hash; }


private:

    QMap< int, QMap< int, Sector> > m_contents;
    QMap<FileDescriptiveEntry,GenericFile *> m_files;
    QByteArray m_hash;

};

#endif // DISKFILE_H
