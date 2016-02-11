#include "genericfile.h"

GenericFile::GenericFile(QByteArray data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
    m_address = 0x00;
}

void GenericFile::setData(QByteArray data)
{
    m_data = data;
}
