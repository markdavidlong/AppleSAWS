#ifndef BINARYFILE_H
#define BINARYFILE_H

#include "genericfile.h"

class BinaryFile : public GenericFile
{
    friend class Dos33DiskImage;

public:

    void dump();

protected:
    BinaryFile(QByteArray data = QByteArray());
    void setData(QByteArray data) override;
};

#endif // BINARYFILE_H
