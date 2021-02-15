#ifndef UTIL_H
#define UTIL_H

#include <QPair>
#include <QByteArray>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <QSettings>

enum class FileTypeFlag : quint8 {
    Text = 0x00,
    Integer = 0x01,
    Applesoft = 0x02,
    Binary = 0x04,
    TypeS = 0x08,
    Relocatable = 0x10,
    TypeA = 0x20,
    TypeB= 0x40,
    IsLockedFlag = 0x80
};

enum class TextAttribute {
    Inverse    = 0x00,  // 0x00 -- 0x3F
    Flash      = 0x01,  // 0x40 -- 0x7F
    NormalLow  = 0x02,  // 0x80 -- 0xBF
    NormalHigh = 0x04   // 0xC0 -- 0xFF
};

enum class TextSet {
    SetInvUC = 0x00,
    SetInvSp = 0x20,
    SetFlUC = 0x40,
    SetFlSp = 0x60,
    SetNormUC = 0x80,
    SetNormSp = 0xA0,
    SetNormAltUC = 0xC0,
    SetNormLC = 0xE0
};




inline QString uint8ToHex(quint8 val) {
    QString retval = QString("%1").arg(val,2,16,QChar('0')).toUpper();
    return retval;
}

inline QString uint16ToHex(quint16 val) {
    QString retval = QString("%1").arg(val,4,16,QChar('0')).toUpper();
    return retval;
}

inline QString uint32ToHex(quint32 val) {
    QString retval = QString("%1").arg(val,8,16,QChar('0')).toUpper();
    return retval;
}

inline quint16 makeWord(quint8 lo, quint8 hi)
{
    return hi*256 + lo;
}

inline QFont fontFromSettings(QString key, QFont &defaultfont)
{
    QSettings settings;
    QString result = settings.value(key, defaultfont.toString()).toString();
    QFont retval;
    retval.fromString(result);
    return retval;
}

inline void fontToSettings(const QString &key, const QFont &font)
{
    QSettings settings;
    settings.setValue(key, font.toString());
}


#endif // UTIL_H
