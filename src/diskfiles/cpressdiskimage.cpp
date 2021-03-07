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

#include "cpressdiskimage.h"
#include <QFile>
#include <QDebug>

CPressDiskImage::CPressDiskImage(QObject *parent) : QObject(parent)
{
    DiskImgLib::Global::AppInit();

    m_image = new DiskImgLib::DiskImg();
}

CPressDiskImage::~CPressDiskImage()
{
    close();
    delete m_image;
}

QString CPressDiskImage::errorString() const
{
    return QString(DiskImgLib::DIStrError(lastDIError()));
}

bool CPressDiskImage::openAndPrepare(const QString &filename)
{
    bool retval = open(filename);
    if (retval)
    {
        auto fs = new CPressDiskFS(this);
        m_fs = QSharedPointer<CPressDiskFS>(fs);
    }
    return retval;
}

bool CPressDiskImage::open(const QString &filename)
{
#if 0
    setImageName(filename);
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        auto contents = file.readAll();
        auto result = open(contents);
        m_image->AnalyzeImage();
        return result;
    }
    else
    {
        return false;
    }
#else
    auto fn = filename;
    setImageName(fn);

    auto result = m_image->OpenImage(qPrintable(fn),'\\',true);
    m_image->AnalyzeImage();
    return isDIOk(result);
#endif
}

bool CPressDiskImage::open(QByteArray &data)
{
    Q_ASSERT(m_image);
    auto result = m_image->OpenImageFromBufferRO((const uint8_t*)(data.data()),data.length());
    m_last_di_error = result;

  //  m_image->AnalyzeImage();
  //  m_image->AnalyzeImageFS();

    return (isDIOk(result));
}

bool CPressDiskImage::close()
{
    Q_ASSERT(m_image);
    auto result = m_image->CloseImage();
    m_last_di_error = result;
    return (isDIOk(result));
}

bool CPressDiskImage::analyzeImage()
{
    Q_ASSERT(m_image);
//    auto result = m_image->AnalyzeImage();
//    m_last_di_error = result;
//    if (isDIOk(result))
    {
 //       m_image->AnalyzeImageFS();
    }
//    return (isDIOk(result));
    return true;
}

bool CPressDiskImage::showAsBlocks() const
{
    Q_ASSERT(m_image);
    return m_image->ShowAsBlocks();
}

CPressDiskImage::OuterFormat CPressDiskImage::GetOuterFormat() const
{
    Q_ASSERT(m_image);
    switch (m_image->GetOuterFormat())
    {
    case DiskImgLib::DiskImg::kOuterFormatNone:
        return OuterFormat::Unknown;
        break;
    case DiskImgLib::DiskImg::kOuterFormatCompress:
        return OuterFormat::Compress;
        break;
    case DiskImgLib::DiskImg::kOuterFormatGzip:
        return OuterFormat::GZip;
        break;
    case DiskImgLib::DiskImg::kOuterFormatBzip2:
        return OuterFormat::BZip2;
        break;
    case DiskImgLib::DiskImg::kOuterFormatZip:
        return OuterFormat::Zip;
        break;
    case DiskImgLib::DiskImg::kOuterFormatUnknown:
    default:
        return OuterFormat::Unknown;
        break;
    }
}

CPressDiskImage::FileFormat CPressDiskImage::GetFileFormat() const
{
    Q_ASSERT(m_image);
    switch (m_image->GetFileFormat())
    {
    case DiskImgLib::DiskImg::kFileFormatUnadorned:
        return FileFormat::Unadorned;
        break;
    case DiskImgLib::DiskImg::kFileFormat2MG:
        return FileFormat::TwoMG;
        break;
    case DiskImgLib::DiskImg::kFileFormatDiskCopy42:
        return FileFormat::DiskCopy42;
        break;
    case DiskImgLib::DiskImg::kFileFormatDiskCopy60:
        return FileFormat::DiskCopy60;
        break;
    case DiskImgLib::DiskImg::kFileFormatDavex:
        return FileFormat::Davex;
        break;
    case DiskImgLib::DiskImg::kFileFormatSim2eHDV:
        return FileFormat::Sim2eHDV;
        break;
    case DiskImgLib::DiskImg::kFileFormatTrackStar:
        return FileFormat::TrackStar;
        break;
    case DiskImgLib::DiskImg::kFileFormatFDI:
        return FileFormat::FDI;
        break;
    case DiskImgLib::DiskImg::kFileFormatNuFX:
        return FileFormat::NuFX;
        break;
    case DiskImgLib::DiskImg::kFileFormatDDD:
        return FileFormat::DDD;
        break;
    case DiskImgLib::DiskImg::kFileFormatDDDDeluxe:
        return FileFormat::DDDDeluxe;
        break;
    case DiskImgLib::DiskImg::kFileFormatUnknown:
    default:
        return FileFormat::Unknown;
        break;
    }
}

