#include "applesoftfile.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QList>

ApplesoftFile::ApplesoftFile(QByteArray data) : GenericFile(data)
{
    m_retokenizer = Q_NULLPTR;
    m_data_end = data.length();

    if (!data.isEmpty())
    {
        setData(data);
    }
    setAddress(0x801);
}

void ApplesoftFile::setData(QByteArray data)
{
    if (!m_retokenizer)
    {
        m_retokenizer = new ApplesoftRetokenizer();
    }

    GenericFile::setData(data);

    quint8 addlo = m_data.at(0);
    quint8 addhi = m_data.at(1);
    m_length = addlo + (addhi * 256);
    m_data.remove(0,2);

    m_retokenizer->setData(m_data);
    m_retokenizer->parse();
    m_data_end = m_retokenizer->getEndOfDataOffset();
    m_lines = m_retokenizer->getRetokenizedLines();
}

QByteArray ApplesoftFile::rawData() {
    QByteArray retval;
    retval.append(m_length % 255);
    retval.append(m_length / 255);
    retval.append(m_data);
    return retval;
}

QStringList ApplesoftFile::extraDataHexValues() {
    QStringList retval;

    QString debugline = "";
    int count = 0;
    foreach (quint8 val, extraData()) {
        debugline.append(QString("%1").arg(val,2,16,QChar('0')).toUpper());
        count++;
        if (count == 16) {
            retval.append(debugline);
            debugline = "";
            count = 0;
        } else {
            debugline.append(" ");
        }
    }
    if (!debugline.simplified().isEmpty()) {
        retval.append(debugline);
    }
    return retval;
}

QByteArray ApplesoftFile::extraData()
{
    return m_data.mid(m_data_end);
}


