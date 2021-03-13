#ifndef MEMORYCELL_H
#define MEMORYCELL_H

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

#include "memrole.h"

#include <Qt>
#include <QHash>

class MemoryCell
{
public:
    MemoryCell(quint8 val = 0);
    virtual ~MemoryCell();

    void setAddress(quint16 address) { m_address = address; }
    quint16 address() const { return m_address; }

    bool setRole(MemRole *role);
    MemRole *getRole(int id);
    bool hasRole(int id) const;
    bool removeRole(int id);

    void setValue(quint8 val) { m_value = val; }
    quint8 value() const { return m_value; }
    operator quint8() const { return m_value; }

    QList<MemRole *> getAllRoles();

private:
    quint8 m_value;
    quint16 m_address;

    QHash<int,MemRole *> m_roles;
};

#endif // MEMORYCELL_H
