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

#include "cpressdiskfs.h"
#include "cpressdiskimage.h"

CPressDiskFS::~CPressDiskFS()
{
    ((CPressDiskImage *) parent())->rawDIDiskImg()->RemoveDiskFS(m_fs);
}

QString CPressDiskFS::volumeName() const
{
    Q_ASSERT(m_fs);
    return QString(m_fs->GetVolumeName());
}

QString CPressDiskFS::volumeID() const
{
    Q_ASSERT(m_fs);
    return QString(m_fs->GetVolumeID());
}

QString CPressDiskFS::bareVolumeName() const
{
    Q_ASSERT(m_fs);
    return QString(m_fs->GetBareVolumeName());
}

bool CPressDiskFS::isDamaged() const
{
    Q_ASSERT(m_fs);
    return m_fs->GetFSDamaged();
}

int CPressDiskFS::numFSBlocks() const
{
    Q_ASSERT(m_fs);
    return (int) m_fs->GetFSNumBlocks();
}

int CPressDiskFS::fileCount() const
{
    Q_ASSERT(m_fs);
    return (int) m_fs->GetFileCount();

}

CPressDiskFS::CPressDiskFS(CPressDiskImage *parent) : QObject(parent)
{
    m_fs = parent->rawDIDiskImg()->OpenAppropriateDiskFS(true);
    m_fs->SetScanForSubVolumes(DiskImgLib::DiskFS::kScanSubEnabled);
    m_fs->Initialize(parent->rawDIDiskImg(),DiskImgLib::DiskFS::kInitFull);
}

