#include "sector.h"

#include <QString>
#include <QDebug>

QByteRef Sector::operator[](uint offset) {
    if (offset > 255) {
        offset = 255;
    }
    return m_data[offset];
}

bool Sector::setData(QByteArray data) {
    if (data.length() != 256) return false;

    m_data = data;
    return true;
}

void Sector::dump() {
    qDebug() << "Dumping Track " << track() << "Sector " << sector() << " ...";
    for (int jdx = 0; jdx < 16; jdx++)
    {
        QString line;
        line += QString("%1 (%2): ").arg(jdx*16,2,16,QChar('0')).arg(jdx*16,3,10,QChar(' '));
        for (int idx = 0; idx < 16; idx++)
        {
            int offset = (jdx*16) + idx;
            quint8 val = m_data[offset];
            line +=  QString("%1 ").arg(val,2,16,QChar('0'));
            if (idx == 7) line += " ";
        }
        line = line.toUpper();
        line += "  ";
        for (int idx = 0; idx < 16; idx++)
        {
            int offset = (jdx*16) + idx;
            quint8 val = m_data[offset];
            if (val > 127) { val -= 128; }
            QChar ch(val);
            line +=  QString("%1").arg(ch.isPrint()?ch:'.');
        }

        qDebug() << line;
    }
}

