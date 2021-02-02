#include "role_asm_opcode.h"
#include "opcodes.h"
#include "memorycell.h"


QString RoleAsmOpcode::mnemonic() const
{
    return OpCodes::getMnemonic(m_parent->value());
}

AddressMode RoleAsmOpcode::addressMode() const
{
    return OpCodes::addressMode(m_parent->value());
}
