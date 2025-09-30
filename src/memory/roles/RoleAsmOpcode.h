#pragma once

#include "MemRole.h"
#include "OpCodes.h"

#include <QString>

class RoleAsmOpcode : public MemRole
{
public:
    static constexpr int RoleID = 1;
    
    RoleAsmOpcode() = default;
    ~RoleAsmOpcode() noexcept override = default;

    [[nodiscard]] constexpr int id() const noexcept override { return RoleID; }
    [[nodiscard]] QString name() const override { return QStringLiteral("RoleAsmOpcode"); }

    [[nodiscard]] QString mnemonic() const;
    [[nodiscard]] AddressMode addressMode() const;

protected:
    AddressMode m_am{AddressMode::AM_InvalidOp};

};

