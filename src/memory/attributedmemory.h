#ifndef ATTRIBUTEDMEMORY_H
#define ATTRIBUTEDMEMORY_H

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


class AttributedMemory
{
public:
    AttributedMemory(quint16 expectedBottom = 0x0000,
                     quint16 expectedTop = 0xffff);

    void setExpectedRange(quint16 bottom, quint16 top);

    quint16 getExpectedBottom() const { return m_expected_bottom; }
    quint16 getExpectedTop() const;
    quint16 getExpectedSize() const { return m_expected_top-m_expected_bottom; }

    bool setValueAt(quint16 address, quint8 withValue, MemRole *andRole = nullptr);
    quint8 valueAt(quint16 address) const { return m_cells[address].value(); };
    bool setRoleAt(quint16 address, MemRole *withRole);
    bool hasRoleAt(quint16 address, int withId);
    MemRole *getRoleAt(quint16 address, int withId);
    QList<MemRole *> getAllRolesAt(quint16 address);
    bool removeRoleAt(quint16 address, int withId);
    bool replaceRoleAt(quint16 address, MemRole *withRole);

    QByteArray getAllValues() const;
    QByteArray getAllValuesInExpectedRange() const;
    QByteArray getAllValuesInRange(quint16 bottom, quint16 top) const;

    quint8 at(quint16 address) const { return m_cells[address].value(); }

    bool addFile(QByteArray data, quint16 start);  // From Memory.h.  Should be replaced?

protected:

    quint16 m_expected_bottom;
    quint16 m_expected_top;

    QList<MemoryCell> m_cells;
};

#endif // ATTRIBUTEDMEMORY_H
