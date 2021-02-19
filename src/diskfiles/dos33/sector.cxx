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

