#pragma once

#include "Util.h"
#include "TSPair.h"

class Sector;

class TrackSectorList
{
public:
    explicit TrackSectorList(Sector* data);

    [[nodiscard]] TSPair getNextTSList() const noexcept { return m_next_tslist; }
    [[nodiscard]] TSPair getSectorOffset() const noexcept { return m_sector_offset; }

    [[nodiscard]] const QList<TSPair>& getDataTSPairs() const noexcept { return m_ts_pairs_for_data; }

private:
    Sector* m_data{nullptr};
    TSPair m_next_tslist{0,0};
    TSPair m_sector_offset{0,0};
    QList<TSPair> m_ts_pairs_for_data;
};

