#ifndef CHUNKBYTELIST_H
#define CHUNKBYTELIST_H

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

#include <QByteArray>
#include <QList>

#include <QSharedPointer>

class ChunkByteList
{
public:
    ChunkByteList();

    ChunkByteList &appendChunk(QByteArray *chunk);
    ChunkByteList &appendChunkList(ChunkByteList other);

    void setPreambleLength(int bytes) { m_preamble_size = bytes; };
    int preambleLength() const { return m_preamble_size; };

    int count() const;
    int size() const;
    int length() const;

    char preambleData(int i) const
    {
        int offset = 0;
        foreach (auto chunk, m_chunk_list)
        {
            int upperbound = offset + chunk->size();
            if (i < upperbound)
            {
                return chunk->at(i-offset);
            }
            else
            {
                offset += chunk->size();
            }
        }
        return 0;
    }

    int actualSize() const { return count() + preambleLength(); }

    bool isEmpty() const;

    char at(int i) const;
    char operator[](int i) const;

    int numChunks() const { return m_chunk_list.size(); }
    QByteArray *getChunk(int chunk) const { return m_chunk_list[chunk]; }

   QByteArray asQByteArray() const {
        QByteArray retval;
        foreach (auto chunk, m_chunk_list)
        {
            retval.append(*chunk);
        }
        return retval;
    }

    explicit operator QByteArray()  { return asQByteArray(); }

private:
    QList<QByteArray *> m_chunk_list;
    int m_preamble_size;
};


#endif // CHUNKBYTELIST_H
