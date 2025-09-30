#include "ApplesoftFile.h"
#include "ApplesoftLine.h"
#include "ApplesoftToken.h"
#include "ApplesoftRetokenizer.h"
#include "Util.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QList>
#include <QPair>

ApplesoftFile::ApplesoftFile(const QByteArray& data) : GenericFile(data)
{
    m_data_end = data.length();

    if (!data.isEmpty())
    {
        setData(data);
    }
    setAddress(0x801);
}

void ApplesoftFile::setData(const QByteArray& data)
{
    if (!m_retokenizer)
    {
        m_retokenizer = std::make_unique<ApplesoftRetokenizer>();    }

    GenericFile::setData(data);

    quint8 addlo = m_data.at(0);
    quint8 addhi = m_data.at(1);
    m_length = makeWord(addlo,addhi);
    m_data.remove(0,2);

    m_retokenizer->setData(m_data);
    m_retokenizer->parse();
    m_data_end = m_retokenizer->getEndOfDataOffset();
    m_lines = m_retokenizer->getRetokenizedLines();

    m_flowTargets = m_retokenizer->getFlowTargets();
}

QByteArray ApplesoftFile::rawData() const noexcept {
    QByteArray retval;
    retval.append(m_length % 255);
    retval.append(m_length / 255);
    retval.append(m_data);
    return retval;
}

QStringList ApplesoftFile::extraDataHexValues() const {
    QStringList retval;

    QString debugline = "";
    int count = 0;
    for (const auto& val : extraData()) {
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

QByteArray ApplesoftFile::extraData() const
{
    return m_data.mid(m_data_end);
}


