#ifndef APPLESTRING_H
#define APPLESTRING_H

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

#include <QVector>
#include <QChar>
#include <QString>
#include <QByteArray>

#include "util.h"

class AppleChar {
public:
    AppleChar() { m_val = 0; }
    AppleChar(quint8 ch) { m_val = ch; }

    QChar printable() const { return printable(m_val); }

    TextAttribute getAttribute() const { return getAttribute(m_val); }
    TextSet getTextSet() const { return getTextSet(m_val); }

    static TextAttribute getAttribute(quint8 val) ;
    static TextSet getTextSet(quint8 val) ;
    static QChar printable(quint8 val) ;
    static QString getHtmlString(quint8 val);
    static QChar getAppleFontChar(quint8 val, bool showCtrlChars = false);

private:
    quint8 m_val;
};

class AppleString : public QByteArray {
public:
    void setData(const QByteArray &data) { insert(0,data); }
    QString printable() const;
    QString appleFontPrintable() const;
    QVector<TextAttribute> attributes() const;

};

#endif // APPLESTRING_H

