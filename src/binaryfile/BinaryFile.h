#pragma once

#include "GenericFile.h"

class BinaryFile : public GenericFile
{
public:
    BinaryFile(QByteArray data = QByteArray());
    void setData(QByteArray data) override;

    virtual quint16 length() const override { return m_length; }

    void dump();

protected:
};

