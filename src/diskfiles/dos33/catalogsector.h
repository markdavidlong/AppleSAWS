#ifndef CATALOGSECTOR_H
#define CATALOGSECTOR_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/



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
    CatalogSector(QSharedPointer<const Sector> sector);

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

    QSharedPointer<const Sector>  getSector() const { return m_data; }

    FileDescriptiveEntry makeFDE(int offset);

    TSPair sectorLocation() const;
private:

private:
    QSharedPointer<const Sector> m_data;
    QList<FileDescriptiveEntry> m_fdes;
    TSPair m_next;
};

#endif // CATALOGSECTOR_H
