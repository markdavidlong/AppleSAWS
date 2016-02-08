#include <QDebug>
#include "binaryfile.h"

BinaryFile::BinaryFile(QByteArray data) : GenericFile(data)
{
    m_length = 0;
    m_address = 0;

    if (!data.isEmpty()) {
        setData(data);
    }
}

void BinaryFile::setData(QByteArray data)
{
    if (data.length() >= 4) {
        QByteArray metadata = data.left(4);
        m_data = data.mid(4);
        m_address = (quint8) metadata[0] + ((quint8) metadata[1]*256);
        m_length =  (quint8) metadata[2] + ((quint8) metadata[3]*256);
    }    
}

void BinaryFile::dump()
{
    qDebug() << QString("Address: %1  Length: %2").arg(m_address,4,16,QChar('0')).arg(m_length,4,16,QChar('0')).toUpper();
    qDebug() << QString("Data Length Recorded: %1").arg(m_data.length(),4,16,QChar('0')).toUpper();
}
