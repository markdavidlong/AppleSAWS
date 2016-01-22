#include "applesoftfile.h"
#include <QDebug>

ApplesoftFile::ApplesoftFile(QByteArray data) : GenericFile(data)
{
    if (!data.isEmpty())
    {
        setData(data);
    }
}

void ApplesoftFile::setData(QByteArray data)
{
    GenericFile::setData(data);

    quint8 addlo = m_data.at(0);
    quint8 addhi = m_data.at(1);
    m_length = addlo + (addhi * 256);
    m_data.remove(0,2);
    parse();
}

void ApplesoftFile::parse(quint16 start_address)
{
    int idx = 0;
    quint8 val = 0;

    quint16 current_address = start_address;
    while (idx < m_data.length()) {
        ApplesoftLine line;
        line.address = current_address;
        line.next_address = (quint8) m_data[idx] + (((quint8) m_data[idx+1]) *256);

        idx++; idx++;
        line.linenum = (quint8) m_data[idx] + (((quint8) m_data[idx+1])*256);

        idx++; idx++;
        if (line.next_address == 0x00) { break; }
        do {
            val = m_data[idx++];
            ApplesoftToken token(val);
            line.tokens.append(token);
        } while (val != 0x00);

        current_address = line.next_address;
        m_lines.append(line);
    }

    m_data_end = idx;

    if (idx < m_data.length()) {
        qDebug() << QString("%1 byte(s) unaccounted for.").arg(m_data.length() - idx);
    }
}

QStringList ApplesoftFile::extraDataHexValues() {
    QStringList retval;

    QString debugline = "";
    int count = 0;
    foreach (quint8 val, extraData()) {
        debugline.append(QString("%1").arg(val,2,16,QChar('0')).toUpper());
        count++;
        if (count == 16) {
            retval.append(debugline);
            debugline = "";
            count = 0;
        } else {
            debugline.append(" ");
        }
    }
    if (!debugline.simplified().isEmpty()) {
        retval.append(debugline);
    }
    return retval;
}

QByteArray ApplesoftFile::extraData()
{
    return m_data.mid(m_data_end);
}

void Retokenizer::retokenize(ApplesoftLine &line)
{
//    QList <QByteArray> string_list;
//    QList <QByteArray> number_list;

//    QByteArray newTokens;

//// Set Strings aside for a bit to simplify parsing
//    QByteArray tmpTokens = line.raw_tokens;
//    QByteArray tmpstr;
//    bool inString = false;
//    for (int idx = 0; idx < line.raw_tokens.length(); idx++)
//    {
//        quint8 byte = tmpTokens.at(idx);

//        if (byte == '"') {
//            if (!inString) {
//                inString = true;
//                tmpstr.append(byte);
//            } else {
//                inString = false;
//                string_list.append(tmpstr);
//                tmpstr = "";
//                newTokens.append(0xff);
//            }
//        } else if (!inString) {
//            newTokens.append(byte);
//        }
//    }
//    if (inString) {
//        string_list.append(tmpstr);
//        newTokens.append(0xff);
//    }




////// Set Numbers aside for a bit too
////    tmpTokens = newTokens;
////    newTokens = "";
////    bool inNumber = false;
////    QByteArray tmpnum;
////    for (int idx = 0; idx < tmpTokens.length(); idx++)
////    {
////        quint8 byte = tmpTokens.at(idx);

////        if ((byte >= '0' && byte <= '9') || byte == '.') {
////            if (!inNumber) {
////                inNumber = true;
////                tmpnum.append(byte);
////            }
////        } else if (inNumber) {
////            inNumber = false;
////            number_list.append(tmpnum);
////            tmpnum = "";
////            newTokens.append(0xfe);
////            newTokens.append(byte);
////        } else {
////            newTokens.append(byte);
////        }
////    }
////    if (inNumber) {
////        number_list.append(tmpnum);
////        newTokens.append(0xfe);
////    }


//    line.advanced_tokens = newTokens;

//    //    QList<QByteArray> tmpParts = tmpTokens.split(':');

////    foreach (QByteArray part, tmpParts) {
////        QByteArray retokenizedPart = retokenzePart(part);
////    }
}

QByteArray Retokenizer::retokenizePart(QByteArray part) {
    QByteArray retval;

    return retval;
}
