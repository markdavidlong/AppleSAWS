#pragma once

#include "GenericFile.h"

class IntBasicFile final : public GenericFile
{
public:
    explicit IntBasicFile(const QByteArray& data = {}) noexcept;

    // Rule of Five
    ~IntBasicFile() override = default;
    IntBasicFile(const IntBasicFile&) = delete;
    IntBasicFile& operator=(const IntBasicFile&) = delete;
    IntBasicFile(IntBasicFile&&) = default;
    IntBasicFile& operator=(IntBasicFile&&) = default;

    [[nodiscard]] QByteArray detokenize();

private:
    [[nodiscard]] static constexpr quint16 get16(quint8 v1, quint8 v2) noexcept;
    [[nodiscard]] QByteArray dumpBufferAsIntBasicFile(const QByteArray& origdata) const;
};

