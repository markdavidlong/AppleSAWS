#ifndef APPLESTRING_H
#define APPLESTRING_H

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


private:
    quint8 m_val;
};

class AppleString : public QByteArray {
public:
    void setData(const QByteArray &data) { insert(0,data); }
    QString printable() const;
    QVector<TextAttribute> attributes() const;

};

#endif // APPLESTRING_H

