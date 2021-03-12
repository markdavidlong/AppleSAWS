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

#include "tracksectorlist.h"

#include "sector.h"

TrackSectorList::TrackSectorList(QSharedPointer<const Sector> data)
{
    m_data = data;

    m_next_tslist.setTrack(m_data->at(0x01));
    m_next_tslist.setSector(m_data->at(0x02));

    m_sector_offset.setTrack(m_data->at(0x05));
    m_sector_offset.setSector(m_data->at(0x06));

    for (int idx = 0x0C; idx < 0xff; idx+=2)
    {
        TSPair ts(m_data->at(idx),m_data->at(idx+1));
        if (ts == TSPair(0,0) && ts.isValid()) {
            break;
        } else {
            m_ts_pairs_for_data.append(ts);
        }
    }
}

bool TrackSectorList::isNextTSListValid() const
{
    return getNextTSList().isValid();
}

bool TrackSectorList::isSectorOffsetValid() const
{
    return getSectorOffset().isValid();
}

QList<TSPair> TrackSectorList::getValidTSPairs() const
{
    QList<TSPair> retval;

    foreach (auto item, m_ts_pairs_for_data)
    {
        if (item.isValid()) { retval.append(item); }
        else
        {
            break;
        }
    }

    return retval;
}
