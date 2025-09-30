#include <QDebug>
#include "TextFile.h"

TextFile::TextFile(const QByteArray& data) : GenericFile(data)
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
}
