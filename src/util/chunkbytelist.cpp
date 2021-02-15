#include "chunkbytelist.h"

ChunkByteList::ChunkByteList() { }

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
    return size;
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

char ChunkByteList::operator[](int i) const
{
    return at(i);
}


