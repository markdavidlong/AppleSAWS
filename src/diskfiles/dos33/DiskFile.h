#pragma once

#include "Util.h"
#include "Sector.h"
#include "Vtoc.h"

#include <QtGlobal>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>

class GenericFile;

class DiskFile
{
public:
    explicit DiskFile(const QString& filename = "");
    ~DiskFile();

    bool read(const QString& filename);

    Sector& getSector(TSPair ts) { return getSector(ts.track(), ts.sector()); }
    const Sector& getSector(TSPair ts) const { return getSector(ts.track(), ts.sector()); }

    Sector& getSector(int track, int sector) {
        return m_contents[track][sector];
    }
    
    const Sector& getSector(int track, int sector) const {
        return m_contents[track][sector];
    }

    [[nodiscard]] VTOC getVTOC() const;
    
    [[nodiscard]] QList<CatalogSector> getCatalogSectors();
    
    [[nodiscard]] GenericFile* getFile(FileDescriptiveEntry fde);
    
    [[nodiscard]] QByteArray getDataFromTrackSectorList(TrackSectorList tsl);
    
    [[nodiscard]] QList<FileDescriptiveEntry> getAllFDEs();
    
    [[nodiscard]] QByteArray fileHash() const noexcept { return m_hash; }
    
    [[nodiscard]] QString getDiskImageName() const noexcept { return m_imageName; }
    [[nodiscard]] QString getFullDiskImageName() const noexcept { return m_fullImageName; }
    [[nodiscard]] QString getMetaDataPath() const;
    
private:
    QMap<int, QMap<int, Sector>> m_contents;
    QMap<FileDescriptiveEntry, GenericFile*> m_files;
    QByteArray m_hash;
    QString m_imageName;
    QString m_fullImageName;
    quint8 m_sectors_per_track{16};

};

