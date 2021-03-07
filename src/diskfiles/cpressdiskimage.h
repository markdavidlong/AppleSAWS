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

#ifndef CPRESSDISKIMAGE_H
#define CPRESSDISKIMAGE_H

#include "DiskImg.h"

#include "cpressdiskfs.h"

#include <QObject>
#include <QSharedPointer>


using DIErrorCode = DiskImgLib::DIError;

class CPressDiskImage : public QObject
{
    Q_OBJECT
public:

    enum class OuterFormat {
        Unknown     = 0,
        None        = 1,
        Compress    = 2,
        GZip        = 3,
        BZip2       = 4,
        Zip         = 10
    };

    enum class FileFormat {          // format of the image "wrapper"
        Unknown    = 0,
        Unadorned  = 1,   // .po, .do, ,nib, .raw, .d13
        TwoMG      = 2,   // .2mg, .2img, $e0/0130
        DiskCopy42 = 3,   // .dsk/.disk, maybe .dc
        DiskCopy60 = 4,   // .dc6 (often just raw format)
        Davex      = 5,   // $e0/8004
        Sim2eHDV   = 6,   // .hdv
        TrackStar  = 7,   // .app (40-track or 80-track)
        FDI        = 8,   // .fdi (5.25" or 3.5")
        NuFX       = 20,  // .shk, .sdk, .bxy
        DDD        = 21,  // .ddd
        DDDDeluxe  = 22,  // $DD, .ddd
    };

    enum class PhysicalFormat {         // format of the image data stream
        Unknown     = 0,
        Sectors     = 1, // sequential 256-byte sectors (13/16/32)
        Nib525_6656 = 2, // 5.25" disk ".nib" (6656 bytes/track)
        Nib525_6384 = 3, // 5.25" disk ".nb2" (6384 bytes/track)
        Nib525_Var  = 4, // 5.25" disk (variable len, e.g. ".app")
    };

    enum class SectorOrder {      // sector ordering for "sector" format images
        Unknown  = 0,
        ProDOS   = 1, // written as series of ProDOS blocks
        DOS      = 2, // written as series of DOS sectors
        CPM      = 3, // written as series of 1K CP/M blocks
        Physical = 4 // written as un-interleaved sectors
    };

    enum class FSFormat {              // main filesystem format (based on NuFX enum)
        Unknown = 0,
        ProDOS = 1,
        DOS33 = 2,
        DOS32 = 3,
        Pascal = 4,
        MacHFS = 5,
        MacMFS = 6,
        Lisa = 7,
        CPM = 8,
        MSDOS = 10,          // any FAT filesystem
        ISO9660 = 12,
        RDOS33 = 20,         // 16-sector RDOS disk
        RDOS32 = 21,         // 13-sector RDOS disk
        RDOS3 = 22,          // 13-sector RDOS disk converted to 16
        GenericPhysicalOrd = 30, // unknown, but physical-sector-ordered
        GenericProDOSOrd = 31,   // unknown, but ProDOS-block-ordered
        GenericDOSOrd = 32,      // unknown, but DOS-sector-ordered
        GenericCPMOrd = 33,      // unknown, but CP/M-block-ordered
        UNIDOS = 40,         // two 400K DOS 3.3 volumes
        OzDOS = 41,          // two 400K DOS 3.3 volumes, weird order
        CFFA4 = 42,          // CFFA image with 4 or 6 partitions
        CFFA8 = 43,          // CFFA image with 8 partitions
        MacPart = 44,        // Macintosh-style partitioned disk
        MicroDrive = 45,     // ///SHH Systeme's MicroDrive format
        FocusDrive = 46,     // Parsons Engineering FocusDrive format
        Gutenberg = 47,      // Gutenberg word processor format
    };

    explicit CPressDiskImage(QObject *parent = nullptr);
    virtual ~CPressDiskImage();

    DIErrorCode lastDIError() const { return m_last_di_error; };
    QString errorString() const;

    bool openAndPrepare(const QString &filename);

    bool open(const QString &filename);
    bool open(QByteArray &data);

    void setImageName(const QString &filename) { m_image_name = filename; }
    QString imageName() const { return m_image_name; }

    bool close();

    bool analyzeImage();


    bool showAsBlocks() const;

    OuterFormat GetOuterFormat(void) const;
    FileFormat GetFileFormat(void) const;
    PhysicalFormat GetPhysicalFormat(void) const;
    SectorOrder GetSectorOrder(void) const;
    FSFormat GetFSFormat(void) const;

    int numTracks(void) const;
    int sectorsPerTrack(void) const;
    int  numBlocks(void) const;

    bool checkForBadBlocks(int startBlock, int numBlocks);

    short dosVolumeNumber() const;

    bool hasSectors() const;
    bool hasBlocks() const;
    bool hasNibbles() const;

    QSharedPointer<CPressDiskFS> diskFS() { return m_fs; }

    QByteArray readTrackSector(int track, int sector);
    QByteArray readBlock(int block);

    QByteArray readBlocks(int startBlock, int numBlocks);

    DiskImgLib::DiskImg* rawDIDiskImg() const { return m_image; }

protected:
    bool isDIOk(DIErrorCode code) { return code == DiskImgLib::kDIErrNone; }

protected:
    DIErrorCode m_last_di_error { DiskImgLib::kDIErrNone };

    DiskImgLib::DiskImg *m_image { nullptr };

    QSharedPointer<CPressDiskFS> m_fs { nullptr };

    QString m_image_name { "[unnamed image]" };

};

#endif // CPRESSDISKIMAGE_H
