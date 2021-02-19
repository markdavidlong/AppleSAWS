#ifndef DOS33DISKTREEVIEW_H
#define DOS33DISKTREEVIEW_H

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



#include <QTreeView>
#include <QWidget>

#include "filedescriptiveentry.h"

class Dos33DiskTreeView : public QTreeView
{
    Q_OBJECT
public:
    Dos33DiskTreeView(QWidget *parent = nullptr);

protected:


signals:
    void fileClicked(const FileDescriptiveEntry &fde);
    void fileRightClicked(const FileDescriptiveEntry &fde);
    void fileDoubleClicked(const FileDescriptiveEntry &fde);
    void diskClicked(QString name);
    void diskRightClicked(QString name);
    void sectorClicked(TSPair ts);
    void sectorRightClicked(TSPair ts);
    void sectorDoubleClicked(TSPair ts);


private slots:
    void handleClick(const QModelIndex &index);
    void handleDoubleClick(const QModelIndex &index);
    void handlePress(const QModelIndex &index);



};

#endif // DOS33DISKTREEVIEW_H
