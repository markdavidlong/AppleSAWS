#ifndef TRACKSECTORLIST_H
#define TRACKSECTORLIST_H

#include "tspair.h"

class Sector;

class TrackSectorList
{
public:
    TrackSectorList(Sector *data);

    TSPair getNextTSList() const { return m_next_tslist; }
    bool isNextTSListValid() const;

    TSPair getSectorOffset() const { return m_sector_offset; }
    bool isSectorOffsetValid() const;

    QList<TSPair> getDataTSPairs() const { return m_ts_pairs_for_data; }
    QList<TSPair> getValidTSPairs() const;


private:
    TSPair m_next_tslist;
    TSPair m_sector_offset;
    QList<TSPair> m_ts_pairs_for_data;

    Sector *m_data;
};

#endif // TRACKSECTORLIST_H
