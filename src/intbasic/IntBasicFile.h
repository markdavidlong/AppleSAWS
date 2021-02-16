#ifndef INTBASICFILE_H
#define INTBASICFILE_H

#include "genericfile.h"


class IntBasicFile : public GenericFile
{
    friend class Dos33DiskImage;

public:
    QByteArray detokenize();

private:
    IntBasicFile(Dos33DiskImage *image, FileDescriptiveEntry &fde);
    QByteArray dumpBufferAsIntBasicFile(QByteArray origdata);
};

#endif // INTBASICFILE_H
