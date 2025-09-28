#pragma once

#include "GenericFile.h"

class TextFile : public GenericFile
{
public:
    TextFile(QByteArray data = QByteArray());
    void setData(QByteArray data);

    void dump();

protected:
};


