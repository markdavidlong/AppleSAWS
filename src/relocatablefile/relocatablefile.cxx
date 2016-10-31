#include <QDebug>
#include "relocatablefile.h"
#include "util.h"

RelocatableFile::RelocatableFile(QByteArray data) : GenericFile(data)
{
 //   qDebug() << "Relocatable file ctor";
    if (!data.isEmpty()) {
        setData(data);
    }
}

void RelocatableFile::setData(QByteArray data)
{
 //   qDebug() << "setData()";
    if (data.length() >= 6) {
        m_starting_ram_address = makeWord(m_data[0],m_data[1]);
        m_ram_image_length = makeWord(m_data[2],m_data[3]);
        m_code_image_length = makeWord(m_data[4],m_data[5]);

        int offset = 0;

        for (int idx = 6; idx < m_code_image_length+6; idx++) {
            quint8 val = m_data[idx];
            m_binary_code_image.append(val);
        }

        for (int N = 0; N < (m_code_image_length/4); N++) {
            offset = m_code_image_length+ 6 +(N*4);

//            qDebug() << "N: " << N << uint8ToHex(m_data[offset])
//                     << uint8ToHex(m_data[offset+1])
//                    << uint8ToHex(m_data[offset+2])
//                    << uint8ToHex(m_data[offset+3]);

            RelocatableDictItem rdi = RelocatableDictItem(m_data[offset],m_data[offset+1],
                    m_data[offset+2],m_data[offset+3]);
            m_relocatable_dict.append(rdi);
            if (rdi.isEndOfRLD()) { break; }
        }



    }
}

void RelocatableFile::dump()
{
    qDebug() << "\nTotalLength: " << length();
    qDebug() << "Starting Ram Address: " << (quint16) m_starting_ram_address << uint16ToHex(m_starting_ram_address);
    qDebug() << "Length of Ram Image: " << (quint16) m_ram_image_length << uint16ToHex(m_ram_image_length);
    qDebug() << "Length of Code Image: " << (quint16) m_code_image_length << uint16ToHex(m_code_image_length);

    int itemIdx = 0;
    foreach (RelocatableDictItem item, m_relocatable_dict) {
        Byte4ReturnType b4rt = item.getByte4();
        QString typestr;
        if (b4rt.first == ESDSymbol) { typestr = "ESDSymbol"; }
        else if (b4rt.first == ByteHi) { typestr = "Hi Byte"; }
        else { typestr = "Lo Byte"; }
        quint16 fo = item.getFieldOffset();
        qDebug() << "  Item #" << itemIdx++
                 << "Field Offset: " << uint16ToHex(fo)
                 << "FieldSize: " << ((item.getFieldSize()==RFS2Byte)?"2-Byte":"1-Byte")
                 << typestr << uint8ToHex(b4rt.second)
                 << ((item.isNotEndOfRLD())?"NotEndOfRLD":"EndOfRLD")
                 << "    " << ((item.isExtern())?"Extern":"Not Extern");
    }


}

QStringList RelocatableFile::decodeRelocatableDict()
{
    QStringList retval;
    int idx = 0;
    foreach (RelocatableDictItem item, m_relocatable_dict) {
        Byte4ReturnType b4rt = item.getByte4();
        QString typestr;
        if (b4rt.first == ESDSymbol) { typestr = "ESDSymbol"; }
        else if (b4rt.first == ByteHi) { typestr = "Hi Byte"; }
        else { typestr = "Lo Byte"; }
        quint16 fo = item.getFieldOffset();
//        qDebug() << "  Item #" << idx
//                 << "Field Offset: " << uint16ToHex(fo)
//                 << "FieldSize: " << ((item.getFieldSize()==RFS2Byte)?"2-Byte":"1-Byte")
//                 << typestr << uint8ToHex(b4rt.second)
//                 << ((item.isNotEndOfRLD())?"NotEndOfRLD":"EndOfRLD")
//                 << "    " << ((item.isExtern())?"Extern":"Not Extern");

        retval.append(QString("Item %1, Offset %2, @ %3, %4 Field, %5")
                .arg(idx++)
                .arg(uint16ToHex(fo))
                .arg(uint16ToHex(fo+address()+6))
                .arg((item.getFieldSize()==RFS2Byte)?"2-Byte":"1-Byte")
                .arg((item.isExtern())?"Extern":"Not Extern"));
    }

    return retval;
}
