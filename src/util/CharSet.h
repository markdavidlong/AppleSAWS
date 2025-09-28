#pragma once

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




