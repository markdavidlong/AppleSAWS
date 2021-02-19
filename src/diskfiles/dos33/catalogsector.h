#ifndef CATALOGSECTOR_H
#define CATALOGSECTOR_H

#include <Qt>
#include <QByteArray>
#include <QDebug>

#include "tspair.h"
#include "util.h"
#include "applestring.h"
#include "filedescriptiveentry.h"

class Sector;



class CatalogSector
{
public:
    CatalogSector(Sector *sector);

    FileDescriptiveEntry getFDE(quint8 number) {
        if (m_fdes.length() == 0) { return FileDescriptiveEntry(); }
        if (number >= m_fdes.length()) {
            number = m_fdes.length() - 1;
        }
        return m_fdes[number];
    }

    QList<FileDescriptiveEntry> getFDEs() { return m_fdes; }

    TSPair nextCatalogSector() { return m_next; }

    void dumpFDEs();

    Sector *getSector() const { return m_data; }

    FileDescriptiveEntry makeFDE(int offset);

    TSPair sectorLocation() const;
private:

private:
    Sector *m_data;
    QList<FileDescriptiveEntry> m_fdes;
    TSPair m_next;
};

#endif // CATALOGSECTOR_H
