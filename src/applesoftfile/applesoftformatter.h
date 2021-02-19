#ifndef APPLESOFTFORMATTER_H
#define APPLESOFTFORMATTER_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/



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
