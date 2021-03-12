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

#include <QDebug>

ASDiskData::ASDiskData(QObject *parent) : QObject(parent)
{

}

ASDiskData::~ASDiskData()
{

}

void ASDiskData::setUseSectors(int numTracks, int numSectors)
{
    m_numsectors = numSectors;
    m_numtracks = numTracks;
    m_dataformat = DataFormat::Sectors;
    m_chunk_data.resize(m_numsectors * m_numtracks * 256); // Todo: magic number
}

void ASDiskData::setUseBlocks(int numBlocks)
{
    m_numblocks = numBlocks;
    m_dataformat = DataFormat::Blocks;
    m_chunk_data.resize(m_numblocks * 512);// Todo: magic number
}

void ASDiskData::addSector(int track, int sector, QByteArray sectordata)
{
    if (sectordata.length() != 256 || track >= m_numtracks || sector >= m_numsectors
            || track < 0 || sector < 0)
    {
        qWarning("Invalid data in addSector.");
        return;
    }
    auto offset = tsToOffset(track,sector);
    m_chunk_data.replace(offset*256,256,sectordata);
}

void ASDiskData::addBlock(int number, QByteArray blockdata)
{
    if (blockdata.length() != 512 ||  number >= m_numblocks || number < 0)
    {
        qWarning("Invalid data in addBlock.");
        return;
    };
    m_chunk_data.replace(number*512,512,blockdata);
}

QByteArray ASDiskData::getSector(int track, int sector) const
{
    auto offset = tsToOffset(track,sector);

    if (offset >= m_chunk_data.size() / 256)
    {
        qWarning("Sector %d,%d out of bounds!  Returning null data!",track,sector);
        return QByteArray();
    }
    return m_chunk_data.mid(offset*256,256);
}

QByteArray ASDiskData::getBlock(int blocknum) const
{
    if (blocknum >= m_chunk_data.size() / 256)
    {
        qWarning("Block %d out of bounds!  Returning null data!",blocknum);
        return QByteArray();
    }
    return m_chunk_data.mid(blocknum*512,512);
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
            m_chunk_data.clear();
            dataStream >> m_chunk_data;
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
    dataStream << m_chunk_data;
    dataStream << m_original_file_contents;

    return dataStream;
}

void ASDiskData::setFSInfo(QString name, int fstypeval)
{
    m_fsname = name;
    m_fstype = fstypeval;
}

int ASDiskData::tsToOffset(int track, int sector) const
{
    int val =  (track * m_numsectors) + sector;
    //qDebug() << "TsToOffset: " << track << "," << sector << " = " << val;
    return val;
}

QDataStream &operator<<(QDataStream &out, const ASDiskData &outObject)
{
    return outObject.write(out);
}

QDataStream &operator>>(QDataStream &in, ASDiskData &inObject)
{
    return inObject.read(in);
}
