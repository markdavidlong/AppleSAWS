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

#ifndef ASDISKDATA_H
#define ASDISKDATA_H

#include <QString>
#include <QVector>
#include <QVariant>
#include <QDataStream>

class ASDiskData : public QObject
{
    Q_OBJECT

    using AttributeMap = QMap<QString, QVariant>;
    using ChunkData    = QByteArray;


public:
 //   using SectorData   = QByteArray;
 //   using BlockData    = QByteArray;

    enum class DataFormat { Sectors, Blocks };

    ASDiskData(QObject *parent = nullptr);
    virtual ~ASDiskData();

    QStringList attributeList() const { return m_metadata.keys(); }
    QVariant getAttribute(QString key) const;
    bool setAttribute(QString key, QVariant value);

    void setUseSectors(int numTracks = 35, int numSectors = 16);
    void setUseBlocks(int numBlocks);

    bool useSectors() const { return m_dataformat == DataFormat::Sectors; }
    bool useBlocks() const  { return m_dataformat == DataFormat::Blocks; }
    DataFormat dataFormat() const { return m_dataformat; }

    void addSector(int track, int sector, QByteArray sectordata);
    void addBlock(int number, QByteArray blockdata);

    QByteArray getSector(int track, int sector) const;
    QByteArray getBlock(int blocknum) const;

    int  numTracks() const;
    int  numSectorsPerTrack() const;
    void setNumSectorsPerTrack(int val) { m_numsectors = val; }
    void setNumTracks(int val) { m_numtracks = val; }
    void setSectorSize(int size) { m_sectorsize = size; }
    int  sectorSize() const;

    int  numBlocks() const;
    void setNumBlocks(int val) { m_numblocks = val; }
    void setBlockSize(int size) { m_blocksize = size; }
    int  blockSize() const;

    void setFilename(const QString &filename) { m_filename = filename; }
    const QString &filename() const { return m_filename; }

    void setFSInfo(QString name, int fstypeval);

    QString fileSystemName() const { return m_fsname; };
    int fileSystemType() const { return m_fstype; }

    void setOriginalFileContents(QByteArray data) { m_original_file_contents = data; }
    QByteArray originalFileContents() const { return m_original_file_contents; }

    QDataStream &read(QDataStream &dataStream);
    QDataStream &write(QDataStream &dataStream) const;

protected:
    int tsToOffset(int track, int sector) const;

protected:
    QString    m_filename     { "[unknown]" };
    QString    m_fsname       { "[unknown]" };
    int        m_fstype       { 0 };

    DataFormat m_dataformat   { DataFormat::Sectors };

    int        m_sectorsize   { 256 };
    int        m_blocksize    { 512 };

    int        m_numsectors   { -1 };
    int        m_numtracks    { -1 };
    int        m_numblocks    { -1 };

    AttributeMap m_metadata;

    QByteArray m_chunk_data;

    QByteArray m_original_file_contents;

    QByteArray m_nulldata;
};

QDataStream &operator<<(QDataStream &out, const ASDiskData &outData);

QDataStream &operator>>(QDataStream &in, ASDiskData &inData);



#endif // ASDISKDATA_H
