#pragma once

#include "Vtoc.h"
#include "CatalogSector.h"
#include "TrackSectorList.h"

#include <QByteArray>

class Sector
{
public:

    Sector() : m_data(256, '\0'), m_track{255}, m_sector{255}
    {
    }

    [[nodiscard]] const VTOC promoteToVTOC() const {
        return VTOC(*this);
    }

    [[nodiscard]] CatalogSector promoteToCatalogSector() {
        return CatalogSector(this);
    }

    [[nodiscard]] TrackSectorList promoteToTrackSectorList()  {
        return TrackSectorList(this);
    }

    [[nodiscard]] int sector() const noexcept { return m_sector; }
    [[nodiscard]] int track() const noexcept { return m_track; }

    void setTrackSector(int track, int sector) noexcept {
        setTrack(track);
        setSector(sector);
    }

    void setTrack(int track) noexcept { m_track = track; }
    void setSector(int sector) noexcept { m_sector = sector; }

    [[nodiscard]] char&  operator[](uint offset);
    [[nodiscard]] char  at(uint offset) const;

    [[nodiscard]] bool setData(const QByteArray& data);

    void dump();

    [[nodiscard]] QByteArray data() const { return m_data; }

    [[nodiscard]] QByteArray& rawData() noexcept { return m_data; }
    [[nodiscard]] const QByteArray& rawData() const noexcept { return m_data; }

private:
    QByteArray m_data;
    int m_track{0};
    int m_sector{0};
};