CPressDiskImage::PhysicalFormat CPressDiskImage::GetPhysicalFormat() const
{
    Q_ASSERT(m_image);
    switch (m_image->GetPhysicalFormat())
    {
    case DiskImgLib::DiskImg::kPhysicalFormatSectors:
        return PhysicalFormat::Sectors;
        break;
    case DiskImgLib::DiskImg::kPhysicalFormatNib525_6656:
        return PhysicalFormat::Nib525_6656;
        break;
    case DiskImgLib::DiskImg::kPhysicalFormatNib525_6384:
        return PhysicalFormat::Nib525_6384;
        break;
    case DiskImgLib::DiskImg::kPhysicalFormatNib525_Var:
        return PhysicalFormat::Nib525_Var;
        break;
    default:
    case DiskImgLib::DiskImg::kPhysicalFormatUnknown:
        return PhysicalFormat::Unknown;
        break;
    }
}

CPressDiskImage::SectorOrder CPressDiskImage::GetSectorOrder() const
{
    Q_ASSERT(m_image);
    switch (m_image->GetSectorOrder())
    {
    case DiskImgLib::DiskImg::kSectorOrderProDOS:
        return SectorOrder::ProDOS;
        break;
    case DiskImgLib::DiskImg::kSectorOrderDOS:
        return SectorOrder::DOS;
        break;
    case DiskImgLib::DiskImg::kSectorOrderCPM:
        return SectorOrder::CPM;
        break;
    case DiskImgLib::DiskImg::kSectorOrderPhysical:
        return SectorOrder::Physical;
        break;
    default:
    case DiskImgLib::DiskImg::kSectorOrderUnknown:
        return SectorOrder::Unknown;
        break;
    }
}

CPressDiskImage::FSFormat CPressDiskImage::GetFSFormat() const
{
    Q_ASSERT(m_image);
    switch (m_image->GetFSFormat())
    {
    case DiskImgLib::DiskImg::kFormatProDOS:
        return FSFormat::ProDOS;
        break;
    case DiskImgLib::DiskImg::kFormatDOS33:
        return FSFormat::DOS33;
        break;
    case DiskImgLib::DiskImg::kFormatDOS32:
        return FSFormat::DOS32;
        break;
    case DiskImgLib::DiskImg::kFormatPascal:
        return FSFormat::Pascal;
        break;
    case DiskImgLib::DiskImg::kFormatMacHFS:
        return FSFormat::MacHFS;
        break;
    case DiskImgLib::DiskImg::kFormatMacMFS:
        return FSFormat::MacMFS;
        break;
    case DiskImgLib::DiskImg::kFormatLisa:
        return FSFormat::Lisa;
        break;
    case DiskImgLib::DiskImg::kFormatCPM:
        return FSFormat::CPM;
        break;
    case DiskImgLib::DiskImg::kFormatMSDOS:
        return FSFormat::MSDOS;
        break;
    case DiskImgLib::DiskImg::kFormatISO9660:
        return FSFormat::ISO9660;
        break;
    case DiskImgLib::DiskImg::kFormatRDOS33:
        return FSFormat::RDOS33;
        break;
    case DiskImgLib::DiskImg::kFormatRDOS32:
        return FSFormat::RDOS32;
        break;
    case DiskImgLib::DiskImg::kFormatRDOS3:
        return FSFormat::RDOS3;
        break;
    case DiskImgLib::DiskImg::kFormatGenericPhysicalOrd:
        return FSFormat::GenericPhysicalOrd;
        break;
    case DiskImgLib::DiskImg::kFormatGenericProDOSOrd:
        return FSFormat::GenericProDOSOrd;
        break;
    case DiskImgLib::DiskImg::kFormatGenericDOSOrd:
        return FSFormat::GenericDOSOrd;
        break;
    case DiskImgLib::DiskImg::kFormatGenericCPMOrd:
        return FSFormat::GenericCPMOrd;
        break;
    case DiskImgLib::DiskImg::kFormatUNIDOS:
        return FSFormat::UNIDOS;
        break;
    case DiskImgLib::DiskImg::kFormatOzDOS:
        return FSFormat::OzDOS;
        break;
    case DiskImgLib::DiskImg::kFormatCFFA4:
        return FSFormat::CFFA4;
        break;
    case DiskImgLib::DiskImg::kFormatCFFA8:
        return FSFormat::CFFA8;
        break;
    case DiskImgLib::DiskImg::kFormatMacPart:
        return FSFormat::MacPart;
        break;
    case DiskImgLib::DiskImg::kFormatMicroDrive:
        return FSFormat::MicroDrive;
        break;
    case DiskImgLib::DiskImg::kFormatFocusDrive:
        return FSFormat::FocusDrive;
        break;
    case DiskImgLib::DiskImg::kFormatGutenberg:
        return FSFormat::Gutenberg;
        break;
    default:
    case DiskImgLib::DiskImg::kFormatUnknown:
        return FSFormat::Unknown;
        break;
    }
}

