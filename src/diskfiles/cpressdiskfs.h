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

#ifndef CPRESSDISKFS_H
#define CPRESSDISKFS_H

#include <QObject>
#include "DiskImg.h"


class CPressDiskFS : public QObject
{
    Q_OBJECT
public:
    friend class CPressDiskImage;

    ~CPressDiskFS();

    QString volumeName() const;
    QString volumeID() const;
    QString bareVolumeName() const;
    bool isDamaged() const;
    int numFSBlocks() const;

    int fileCount() const;



protected:
    explicit CPressDiskFS(CPressDiskImage *parent);

    DiskImgLib::DiskFS *m_fs { nullptr };

};

#endif // CPRESSDISKFS_H
