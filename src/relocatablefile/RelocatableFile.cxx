#include "RelocatableFile.h"
#include <QDebug>

// This file format is documented in the "Apple 6502 Assembler/Editor" manual.

RelocatableFile::RelocatableFile(const QByteArray &data) noexcept
    : GenericFile(data)
{
    if (!data.isEmpty())
    {
        setData(data);
    }
}

void RelocatableFile::setData(const QByteArray &data)
{
    if (data.length() >= 6)
    {
        m_starting_ram_address = makeWord(m_data[0], m_data[1]);
        m_address = m_starting_ram_address;
        m_ram_image_length = makeWord(m_data[2], m_data[3]);
        m_code_image_length = makeWord(m_data[4], m_data[5]);

        m_data = m_data.mid(6);
        int offset = 0;

        for (int idx = 0; idx < m_code_image_length; idx++)
        {
            quint8 val = m_data[idx];
            m_binary_code_image.append(val);
        }

        for (int N = 0; N < (m_code_image_length); N += 4)
        {

            offset = m_code_image_length + N;

            //            qDebug() << "N: " << N << uint8ToHex(m_data[offset])
            //                     << uint8ToHex(m_data[offset+1])
            //                    << uint8ToHex(m_data[offset+2])
            //                    << uint8ToHex(m_data[offset+3]);

            RelocatableDictItem rdi = RelocatableDictItem(m_data[offset], m_data[offset + 1],
                                                          m_data[offset + 2], m_data[offset + 3]);
            if (rdi.isEndOfRLD())
            {
                break;
            }
            m_relocatable_dict.append(rdi);
        }

        //   m_data = m_binary_code_image;
    }
}

void RelocatableFile::dump()
{
    qDebug() << "\nTotalLength: " << length();
    qDebug() << "Starting Ram Address: " << m_starting_ram_address << uint16ToHex(m_starting_ram_address);
    qDebug() << "Length of Ram Image: " << m_ram_image_length << uint16ToHex(m_ram_image_length);
    qDebug() << "Length of Code Image: " << m_code_image_length << uint16ToHex(m_code_image_length);

    int itemIdx = 0;
    for (const auto &item : m_relocatable_dict)
    {
        const auto b4rt = item.getByte4();
        QString typestr;
        if (b4rt.first == RelocatableTypes::Byte4Type::ESDSymbol)
        {
            typestr = QStringLiteral("ESDSymbol");
        }
        else if (b4rt.first == RelocatableTypes::Byte4Type::ByteHi)
        {
            typestr = QStringLiteral("Hi Byte");
        }
        else
        {
            typestr = QStringLiteral("Lo Byte");
        }
        const quint16 fo = item.getFieldOffset();
        qDebug() << "  Item #" << itemIdx++
                 << "Field Offset: " << uint16ToHex(fo)
                 << "FieldSize: " << ((item.getFieldSize() == RelocatableTypes::FieldSize::TwoByte) ? "2-Byte" : "1-Byte")
                 << typestr << uint8ToHex(b4rt.second)
                 << ((item.isNotEndOfRLD()) ? "NotEndOfRLD" : "EndOfRLD")
                 << "    " << ((item.isExtern()) ? "Extern" : "Not Extern");
    }
}

QStringList RelocatableFile::decodeRelocatableDict() const
{
    QStringList retval;
    int idx = 0;
    for (const auto &item : m_relocatable_dict)
    {
        const auto b4rt = item.getByte4();
        QString typestr;
        if (b4rt.first == RelocatableTypes::Byte4Type::ESDSymbol)
        {
            typestr = QStringLiteral("ESDSymbol");
        }
        else if (b4rt.first == RelocatableTypes::Byte4Type::ByteHi)
        {
            typestr = QStringLiteral("Hi Byte");
        }
        else
        {
            typestr = QStringLiteral("Lo Byte");
        }
        const quint16 fo = item.getFieldOffset();

        retval.append(QStringLiteral("Item %1, Offset %2, @ %3, %4 Field, %5")
                          .arg(idx++)
                          .arg(uint16ToHex(fo))
                          .arg(uint16ToHex(fo + address() + 6))
                          .arg((item.getFieldSize() == RelocatableTypes::FieldSize::TwoByte) ? "2-Byte" : "1-Byte")
                          .arg((item.isExtern()) ? "Extern" : "Not Extern"));
    }

    return retval;
}
