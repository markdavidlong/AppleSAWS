#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QByteArray>
#include <QStringList>

class Disassembler
{
public:
    static QStringList disassemble(quint16 address, QByteArray values);
};

#endif // DISASSEMBLER_H
