#pragma once

#include "GenericFile.h"
#include "Util.h"
#include "RelocatableTypes.h"
#include "RelocatableDictItem.h"

#include <QByteArray>
#include <memory>


class RelocatableFile final : public GenericFile
{
public:
    explicit RelocatableFile(const QByteArray& data = {}) noexcept;
    
    // Rule of Five - not needed since we use default destructor and no custom copy/move
    ~RelocatableFile() override = default;
    RelocatableFile(const RelocatableFile&) = delete;
    RelocatableFile& operator=(const RelocatableFile&) = delete;
    RelocatableFile(RelocatableFile&&) = default;
    RelocatableFile& operator=(RelocatableFile&&) = default;

    void setData(const QByteArray& data) override;

    [[nodiscard]] quint16 length() const noexcept override { return static_cast<quint16>(m_data.length()); }

    void dump();

    [[nodiscard]] const QByteArray& getBinaryCodeImage() const noexcept { return m_binary_code_image; }
    [[nodiscard]] const QList<RelocatableDictItem>& getRelocatableDict() const noexcept { return m_relocatable_dict; }

    [[nodiscard]] constexpr quint16 startingAddress() const noexcept { return m_starting_ram_address; }
    [[nodiscard]] constexpr quint16 codeImageLength() const noexcept { return m_code_image_length; }
    [[nodiscard]] constexpr quint16 ramImageLength() const noexcept { return m_ram_image_length; }

    [[nodiscard]] QStringList decodeRelocatableDict() const;

private:
    quint16 m_starting_ram_address{0};
    quint16 m_ram_image_length{0};
    quint16 m_code_image_length{0};

    QByteArray m_binary_code_image;
    QList<RelocatableDictItem> m_relocatable_dict;
};

