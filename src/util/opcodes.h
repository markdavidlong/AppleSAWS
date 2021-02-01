#ifndef OPCODES_H
#define OPCODES_H

#include "util.h"

#include <QString>
#include <QHash>


enum AddressMode {
    AM_InvalidOp,
    AM_Absolute,                    // a
    AM_AbsoluteIndexedIndirect,     // (a,x)
    AM_AbsoluteIndexedWithX,        // a,x
    AM_AbsoluteIndexedWithY,        // a,y
    AM_AbsoluteIndirect,            // (a)
    AM_Immediate,                   // #
    AM_Implied,                     // i
    AM_Accumulator,                 // A
    AM_ProgramCounterRelative,      // r
    AM_ZeroPage,                    // zp
    AM_ZeroPageIndexedIndirect,     // (zp,x)
    AM_ZeroPageIndexedWithX,        // zp,x
    AM_ZeroPageIndexedWithY,        // zp,y
    AM_ZeroPageIndirect,            // (zp)
    AM_ZeroPageIndirectIndexedWithY // (zp),y
};

struct AssyInstruction {

public:

    AssyInstruction(quint8 opcode = 0x00, QString mnemonic = "???", AddressMode am = AM_InvalidOp);

    AddressMode addressMode() { return m_addressMode; }

    QString mnemonic() { return m_mnemonic; }

    quint8 opcode() { return m_opcode; }

    quint8 numArgs();

    QString debugStr() { return QString("%1 %2 %3").arg(uint8ToHex(m_opcode)).arg(m_mnemonic).arg(m_addressMode); }

private:
    QString m_mnemonic;
    quint8 m_opcode;
    AddressMode m_addressMode;
};

class OpCodes
{
public:

    static QString getMnemonic(quint8 opcode)
    {
        return getAssyInstruction(opcode).mnemonic();
    }

    static AssyInstruction &getAssyInstruction(quint8 id)
    {
        static QHash<quint8,AssyInstruction> m_opcodeinfo;
        if (m_opcodeinfo.size() == 0)
        {
            makeOpcodeTable(&m_opcodeinfo);
        }
        return m_opcodeinfo[id];
    }

protected:
    OpCodes();
    static void makeOpcodeTable(QHash<quint8,AssyInstruction>* m_opcodeinfo);

};


#endif // OPCODES_H
