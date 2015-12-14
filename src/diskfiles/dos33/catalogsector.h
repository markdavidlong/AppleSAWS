#ifndef CATALOGSECTOR_H
#define CATALOGSECTOR_H

#include <Qt>
#include <QByteArray>
#include <QDebug>

#include "util.h"
#include "applestring.h"
#include "filedescriptiveentry.h"

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

private:
    FileDescriptiveEntry makeFDE(int offset);

private:
    Sector *m_data;
    QList<FileDescriptiveEntry> m_fdes;
    TSPair m_next;
};

#endif // CATALOGSECTOR_H
