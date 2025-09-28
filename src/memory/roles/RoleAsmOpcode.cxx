#include "RoleAsmOpcode.h"
#include "opcodes.h"
#include "MemoryCell.h"


QString RoleAsmOpcode::mnemonic() const
{
    return OpCodes::getMnemonic(m_parent->value());
}

AddressMode RoleAsmOpcode::addressMode() const
{
    return OpCodes::addressMode(m_parent->value());
}
