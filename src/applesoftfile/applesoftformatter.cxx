#include "applesoftformatter.h"
#include "util.h"

ApplesoftFormatter::ApplesoftFormatter(QObject *parent) :
    QObject(parent)
{
    m_file = 0;
}

void ApplesoftFormatter::setFile(ApplesoftFile *file)
{
    m_file = file;
    emit newFile(file);
}

QString ApplesoftFormatter::formatText()
{
    if (!m_file) {
        return ("No File");
    }

    QString retval;

    foreach (ApplesoftLine line, m_file->getLines()) {
        QString linestring = QString("%1  ").arg(line.linenum,5,10,QChar(' '));

        retval.append(linestring);
        foreach (ApplesoftToken token, line.tokens)
        {
            QString tokenstr = token.getRawPrintableString();

            if (m_format_options.testFlag(BreakAfterReturn)) {
                if (token.getTokenId() == ApplesoftToken::ASReturn)
                {
                    tokenstr += "\n";
                }
            }

#define noDEBUGTOKENS
#ifdef DEBUGTOKENS
            if (token.getTokenId() >= 0x80)
            {
                tokenstr = QString("{%1 (%2)}").arg(tokenstr).arg(uint16ToHex(token.getTokenId()));
               // tokenstr = " __ ";
            }

#endif

   //         if (m_format_options.testFlag(ShowCtrlChars))
            {
                tokenstr.replace(QChar(0x7f),QChar(0x2401));

                for (int idx = 1; idx <= 0x1f; idx++) {
                    if (idx == '\n') continue;
                    tokenstr.replace(QChar(idx),QChar(idx+0x2400));
      //              tokenstr.replace(QChar(idx), QString("<%1>").arg(uint8ToHex(idx)));
                }
            }

            retval.append(tokenstr);
        }
        retval.append("\n");

    }

    return retval;
}
