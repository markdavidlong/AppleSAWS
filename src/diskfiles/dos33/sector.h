#ifndef SECTOR_H
#define SECTOR_H

#include <QByteArray>

#include "rawdiskimage.h"

#include "vtoc.h"
#include "catalogsector.h"
#include "tracksectorlist.h"

class Sector
{
public:

    Sector(SectorData *data = nullptr) {
    //    m_data.resize(256);
        setData(data);
        m_track = 255;
        m_sector = 255;
    }

    void setData(SectorData *data)
    {
        m_raw_data = data;
    }

    VTOC promoteToVTOC() {
        return VTOC(this);
    }

    CatalogSector asCatalogSector() {
        return CatalogSector(this);
    }

    TrackSectorList asTrackSectorList()  {
        return TrackSectorList(this);
    }

    int sector() { return m_sector; }
    int track() { return m_track; }

    void setTrackSector(int track, int sector) {
        setTrack(track);
        setSector(sector);
    }

    void setTrackSector(TSPair ts)
    {
        setTrackSector(ts.track(),ts.sector());
    }

    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    quint8 operator[](uint offset) const;
    quint8 at(int offset) { return m_raw_data->at(offset); }

    void dump();

    SectorData *rawData() { return m_raw_data; }

private:
  //  QByteArray m_data;
    int m_track;
    int m_sector;

    SectorData *m_raw_data;
};

#endif // SECTOR_H
