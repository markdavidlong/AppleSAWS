#pragma once

#include "GenericFile.h"

class TextFile : public GenericFile
{
public:
    TextFile(const QByteArray& data = QByteArray());
    void setData(const QByteArray& data) override;

    void dump();

protected:
};


