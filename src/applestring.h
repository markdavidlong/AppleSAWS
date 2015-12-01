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

    QChar printable() const;

    TextAttribute getAttribute() const;
    TextSet getTextSet() const;

private:
    quint8 m_val;
};

class AppleString : public QByteArray {
public:
    QString printable() const;
    QVector<TextAttribute> attributes() const;

};

#endif // APPLESTRING_H
