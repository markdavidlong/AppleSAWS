#pragma once

#include "Vtoc.h"
#include "CatalogSector.h"
#include "TrackSectorList.h"

#include <QByteArray>

class Sector
{
public:

    Sector() {
        m_data.resize(256);
        m_track = 255;
        m_sector = 255;
    }

    const VTOC promoteToVTOC() const {
        return VTOC(*this);
    }

    CatalogSector promoteToCatalogSector() {
        return CatalogSector(this);
    }

    TrackSectorList promoteToTrackSectorList()  {
        return TrackSectorList(this);
    }

    int sector() const { return m_sector; }
    int track() const { return m_track; }

    void setTrackSector(int track, int sector) {
        setTrack(track);
        setSector(sector);
    }

    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    char&  operator[](uint offset);
    char  at(uint offset) const;

    bool setData(QByteArray data);

    void dump();

    QByteArray data() const { return m_data; }

    QByteArray rawData() { return m_data; }

private:
    QByteArray m_data;
    int m_track;
    int m_sector;
};

