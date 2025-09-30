#include "GenericFile.h"

GenericFile::GenericFile(const QByteArray& data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}

void GenericFile::setData(const QByteArray& data)
{
    m_data = data;
    m_length = data.size();
}
