#include "Vtoc.h"

#include "Sector.h"

#include <QDebug>
#include <QString>


VTOC::VTOC(const Sector &sectorData)
{
    track = sectorData.track();
    sector = sectorData.sector();
    firstCatalogSector = TSPair(sectorData.data().at(0x01), sectorData.data().at(0x02));
    dosVersion = sectorData.data().at(0x03);
    volumeNumber = sectorData.data().at(0x06);
    maxTSPairs = sectorData.data().at(0x27);
    lastTrackAllocated = sectorData.data().at(0x30);
    directionOfAllocation = (qint8) sectorData.data().at(0x31);
    tracksPerDisk = sectorData.data().at(0x34);
    sectorsPerDisk = sectorData.data().at(0x35);
    bytesPerSector = makeWord(sectorData.data().at(0x36),
                             sectorData.data().at(0x37));
    makeSectorsInUse(sectorData,tracksPerDisk,sectorsPerDisk);                             
}

void VTOC::makeSectorsInUse(const Sector &data, quint8 tracksPerDisk, quint8 sectorsPerDisk)
{
    m_sectorsInUse.resize(tracksPerDisk * sectorsPerDisk);
    m_sectorsInUse.fill(false);
    for (quint8 track = 0; track < tracksPerDisk; track++)
    {
        quint8 baseaddr = (track * 4) + 0x38;
        quint16 word = makeWord(data.data().at(baseaddr+1),
                                data.data().at(baseaddr));
        for (quint8 sec = 0; sec < sectorsPerDisk; sec++)
        {
            quint16 bitpos = (quint16) 0x01 << (quint16) sec;
            bool inuse = !(word & bitpos);
            m_sectorsInUse.setBit((track * sectorsPerDisk) + sec, inuse);
        }
    }
}

bool VTOC::isSectorInUse(TSPair ts) const {
    quint8 track = ts.track();
    quint8 sec = ts.sector();
    return m_sectorsInUse.testBit((track * sectorsPerDisk) + sec);
}

void VTOC::dump() const
{
    qDebug() << "Dumping VTOC Track " << track << "Sector " << sector << " ...";
    qDebug() << "   Track number of first catalog sector: " << QString::number(firstCatalogSector.track());
    qDebug() << "   Sector number of first catalog sector: " << QString::number(firstCatalogSector.sector());
    qDebug() << "   Release number of DOS used to INIT disk: " << QString::number(dosVersion);
    qDebug() << "   Disk Volume Number: " << QString::number(volumeNumber);
    qDebug() << "   Max track/sector pairs that fit in t/s list sector (122=256): " << QString::number(maxTSPairs);
    qDebug() << "   Last track where sectors were allocated: " << QString::number(lastTrackAllocated);
    qDebug() << "   Direction of track allocations (+/- 1): " << QString::number(directionOfAllocation);
    qDebug() << "   Number tracks per disk: " << QString::number(tracksPerDisk);
    qDebug() << "   Number sectors per disk: " << QString::number(sectorsPerDisk);
    qDebug() << "   Number bytes/sector: " << QString::number(bytesPerSector);
    qDebug() << "   Track Usage (.=free, 0-F=used):";
    for (quint8 track = 0; track < tracksPerDisk;track++)
    {
        qDebug() << "      " << QString("Track %1:").arg((int) track,2,10,QChar('0')) << buildUseString(track);
    }
 }

QString VTOC::buildUseString(quint8 track) const {
    QString usestr;
    for (qint8 sec = 0x0f; sec >= 0; sec--)
    {
        usestr.append(isSectorInUse(TSPair(track,sec))?QString::number(sec,16).toUpper():"-");
    }
    return usestr;
}


