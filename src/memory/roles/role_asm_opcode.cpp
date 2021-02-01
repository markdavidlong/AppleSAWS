#include "role_asm_opcode.h"
#include "opcodes.h"
#include "memorycell.h"

RoleAsmOpcode::RoleAsmOpcode()
{

}

RoleAsmOpcode::~RoleAsmOpcode()
{

}

QString RoleAsmOpcode::getMnemonic()
{
    return OpCodes::getMnemonic(m_parent->value());
}
