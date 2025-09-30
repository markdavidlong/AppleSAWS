#include "BinaryFile.h"
#include "Util.h"

#include <QDebug>
#include <QByteArray>

BinaryFile::BinaryFile(const QByteArray& data) : GenericFile(data)
{
    m_length = 0;

    if (!data.isEmpty()) {
        setData(data);
    }
}

void BinaryFile::setData(const QByteArray& data)
{
    if (data.length() >= 4) {
        QByteArray metadata = data.left(4);
        m_data = data.mid(4);
        setAddress(makeWord(metadata[0],metadata[1]));
        m_length = makeWord(metadata[2],metadata[3]);
    }    
}

void BinaryFile::dump()
{
    qDebug() << QString("Address: %1  Length: %2")
                .arg((quint16) address(),4,16,QChar('0'))
                .arg(m_length,4,16,QChar('0')).toUpper();
    qDebug() << QString("Data Length Recorded: %1")
                .arg(m_data.length(),4,16,QChar('0')).toUpper();
}
