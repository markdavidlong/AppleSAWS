#include "sector.h"

#include <QString>
#include <QDebug>

quint8 Sector::operator[](uint offset) const {
    if (offset > 255) {
        offset = 255;
    }
    if ((int) offset >= m_raw_data->size()) return 0;

    return m_raw_data->at(offset);
}


void Sector::dump() {
    qDebug() << "Dumping Track " << track() << "Sector " << sector() << " ...";
    for (int jdx = 0; jdx < 16; jdx++)
    {
        QString line;
        line += QString("%1 (%2): ")
                .arg(uint16ToHex(jdx*16)).arg(jdx*16,3,10,QChar(' '));
        for (int idx = 0; idx < 16; idx++)
        {
            int offset = (jdx*16) + idx;
            quint8 val = m_raw_data->at(offset);
            line +=  QString("%1 ").arg(uint16ToHex(val));
            if (idx == 7) line += " ";
        }
        line = line.toUpper();
        line += "  ";
        for (int idx = 0; idx < 16; idx++)
        {
            int offset = (jdx*16) + idx;
            quint8 val = m_raw_data->at(offset);
            if (val > 127) { val -= 128; }
            QChar ch(val);
            line +=  QString("%1").arg(ch.isPrint()?ch:'.');
        }

        qDebug() << line;
    }
}

