#pragma once

#include <QString>

// IDs:
//  1: RoleAsmOpcode
//  2: RoleAsmOperand

class MemoryCell;

class MemRole
{
public:
    MemRole() = default;
    virtual ~MemRole() noexcept = default;

    [[nodiscard]] virtual int id() const = 0;
    [[nodiscard]] virtual QString name() const = 0;

    // Non-owning pointer to parent MemoryCell (lifetime managed elsewhere)
    virtual void setParent(MemoryCell* parent) noexcept;
    [[nodiscard]] virtual MemoryCell* parent() const noexcept { return m_parent; }
    [[nodiscard]] virtual bool hasParent() const noexcept { return m_parent != nullptr; }

protected:
    // Non-owning pointer - MemoryCell lifetime is managed by container/owner
    MemoryCell* m_parent{nullptr};

};

