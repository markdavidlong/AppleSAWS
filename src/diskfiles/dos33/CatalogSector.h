#pragma once

#include "FileDescriptiveEntry.h"
#include "Util.h"

#include <QtGlobal>
#include <QByteArray>
#include <QDebug>

class Sector;



class CatalogSector
{
public:
    explicit CatalogSector(Sector* sector);

    // Const and non-const overloads for getFDE
    [[nodiscard]] FileDescriptiveEntry& getFDE(quint8 number);
    [[nodiscard]] const FileDescriptiveEntry& getFDE(quint8 number) const;

    [[nodiscard]] QList<FileDescriptiveEntry> getFDEs() const;

    [[nodiscard]] TSPair nextCatalogSector() const noexcept;

    void dumpFDEs() const;

    [[nodiscard]] Sector* getSector() const noexcept;

private:
    [[nodiscard]] FileDescriptiveEntry makeFDE(int offset) const;

private:
    Sector *m_data{nullptr};
    QList<FileDescriptiveEntry> m_fdes;
    TSPair m_next{0,0};
};

