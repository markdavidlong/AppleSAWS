#ifndef CHARSETCHARACTER_H
#define CHARSETCHARACTER_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include <QByteArray>
#include <QMap>



class CharSetCharacter
{
public:
    CharSetCharacter(quint8 asciival = 0, QByteArray bytes = QByteArray());
    void setAsciiVal(quint8 val) { m_asciival = val; }
    quint8 asciiVal() const { return m_asciival; }

    void setData(QByteArray bytes);
    void setData(quint8 b0 = 0, quint8 b1 = 0, quint8 b2 = 0, quint8 b3 = 0,
                 quint8 b4 = 0, quint8 b5 = 0, quint8 b6 = 0, quint8 b7 = 0);
    QByteArray data() const { return m_data; }

private:
    quint8 m_asciival;
    QByteArray m_data;
};


class CharacterSet
{
public:
    void buildSetFromSetBlob(QByteArray data);
    CharSetCharacter operator[](int asciival) const { return m_charmap[asciival]; }
    QList<CharSetCharacter> allCharacters() const;
    bool contains(int asciival) { return m_charmap.keys().contains(asciival); }

private:
    QMap<int, CharSetCharacter> m_charmap;
};




#endif // CHARSETCHARACTER_H
