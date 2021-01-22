#ifndef TRACKSECTORLIST_H
#define TRACKSECTORLIST_H

#include "util.h"

class Sector;

class TrackSectorList
{
public:
    TrackSectorList(Sector *data);

    TSPair getNextTSList() const { return m_next_tslist; }
    TSPair getSectorOffset() const { return m_sector_offset; }

    QList<TSPair> getDataTSPairs() { return m_ts_pairs_for_data; }

private:

    TSPair m_next_tslist;
    TSPair m_sector_offset;
    QList<TSPair> m_ts_pairs_for_data;

    Sector *m_data;
};

#endif // TRACKSECTORLIST_H
