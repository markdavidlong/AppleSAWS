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

#include "chunkbytelist.h"

ChunkByteList::ChunkByteList()
{
    m_preamble_size = 0;
}

ChunkByteList &ChunkByteList::appendChunk(QByteArray *chunk)
{
    if (chunk)
    {
        m_chunk_list.append(chunk);
    }
    return *this;
}

ChunkByteList &ChunkByteList::appendChunkList(ChunkByteList other)
{
    if (&other != this)
    {
        for (auto count = 0; count < other.numChunks(); count++)
        {
            m_chunk_list.append(other.getChunk(count));
        }
    }
    return *this;
}

int ChunkByteList::count() const
{
    int size = 0;
    foreach (auto chunk, m_chunk_list)
    {
        size += chunk->size();
    }
    return size - preambleLength();
}

int ChunkByteList::size() const
{
    return count();
}

int ChunkByteList::length() const
{
    return count();
}

bool ChunkByteList::isEmpty() const
{
    if (m_chunk_list.size() == 0) return true;

    foreach (auto chunk, m_chunk_list)
    {
        if (!chunk->isEmpty()) return false;
    }
    return false;
}

char ChunkByteList::at(int i) const
{
    return preambleData(i + preambleLength());
}

char ChunkByteList::operator[](int i) const
{
    return at(i);
}


