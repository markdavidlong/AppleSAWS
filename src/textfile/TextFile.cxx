#include "TextFile.h"
#include <QDebug>

TextFile::TextFile(const QByteArray& data) noexcept 
    : GenericFile(data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}

void TextFile::setData(const QByteArray& data)
{
    m_data = data;
}

void TextFile::dump()
{
    // TODO: Implement text file dumping functionality
}
