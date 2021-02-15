#ifndef INTBASICFILE_H
#define INTBASICFILE_H

#include "genericfile.h"


class IntBasicFile : public GenericFile
{
    friend class Dos33DiskImage;

public:
    QByteArray detokenize();

private:
    IntBasicFile(QByteArray data = QByteArray());
    quint16 get16(quint8 v1, quint8 v2);
    QByteArray dumpBufferAsIntBasicFile(QByteArray origdata);
};

#endif // INTBASICFILE_H
