#include "applesoftfile.h"
#include <QDebug>

ApplesoftFile::ApplesoftFile(QByteArray data) : GenericFile(data)
{
    if (m_tokens.size() == 0)
    {
        makeTokens();
    }

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

QList<ApplesoftLine> ApplesoftFile::detokenize()
{
    QList<ApplesoftLine> retval;

/*
    qDebug() << "Length: " << m_length;
    for (int idx = 0; idx < m_length; idx += 8)
    {
        qDebug()
                 << QString("%1 (%2): %3 %4 %5 %6 %7 %8 %9 %10")
                .arg((quint16) idx + 0x801,4,16,QChar('0')).toUpper()
                .arg((quint16) idx + 0x801,4,10,QChar('0'))
                .arg((quint8) m_data[idx],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+1],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+2],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+3],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+4],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+5],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+6],2,16,QChar('0')).toUpper()
                .arg((quint8) m_data[idx+7],2,16,QChar('0')).toUpper();

    }
*/
    int idx = 0;
    quint8 val = 0;
    while (idx < m_data.length()) {
        ApplesoftLine line;
        line.next_address = (quint8) m_data[idx] + (((quint8) m_data[idx+1]) *256);
        line.tokens.append(m_data[idx]);
        line.tokens.append(m_data[idx+1]);
        idx++; idx++;
        line.linenum = (quint8) m_data[idx] + (((quint8) m_data[idx+1])*256);
        line.tokens.append(m_data[idx]);
        line.tokens.append(m_data[idx+1]);
        idx++; idx++;
        if (line.next_address == 0x00) { break; }
        do {
            val = m_data[idx++];
            line.tokens.append(val);
            if (val >= 0x80) {
                line.detokenized_line.append(" " + m_tokens[val]);
            } else {
                if (val >= 0x20) {
                    line.detokenized_line.append(val);
                } else if (val == 0x7F) {
                    QChar ch = QChar(0x2401);
                    line.detokenized_line.append(ch);
                }else if (val > 0x00) {
                    QChar ch = QChar(0x00 + val);
                    line.detokenized_line.append(ch);
                }
            }
        } while (val != 0x00);
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

void ApplesoftFile::makeTokens()
{
    m_tokens[0x80] = "END ";     m_tokens[0x81] = "FOR ";     m_tokens[0x82] = "NEXT ";
    m_tokens[0x83] = "DATA ";    m_tokens[0x84] = "INPUT ";   m_tokens[0x85] = " DEL ";
    m_tokens[0x86] = "DIM ";     m_tokens[0x87] = "READ ";    m_tokens[0x88] = "GR ";
    m_tokens[0x89] = "TEXT ";    m_tokens[0x8A] = "PR# ";     m_tokens[0x8B] = "IN# ";
    m_tokens[0x8C] = "CALL ";    m_tokens[0x8D] = "PLOT ";    m_tokens[0x8E] = "HLIN ";
    m_tokens[0x8F] = "VLIN ";    m_tokens[0x90] = "HGR2 ";    m_tokens[0x91] = "HGR ";
    m_tokens[0x92] = "HCOLOR= "; m_tokens[0x93] = "HPLOT ";   m_tokens[0x94] = "DRAW ";
    m_tokens[0x95] = "XDRAW ";   m_tokens[0x96] = "HTAB ";    m_tokens[0x97] = "HOME ";
    m_tokens[0x98] = "ROT= ";    m_tokens[0x99] = "SCALE= ";  m_tokens[0x9A] = "SHLOAD ";
    m_tokens[0x9B] = "TRACE ";   m_tokens[0x9C] = "NOTRACE "; m_tokens[0x9D] = "NORMAL ";
    m_tokens[0x9E] = "INVERSE "; m_tokens[0x9F] = "FLASH ";   m_tokens[0xA0] = "COLOR= ";
    m_tokens[0xA1] = "POP ";     m_tokens[0xA2] = "VTAB ";    m_tokens[0xA3] = "HIMEM: ";
    m_tokens[0xA4] = "LOMEM: ";  m_tokens[0xA5] = "ONERR ";   m_tokens[0xA6] = "RESUME ";
    m_tokens[0xA7] = "RECALL ";  m_tokens[0xA8] = "STORE ";   m_tokens[0xA9] = "SPEED= ";
    m_tokens[0xAA] = "LET ";     m_tokens[0xAB] = "GOTO ";    m_tokens[0xAC] = "RUN ";
    m_tokens[0xAD] = "IF ";      m_tokens[0xAE] = "RESTORE "; m_tokens[0xAF] = "& ";
    m_tokens[0xB0] = "GOSUB ";   m_tokens[0xB1] = "RETURN ";  m_tokens[0xB2] = "REM ";
    m_tokens[0xB3] = "STOP ";    m_tokens[0xB4] = "ON ";      m_tokens[0xB5] = "WAIT ";
    m_tokens[0xB6] = "LOAD ";    m_tokens[0xB7] = "SAVE ";    m_tokens[0xB8] = "DEF ";
    m_tokens[0xB9] = "POKE ";    m_tokens[0xBA] = "PRINT ";   m_tokens[0xBB] = "CONT ";
    m_tokens[0xBC] = "LIST ";    m_tokens[0xBD] = "CLEAR ";   m_tokens[0xBE] = "GET ";
    m_tokens[0xBF] = "NEW ";     m_tokens[0xC0] = "TAB ";     m_tokens[0xC1] = " TO ";
    m_tokens[0xC2] = "FN ";      m_tokens[0xC3] = "SPC( ";    m_tokens[0xC4] = "THEN ";
    m_tokens[0xC5] = " AT ";     m_tokens[0xC6] = "NOT ";     m_tokens[0xC7] = " STEP ";
    m_tokens[0xC8] = "+ ";       m_tokens[0xC9] = "- ";       m_tokens[0xCA] = "* ";
    m_tokens[0xCB] = "/ ";       m_tokens[0xCC] = "^ ";       m_tokens[0xCD] = " AND";
    m_tokens[0xCE] = " OR";      m_tokens[0xCF] = "> ";       m_tokens[0xD0] = "= ";
    m_tokens[0xD1] = "< ";       m_tokens[0xD2] = "SGN ";     m_tokens[0xD3] = "INT ";
    m_tokens[0xD4] = "ABS ";     m_tokens[0xD5] = "USR ";     m_tokens[0xD6] = "FRE ";
    m_tokens[0xD7] = "SCRN ( ";  m_tokens[0xD8] = "PDL ";     m_tokens[0xD9] = "POS ";
    m_tokens[0xDA] = "SQR ";     m_tokens[0xDB] = "RND ";     m_tokens[0xDC] = "LOG ";
    m_tokens[0xDD] = "EXP ";     m_tokens[0xDE] = "COS ";     m_tokens[0xDF] = "SIN ";
    m_tokens[0xE0] = "TAN ";     m_tokens[0xE1] = "ATN ";     m_tokens[0xE2] = "PEEK ";
    m_tokens[0xE3] = "LEN ";     m_tokens[0xE4] = "STR$ ";    m_tokens[0xE5] = "VAL ";
    m_tokens[0xE6] = "ASC ";     m_tokens[0xE7] = "CHR$ ";    m_tokens[0xE8] = "LEFT$ ";
    m_tokens[0xE9] = "RIGHT$ ";  m_tokens[0xEA] = "MID$ ";    m_tokens[0xEB] = "{Token 0xEB} ";
    m_tokens[0xEC] = "{Token 0xEC} ";
    m_tokens[0xED] = "{Token 0xED} ";
    m_tokens[0xEE] = "{Token 0xEE} ";
    m_tokens[0xEF] = "{Token 0xEF} ";
    m_tokens[0xF0] = "{Token 0xF0} ";
    m_tokens[0xF1] = "{Token 0xF1} ";
    m_tokens[0xF2] = "{Token 0xF2} ";
    m_tokens[0xF3] = "{Token 0xF3} ";
    m_tokens[0xF4] = "{Token 0xF4} ";
    m_tokens[0xF5] = "{Token 0xF5} ";
    m_tokens[0xF6] = "{Token 0xF6} ";
    m_tokens[0xF7] = "{Token 0xF7} ";
    m_tokens[0xF8] = "{Token 0xF8} ";
    m_tokens[0xF9] = "{Token 0xF9} ";
    m_tokens[0xFA] = "{Token 0xFA} ";
    m_tokens[0xFB] = "{Token 0xFB} ";
    m_tokens[0xFC] = "{Token 0xFC} ";
    m_tokens[0xFD] = "{Token 0xFD} ";
    m_tokens[0xFE] = "{Token 0xFE} ";
    m_tokens[0xFF] = "{Token 0xFF} ";
}
