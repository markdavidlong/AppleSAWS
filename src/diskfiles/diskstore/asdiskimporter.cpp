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

#include "asdiskimporter.h"

#include "DiskImg.h"

#include <QDir>
#include <QFileInfo>

ASDiskImporter::ASDiskImporter(QObject *parent) : QObject(parent)
{
    if (!DiskImgLib::Global::GetAppInitCalled())
    {
        DiskImgLib::Global::AppInit();
    }
}

bool ASDiskImporter::importImage(QString filename, ASDiskData &into)
{
    bool retval = false;

    DiskImgLib::DiskImg image;
    auto localname = QDir::toNativeSeparators(filename);
    auto fileinfo = QFileInfo(filename);
    auto result = image.OpenImage(filename.toLatin1(),QDir::separator().toLatin1(),true);

    if (result == DiskImgLib::kDIErrNone)
    {
        result = image.AnalyzeImage();
        if (result == DiskImgLib::kDIErrNone)
        {
            auto filesystemformat = image.GetFSFormat();
            QString fsformatstr = DiskImgLib::DiskImg::ToString(filesystemformat);

            auto fs = image.OpenAppropriateDiskFS(false);
            result = fs->Initialize(&image,DiskImgLib::DiskFS::kInitFull);
            if (result == DiskImgLib::kDIErrNone)
            {
                into.setFilename(fileinfo.fileName());

                into.setFSInfo(fsformatstr, (int) filesystemformat);

                int numblocks = image.GetNumBlocks();
                into.setNumBlocks(numblocks);
                int numtracks = image.GetNumTracks();
                into.setNumTracks(numtracks);
                int numsectors = image.GetNumSectPerTrack();\
                into.setNumSectorsPerTrack(numsectors);



                if (image.ShowAsBlocks())
                {
                    into.setUseBlocks(numblocks);

                    char *buffer = new char[512]; // Todo: Remove magic number
                    for (int idx = 0; idx < numblocks; idx++)
                    {
                        image.ReadBlock(idx,buffer);
                        QByteArray array(buffer, 512);
                        into.addBlock(idx,array);
                    }
                    delete[] buffer;
                }
                else
                {
                    into.setUseSectors(numtracks,numsectors);

                    char *buffer = new char[256]; // Todo: Remove magic number
                    for (int track = 0; track < numtracks; track++)
                    {
                        for (int sector = 0; sector < numsectors; sector++)
                        {
                            image.ReadTrackSector(track,sector,buffer);
                            QByteArray array(buffer, 256);
                            into.addSector(track,sector,array);
                        }
                    }
                    delete[] buffer;

                }

                QFile orig(filename);
                if (orig.open(QIODevice::ReadOnly))
                {
                    auto data = orig.readAll();
                    orig.close();
                    into.setOriginalFileContents(data);
                    retval = true;
                }

            }
            image.RemoveDiskFS(fs);
        }
        image.CloseImage();
    }

    return retval;
}
