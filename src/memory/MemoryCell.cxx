#include "MemoryCell.h"

MemoryCell::MemoryCell(quint8 val)
{
    m_value = val;
}

MemoryCell::~MemoryCell()
{
    auto keys = m_roles.keys();

    foreach (auto key, keys)
    {
        delete (m_roles.take(key));
    }
}

bool MemoryCell::setRole(MemRole *role)
{
    if (!role) {
        qWarning("No role given!");
        return false;
    }

    if (hasRole(role->id()))
    {
        qWarning("Address already has this role.");
        return false;
    }

    m_roles.insert(role->id(),role);
    role->setParent(this);
    return true;
}

MemRole *MemoryCell::getRole(int id)
{
    if (hasRole(id))
    {
        return m_roles[id];
    }
    return nullptr;
}

bool MemoryCell::hasRole(int id) const
{
    return m_roles.contains(id);
}

bool MemoryCell::removeRole(int id)
{
    if (hasRole(id))
    {
        m_roles.remove(id);
        return true;
    }
    return false;
}

QList<MemRole *> MemoryCell::getAllRoles()
{
    return m_roles.values();
}


