#pragma once

#include "GenericFile.h"


class IntBasicFile : public GenericFile
{
public:
    IntBasicFile(const QByteArray& data = QByteArray());

    QByteArray detokenize();
private:
    quint16 get16(quint8 v1, quint8 v2);
    QByteArray dumpBufferAsIntBasicFile(QByteArray origdata);
};

