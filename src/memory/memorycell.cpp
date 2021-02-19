/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "memorycell.h"

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


