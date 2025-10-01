#include "TSPair.h"

void TSPair::setTrack(quint8 tracknum) noexcept
{
    if (tracknum > 34 && tracknum != 0xff)
    {
        qWarning("Setting a track with value %d (> 34 and not 256).", tracknum);
    }
    m_track = tracknum;
}

void TSPair::setSector(quint8 secnum) noexcept  
{
    if (secnum > 15 && m_track != 0xff)
    {
        qWarning("Setting a sector with value %d (> 15) on track %d.", secnum, m_track);
    }
    m_sector = secnum;
}

bool TSPair::isValid() const noexcept
{
    auto retval = (m_track != 0xff && m_track < 35) && m_sector < 16;
    return retval;
}

void TSPair::dump() const
{
    qDebug() << "TSPair: track:" << track() << "sector:" << sector();
}