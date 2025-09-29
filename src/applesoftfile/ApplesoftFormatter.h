#pragma once

#include <QObject>
#include <QFlags>

#include "ApplesoftFile.h"
#include "ApplesoftToken.h"

// Forward declarations
class QTextDocument;

class ApplesoftFormatter : public QObject
{
    Q_OBJECT

public:
    enum class FormatOption
    {
        NoOptions = 0x00,

        SyntaxHighlighting = 0x01,
        ShowCtrlChars = 0x02,
        BreakAfterReturn = 0x04,
        ReindentCode = 0x08,
        ShowIntsAsHex = 0x10,

        AllFlags = ~0
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
    FormatOptions m_format_options{FormatOption::NoOptions};
    ApplesoftFile *m_file{nullptr};
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ApplesoftFormatter::FormatOptions)
