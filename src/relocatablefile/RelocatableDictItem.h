#pragma once

#include <QtGlobal>
#include "RelocatableTypes.h"
#include "Util.h"

class RelocatableDictItem final {
public:
    RelocatableDictItem() noexcept { init(0, 0, 0, 0); }
    
    explicit RelocatableDictItem(quint8 rld, quint8 fo_low, quint8 fo_hi, quint8 lbsym) noexcept {
        init(rld, fo_low, fo_hi, lbsym);
    }
    
    explicit RelocatableDictItem(quint8 rld, quint16 fo, quint8 lbsym) noexcept {
        init(rld, fo, lbsym);
    }

    void init(quint8 rld, quint8 fo_low, quint8 fo_hi, quint8 lbsym) noexcept {
        m_rld_flag = rld;
        m_halfword_or_sym_num = lbsym;
        m_field_offset = makeWord(fo_low, fo_hi);
    }

    void init(quint8 rld, quint16 fo, quint8 lbsym) noexcept {
        m_rld_flag = rld;
        m_halfword_or_sym_num = lbsym;
        m_field_offset = fo;
    }

    [[nodiscard]] RelocatableTypes::FieldSize getFieldSize() const noexcept {
        return (m_rld_flag & 0x80) ? RelocatableTypes::FieldSize::TwoByte : RelocatableTypes::FieldSize::OneByte;
    }
    
    [[nodiscard]] RelocatableTypes::WordUpperLower getWordBytePos() const noexcept {
        return (m_rld_flag & 0x40) ? RelocatableTypes::WordUpperLower::HighByte : RelocatableTypes::WordUpperLower::LowByte;
    }
    
    [[nodiscard]] RelocatableTypes::Endianness getEndianness() const noexcept {
        return (m_rld_flag & 0x20) ? RelocatableTypes::Endianness::HiLow : RelocatableTypes::Endianness::LowHi;
    }
    
    [[nodiscard]] constexpr bool isExtern() const noexcept { return (m_rld_flag & 0x10) != 0; }
    [[nodiscard]] constexpr bool isNotEndOfRLD() const noexcept { return (m_rld_flag & 0x01) != 0; }
    [[nodiscard]] constexpr bool isEndOfRLD() const noexcept { return (m_rld_flag & 0x01) == 0; }

    [[nodiscard]] RelocatableTypes::Byte4ReturnType getByte4() const noexcept {
        RelocatableTypes::Byte4ReturnType retval;

        if (isExtern()) {
            retval.first = RelocatableTypes::Byte4Type::ESDSymbol;
        } else if (getWordBytePos() == RelocatableTypes::WordUpperLower::HighByte) {
            retval.first = RelocatableTypes::Byte4Type::ByteHi;
        } else {
            retval.first = RelocatableTypes::Byte4Type::ByteLo;
        }

        retval.second = m_halfword_or_sym_num;
        return retval;
    }

    [[nodiscard]] constexpr quint16 getFieldOffset() const noexcept { return m_field_offset; }
    [[nodiscard]] constexpr quint8 getRLDFlag() const noexcept { return m_rld_flag; }
    [[nodiscard]] constexpr quint8 getRawHalfWordOrSymNum() const noexcept { return m_halfword_or_sym_num; }

    // Structured binding and comparison support
    [[nodiscard]] constexpr auto operator<=>(const RelocatableDictItem& other) const noexcept = default;

private:
    quint8 m_rld_flag{0};
    quint16 m_field_offset{0};
    quint8 m_halfword_or_sym_num{0};
};