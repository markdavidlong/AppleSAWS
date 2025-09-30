#include "DisassembledItem.h"
#include "Util.h"
#include "OpCodes.h"

DisassembledItem::DisassembledItem(quint8 opcode) : m_opcode{opcode}
{
}

QString DisassembledItem::disassembledString() {
    QString retval = rawDisassembledString();
    if (hasArg()) {
        if (retval.contains("_ARG16_")) {
            retval.replace("_ARG16_","$"+arg16Str());
        } else if (retval.contains("_ARG8_")) {
            retval.replace("_ARG8_","$"+arg8Str());
        }
    }
    return retval;
}

QString DisassembledItem::hexAddress() const { 
    return uint16ToHex(m_address); 
}

bool DisassembledItem::isBranch() const { 
    return OpCodes::isBranch(m_opcode); 
}

bool DisassembledItem::isJump() const { 
    return OpCodes::isJump(m_opcode); 
}

bool DisassembledItem::isJsr() const { 
    return OpCodes::isJsr(m_opcode); 
}

bool DisassembledItem::isReturn() const { 
    return OpCodes::isReturn(m_opcode); 
}

bool DisassembledItem::isBreak() const { 
    return OpCodes::isBreak(m_opcode); 
}

QString DisassembledItem::arg8Str() { 
    return uint8ToHex(arg8());  
}

QString DisassembledItem::arg16Str() { 
    return uint16ToHex(arg16());  
}