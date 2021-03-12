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

#include <QDebug>
#include "binaryfile.h"
#include "util.h"

BinaryFile::BinaryFile(Dos33DiskImage *image, FileDescriptiveEntry &fde)
    : GenericFile(image,fde)
{
    setupData();
}

void BinaryFile::setupData()
{
    QByteArray data = rawData();

    if (data.length() >= 4) {
        QByteArray metadata = data.left(4);
        GenericFile::setData(data.mid(4));
        setAddress(makeWord(metadata[0],metadata[1]));
        setLength(makeWord(metadata[2],metadata[3]));
    }
}

void BinaryFile::dump()
{
    qDebug() << QString("Address: %1  Length: %2")
                .arg((quint16) address(),4,16,QChar('0'))
                .arg(length(),4,16,QChar('0')).toUpper();
}
