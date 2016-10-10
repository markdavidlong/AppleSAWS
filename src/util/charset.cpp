#include "charset.h"
#include <QDebug>


CharSetCharacter::CharSetCharacter(quint8 asciival, QByteArray bytes)
{
    setAsciiVal(asciival);
    setData(bytes);
}

void CharSetCharacter::setData(QByteArray bytes)
{
    bytes.resize(8);
    m_data = bytes;
}

void CharSetCharacter::setData(quint8 b0, quint8 b1, quint8 b2, quint8 b3, quint8 b4, quint8 b5, quint8 b6, quint8 b7)
{
    QByteArray data;
    data.append(b0);
    data.append(b1);
    data.append(b2);
    data.append(b3);
    data.append(b4);
    data.append(b5);
    data.append(b6);
    data.append(b7);

    setData(data);
}

void CharacterSet::buildSetFromSetBlob(QByteArray data)
{
    if (data.size() != 768) {
        qWarning("Unexpected character set size: %d (not 768).  Resizing.",data.size());
        data.resize(768);
    }
qDebug() << "BuildSet: " << data;
    int val = 32;
    for (int idx = 0; idx < 768; idx+=8)
    {
        QByteArray ch = data.mid(idx,8);
        qDebug() << "Added character " << idx / 8 << "Data: " << ch;

        CharSetCharacter csc = CharSetCharacter(val,ch);
        m_charmap.insert(val,csc);
        val++;
    }
}

QList<CharSetCharacter> CharacterSet::allCharacters() const
{
    qDebug() << "allCharacters: " << m_charmap.values().count();
    return m_charmap.values();


}
