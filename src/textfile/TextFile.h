#pragma once

#include "GenericFile.h"

class TextFile final : public GenericFile
{
public:
    explicit TextFile(const QByteArray& data = {}) noexcept;
    
    // Rule of Five
    ~TextFile() override = default;
    TextFile(const TextFile&) = delete;
    TextFile& operator=(const TextFile&) = delete;
    TextFile(TextFile&&) = default;
    TextFile& operator=(TextFile&&) = default;

    void setData(const QByteArray& data) override;
    void dump();
};


