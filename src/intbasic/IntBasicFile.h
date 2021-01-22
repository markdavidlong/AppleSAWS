#ifndef INTBASICFILE_H
#define INTBASICFILE_H

#include "genericfile.h"


class IntBasicFile : public GenericFile
{
public:
    IntBasicFile(QByteArray data = QByteArray());

    QByteArray detokenize();
private:
    quint16 get16(quint8 v1, quint8 v2);
    QByteArray dumpBufferAsIntBasicFile(QByteArray origdata);
};

#endif // INTBASICFILE_H
