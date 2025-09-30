#pragma once

#include <QPair>
#include <QtGlobal>

namespace RelocatableTypes {
    
    // Modern scoped enums instead of global enums
    enum class FieldSize : quint8 {
        OneByte,
        TwoByte
    };
    
    enum class WordUpperLower : quint8 {
        LowByte,
        HighByte
    };
    
    enum class Endianness : quint8 {
        LowHi,
        HiLow
    };
    
    enum class Byte4Type : quint8 {
        ByteLo,
        ByteHi,
        ESDSymbol
    };
    
    // Modern type alias instead of typedef
    using Byte4ReturnType = QPair<Byte4Type, quint8>;
    
} // namespace RelocatableTypes