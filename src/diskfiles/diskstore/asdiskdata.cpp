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

#include "asdiskdata.h"

ASDiskData::ASDiskData()
{

}

ASDiskData::~ASDiskData()
{

}

void ASDiskData::setUseSectors(int numSectors, int numTracks)
{
    m_numsectors = numSectors;
    m_numtracks = numTracks;
    m_dataformat = DataFormat::Sectors;
}

void ASDiskData::setUseBlocks(int numBlocks)
{
    m_numblocks = numBlocks;
    m_dataformat = DataFormat::Blocks;
}

void ASDiskData::addSector(int track, int sector, QByteArray sectordata)
{
    auto offset = tsToOffset(track,sector);
    m_chunks[offset] = sectordata;
}

void ASDiskData::addBlock(int number, QByteArray blockdata)
{
    m_chunks[number] = blockdata;
}

ASDiskData::SectorData &ASDiskData::getSector(int track, int sector)
{
    auto offset = tsToOffset(track,sector);
    return m_chunks[offset];
}

ASDiskData::BlockData &ASDiskData::getBlock(int blocknum)
{
    return m_chunks[blocknum];
}

int ASDiskData::numTracks() const
{
    return m_numtracks;
}

int ASDiskData::numSectorsPerTrack() const
{
    return m_numsectors;
}

int ASDiskData::sectorSize() const
{
    return m_sectorsize;
}

int ASDiskData::numBlocks() const
{
    return m_numblocks;
}

int ASDiskData::blockSize() const
{
    return m_blocksize;
}

QDataStream &ASDiskData::read(QDataStream &dataStream)
{
    QString id;
    dataStream >> id;
    if (id == "ASDiskData")
    {
        quint32 version;
        dataStream >> version;

        if (version == 1)
        {
            bool doWeUseSectors;
            dataStream >> doWeUseSectors;
            if (doWeUseSectors)
            {
                m_dataformat = DataFormat::Sectors;
            }
            else
            {
                m_dataformat = DataFormat::Blocks;
            }
            dataStream >> m_sectorsize;
            dataStream >> m_blocksize;
            dataStream >> m_numsectors;
            dataStream >> m_numtracks;
            dataStream >> m_numblocks;
            dataStream >> m_filename;
            dataStream >> m_fstype;
            dataStream >> m_fsname;
            m_metadata.clear();
            dataStream >> m_metadata;
            m_chunks.clear();
            dataStream >> m_chunks;
            m_original_file_contents.clear();
            dataStream >> m_original_file_contents;
        }
        else
        {
            qWarning("Unrecognized version in ASDiskData stream!");
        }
    }
    else
    {
        qWarning("Cannot read ASDiskData from input stream!");
    }

    return dataStream;
}

QDataStream &ASDiskData::write(QDataStream &dataStream) const
{
    dataStream << "ASDiskData" << quint32(1); // ID & Version
    dataStream << (m_dataformat == DataFormat::Sectors);
    dataStream << m_sectorsize << m_blocksize;
    dataStream << m_numsectors << m_numtracks << m_numblocks;
    dataStream << m_filename;
    dataStream << m_fstype;
    dataStream << m_fsname;
    dataStream << m_metadata;
    dataStream << m_chunks;
    dataStream << m_original_file_contents;

    return dataStream;
}

void ASDiskData::setFSInfo(QString name, int fstypeval)
{
    m_fsname = name;
    m_fstype = fstypeval;
}

int ASDiskData::tsToOffset(int track, int sector)
{
    return (track * m_numsectors) + sector;
}

QDataStream &operator<<(QDataStream &out, const ASDiskData &outObject)
{
    return outObject.write(out);
}

QDataStream &operator>>(QDataStream &in, ASDiskData &inObject)
{
    return inObject.read(in);
}
