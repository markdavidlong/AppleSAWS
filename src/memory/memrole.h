#ifndef MEMROLE_H
#define MEMROLE_H

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

#endif // MEMROLE_H
