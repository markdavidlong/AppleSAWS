#ifndef CHUNKBYTELIST_H
#define CHUNKBYTELIST_H

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
