#ifndef FILEDESCRIPTIVEENTRY_H
#define FILEDESCRIPTIVEENTRY_H

#include "tspair.h"
#include <QDebug>
#include "applestring.h"

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
