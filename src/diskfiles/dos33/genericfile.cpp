/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

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
            m_data_cache = rawData();
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


QByteArray GenericFile::rawData()
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

QByteArray GenericFile::peekFirstSector() const
{
    auto tsl = m_diskfile->getSector(m_fde.firstTSListSector())->asTrackSectorList();
    TSPairList pairs = tsl.getValidTSPairs();

    return m_diskfile->getSector(pairs.first())->rawData();

}

void GenericFile::initDataFromImage()
{
    if (!m_data_loaded)
    {
        if (m_diskfile)
        {
            auto tsl = m_diskfile->getSector(m_fde.firstTSListSector())->asTrackSectorList();
            TSPairList pairs = tsl.getValidTSPairs();
            m_data_loaded = true;
            m_data = m_diskfile->getDataFromTSPairList(pairs);
        }
    }
}
