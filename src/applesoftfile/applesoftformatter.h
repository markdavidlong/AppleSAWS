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
        NoOptions =        0x00,

        FormatHTML =       0x01,
        ShowCtrlChars =    0x02,
        BreakAfterReturn = 0x04,
        PrettyFlags =      ShowCtrlChars | BreakAfterReturn | FormatHTML,

        ReindentCode =     0x08,
        ShowIntsAsHex =    0x10,

        AllFlags = 0xffffffff
    };
    Q_DECLARE_FLAGS(FormatOptions, FormatOption)

public:
    explicit ApplesoftFormatter(QObject *parent = 0);
    void setFlags(FormatOptions options) { m_format_options = options; }
    void setFile(ApplesoftFile *file);
    FormatOptions flags() { return m_format_options; }

    void formatText();
    QString getFormattedText() const { return m_formattedText; }

    QList<QPair<quint16, quint16> > flowTargets() const { return m_flowTargets; }

    void newFormatText();
    void formatDocument(QTextDocument *doc);
signals:
    void newFile(ApplesoftFile *file);

public slots:

private:
    FormatOptions m_format_options;
    QList<QPair<quint16, quint16> > m_flowTargets;

    ApplesoftFile *m_file;

    QString m_formattedText;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(ApplesoftFormatter::FormatOptions)

#endif // APPLESOFTFORMATTER_H
