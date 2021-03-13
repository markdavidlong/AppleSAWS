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

#include "filedescriptiveentry.h"
#include "genericfile.h"

FileDescriptiveEntry::FileDescriptiveEntry() {
    fileTypeFlags = 0;
    lengthInSectors = 0;
    deleted = false;
}

bool FileDescriptiveEntry::operator<(const FileDescriptiveEntry &f1) const {
    return f1.filename < filename;
}

QString FileDescriptiveEntry::fileTypeIdentifier() {
    if      (fileTypeFlags & (quint8) FileTypeFlag::Integer)     { return "I"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::Applesoft)   { return "A"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::Relocatable) { return "R"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::Binary)      { return "B"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::TypeS)       { return "S"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::TypeA)       { return "a"; }
    else if (fileTypeFlags & (quint8) FileTypeFlag::TypeB)       { return "b"; }
    else return "T";
}

bool FileDescriptiveEntry::isLocked() { return (fileTypeFlags & (quint8) FileTypeFlag::IsLockedFlag); }

void FileDescriptiveEntry::dump() {

    qDebug() << "First TS List Sector: Track: " << QString("%1").arg(firstTSListSector().track(),2,16,QChar('0')).toUpper()
             << " Sector: " << QString("%1").arg(firstTSListSector().sector(),2,16,QChar('0')).toUpper();
    qDebug() << "File Type and Flags: " << QString::number((quint8) fileTypeFlags) << "(" << fileTypeIdentifier() << "," << (isLocked()?"Locked":"Unlocked") << ")";
    qDebug() << "Filename: " << filename.printable();
    qDebug() << "Length in Sectors: " << lengthInSectors;
}

void FileDescriptiveEntry::catalog() {
    QString output = QString("%1 %2 %3 %4").arg(QString(isLocked()?"*":" "))
            .arg(lengthInSectors,3,10,QChar('0'))
            .arg(fileTypeIdentifier())
            .arg(filename.printable().trimmed());
    qDebug() << output;
}

void FileDescriptiveEntry::setFirstTSListSector(TSPair ts)
{
    if (ts.isValid())
    {
        m_firstTSListSector = ts;
    }
    else
    {
        m_firstTSListSector = TSPair(0,0);
    }
}

TSPair FileDescriptiveEntry::firstTSListSector() const { return m_firstTSListSector; }

void FileDescriptiveEntry::updateGenericFile(GenericFile *file)
{
    if (file)
    {
        file->setFileType(fileTypeIdentifier());
        file->setLocked(isLocked());
        file->setSectorCount(lengthInSectors);
        file->setFilename(filename);
    }
}
