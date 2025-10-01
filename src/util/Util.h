#pragma once

#include <QPair>
#include <QByteArray>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <QSettings>

typedef enum
{
    DOSTextFile = 0x00,
    DOSIntegerBasicFile = 0x01,
    DOSApplesoftBasicFile = 0x02,
    DOSRawBinaryFile = 0x04,
    DOSTypeSFile = 0x08,
    DOSRelocatableFile = 0x10,
    DOSTypeAFile = 0x20,
    DOSTypeBFile = 0x40,
    DOSIsLocked = 0x80
} FileTypeFlag;

typedef enum
{
    Inverse = 0x00,   // 0x00 -- 0x3F
    Flash = 0x01,     // 0x40 -- 0x7F
    NormalLow = 0x02, // 0x80 -- 0xBF
    NormalHigh = 0x04 // 0xC0 -- 0xFF
} TextAttribute;

typedef enum
{
    SetInvUC = 0x00,
    SetInvSp = 0x20,
    SetFlUC = 0x40,
    SetFlSp = 0x60,
    SetNormUC = 0x80,
    SetNormSp = 0xA0,
    SetNormAltUC = 0xC0,
    SetNormLC = 0xE0
} TextSet;

inline QString uint8ToHex(quint8 val)
{
    QString retval = QString("%1").arg(val, 2, 16, QChar('0')).toUpper();
    return retval;
}

inline QString uint16ToHex(quint16 val)
{
    QString retval = QString("%1").arg(val, 4, 16, QChar('0')).toUpper();
    return retval;
}

inline QString uint32ToHex(quint32 val)
{
    QString retval = QString("%1").arg(val, 8, 16, QChar('0')).toUpper();
    return retval;
}

inline quint16 makeWord(quint8 lo, quint8 hi) 
{
    return hi * 256 + lo;
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

