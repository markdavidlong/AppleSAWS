#ifndef DOS33TREEITEM_H
#define DOS33TREEITEM_H

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



#include <QStandardItem>

class Dos33TreeItem : public QStandardItem
{
public:
    Dos33TreeItem();
    Dos33TreeItem(const QIcon &icon, const QString &text);
    Dos33TreeItem(const QString &text);

    ~Dos33TreeItem() { }

private:
    Dos33TreeItem(int rows, int columns = 1) : QStandardItem(rows,columns) { }

};

#endif // DOS33TREEITEM_H
