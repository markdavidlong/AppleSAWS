#include "FileDescriptiveEntry.h"

#include <QString>
#include <QDebug>

QString FileDescriptiveEntry::fileType() const {
    if (fileTypeAndFlags & DOSIntegerBasicFile) { return "I"; }
    if (fileTypeAndFlags & DOSApplesoftBasicFile) { return "A"; }
    if (fileTypeAndFlags & DOSRelocatableFile) { return "R"; }
    if (fileTypeAndFlags & DOSRawBinaryFile) { return "B"; }
    if (fileTypeAndFlags & DOSTypeSFile) { return "S"; }
    if (fileTypeAndFlags & DOSTypeAFile) { return "a"; }
    if (fileTypeAndFlags & DOSTypeBFile) { return "b"; }
    return "T";
}

void FileDescriptiveEntry::dump() const {
    qDebug() << "First TS List Sector: Track: " << QString("%1").arg(firstTSListSector().track(),2,16,QChar('0')).toUpper()
                                 << " Sector: " << QString("%1").arg(firstTSListSector().sector(),2,16,QChar('0')).toUpper();
    qDebug() << "File Type and Flags: " << QString::number((quint8)fileTypeAndFlags) << "(" << fileType() << "," << (isLocked()?"Locked":"Unlocked") << ")";
    qDebug() << "Filename: " << filename.printable();
    qDebug() << "Length in Sectors: " << lengthInSectors;
}

void FileDescriptiveEntry::catalog() const {
    QString output = QString("%1 %2 %3 %4").arg(QString(isLocked()?"*":" "))
                                 .arg(lengthInSectors,3,10,QChar('0'))
                                 .arg(fileType())
                                 .arg(filename.printable().trimmed());
    qDebug() << output;
}

void FileDescriptiveEntry::setFirstTSListSector(TSPair ts) {
    if (ts.isValid()) {
        m_firstTSListSector = ts;
    } else {
        m_firstTSListSector = TSPair(0,0);
    }
}