#include "Vtoc.h"

#include <QDebug>
#include <QString>

#include "Sector.h"
#include "util.h"

VTOC::VTOC(Sector *data)
{
    m_data = data;
}

TSPair VTOC::firstCatalogSector() {
  //  return TSPair(0x11,0x0f); // Force to look at the normal location
    return TSPair(m_data->rawData()[0x01], m_data->rawData()[0x02]);
}

quint8 VTOC::dosVersion() {
    return m_data->rawData()[0x03];
}

quint8 VTOC::volumeNumber() {
    return m_data->rawData()[0x06];
}

quint8 VTOC::maxTSPairs() {
    return m_data->rawData()[0x27];
}

quint8 VTOC::lastTrackAllocated() {
    return m_data->rawData()[0x30];
}

qint8 VTOC::directionOfAllocation() {
    return m_data->rawData()[0x31];
}

quint8 VTOC::tracksPerDisk() {
    return m_data->rawData()[0x34];
}

quint8 VTOC::sectorsPerDisk() {
    return m_data->rawData()[0x35];
}

qint16 VTOC::bytesPerSector() {
    return makeWord(m_data->rawData()[0x36],
                    m_data->rawData()[0x37]);
}

bool VTOC::isSectorInUse(TSPair ts) {
    quint8 track = ts.track();
    quint8 sec = ts.sector();
    quint8 baseaddr = (track * 4) + 0x38;

    //quint16 word = (((quint16) m_data->rawData()[baseaddr]) *256) + (quint8) m_data->rawData()[baseaddr+1];
    quint16 word = makeWord(m_data->rawData()[baseaddr+1],
                            m_data->rawData()[baseaddr]);
    quint16 bitpos = (quint16) 0x01 << (quint16) sec;

    return !(word & bitpos);
}

void VTOC::dump()
{
    /*
    for (qint8 idx = 0x0f; idx >= 0; idx--) {
        quint8 shift;
        if (idx < 0x08) { shift = idx; } else { shift = idx-0x08; }
        qDebug() << "Idx: " << idx << "Shift: " << (quint8) shift << "Bitpos: " << (quint8) (0x01 << shift);
    }
*/
    qDebug() << "Dumping VTOC Track " << m_data->track() << "Sector " << m_data->sector() << " ...";
    qDebug() << "   Track number of first catalog sector: " << QString::number(firstCatalogSector().track());
    qDebug() << "   Sector number of first catalog sector: " << QString::number(firstCatalogSector().sector());
    qDebug() << "   Release number of DOS used to INIT disk: " << QString::number(dosVersion());
    qDebug() << "   Disk Volume Number: " << QString::number(volumeNumber());
    qDebug() << "   Max track/sector pairs that fit in t/s list sector (122=256): " << QString::number(maxTSPairs());
    qDebug() << "   Last track where sectors were allocated: " << QString::number(lastTrackAllocated());
    qDebug() << "   Direction of track allocations (+/- 1): " << QString::number(directionOfAllocation());
    qDebug() << "   Number tracks per disk: " << QString::number(tracksPerDisk());
    qDebug() << "   Number sectors per disk: " << QString::number(sectorsPerDisk());
    qDebug() << "   Number bytes/sector: " << QString::number(bytesPerSector());
    qDebug() << "   Track Usage (.=free, 0-F=used):";
    for (quint8 track = 0; track < m_data->rawData()[0x34];track++)
    {
        qDebug() << "      " << QString("Track %1:").arg((int) track,2,10,QChar('0')) << buildUseString(track);
    }
 }

QString VTOC::buildUseString(quint8 track) {
    QString usestr;
    for (qint8 sec = 0x0f; sec >= 0; sec--)
    {
        usestr.append(isSectorInUse(TSPair(track,sec))?QString::number(sec,16).toUpper():"-");
    }
    return usestr;
}


