#pragma once

#include "util.h"
#include "applestring.h"

#include <QDebug>

struct FileDescriptiveEntry {
    int fileTypeAndFlags;
    AppleString filename;
    quint16 lengthInSectors;
    bool deleted;

    FileDescriptiveEntry() {
        fileTypeAndFlags = 0;
        lengthInSectors = 0;
        deleted = false;
    }

    bool operator<(const FileDescriptiveEntry& f1) const {
        return f1.filename < filename;
    }

    QString fileType() {
        if (fileTypeAndFlags & DOSIntegerBasicFile) { return "I"; }
        if (fileTypeAndFlags & DOSApplesoftBasicFile) { return "A"; }
        if (fileTypeAndFlags & DOSRelocatableFile) { return "R"; }
        if (fileTypeAndFlags & DOSRawBinaryFile) { return "B"; }
        if (fileTypeAndFlags & DOSTypeSFile) { return "S"; }
        if (fileTypeAndFlags & DOSTypeAFile) { return "a"; }
        if (fileTypeAndFlags & DOSTypeBFile) { return "b"; }
        return "T";
    }

    bool isLocked() { return (fileTypeAndFlags & DOSIsLocked); }

    void dump() {
        
	    qDebug() << "First TS List Sector: Track: " << QString("%1").arg(firstTSListSector().track(),2,16,QChar('0')).toUpper()
                                     << " Sector: " << QString("%1").arg(firstTSListSector().sector(),2,16,QChar('0')).toUpper();
        qDebug() << "File Type and Flags: " << QString::number((quint8)fileTypeAndFlags) << "(" << fileType() << "," << (isLocked()?"Locked":"Unlocked") << ")";
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
    TSPair &firstTSListSector()  { return m_firstTSListSector; }

private:
    TSPair m_firstTSListSector;

};
