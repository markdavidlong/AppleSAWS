#ifndef VTOC_H
#define VTOC_H

#include <Qt>
#include "util.h"
#include "tspair.h"

class Sector;
class QString;

class VTOC
{
public:
    VTOC(Sector *data);

    void   dump();
    TSPair firstCatalogSector();
    quint8 dosVersion();
    quint8 volumeNumber();
    quint8 maxTSPairs();
    quint8 lastTrackAllocated();
    qint8  directionOfAllocation();
    quint8 tracksPerDisk();
    quint8 sectorsPerDisk();
    qint16 bytesPerSector();

    bool   isSectorInUse(TSPair ts) const;

    QList<TSPair> sectorsInUse() const;
    QList<TSPair> sectorsNotInUse() const;


private:
    QString buildUseString(quint8 track);

    Sector *m_data;
};

#endif // VTOC_H
