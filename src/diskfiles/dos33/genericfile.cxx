#include "genericfile.h"

GenericFile::GenericFile(QByteArray data)
{
    m_data_loaded = false;
    m_diskfile = 0;
    if (!data.isEmpty()) {
        setData(data);
        m_data_loaded = true;
    }
    m_address = 0x00;
    m_length = 0x00;
    m_ignore_offset = 0;
}

GenericFile::GenericFile(Dos33DiskImage *image, TSPairList pairs)
{
    m_diskfile = image;
    m_ts_pairs = pairs;
    m_length = 0x00;
    m_address = 0x00;
    m_ignore_offset = 0;
    m_data_loaded = false;
}

quint8 GenericFile::dataAt(int offset) const
{
    return m_data[offset ];
}

void GenericFile::setData(QByteArray data)
{
    m_data_loaded = true;
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



quint16 GenericFile::dataWordAt(int offset) const
{
    return makeWord(dataAt(offset),dataAt(offset+1));
}
