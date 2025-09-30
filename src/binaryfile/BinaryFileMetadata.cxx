#include "BinaryFileMetadata.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

BinaryFileMetadata::BinaryFileMetadata(GenericFile &file, quint16 defaultAddress, QObject *parent)
    : QObject(parent), m_file(file), m_defaultAddress(defaultAddress)
{
    load();
}

void BinaryFileMetadata::load()
{
    QFile infile(QString("%1%2%3")
                 .arg(m_file.diskFile()->getMetaDataPath())
                 .arg(m_file.filename())
                 .arg(".bfm"));
    if (infile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Loading binary file metadata from" << QString("%1%2%3")
                    .arg(m_file.diskFile()->getMetaDataPath())
                    .arg(m_file.filename())
                    .arg(".bfm");
        QDataStream ds(&infile);
        ds >> m_eps;
        ds >> m_as;
        infile.close();
    }
    else qDebug() << "Cannot open " << QString("%1%2%3")
                     .arg(m_file.diskFile()->getMetaDataPath())
                     .arg(m_file.filename())
                     .arg(".bfm") << "for reading";

}

void BinaryFileMetadata::save()
{
    QFile outfile(QString("%1%2%3")
                  .arg(m_file.diskFile()->getMetaDataPath())
                  .arg(m_file.filename())
                  .arg(".bfm"));
    if (outfile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Saving binary file metadata to" << QString("%1%2%3")
                    .arg(m_file.diskFile()->getMetaDataPath())
                    .arg(m_file.filename())
                    .arg(".bfm");
        QDataStream ds(&outfile);
        ds << m_eps;
        ds << m_as;
        outfile.close();
    }
    else qDebug() << "Cannot open " << QString("%1%2%3")
                     .arg(m_file.diskFile()->getMetaDataPath())
                     .arg(m_file.filename())
                     .arg(".bfm") << "for writing";

}




