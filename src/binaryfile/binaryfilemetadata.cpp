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

#include "binaryfilemetadata.h"
#include "genericfile.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

BinaryFileMetadata::BinaryFileMetadata(GenericFile *file, quint16 defaultAddress, QObject *parent)
    : QObject(parent)
{
    m_file = file;
    m_defaultAddress = defaultAddress;

    m_eps = new EntryPoints(this);
    m_as = new AssemblerSymbols(this);

    load();
}

void BinaryFileMetadata::load()
{
    QFile infile(QString("%1%2%3")
                 .arg(m_file->diskFile()->getMetaDataPath())
                 .arg(m_file->filename())
                 .arg(".bfm"));
    if (infile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Loading binary file metadata from" << QString("%1%2%3")
                    .arg(m_file->diskFile()->getMetaDataPath())
                    .arg(m_file->filename())
                    .arg(".bfm");
        QDataStream ds(&infile);
        ds >> *m_eps;
        ds >> *m_as;
        infile.close();
    }
    else qDebug() << "Cannot open " << QString("%1%2%3")
                     .arg(m_file->diskFile()->getMetaDataPath())
                     .arg(m_file->filename())
                     .arg(".bfm") << "for reading";

}

void BinaryFileMetadata::save()
{
    QFile outfile(QString("%1%2%3")
                  .arg(m_file->diskFile()->getMetaDataPath())
                  .arg(m_file->filename())
                  .arg(".bfm"));
    if (outfile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Saving binary file metadata to" << QString("%1%2%3")
                    .arg(m_file->diskFile()->getMetaDataPath())
                    .arg(m_file->filename())
                    .arg(".bfm");
        QDataStream ds(&outfile);
        ds << *m_eps;
        ds << *m_as;
        outfile.close();
    }
    else qDebug() << "Cannot open " << QString("%1%2%3")
                     .arg(m_file->diskFile()->getMetaDataPath())
                     .arg(m_file->filename())
                     .arg(".bfm") << "for writing";

}




