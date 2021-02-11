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

QString GenericFile::getFileType() const
{
    if (m_file_type == "A" || m_file_type == "B" || m_file_type == "T" ||
        m_file_type == "I" || m_file_type == "R" || m_file_type == "S" ||
        m_file_type == "a" || m_file_type == "b")
    {
        return m_file_type;
    }
    else
    {
        return "?";
    }
}
