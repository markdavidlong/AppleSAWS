#ifndef CHUNKBYTELIST_H
#define CHUNKBYTELIST_H

#include <QByteArray>
#include <QList>

class ChunkByteList
{
public:
    ChunkByteList();

    ChunkByteList &appendChunk(QByteArray *chunk);
    ChunkByteList &appendChunkList(ChunkByteList other);

    int count() const;
    int size() const;
    int length() const;

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

    operator QByteArray()  { return asQByteArray(); }

private:
    QList<QByteArray *> m_chunk_list;
};


#endif // CHUNKBYTELIST_H
