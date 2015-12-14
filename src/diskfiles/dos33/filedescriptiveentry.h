#ifndef FILEDESCRIPTIVEENTRY_H
#define FILEDESCRIPTIVEENTRY_H

#include "util.h"
#include <QDebug>
#include "applestring.h"

struct FileDescriptiveEntry {
    TSPair firstTSListSector;
    int fileTypeAndFlags;
    AppleString filename;
    quint16 lengthInSectors;

    bool operator<(const FileDescriptiveEntry& f1) const {
        return f1.filename < filename;
    }

    QString fileType() {
        if (fileTypeAndFlags & IntegerBasicFile) { return "I"; }
        if (fileTypeAndFlags & ApplesoftBasicFile) { return "A"; }
        if (fileTypeAndFlags & RelocatableFile) { return "R"; }
        if (fileTypeAndFlags & RawBinaryFile) { return "B"; }
        if (fileTypeAndFlags & TypeSFile) { return "S"; }
        if (fileTypeAndFlags & TypeAFile) { return "a"; }
        if (fileTypeAndFlags & TypeBFile) { return "b"; }
        return "T";
    }

    bool isLocked() { return (fileTypeAndFlags & IsLocked); }

    void dump() {
        qDebug() << "First TS List Sector: Track: " << QString("%1").arg(firstTSListSector.track,2,16,QChar('0')).toUpper()
                                     << " Sector: " << QString("%1").arg(firstTSListSector.sector,2,16,QChar('0')).toUpper();
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
};

#endif // FILEDESCRIPTIVEENTRY_H
