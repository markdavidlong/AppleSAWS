#pragma once

#include <QString>

// IDs:
//  1: RoleAsmOpcode
//  2: RoleAsmOperand

class MemoryCell;

class MemRole
{
public:
    MemRole();
    virtual ~MemRole() { }

    virtual int id() const = 0;
    virtual QString name() const = 0;

    virtual void setParent(MemoryCell *parent);
    virtual MemoryCell *parent( ) const  { return m_parent; }

protected:
    MemoryCell *m_parent;

};

