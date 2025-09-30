#pragma once

#include "MemRole.h"

#include <QString>

class RoleAsmOperand : public MemRole
{
public:
    static constexpr int RoleID = 2;

    enum class Type {
        Byte,
        WordLo,
        WordHi,
    };

    RoleAsmOperand() = default;

    [[nodiscard]] constexpr int id() const noexcept override { return RoleID; }
    [[nodiscard]] QString name() const override { return QStringLiteral("RoleAsmOperand"); }

    void setOperandType(Type type) noexcept { m_optype = type; }
    [[nodiscard]] constexpr Type operandType() const noexcept { return m_optype; }

protected:
    Type m_optype{Type::Byte};

};

