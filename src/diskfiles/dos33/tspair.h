#ifndef TSPAIR_H
#define TSPAIR_H

#include <Qt>
#include <QDebug>

class TSPair
{
public:
    TSPair() { m_track = m_sector = 0; }
    TSPair(quint8 trackval, quint8 secval) { m_track=trackval; m_sector = secval; }
    TSPair(QPair<quint8,quint8> pair) { m_track = pair.first; m_sector = pair.second;}

    void setTrack(quint8 tracknum)
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
        auto retval= (m_track != 0xff && m_track < 35) && m_sector < 16;

        return retval; }

    quint8 track() const { return m_track; }
    quint8 sector() const { return m_sector; }

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

    QPair<quint8,quint8> toQPair() { return QPair<quint8,quint8>(track(),sector()); }

    void dump() const {
        qDebug() << "TSPair: track: " << track() << " sector: " << sector();
    }
private:
    quint8 m_track;
    quint8 m_sector;
};
Q_DECLARE_METATYPE(TSPair);

#endif // TSPAIR_H
