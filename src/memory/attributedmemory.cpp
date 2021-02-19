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

#include "attributedmemory.h"

#include <QDebug>

AttributedMemory::AttributedMemory(quint16 expectedBottom,
                                   quint16 expectedTop)
{
    setExpectedRange(expectedBottom, expectedTop);

    for (int idx = 0; idx <= 0xffff; idx++)
    {
        MemoryCell cell;
        cell.setAddress(idx);
        m_cells.append(cell);
    }
}

void AttributedMemory::setExpectedRange(quint16 bottom, quint16 top)
{
    m_expected_top = qMax(bottom,top);
    m_expected_bottom = qMin(bottom,top);
}

bool AttributedMemory::setValueAt(quint16 address,
                                  quint8 withValue,
                                  MemRole *andRole)
{
    m_cells[address].setValue(withValue);
    if (andRole)
    {
        return setRoleAt(address, andRole);
    }
    return true;
}

bool AttributedMemory::setRoleAt(quint16 address,
                                 MemRole *withRole)
{
    if (withRole)
    {
        return m_cells[address].setRole(withRole);
    }
    return false;
}

bool AttributedMemory::replaceRoleAt(quint16 address,
                                     MemRole *withRole)
{
    if (!withRole) return false;

    removeRoleAt(address,withRole->id());
    return setRoleAt(address,withRole);
}

bool AttributedMemory::hasRoleAt(quint16 address, int withId)
{
    return m_cells[address].hasRole(withId);
}

MemRole *AttributedMemory::getRoleAt(quint16 address, int withId)
{
    return m_cells[address].getRole(withId);
}

QList<MemRole *> AttributedMemory::getAllRolesAt(quint16 address)
{
    return m_cells[address].getAllRoles();
}

bool AttributedMemory::removeRoleAt(quint16 address, int withId)
{
    return m_cells[address].removeRole(withId);
}

QByteArray AttributedMemory::getAllValues() const
{
    return getAllValuesInRange(0x0000,0x0ffff);
}

QByteArray AttributedMemory::getAllValuesInExpectedRange() const
{
    return getAllValuesInRange(m_expected_bottom,m_expected_top);
}

QByteArray AttributedMemory::getAllValuesInRange(quint16 bottom, quint16 top) const
{
    quint16 expbot = qMin(bottom,top);
    quint16 exptop = qMax(bottom,top);

    QByteArray retval;
    for (int idx = expbot; idx <= exptop; idx++)
    {
        retval.append(m_cells.at(idx).value());
    }
    return retval;
}

bool AttributedMemory::addFile(QByteArray data, quint16 start)
{
        if (start+data.length() > 65536) {
            qWarning() << "Memory overflow adding data."; return false;
        }

        for (int idx = 0; idx < data.length(); idx++)
        {
            m_cells[start+idx].setValue(data[idx]);
        }

        return true;
}


