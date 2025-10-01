/**
 * @file CharSetCharacter.cxx
 * @brief Implementation of CharSetCharacter class
 */

#include "CharSetCharacter.h"

CharSetCharacter::CharSetCharacter(quint8 asciival, QByteArray bytes) noexcept
    : m_asciival(asciival)
{
    setData(bytes);
}

void CharSetCharacter::setData(QByteArray bytes)
{
    bytes.resize(8);
    m_data = bytes;
}

void CharSetCharacter::setData(quint8 b0, quint8 b1, 
                                quint8 b2, quint8 b3, 
                                quint8 b4, quint8 b5, 
                                quint8 b6, quint8 b7)
{
    QByteArray data;
    data.reserve(8);
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