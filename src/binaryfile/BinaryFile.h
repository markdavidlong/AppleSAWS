#pragma once

#include "GenericFile.h"

// Forward declarations
class QByteArray;

class BinaryFile : public GenericFile
{
public:
    explicit BinaryFile(const QByteArray& data = QByteArray());
    void setData(const QByteArray& data) override;

    [[nodiscard]] virtual quint16 length() const noexcept override { return m_length; }

    void dump();

protected:
};

