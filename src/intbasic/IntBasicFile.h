#pragma once

#include "GenericFile.h"


class IntBasicFile : public GenericFile
{
public:
    explicit IntBasicFile(const QByteArray& data = QByteArray());

    [[nodiscard]] QByteArray detokenize();
private:
    [[nodiscard]] quint16 get16(quint8 v1, quint8 v2);
    [[nodiscard]] QByteArray dumpBufferAsIntBasicFile(const QByteArray& origdata);
};

