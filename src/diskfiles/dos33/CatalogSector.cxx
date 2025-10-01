#include "CatalogSector.h"
#include "Sector.h"
#include "AppleString.h"


CatalogSector::CatalogSector(Sector* data) : m_data(data), m_next(0, 0)
{
 //   qDebug() << "### Start CatalogSector ctor";

    TSPair next(m_data->rawData()[0x01],m_data->rawData()[0x02]);

    if (next.isValid() && next.track() == 17)
    {
       // next.dump();
    //    qDebug("Next track sector is valid.");
        m_next = next;
    }
    else  // Don't add if we've hit the last catalog sector (no next because next -> t0,s0)
    {
     //   qWarning() << "Track sector " << next.track() << "," << next.sector() << "is invalid! Not adding to catalog.";
     //   m_next.dump();
    }
    //m_next.setTrack(m_data->rawData()[0x01]);
    //m_next.setSector(m_data->rawData()[0x02]);

    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = makeFDE(idx*0x23+0x0B);
        if (fde.firstTSListSector() != TSPair(0,0)) {
            if (fde.firstTSListSector().isValid())
            {
                m_fdes.append(fde);
            }
            else qDebug() << "Not appending invalid TSPair.";
        }
    //    else { qWarning("fde.firstTSListSector() is 0,0"); }
    }
  //  qDebug() << "### End CatalogSector ctor";
}

void CatalogSector::dumpFDEs() const {
    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = m_fdes[idx];
        if (fde.firstTSListSector() != TSPair(0,0)) {
            qDebug() << "FDE #"<<idx;
            fde.dump();
        }
    }
}

FileDescriptiveEntry CatalogSector::makeFDE(int offset) const
{
    FileDescriptiveEntry fde;
    fde.firstTSListSector().setTrack(m_data->rawData()[offset + 0x00]);
    fde.firstTSListSector().setSector(m_data->rawData()[offset + 0x01]);
    fde.fileTypeAndFlags = m_data->rawData()[offset + 0x02];
    fde.lengthInSectors = makeWord( m_data->rawData()[offset + 0x21],
                                     m_data->rawData()[offset + 0x22]);
    
    if (fde.lengthInSectors > 16*35)
    {
        fde.lengthInSectors = -1;
    }

    for (int idx = 0x03; idx <= 0x20; idx++) {
        fde.filename.append(m_data->rawData()[idx+offset]);
    }

    if (fde.firstTSListSector().track() == 0xFF)
    {
        //TODO: Double check this stuff. applevision.dsk is a good example.
        qDebug() << "File" << fde.filename.printable() << "is deleted";
        fde.deleted = true;
        qDebug() << fde.filename;
        fde.firstTSListSector().setTrack(m_data->rawData()[offset + 0x20]);
        qDebug() << "   New track: " <<  (quint8) fde.firstTSListSector().track();
        qDebug() << "   Sector: " << fde.firstTSListSector().sector();
    }

    return fde;
}

FileDescriptiveEntry& CatalogSector::getFDE(quint8 number) {
    if (number >= m_fdes.length()) {
        number = m_fdes.length() - 1;
    }
    return m_fdes[number];
}

const FileDescriptiveEntry& CatalogSector::getFDE(quint8 number) const {
    if (number >= m_fdes.length()) {
        number = m_fdes.length() - 1;
    }
    return m_fdes[number];
}

QList<FileDescriptiveEntry> CatalogSector::getFDEs() const {
    return m_fdes;
}

TSPair CatalogSector::nextCatalogSector() const noexcept {
    return m_next;
}

Sector* CatalogSector::getSector() const noexcept {
    return m_data;
}
