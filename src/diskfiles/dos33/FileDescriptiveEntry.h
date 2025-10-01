#pragma once

#include "Util.h"
#include "TSPair.h"
#include "AppleString.h"

#include <QDebug>

struct FileDescriptiveEntry {
    int fileTypeAndFlags{0};
    AppleString filename;
    quint16 lengthInSectors{0};
    bool deleted{false};

    [[nodiscard]] bool operator<(const FileDescriptiveEntry& f1) const noexcept {
        return f1.filename < filename;
    }

    [[nodiscard]] QString fileType() const;
    [[nodiscard]] bool isLocked() const noexcept { return (fileTypeAndFlags & DOSIsLocked); }

    void dump() const;
    void catalog() const;

    void setFirstTSListSector(TSPair ts);
    
    // Const and non-const overloads for firstTSListSector
    [[nodiscard]] TSPair& firstTSListSector() noexcept { return m_firstTSListSector; }
    [[nodiscard]] const TSPair& firstTSListSector() const noexcept { return m_firstTSListSector; }

private:
    TSPair m_firstTSListSector{0,0};

};
