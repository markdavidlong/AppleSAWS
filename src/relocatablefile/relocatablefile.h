#ifndef RELOCATABLEFILE_H
#define RELOCATABLEFILE_H

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



#include <QPair>
#include <QByteArray>

#include "genericfile.h"
#include "util.h"


typedef enum { RFS1Byte, RFS2Byte } FieldSize;
typedef enum { ULIsHighByte, ULIsLowByte } WordUpperLower;
typedef enum { LowHi, HiLow } Endianness;
typedef enum { ByteLo, ByteHi, ESDSymbol } Byte4Type;

typedef QPair<Byte4Type, quint8> Byte4ReturnType;

class RelocatableDictItem {
public:
    RelocatableDictItem() {init(0,0,0,0); }
    RelocatableDictItem(quint8 rld, quint8 fo_low, quint8 fo_hi, quint8 lbsym) {
        init(rld,fo_low, fo_hi,lbsym);
    }
    RelocatableDictItem(quint8 rld, quint16 fo, quint8 lbsym) {
        init(rld,fo,lbsym);
    }

    void init(quint8 rld, quint8 fo_low, quint8 fo_hi, quint8 lbsym) {
        m_rld_flag = rld;
        m_halfword_or_sym_num = lbsym;
        m_field_offset = makeWord(fo_low,fo_hi);
    }

    void init(quint8 rld, quint16 fo, quint8 lbsym) {
        m_rld_flag = rld;
        m_halfword_or_sym_num = lbsym;
        m_field_offset = fo;
    }

    FieldSize getFieldSize() { return (m_rld_flag & 0x80)?RFS2Byte:RFS1Byte; }
    WordUpperLower getWordBytePos() { return (m_rld_flag & 0x40)?ULIsHighByte:ULIsLowByte; }
    Endianness getEndianness() { return (m_rld_flag & 0x20)?HiLow:LowHi; }
    bool isExtern() { return (m_rld_flag & 0x10); }
    bool isNotEndOfRLD() { return (m_rld_flag & 0x01); }
    bool isEndOfRLD() { return !(m_rld_flag & 0x01); }

    Byte4ReturnType getByte4() {
        Byte4ReturnType retval;

        if (isExtern()) {
            retval.first = ESDSymbol;
        } else if (getWordBytePos() == ULIsHighByte) {
            retval.first = ByteHi;
        } else {
            retval.first = ByteLo;
        }

        retval.second = m_halfword_or_sym_num;

        return retval;
    }

    quint16 getFieldOffset() { return m_field_offset; }

    quint8 getRLDFlag() { return m_rld_flag; }
    quint8 getRawHalfWordOrSymNum() { return m_halfword_or_sym_num; }

private:

    quint8  m_rld_flag;
    quint16 m_field_offset;
    quint8 m_halfword_or_sym_num;

};



class RelocatableFile : public GenericFile
{
    friend class Dos33DiskImage;

public:


    void dump();

    QByteArray getBinaryCodeImage() { return m_binary_code_image; }
    QList<RelocatableDictItem> getRelocatableDict() { return m_relocatable_dict; }

    quint16 codeImageLength() { return m_code_image_length; }

    QStringList decodeRelocatableDict();

protected:
    RelocatableFile(Dos33DiskImage *image, FileDescriptiveEntry &fde);
    void setupData();

    quint16 m_ram_image_length;
    quint16 m_code_image_length;

    QByteArray m_binary_code_image;

    QList<RelocatableDictItem> m_relocatable_dict;


};

#endif
