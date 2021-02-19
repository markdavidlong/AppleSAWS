#ifndef TSPAIR_H
#define TSPAIR_H

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



#include <Qt>
#include <QDebug>

class TSPair
{
public:
    TSPair() { m_track = m_sector = 0; }
    TSPair(int trackval, int secval) { m_track=trackval; m_sector = secval; }
    TSPair(QPair<int,int> pair) { m_track = pair.first; m_sector = pair.second;}

    void setTrack(int tracknum)
    {
        if (tracknum > 34 && tracknum != 0xff) {
            qWarning("Setting a track with value %d (> 34 and not 256).",tracknum);
        }
        m_track = tracknum;
    }

    void setSector(quint8 secnum)
    {
        if (secnum > 15 && m_track != 0xff) {
            qWarning("Setting a sector with value %d (> 15) on track %d.",
                     secnum, m_track);
        }
        m_sector = secnum;
    }

    bool isValid() const
    {
        auto retval = (m_track == 0xff || m_track < 35) && m_sector < 16;

        return retval;
    }

    int  track() const { return m_track; }
    int sector() const { return m_sector; }

    bool operator==(const TSPair &other) const {
        if (other.track() == track() && other.sector() == sector()) return true;
        return false;
    }

    bool operator!=(const TSPair &other) const {
        return !(operator==(other));
    }

    bool operator<(const TSPair &other) const {
        if (m_track == other.track())
        {
            return (m_sector < other.sector());
        }
        else
        {
            return (m_track < other.track());
        }
    }

    QPair<int,int> toQPair() { return QPair<quint8,quint8>(track(),sector()); }

    void dump() const {
        qDebug() << "TSPair: track: " << track() << " sector: " << sector();
    }
private:
    int m_track;
    int m_sector;
};
Q_DECLARE_METATYPE(TSPair);

#endif // TSPAIR_H
