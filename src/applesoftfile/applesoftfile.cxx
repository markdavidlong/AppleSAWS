#include "applesoftfile.h"
#include "util.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QList>

ApplesoftFile::ApplesoftFile(QByteArray data) : GenericFile(data)
{
    m_retokenizer = Q_NULLPTR;
    m_data_end = data.length();

    setAddress(0x801);
}

void ApplesoftFile::processData()
{
    if (!m_retokenizer)
    {
        m_retokenizer = new ApplesoftRetokenizer();
    }

    m_length = dataWordAt(0);
    setIgnoreOffset(2);

    m_retokenizer->setData(data());
    m_retokenizer->parse();
    m_data_end = m_retokenizer->getEndOfDataOffset();
    m_lines = m_retokenizer->getRetokenizedLines();

    m_flowTargets = m_retokenizer->getFlowTargets();

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
    return data().mid(m_data_end);
}


