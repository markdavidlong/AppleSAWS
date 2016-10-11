#include "applesoftfile.h"
#include <QDebug>

ApplesoftFile::ApplesoftFile(QByteArray data) : GenericFile(data)
{
    if (!data.isEmpty())
    {
        setData(data);
    }
    setAddress(0x801);
}

void ApplesoftFile::setData(QByteArray data)
{
    GenericFile::setData(data);

    quint8 addlo = m_data.at(0);
    quint8 addhi = m_data.at(1);
    m_length = addlo + (addhi * 256);
    m_data.remove(0,2);
  //  m_data = m_data.left(m_length);
    parse();
}

QByteArray ApplesoftFile::rawData() {
    QByteArray retval;
    retval.append(m_length % 255);
    retval.append(m_length / 255);
    retval.append(m_data);
    return retval;
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

        Retokenizer ret;
        ret.retokenize(line);

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
    Q_UNUSED(line);


    QVector<ApplesoftToken> replacements;
    QVector<ApplesoftToken> tmptokens = line.tokens;
    ApplesoftToken token;

    QByteArray buffer;

    bool inRem = false;

    // Handle REMs

    while (!tmptokens.isEmpty())
    {
        token = tmptokens.takeFirst();

        if (!inRem) {
            replacements.append(token);
            if (token.getByteValue() == ApplesoftToken::ASRem)
            {
                inRem = true;
            }
        }
        else
        {
            buffer.append(token.getByteValue());
        }
    }
    if (inRem) {
        ApplesoftToken remstrtoken(ApplesoftToken::RemStringTokenVal, buffer);
        replacements.append(remstrtoken);
        buffer.clear();
        inRem = false;
    }

    line.tokens = replacements;
    replacements.clear();

    // Handle DATAs

    // Handle Strings

    tmptokens = line.tokens;
    bool inString = false;

    while (!tmptokens.isEmpty())
    {
        token = tmptokens.takeFirst();
        if (token.getTokenId() >= 0x80)
        {
            replacements.append(token);
            continue;
        }

        if (token.getByteValue() == '"')
        {
            if (!inString)
            {
                inString = true;
                buffer.append(token.getByteValue());
                continue;
            }
            else
            {
                buffer.append(token.getByteValue());
                ApplesoftToken strtoken(ApplesoftToken::StringTokenVal, buffer);
                replacements.append(strtoken);
                buffer.clear();
                inString = false;
                continue;
            }
        }

        if (inString)
        {
            buffer.append(token.getByteValue());
            continue;
        }

        replacements.append(token);
    }

    line.tokens = replacements;

}

QByteArray Retokenizer::retokenizePart(QByteArray part) {
    QByteArray retval;

    Q_UNUSED(part);

    return retval;
}
