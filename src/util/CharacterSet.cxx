/**
 * @file CharacterSet.cxx
 * @brief Implementation of CharacterSet class
 */

#include "CharacterSet.h"

#include <QDebug>

void CharacterSet::buildSetFromSetBlob(QByteArray data)
{
    constexpr int ExpectedSize = 768;
    constexpr int StartAscii = 32;
    constexpr int BytesPerChar = 8;
    
    if (data.size() != ExpectedSize) {
        qWarning("Unexpected character set size: %d (not %d). Resizing.", 
                 static_cast<int>(data.size()), ExpectedSize);
        data.resize(ExpectedSize);
    }
    
    int val = StartAscii;
    for (int idx = 0; idx < ExpectedSize; idx += BytesPerChar)
    {
        const QByteArray ch = data.mid(idx, BytesPerChar);
        const CharSetCharacter csc(static_cast<quint8>(val), ch);
        m_charmap.insert(val, csc);
        ++val;
    }
}

QList<CharSetCharacter> CharacterSet::allCharacters() const
{
    return m_charmap.values();
}
