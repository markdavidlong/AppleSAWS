#ifndef DOS33IMAGEMODEL_H
#define DOS33IMAGEMODEL_H

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



#include "dos33treeitem.h"
#include "dos33diskimage.h"

#include <QStandardItemModel>
#include <QObject>

#include <QIcon>

enum class Dos33TreeRole {
    ItemType = Qt::UserRole,
    GetTSPair,
    GetFDE,
};

enum class Dos33ItemType {
    Unknown,
    Sector,
    File,
    DiskImage
};


class Dos33ImageModel : public QStandardItemModel
{
    Q_OBJECT

    using ImageMap = QMap<QString, Dos33DiskImage *>;

public:
    Dos33ImageModel(QObject *parent = nullptr);
    ~Dos33ImageModel();

    bool addDiskImage(Dos33DiskImage *image, QString name = "");
    Dos33DiskImage *getDiskImage(QString name);
    Dos33DiskImage *removeDiskImage(QString name);



private:
    QIcon m_icon_A;
    QIcon m_icon_a;
    QIcon m_icon_B;
    QIcon m_icon_b;
    QIcon m_icon_I;
    QIcon m_icon_R;
    QIcon m_icon_S;
    QIcon m_icon_T;
    QIcon m_icon_disk;

    ImageMap m_images;
};

#endif // DOS33IMAGEMODEL_H
