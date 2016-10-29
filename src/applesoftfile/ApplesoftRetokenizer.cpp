#include "ApplesoftRetokenizer.h"
#include "applesofttoken.h"

#include <QDebug>
#include <QRegularExpression>

ApplesoftRetokenizer::ApplesoftRetokenizer()
{
    m_isParsed = false;
}

void ApplesoftRetokenizer::setData(QByteArray data)
{
    m_data = data;
    m_data_end = data.length();
    m_isParsed = false;
}

void ApplesoftRetokenizer::parse(quint16 start_address)
{
    if (m_isParsed)
    {
        qWarning("File is already parsed.  Not reparsing.");
        return;
    }

    //TODO:  This could be changed to search for hidden space between applesoft lines
    int idx = 0;
    quint8 val = 0;
    m_retokenized_lines.clear();

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


        retokenizeLine(line);

        current_address = line.next_address;

        m_retokenized_lines.append(line);
    }

    m_data_end = idx;

    if (idx < m_data.length()) {
        qDebug() << QString("%1 byte(s) unaccounted for.").arg(m_data.length() - idx);
    }

    retokenizeLinesForFormatting();

    m_isParsed = true;
}

void ApplesoftRetokenizer::retokenizeLinesForFormatting()
{
    QVector<ApplesoftLine> retLines;

    foreach(ApplesoftLine line, m_retokenized_lines)
    {
        int indentlevel = 1;
     //   quint16 linenum = line.linenum;

        bool firstToken = true;
        ApplesoftToken previousToken;
        QMutableVectorIterator<ApplesoftToken> tokenIt(line.tokens);
        while (tokenIt.hasNext())
        {
            ApplesoftToken token = tokenIt.next();
            bool isFlowTarget = false;

            QString tokenstr = token.getRawPrintableString();
            if (firstToken)
            {
                if (!tokenstr.startsWith(" "))
                {
                    ApplesoftToken tmptoken(ApplesoftToken::OptFmtLeadingSpaceTokenValue);
                    tokenIt.remove();
                    tokenIt.insert(tmptoken);
                    tokenIt.insert(token);
                }
                firstToken = false;
            }

            quint16 preTokenId = previousToken.getTokenId();
            if (preTokenId == ApplesoftToken::ASGoto ||
                preTokenId == ApplesoftToken::ASGosub ||
                preTokenId == ApplesoftToken::ASThen)
            {
                isFlowTarget = false;
                if (preTokenId == ApplesoftToken::ASGoto || preTokenId == ApplesoftToken::ASGosub)
                {
                    isFlowTarget = true;
                }
                else if (preTokenId == ApplesoftToken::ASThen
                         && token.getTokenId() == ApplesoftToken::IntegerTokenVal)
                {
                    isFlowTarget = true;
                }
                if (isFlowTarget)
                {
                    QPair<quint16,quint16> pair;
                    pair.first = line.linenum;
                    pair.second = token.getWordValue();
                    m_flowTargets.append(pair);

                    ApplesoftToken tmptoken(ApplesoftToken::OptFmtFlagFlowTargetNextTokenValue);
                    tokenIt.remove();
                    tokenIt.insert(tmptoken);
                    tokenIt.insert(token);
                }
            }

            if (token.getTokenId() == ApplesoftToken::ASReturn)
            {
                ApplesoftToken tmptoken(ApplesoftToken::OptFmtReturnLineBreakTokenValue);
                tokenIt.insert(tmptoken);
            }

            if (token.getTokenId() == ':')
            {
                ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentLineBreakTokenValue);
                tokenIt.insert(tmptoken);
                for (int ind = 0; ind < indentlevel; ind++)
                {
                    ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentTabTokenValue);
                    tokenIt.insert(tmptoken);
                }
                if (!tokenIt.peekNext().getRawPrintableString().startsWith(" "))
                {
                    ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentSpaceTokenValue);
                    tokenIt.insert(tmptoken);
                }
            }
            if (token.getTokenId() == ApplesoftToken::ASThen)
            {
                indentlevel++;
                if (tokenIt.peekNext().getTokenId() != ApplesoftToken::IntegerTokenVal)
                {
                    ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentLineBreakTokenValue);
                    tokenIt.insert(tmptoken);
                    for (int ind = 0; ind < indentlevel; ind++)
                    {
                        ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentTabTokenValue);
                        tokenIt.insert(tmptoken);
                    }
                    if (!tokenIt.peekNext().getRawPrintableString().startsWith(" "))
                    {
                        ApplesoftToken tmptoken(ApplesoftToken::OptFmtIndentSpaceTokenValue);
                        tokenIt.insert(tmptoken);
                    }
                }
            }

            previousToken = token;
        }
        retLines.append(line);
    }

    m_retokenized_lines = retLines;
}

