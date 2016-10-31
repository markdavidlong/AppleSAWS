#include "catalogsector.h"
#include "sector.h"

CatalogSector::CatalogSector(Sector *data)
{
    m_data = data;

    m_next.track = m_data->rawData()[0x01];
    m_next.sector = m_data->rawData()[0x02];

    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = makeFDE(idx*0x23+0x0B);
        if (fde.firstTSListSector != TSPair(0,0)) {
            m_fdes.append(fde);
//            qDebug() << "FDE #"<<idx;
//            fde.dump();
        }
    }
}

void CatalogSector::dumpFDEs() {
    for (int idx = 0; idx<7; idx++)
    {
        FileDescriptiveEntry fde = m_fdes[idx];
        if (fde.firstTSListSector != TSPair(0,0)) {
            qDebug() << "FDE #"<<idx;
            fde.dump();
        }
    }
}

FileDescriptiveEntry CatalogSector::makeFDE(int offset)
{
    FileDescriptiveEntry fde;
    fde.firstTSListSector.track = m_data->rawData()[offset + 0x00];
    fde.firstTSListSector.sector = m_data->rawData()[offset + 0x01];
    fde.fileTypeAndFlags = m_data->rawData()[offset + 0x02];
    //fde.lengthInSectors = m_data->rawData()[offset + 0x21] + (m_data->rawData()[offset + 0x22] * 256);
    fde.lengthInSectors = makeWord( m_data->rawData()[offset + 0x21], m_data->rawData()[offset + 0x22]);
    for (int idx = 0x03; idx <= 0x20; idx++) {
        fde.filename.append(m_data->rawData()[idx+offset]);
    }
    return fde;
}
