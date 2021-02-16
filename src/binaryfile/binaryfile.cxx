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
    QByteArray data = rawData().asQByteArray();

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
