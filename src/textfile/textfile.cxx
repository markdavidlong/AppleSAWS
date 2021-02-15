#include <QDebug>
#include "textfile.h"

TextFile::TextFile(QByteArray data) : GenericFile(data)
{
    if (!data.isEmpty()) {
        setData(data);
    }
}


void TextFile::dump()
{
}
