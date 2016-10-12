#include "applesoftfile.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QList>

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
    line.tokens = retokenizeRems(line.tokens);
    line.tokens = retokenizeStrings(line.tokens);
    line.tokens = retokenizeDataStatements(line.tokens);
    line.tokens = retokenizeVariables(line.tokens);
    line.tokens = retokenizeNumbers(line.tokens);


}

QVector<ApplesoftToken> Retokenizer::retokenizeRems(QVector<ApplesoftToken>&datatokens)
{
    // Handle REMs
    ApplesoftToken token;
    QVector<ApplesoftToken> replacements;
    QVector<ApplesoftToken> tmptokens = datatokens;
    QByteArray buffer;

    bool inRem = false;

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

    return replacements;
}

QVector<ApplesoftToken> Retokenizer::retokenizeStrings(QVector<ApplesoftToken>&datatokens)
{
    // Handle Strings
    QVector<ApplesoftToken> replacements;
    QVector<ApplesoftToken> tmptokens = datatokens;
    QString buffer;
    ApplesoftToken token;

    bool inString = false;

    while (!tmptokens.isEmpty())
    {
        token = tmptokens.takeFirst();
        if (token.getTokenId() >= 0x80)
        {
            replacements.append(token);
         //   continue;
        } else

        if (token.getWordValue() == '"')
        {
            if (!inString)
            {
                inString = true;
                buffer.append(token.getWordValue());
              //  continue;
            }
            else
            {
                buffer.append(token.getWordValue());
                ApplesoftToken strtoken(ApplesoftToken::StringTokenVal, buffer);
                replacements.append(strtoken);
                buffer.clear();
                inString = false;
//                continue;
            }
        } else

        if (inString)
        {
            buffer.append(token.getWordValue());
            // continue;
        } else

        replacements.append(token);
    }

    return replacements;
}

QVector<ApplesoftToken> Retokenizer::retokenizeDataStatements(QVector<ApplesoftToken>&datatokens)
{
    // Handle DATAs
    QVector<ApplesoftToken> tmptokens = datatokens;
    QVector<ApplesoftToken> replacements;
    ApplesoftToken token;

    QVector<ApplesoftToken> datatokenbuffer;
    bool inData = false;
    while (!tmptokens.isEmpty())
    {
        token = tmptokens.takeFirst();

        if (!inData) {
            replacements.append(token);
            if (token.getTokenId() == ApplesoftToken::ASData)
            {
                inData = true;
            }
        }
        else
        {
            datatokenbuffer.append(token);
        }
    }
    if (inData) {
        QVector<ApplesoftToken> dataTokens;
        dataTokens = processDataPayload(datatokenbuffer);
        replacements.append(dataTokens);
        datatokenbuffer.clear();
        inData = false;
    }
    return replacements;
}


