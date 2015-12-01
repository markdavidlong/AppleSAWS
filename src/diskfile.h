#ifndef DISKFILE_H
#define DISKFILE_H

#include <Qt>
#include <QMap>
#include <QString>
#include <QDebug>

#include "util.h"
#include "sector.h"
#include "vtoc.h"



class DiskFile
{
public:
    DiskFile(QString filename = "");

    bool read(QString filename);

    Sector &getSector(TSPair ts) { return getSector(ts.track, ts.sector); }

    Sector &getSector(int track, int sector) {
        return m_contents[track][sector];
    }

    VTOC getVTOC();

    QList<CatalogSector> getCatalogSectors();

    QByteArray getFile(FileDescriptiveEntry fde);

    QByteArray getDataFromTrackSectorList(TrackSectorList tsl);

    QList<FileDescriptiveEntry> getAllFDEs();


private:

    QMap< int, QMap< int, Sector> > m_contents;

};

#endif // DISKFILE_H
