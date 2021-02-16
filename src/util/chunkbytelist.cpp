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


