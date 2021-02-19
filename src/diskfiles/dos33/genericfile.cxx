#include "genericfile.h"


quint8 GenericFile::rawDataAt(int offset)
{
    return rawData().at(offset);
}

quint16 GenericFile::rawDataWordAt(int offset)
{
    return makeWord(dataAt(offset),dataAt(offset+1));
}

quint8 GenericFile::dataAt(int offset)
{
    if (offset >= data().size())
    {
        qWarning("Data offset (%d) > size (%d)",offset,data().size());
        return 0;
    }
    return data().at(offset);
}


quint16 GenericFile::dataWordAt(int offset)
{
    return makeWord(dataAt(offset),dataAt(offset+1));
}

QByteArray GenericFile::data()
{
    if (m_local_data.size() == 0)
    {
        if (m_data_cache.size() == 0)
        {
            m_data_cache = rawData().asQByteArray();
        }
    }
    else
    {
        return m_local_data;
    }
    return m_data_cache;
}

void GenericFile::setData(QByteArray data)
{
    m_local_data = data;
}

void GenericFile::resetToDefaultData()
{
    m_local_data.clear();
}


ChunkByteList &GenericFile::rawData()
{
    if (!m_data_loaded)
    {
        initDataFromImage();
    }
    return m_data;
}

void GenericFile::updateFromFDE(FileDescriptiveEntry &fde)
{
    fde.updateGenericFile(this);
    setLength(fde.lengthInSectors * m_diskfile->rawImage()->sectorSize());
}

SectorData *GenericFile::peekFirstSector() const
{
    SectorData *retval = nullptr;

    if (m_diskfile)
    {
        auto tsl = m_diskfile->getSector(m_fde.firstTSListSector()).asTrackSectorList();
        TSPairList pairs = tsl.getValidTSPairs();
        if (pairs.size())
        {
            retval = m_diskfile->getSector(pairs.first()).rawData();
        }
    }
    return retval;
}

void GenericFile::initDataFromImage()
{
    if (!m_data_loaded)
    {
        if (m_diskfile)
        {
            auto tsl = m_diskfile->getSector(m_fde.firstTSListSector()).asTrackSectorList();
            TSPairList pairs = tsl.getValidTSPairs();
            m_data_loaded = true;
            m_data = m_diskfile->getDataFromTSPairList(pairs);
        }
    }
}
