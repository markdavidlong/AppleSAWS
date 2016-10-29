#ifndef APPLESOFTFORMATTER_H
#define APPLESOFTFORMATTER_H

#include <QObject>
#include <QFlags>
#include <QTextDocument>
#include <QSharedPointer>

#include "applesoftfile.h"

class ApplesoftFormatter : public QObject
{
    Q_OBJECT

public:
    enum FormatOption {
        NoOptions          = 0x00,

        SyntaxHighlighting = 0x01,
        ShowCtrlChars      = 0x02,
        BreakAfterReturn   = 0x04,
        ReindentCode       = 0x08,
        ShowIntsAsHex      = 0x10,

        AllFlags = 0xffffffff
    };
    Q_DECLARE_FLAGS(FormatOptions, FormatOption)

public:
    explicit ApplesoftFormatter(QObject *parent = 0);
    void setFlags(FormatOptions options) { m_format_options = options; }
    void setFile(ApplesoftFile *file);

    FormatOptions flags() { return m_format_options; }

    void formatDocument(QTextDocument *doc);

signals:
    void newFile(ApplesoftFile *file);

public slots:

private:
    FormatOptions m_format_options;

    ApplesoftFile *m_file;


};

Q_DECLARE_OPERATORS_FOR_FLAGS(ApplesoftFormatter::FormatOptions)

#endif // APPLESOFTFORMATTER_H
