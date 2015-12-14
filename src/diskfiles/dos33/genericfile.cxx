#include "genericfile.h"

GenericFile::GenericFile(QByteArray data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}

void GenericFile::setData(QByteArray data)
{
    m_data = data;
}