int CPressDiskImage::numTracks() const
{
    Q_ASSERT(m_image);
    return (int) m_image->GetNumTracks();
}

int CPressDiskImage::sectorsPerTrack() const
{
    Q_ASSERT(m_image);
    return (int) m_image->GetNumSectPerTrack();
}

int CPressDiskImage::numBlocks() const
{
    Q_ASSERT(m_image);
    return (int) m_image->GetNumBlocks();
}

bool CPressDiskImage::checkForBadBlocks(int startBlock, int numBlocks)
{
    Q_ASSERT(m_image);
    return m_image->CheckForBadBlocks(startBlock, numBlocks);
}

short CPressDiskImage::dosVolumeNumber() const
{
    Q_ASSERT(m_image);
    return m_image->GetDOSVolumeNum();
}

bool CPressDiskImage::hasSectors() const
{
    Q_ASSERT(m_image);
    return m_image->GetHasSectors();
}

bool CPressDiskImage::hasBlocks() const
{
    Q_ASSERT(m_image);
    return m_image->GetHasBlocks();
}

bool CPressDiskImage::hasNibbles() const
{
    Q_ASSERT(m_image);
    return m_image->GetHasNibbles();
}

QByteArray CPressDiskImage::readTrackSector(int track, int sector)
{
    QByteArray retval;
    Q_ASSERT(m_image);
    char data[256];
    auto result = m_image->ReadTrackSector(track,sector,data);
    m_last_di_error = result;
    if (isDIOk(result))
    {
        retval.resize(256);
        for (int idx = 0; idx < 256; idx++)
        {
            retval[idx] = data[idx];
        }
    }
 //   qDebug() << "Read track " << track << " sector " << sector << " = " << retval.size() << "bytes";
    return retval;
}

QByteArray CPressDiskImage::readBlock(int block)
{
    QByteArray retval;
    Q_ASSERT(m_image);
    char data[512];
    auto result = m_image->ReadBlock(block,data);
    m_last_di_error = result;
    if (isDIOk(result))
    {
        retval.resize(512);
        for (int idx = 0; idx < 512; idx++)
        {
            retval[idx] = data[idx];
        }
    }
    return retval;
}

QByteArray CPressDiskImage::readBlocks(int startBlock, int numBlocks)
{
    QByteArray retval;
    Q_ASSERT(m_image);
    char *data = new char[512*numBlocks];

    auto result = m_image->ReadBlocks(startBlock,numBlocks,data);
    m_last_di_error = result;
    if (isDIOk(result))
    {
        retval.resize(512*numBlocks);
        for (int idx = 0; idx < 512*numBlocks; idx++)
        {
            retval[idx] = data[idx];
        }
    }
    delete[] data;
    return retval;
}