QVector<ApplesoftToken> Retokenizer::processDataPayload(QVector<ApplesoftToken>& datatokens)
{
    QVector<ApplesoftToken> retval;

    ApplesoftToken token;

    QString stringbuffer;

    while (!datatokens.isEmpty())
    {
        token = datatokens.takeFirst();
        if (token.getTokenId() == ApplesoftToken::StringTokenVal)
        {
            ApplesoftToken newToken(ApplesoftToken::DataStringTokenVal, token.getStringValue());
            retval.append(newToken);
            continue;
        }
        if (token.getWordValue() == ',')
        {
            if (!stringbuffer.isEmpty())
            {
                ApplesoftToken datastrtoken(ApplesoftToken::DataStringTokenVal, stringbuffer);
                retval.append(datastrtoken);
                stringbuffer.clear();
            }
            retval.append(token);
            continue;

        }
        stringbuffer.append(token.getWordValue());
    }
    if (!stringbuffer.isEmpty())
    {
        ApplesoftToken datastrtoken(ApplesoftToken::DataStringTokenVal, stringbuffer);
        retval.append(datastrtoken);
        stringbuffer.clear();
    }
    return retval;
}
QVector<ApplesoftToken> Retokenizer::retokenizeVariables(QVector<ApplesoftToken>&datatokens)
{
    // Handle variable names
    QList<ApplesoftToken> tmptokens = QList<ApplesoftToken>::fromVector(datatokens);
    ApplesoftToken token;

    QRegularExpression varregexp("[A-Za-z][A-Za-z0-9]*[$%]?\\(?");

    QString parsestring;
    // Parse the tokens to find assist
    for (int idx = 0; idx < tmptokens.count();idx++)
    {
        token = datatokens.at(idx);

        if (token.getTokenId() < 0x0080 && token.getTokenId() > 0x0000)
        {
            parsestring.append(QChar(token.getWordValue()));
        }
        else
        {
            parsestring.append("_");
        }
    }
    QList<QRegularExpressionMatch> matchstack;
    QRegularExpressionMatchIterator matches = varregexp.globalMatch(parsestring);
 //   qDebug() << parsestring;
    while (matches.hasNext()) {
        QRegularExpressionMatch rematch = matches.next();
        matchstack.push_front(rematch);

//        qDebug() << "Capture " << " = " << rematch.capturedTexts() << "From: " << rematch.capturedStart()
//                 << "To: " << rematch.capturedEnd()-1 << "("<<rematch.capturedLength()<<")";
    }

    foreach(QRegularExpressionMatch rematch, matchstack)
    {
        QString text = rematch.captured(0);
        int start = rematch.capturedStart();
        int length = rematch.capturedLength();

        quint16 tokentype = ApplesoftToken::FloatVarTokenVal;
        if (text.contains("$")) {
            tokentype = ApplesoftToken::StringVarTokenVal;
        } else if (text.contains("%")) {
            tokentype = ApplesoftToken::IntVarTokenVal;
        }

        if (text.contains("(")) {
            if (tokentype == ApplesoftToken::FloatVarTokenVal) {
                tokentype = ApplesoftToken::FloatAryVarTokenVal;
            } else if (tokentype == ApplesoftToken::StringVarTokenVal) {
                tokentype = ApplesoftToken::StringAryVarTokenVal;
            } else {
                tokentype = ApplesoftToken::IntAryVarTokenVal;
            }
        }

        ApplesoftToken vartoken = ApplesoftToken(tokentype,text);

        for (int idx = 0; idx < length; idx++)
        {
            tmptokens.removeAt(start);
        }
        tmptokens.insert(start,vartoken);

    }

    datatokens = tmptokens.toVector();
    return datatokens;
}

QVector<ApplesoftToken> Retokenizer::retokenizeNumbers(QVector<ApplesoftToken>&datatokens)
{
    // Handle numbers
    QList<ApplesoftToken> tmptokens = QList<ApplesoftToken>::fromVector(datatokens);
    ApplesoftToken token;

    QRegularExpression varregexp("[0-9]+(\\.[0-9]*)?");

    QString parsestring;
    // Parse the tokens to find assist
    for (int idx = 0; idx < tmptokens.count();idx++)
    {
        token = datatokens.at(idx);

        if (token.getTokenId() < 0x0080 && token.getTokenId() > 0x0000)
        {
            parsestring.append(QChar(token.getWordValue()));
        }
        else
        {
            parsestring.append("_");
        }
    }
    QList<QRegularExpressionMatch> matchstack;
    QRegularExpressionMatchIterator matches = varregexp.globalMatch(parsestring);
//    qDebug() << parsestring;
    while (matches.hasNext()) {
        QRegularExpressionMatch rematch = matches.next();
        matchstack.push_front(rematch);

//        qDebug() << "Capture " << " = " << rematch.capturedTexts() << "From: " << rematch.capturedStart()
//                 << "To: " << rematch.capturedEnd()-1 << "("<<rematch.capturedLength()<<")";
    }

    foreach(QRegularExpressionMatch rematch, matchstack)
    {
        QString text = rematch.captured(0);
        int start = rematch.capturedStart();
        int length = rematch.capturedLength();

        quint16 tokentype = ApplesoftToken::IntegerTokenVal;
        if (text.contains(".")) {
            tokentype = ApplesoftToken::FloatTokenVal;
        }

        ApplesoftToken vartoken = ApplesoftToken(tokentype,text);

        for (int idx = 0; idx < length; idx++)
        {
            tmptokens.removeAt(start);
        }
        tmptokens.insert(start,vartoken);

    }

    datatokens = tmptokens.toVector();
    return datatokens;
}
