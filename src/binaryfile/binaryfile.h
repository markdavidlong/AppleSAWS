#ifndef BINARYFILE_H
#define BINARYFILE_H

#include "genericfile.h"

class BinaryFile : public GenericFile
{
    friend class Dos33DiskImage;

public:

    void dump();

protected:
    BinaryFile(Dos33DiskImage *image, FileDescriptiveEntry &fde);
    void setupData();
};

#endif // BINARYFILE_H
