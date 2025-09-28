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
    CatalogSector(Sector *sector);

    FileDescriptiveEntry &getFDE(quint8 number) {
        if (number >= m_fdes.length()) {
            number = m_fdes.length() - 1;
        }
        return m_fdes[number];
    }

    QList<FileDescriptiveEntry> getFDEs() { return m_fdes; }

    TSPair nextCatalogSector() { return m_next; }

    void dumpFDEs();

    Sector *getSector() const { return m_data; }

private:
    FileDescriptiveEntry makeFDE(int offset);

private:
    Sector *m_data;
    QList<FileDescriptiveEntry> m_fdes;
    TSPair m_next;
};

