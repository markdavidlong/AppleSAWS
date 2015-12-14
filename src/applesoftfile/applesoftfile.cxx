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
    m_detokenized = detokenize();
}

QList<ApplesoftLine> ApplesoftFile::detokenize(quint16 start_address)
{
    QList<ApplesoftLine> retval;

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
   //         line.raw_tokens.append(val);

//            if (val >= 0x80) {
//            //    line.detokenized_line.append("~");
//               line.detokenized_line.append(m_tokens[val]);
//            } else {
//                if (val >= 0x20) {
//                    line.detokenized_line.append(val);
//                } else if (val == 0x7F) {
//                    QChar ch = QChar(0x2401);
//                    line.detokenized_line.append(ch);
//                }else if (val > 0x00) {
//                    QChar ch = QChar(0x2400 + val);
//                    line.detokenized_line.append(ch);
//                }
//            }
        } while (val != 0x00);

        Retokenizer::retokenize(line);

        //Make Detokenized line
        foreach (ApplesoftToken token, line.tokens) {
            quint16 tokenval = token.getTokenId();
            line.detokenized_line.append(token.getRawPrintableString());
//            if (tokenval >= 0x80 && tokenval < 0xFE) {
//                line.detokenized_line.append("{kwd}");
//            } else if (tokenval >= 0xF0) {
//               line.detokenized_line.append(ApplesoftToken::getStringForToken(tokenval));
//            } else {
//                if (tokenval >= 0x20) {
//                    line.detokenized_line.append(tokenval);
//                } else if (tokenval == 0x7F) {
//                    QChar ch = QChar(0x2401);
//                    line.detokenized_line.append(ch);
//                }else if (tokenval > 0x00) {
//                    QChar ch = QChar(0x2400 + tokenval);
//                    line.detokenized_line.append(ch);
//                }
//            }
        }


        current_address = line.next_address;
        retval.append(line);
    }

    m_data_end = idx;

    if (idx < m_data.length()) {
        qDebug() << QString("%1 byte(s) unaccounted for.").arg(m_data.length() - idx);
    }

    return retval;
}

void ApplesoftFile::list() {
    foreach (ApplesoftLine line,detokenized()) {
        QString debugline = QString("%1  %2  %3").arg(line.next_address,4,16,QChar('0')).arg(line.linenum).arg(line.detokenized_line);
        qDebug() << debugline;
/*
        debugline = "";
        foreach (quint8 val, line.tokens) {
            debugline.append(QString("%1 ").arg(val,2,16,QChar('0')));
        }
        qDebug() << "  " << debugline;
        qDebug() << "\n";
*/
    }

    qDebug() << extraDataHexValues().join("\n");
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
