#pragma once

#include "GenericFile.h"

// Forward declarations
class QByteArray;

class BinaryFile : public GenericFile
{
public:
    BinaryFile(const QByteArray& data = QByteArray());
    void setData(const QByteArray& data) override;

    [[nodiscard]] virtual quint16 length() const override { return m_length; }

    void dump();

protected:
};

