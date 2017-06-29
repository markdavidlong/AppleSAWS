#include "genericfile.h"

GenericFile::GenericFile(QByteArray data)
{
    m_diskfile = 0;
    if (!data.isEmpty()) {
        setData(data);
    }
    m_address = 0x00;
    m_length = 0x00;
}

void GenericFile::setData(QByteArray data)
{
    m_data = data;
    m_length = data.size();
}
