#ifndef ROLEASMOPERAND_H
#define ROLEASMOPERAND_H

#include <QString>
#include "memrole.h"

class RoleAsmOperand : public MemRole
{

public:
    static const int RoleID = 2;

    typedef enum
    {
        Byte,
        WordLo,
        WordHi,
    } Type;

    RoleAsmOperand();

    virtual int id() const override { return RoleID; }
    virtual QString name() const override { return "RoleAsmOperand"; }

    void setOperandType(Type type) { m_optype = type; }
    Type operandType() const { return m_optype; }

protected:
    Type m_optype;

};

#endif // ROLEASMOPERAND_H
