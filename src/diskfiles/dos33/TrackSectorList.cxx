#include "TrackSectorList.h"

#include "Sector.h"

TrackSectorList::TrackSectorList(Sector *data)
{
    m_data = data;

    m_next_tslist.setTrack(m_data->rawData()[0x01]);
    m_next_tslist.setSector(m_data->rawData()[0x02]);

    m_sector_offset.setTrack(m_data->rawData()[0x05]);
    m_sector_offset.setSector(m_data->rawData()[0x06]);

    for (int idx = 0x0C; idx < 0xff; idx+=2)
    {
        TSPair ts(m_data->rawData()[idx],m_data->rawData()[idx+1]);
        if (ts == TSPair(0,0) && ts.isValid()) {
            break;
        } else {
            m_ts_pairs_for_data.append(ts);
        }
    }
}
