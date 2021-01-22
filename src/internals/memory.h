#ifndef MEMORY_H
#define MEMORY_H

#include <QByteArray>

class Memory {
public:
    Memory();

    bool addFile(QByteArray data, quint16 start);

    QByteArray &values() { return m_memory; }

    quint8 at(quint16 addr) { return m_memory.at(addr); }

private:
    QByteArray m_memory;
};


#endif
