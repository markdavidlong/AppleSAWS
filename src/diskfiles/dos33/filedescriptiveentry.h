#ifndef FILEDESCRIPTIVEENTRY_H
#define FILEDESCRIPTIVEENTRY_H

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

#include "tspair.h"
#include "applestring.h"

#include <QDebug>

class GenericFile;


class FileDescriptiveEntry
{
public:

    quint8  fileTypeFlags;
    AppleString filename;
    quint16 lengthInSectors;
    bool deleted;

    FileDescriptiveEntry();

    bool operator<(const FileDescriptiveEntry& f1) const;

    QString fileTypeIdentifier();

    bool isLocked();

    void dump();

    void catalog();

    void setFirstTSListSector(TSPair ts);
    TSPair firstTSListSector() const;

    void updateGenericFile(GenericFile *file);

private:
    TSPair m_firstTSListSector;

};
Q_DECLARE_METATYPE(FileDescriptiveEntry);
#endif // FILEDESCRIPTIVEENTRY_H
