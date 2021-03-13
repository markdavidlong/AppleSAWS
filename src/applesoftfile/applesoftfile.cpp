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

#include "applesoftfile.h"
#include "util.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QList>

ApplesoftFile::ApplesoftFile(Dos33DiskImage *image, FileDescriptiveEntry &fde)
    : GenericFile(image,fde)
{
    m_retokenizer = Q_NULLPTR;
    m_data_end = data().length();

    setAddress(0x801);

    processData();
}

void ApplesoftFile::processData()
{
    if (!m_retokenizer)
    {
        m_retokenizer = new ApplesoftRetokenizer();
    }

    m_length = dataWordAt(0);
    QByteArray tmp = rawData().mid(2);

    m_retokenizer->setData(tmp);
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


