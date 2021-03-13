#ifndef TRACKSECTORLIST_H
#define TRACKSECTORLIST_H

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

#include "tspair.h"

#include <QSharedPointer>

class Sector;

using TSPairList = QList<TSPair>;

class TrackSectorList
{
public:
    TrackSectorList(QSharedPointer<const Sector> data);

    TSPair getNextTSList() const { return m_next_tslist; }
    bool isNextTSListValid() const;

    TSPair getSectorOffset() const { return m_sector_offset; }
    bool isSectorOffsetValid() const;

    TSPairList getDataTSPairs() const { return m_ts_pairs_for_data; }
    TSPairList getValidTSPairs() const;


private:
    TSPair m_next_tslist;
    TSPair m_sector_offset;
    TSPairList m_ts_pairs_for_data;

    QSharedPointer<const Sector> m_data;
};

#endif // TRACKSECTORLIST_H
