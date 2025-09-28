#include <QDebug>
#include "TextFile.h"

TextFile::TextFile(QByteArray data) : GenericFile(data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}

void TextFile::setData(QByteArray data)
{
    m_data = data;
}

void TextFile::dump()
{
}
