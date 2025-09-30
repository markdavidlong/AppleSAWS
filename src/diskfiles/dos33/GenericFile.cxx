#include "GenericFile.h"

#include <utility>

GenericFile::GenericFile(const QByteArray& data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}

void GenericFile::setData(const QByteArray& data)
{
    m_data = data;
    m_length = static_cast<quint16>(data.size());
}

void GenericFile::setData(QByteArray&& data)
{
    m_length = static_cast<quint16>(data.size());
    m_data = std::move(data);
}
