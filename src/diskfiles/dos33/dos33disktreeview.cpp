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

#include "dos33disktreeview.h"

#include <QGuiApplication>

Dos33DiskTreeView::Dos33DiskTreeView(QWidget *parent) : QTreeView(parent)
{
    connect(this, &Dos33DiskTreeView::clicked,
            this, &Dos33DiskTreeView::handleClick);
    connect(this, &Dos33DiskTreeView::pressed,
            this, &Dos33DiskTreeView::handlePress);
    connect(this, &Dos33DiskTreeView::doubleClicked,
            this, &Dos33DiskTreeView::handleDoubleClick);

}

void Dos33DiskTreeView::handleClick(const QModelIndex &/*index*/)
{

}

void Dos33DiskTreeView::handleDoubleClick(const QModelIndex &/*index*/)
{

}

void Dos33DiskTreeView::handlePress(const QModelIndex &/*index*/)
{
    if (QGuiApplication::mouseButtons() & Qt::RightButton)
    {

    }
}
