#ifndef SECTOR_H
#define SECTOR_H

#include <QByteArray>

#include "vtoc.h"
#include "catalogsector.h"
#include "tracksectorlist.h"

class Sector
{
public:

    Sector() {
        m_data.resize(256);
        m_track = 255;
        m_sector = 255;
    }

    VTOC promoteToVTOC() {
        return VTOC(this);
    }

    CatalogSector promoteToCatalogSector() {
        return CatalogSector(this);
    }

    TrackSectorList promoteToTrackSectorList()  {
        return TrackSectorList(this);
    }

    int sector() { return m_sector; }
    int track() { return m_track; }

    void setTrackSector(int track, int sector) {
        setTrack(track);
        setSector(sector);
    }

    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    char&  operator[](uint offset);

    bool setData(QByteArray data);

    void dump();

    QByteArray rawData() { return m_data; }

private:
    QByteArray m_data;
    int m_track;
    int m_sector;
};

#endif // SECTOR_H
