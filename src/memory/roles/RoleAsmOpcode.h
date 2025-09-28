#pragma once

#include "MemRole.h"
#include "OpCodes.h"

#include <QString>

class RoleAsmOpcode : public MemRole
{

public:
    static const int RoleID = 1;
    RoleAsmOpcode() { m_am = AddressMode::AM_InvalidOp; }
    virtual ~RoleAsmOpcode() { }

    virtual int id() const override { return RoleID; }
    virtual QString name() const override { return "RoleAsmOpcode"; }

    QString mnemonic() const;
    AddressMode addressMode() const;

protected:
    AddressMode m_am;

};

