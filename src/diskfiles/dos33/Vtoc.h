#pragma once

#include "Util.h"
#include "TSPair.h"

#include <QtGlobal>
#include <QBitArray>

class Sector;
class QString;


struct VTOC
{
public:
    explicit VTOC(const Sector& data);

    int track{0};
    int sector{0};
    TSPair firstCatalogSector{0, 0};
    quint8 dosVersion{0};
    quint8 volumeNumber{0};
    quint8 maxTSPairs{0};
    quint8 lastTrackAllocated{0};
    qint8 directionOfAllocation{0};
    quint8 tracksPerDisk{0};
    quint8 sectorsPerDisk{0};
    qint16 bytesPerSector{0};
    
    [[nodiscard]] bool isSectorInUse(TSPair ts) const noexcept;
    void dump() const;

private:
    void makeSectorsInUse(const Sector& data, quint8 tracksPerDisk, quint8 sectorsPerDisk);
    [[nodiscard]] QString buildUseString(quint8 track) const;

    QBitArray m_sectorsInUse; // One bit per sector on disk.  1 = in use, 0 = free
};

