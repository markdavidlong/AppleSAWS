#ifndef UTIL_H
#define UTIL_H

#include <QPair>
#include <QByteArray>
#include <QDebug>

typedef enum {
    TextFile = 0x00,
    IntegerBasicFile = 0x01,
    ApplesoftBasicFile = 0x02,
    RawBinaryFile = 0x04,
    TypeSFile = 0x08,
    RelocatableFile = 0x10,
    TypeAFile = 0x20,
    TypeBFile= 0x40,
    IsLocked = 0x80
} FileTypeFlag;

typedef enum {
    Inverse    = 0x00,  // 0x00 -- 0x3F
    Flash      = 0x01,  // 0x40 -- 0x7F
    Normal     = 0x02,  // 0x80 -- 0xBF
    NormalHigh = 0x04   // 0xC0 -- 0xFF
} TextAttribute;

typedef enum {
    SetInvUC = 0x00,
    SetInvSp = 0x20,
    SetFlUC = 0x40,
    SetFlSp = 0x60,
    SetNormUC = 0x80,
    SetNormSp = 0xA0,
    SetNormAltUC = 0xC0,
    SetNormLC = 0xE0
} TextSet;

struct TSPair {
    TSPair() { track = sector = 0; }
    TSPair(quint8 trackval, quint8 secval) { track=trackval; sector = secval; }
    quint8 track;
    quint8 sector;

    bool operator==(const TSPair &other) {
        if (other.track == track && other.sector == sector) return true;
        return false;
    }

    bool operator!=(const TSPair &other) {
        return !(operator==(other));
    }

    void dump() { qDebug() << "TSPair: track: " << track << " sector: " << sector; }
};



#endif // UTIL_H
