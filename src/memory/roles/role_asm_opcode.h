#ifndef ROLE_ASM_OPCODE_H
#define ROLE_ASM_OPCODE_H

#include "memrole.h"

#include <QString>

class RoleAsmOpcode : public MemRole
{
public:
    RoleAsmOpcode();
    virtual ~RoleAsmOpcode();

    virtual int id() const { return 1; }

    QString getMnemonic();
};

#endif // ROLE_ASM_OPCODE_H
