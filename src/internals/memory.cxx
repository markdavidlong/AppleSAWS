#include "memory.h"
#include <QDebug>

Memory::Memory()
{
    quint8 zero = 0;
    for (int idx = 0; idx < 65536; idx++) {
        m_memory.append(zero);
    }
}

bool Memory::addFile(QByteArray data, quint16 start)
{
    if (start+data.length() > 65536) {
        qDebug() << "Memory overflow adding data."; return false;
    }

    m_memory.replace(start,data.length(),data);

    return true;
}
