#include "tracksectorlist.h"

#include "sector.h"

TrackSectorList::TrackSectorList(Sector *data)
{
    m_data = data;

    m_next_tslist.track = m_data->rawData()[0x01];
    m_next_tslist.sector = m_data->rawData()[0x02];

    m_sector_offset.track = m_data->rawData()[0x05];
    m_sector_offset.sector = m_data->rawData()[0x06];

    for (int idx = 0x0C; idx < 0xff; idx+=2)
    {
        TSPair ts(m_data->rawData()[idx],m_data->rawData()[idx+1]);
        if (ts == TSPair(0,0)) {
            break;
        } else {
            m_ts_pairs_for_data.append(ts);
        }
    }
}
