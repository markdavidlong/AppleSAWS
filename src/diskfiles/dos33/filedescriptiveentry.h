#ifndef FILEDESCRIPTIVEENTRY_H
#define FILEDESCRIPTIVEENTRY_H

#include "tspair.h"
#include <QDebug>
#include "applestring.h"

struct FileDescriptiveEntry {
    quint8  fileTypeFlags;
    AppleString filename;
    quint16 lengthInSectors;
    bool deleted;

    FileDescriptiveEntry() {
        fileTypeFlags = 0;
        lengthInSectors = 0;
        deleted = false;
    }

    bool operator<(const FileDescriptiveEntry& f1) const {
        return f1.filename < filename;
    }

    QString fileType() {
        if (fileTypeFlags & (quint8) FileTypeFlag::Integer) { return "I"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::Applesoft) { return "A"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::Relocatable) { return "R"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::Binary) { return "B"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::TypeS) { return "S"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::TypeA) { return "a"; }
        if (fileTypeFlags & (quint8) FileTypeFlag::TypeB) { return "b"; }
        return "T";
    }

    bool isLocked() { return (fileTypeFlags & (quint8) FileTypeFlag::IsLockedFlag); }

    void dump() {
        
	    qDebug() << "First TS List Sector: Track: " << QString("%1").arg(firstTSListSector().track(),2,16,QChar('0')).toUpper()
                                     << " Sector: " << QString("%1").arg(firstTSListSector().sector(),2,16,QChar('0')).toUpper();
        qDebug() << "File Type and Flags: " << QString::number((quint8) fileTypeFlags) << "(" << fileType() << "," << (isLocked()?"Locked":"Unlocked") << ")";
        qDebug() << "Filename: " << filename.printable();
        qDebug() << "Length in Sectors: " << lengthInSectors;
    }

    void catalog() {
        QString output = QString("%1 %2 %3 %4").arg(QString(isLocked()?"*":" "))
                                     .arg(lengthInSectors,3,10,QChar('0'))
                                     .arg(fileType())
                                     .arg(filename.printable().trimmed());
        qDebug() << output;
    }

    void setFirstTSListSector(TSPair ts) 
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
    TSPair firstTSListSector() const { return m_firstTSListSector; }

private:
    TSPair m_firstTSListSector;

};

#endif // FILEDESCRIPTIVEENTRY_H