void ApplesoftRetokenizer::retokenizeLine(ApplesoftLine &line)
{
    qDebug() << "Retokenize line";
    QList<ApplesoftToken> tmptokens = QList<ApplesoftToken>::fromVector(line.tokens);
    tmptokens = retokenizeRems(tmptokens);
    tmptokens = retokenizeStrings(tmptokens);
    tmptokens = retokenizeDataStatements(tmptokens);
    tmptokens = retokenizeVariables(tmptokens);
    tmptokens = retokenizeNumbers(tmptokens);
    tmptokens = retokenizeNegativeNumbers(tmptokens);
    line.tokens = tmptokens.toVector();
}

QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeRems(QList<ApplesoftToken>&tmptokens)
{
    // Handle REMs
    ApplesoftToken token;
    QList<ApplesoftToken> replacements;

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

QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeStrings(QList<ApplesoftToken>&tmptokens)
{
    // Handle Strings
    QList<ApplesoftToken> replacements;

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

QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeDataStatements(QList<ApplesoftToken>&tmptokens)
{
    // Handle DATAs

    QList<ApplesoftToken> replacements;
    ApplesoftToken token;

    QList<ApplesoftToken> datatokenbuffer;
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
        QList<ApplesoftToken> dataTokens;
        dataTokens = retokenizeDataPayload(datatokenbuffer);
        replacements.append(dataTokens);
        datatokenbuffer.clear();
        inData = false;
    }
    return replacements;
}


QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeDataPayload(QList<ApplesoftToken>& tmptokens)
{
    QList<ApplesoftToken> retval;

    ApplesoftToken token;

    QString stringbuffer;

    while (!tmptokens.isEmpty())
    {
        token = tmptokens.takeFirst();
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
QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeVariables(QList<ApplesoftToken>&tmptokens)
{
    // Handle variable names
    ApplesoftToken token;

    QRegularExpression varregexp("[A-Za-z][A-Za-z0-9]*[$%]?\\(?");

    QString parsestring;
    // Parse the tokens to find assist
    for (int idx = 0; idx < tmptokens.count();idx++)
    {
        token = tmptokens.at(idx);

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

    return tmptokens;
}

QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeNumbers(QList<ApplesoftToken>&tmptokens)
{
    // Handle numbers
    ApplesoftToken token;

    QRegularExpression varregexp("[0-9]+(\\.[0-9]*)?");

    QString parsestring;
    // Parse the tokens to find assist
    for (int idx = 0; idx < tmptokens.count();idx++)
    {
        token = tmptokens.at(idx);

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

    return tmptokens;
}

QList<ApplesoftToken> ApplesoftRetokenizer::retokenizeNegativeNumbers(QList<ApplesoftToken>&tmptokens)
{
    //TODO:  Code to make determination of negative numbers vs. unary minus/math formulas.
    //  Prefixed '-' tokens for negative numbers should get merged with the integer value token.
    //  So, need to determine when we're in an expression vs when we're starting a number.
    //
    //  A = -1 should retokenize.
    //  A = - 4 - 1 should retokenize -4
    //  A = - 4 - - 1 should retokenize -4 and -1
    //  A = 3 - 1 shoud not retokenize
    //  A = A - 1 should not
    //  A = PEEK(123) - 5 should not
    //  A = 4 * - 1 should
    //  A = (1 + 2) - 4 should not
    //  A = (1 + 2) + - 4 should
    //  A = (1 + 2) - - 4 should
    //  POKE - 4, 1 should
    //  PRINT + - 4 should
    //  PRINT - 4 should
    //  PRINT + + + - - - 4 should retokenize the last -4.
    //  A = 1 - - 4 should
    //  A = 1 - - - 4 should, probably, but it's errorprone to say the least,
    //      as are any multiple arbitrary +/-'s.  Have to hope for the best here.
    //      Best bet would be to look at how AppleSoft handles these values.
    //  A = - 0 is the same as 0

    ApplesoftToken token;

    QMutableListIterator<ApplesoftToken>it(tmptokens);

    bool lastWasInt = false;

    while (it.hasNext())
    {
        token = it.next();
        if (token.getTokenId() == ApplesoftToken::IntegerTokenVal) lastWasInt = true;
        else if (token.getTokenId() == ApplesoftToken::FloatTokenVal) lastWasInt = true;
        else if (token.getTokenId() == ApplesoftToken::IntVarTokenVal) lastWasInt = true;
        else if (token.getTokenId() == ApplesoftToken::FloatVarTokenVal) lastWasInt = true;
        else if (token.getTokenId() == ')') lastWasInt = true;
        else
            if (token.getTokenId() == ApplesoftToken::ASMINUS)
            {
                if (!lastWasInt && it.hasNext() && it.peekNext().getTokenId() == ApplesoftToken::IntegerTokenVal)
                {
                    it.remove();
                    token = it.next();
                    it.remove();
                    int val = token.getUnsignedIntegerValue() * -1;
                    token.setValue(val);
                    it.insert(token);
                    lastWasInt = true;
                }
                else
                {
                    lastWasInt = false;
                }
            }
            else
            {
                lastWasInt = false;
            }
    }

    return tmptokens;
}

