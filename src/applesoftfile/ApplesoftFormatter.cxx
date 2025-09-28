#include "ApplesoftFormatter.h"
#include "util.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QChar>

#define HEXPREFIX "0x"

ApplesoftFormatter::ApplesoftFormatter(QObject *parent) :
    QObject(parent)
{
    m_file = Q_NULLPTR;
}

void ApplesoftFormatter::setFile(ApplesoftFile *file)
{
    m_file = file;
    emit newFile(file);
}

void ApplesoftFormatter::formatDocument(QTextDocument *doc)
{
    if (!doc) return;

    doc->clear();
    QTextCursor cursor(doc);
    cursor.beginEditBlock();

    bool synhl = (m_format_options.testFlag(SyntaxHighlighting));

    foreach (ApplesoftLine line, m_file->getLines())
    {
        QString linestring = QString("%1 ").arg(line.linenum,5,10,QChar(' '));

        if (synhl)
        {
            cursor.insertText(linestring,
                              ApplesoftToken::textFormat(ApplesoftToken::LineNumberTokenVal));
        }
        else
        {
            cursor.insertText(linestring,ApplesoftToken::defaultTextFormat());
        }

        QVectorIterator<ApplesoftToken>tokenIt(line.tokens);
        bool isBranchTarget = false;
        while (tokenIt.hasNext())
        {
            ApplesoftToken token = tokenIt.next();
            if (token.isOptFmtToken())
            {
                switch (token.getTokenId())
                {

                case ApplesoftToken::OptFmtFlagFlowTargetNextTokenValue:
                {
                    if (m_format_options.testFlag(ShowIntsAsHex))
                        isBranchTarget = true;
                    break;
                }

                case ApplesoftToken::OptFmtIndentLineBreakTokenValue:
                {
                    if (m_format_options.testFlag(ReindentCode))
                        cursor.insertBlock();
                    break;
                }
                case ApplesoftToken::OptFmtIndentSpaceTokenValue:
                {
                    if (m_format_options.testFlag(ReindentCode))
                        cursor.insertText(" ",ApplesoftToken::defaultTextFormat());
                    break;
                }
                case ApplesoftToken::OptFmtIndentTabTokenValue:
                {
                    if (m_format_options.testFlag(ReindentCode))
                        cursor.insertText("      ",ApplesoftToken::defaultTextFormat());
                    break;
                }
                case ApplesoftToken::OptFmtLeadingSpaceTokenValue:
                {
                    cursor.insertText(" ",ApplesoftToken::defaultTextFormat());
                    break;
                }
                case ApplesoftToken::OptFmtReturnLineBreakTokenValue:
                {
                    if (m_format_options.testFlag(BreakAfterReturn))
                        cursor.insertBlock();
                    break;
                }
                default:
                {
                    break;
                }
                }
            }  // isOptFmt
            else
            {
                QString tokenstr = token.getRawPrintableString();


                QTextCharFormat fmt = ApplesoftToken::defaultTextFormat();
                if (synhl) fmt = token.textFormat();


                if (token.getTokenId() == ApplesoftToken::IntegerTokenVal)
                {
                    if (m_format_options.testFlag(ShowIntsAsHex) && !isBranchTarget)
                    {
                        quint32 ui32val = token.getUnsignedIntegerValue();
                        qint32  i32val = token.getIntegerValue();
                        if ((i32val < 128 && i32val >= -128) || ui32val < 256)
                        {
                            quint8 ui8 = ui32val;
                            tokenstr = HEXPREFIX+uint8ToHex(ui8);
                        }
                        else if ((i32val < 32768 && i32val >= -32768) || ui32val < 65536)
                        {
                            quint16 ui16 = ui32val;
                            tokenstr = HEXPREFIX+uint16ToHex(ui16);
                        }
                        else
                        {
                            tokenstr = HEXPREFIX+uint32ToHex(ui32val);
                        }
                    } // isShowIntsAsHex
                    isBranchTarget = false;
                }

                //                if (m_format_options.testFlag(ShowCtrlChars))
                //                {
                //                    tokenstr.replace(QChar(0x7f),QChar(0x2401));

                //                    for (int idx = 1; idx <= 0x1f; idx++) {
                //                        if (idx == '\n') continue;
                //                        tokenstr.replace(QChar(idx),QChar(idx+0x2400));
                //                    }
                //                }  // if ShowCtrlChars



                QTextCharFormat invFormat = ApplesoftToken::defaultInverseTextFormat();
                if (m_format_options.testFlag(SyntaxHighlighting))
                    invFormat = ApplesoftToken::textFormat(
                                ApplesoftToken::ControlCharTokenVal);


                foreach (QChar ch, tokenstr)
                {
                    if (ch == QChar(0x7f))
                    {
                        cursor.insertText(QChar(0x00d7),invFormat);
                    }
                    else if (ch < QChar(' '))
                    {
                        if (m_format_options.testFlag(ShowCtrlChars))
                        {
			    QChar newch = QChar(static_cast<ushort>(ch.unicode() + 0x40));
                            cursor.insertText(newch,invFormat);
                        }
                    }
                    else cursor.insertText(ch,fmt);
                }

            }

        } // while tokenIt.hasNext()

        //formattedText.append("\n");
        cursor.insertBlock();

    } // foreach line
    cursor.endEditBlock();
}
