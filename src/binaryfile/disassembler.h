#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QByteArray>
#include <QStringList>
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
    AssyInstruction(quint8 opcode = 0x00, QString mnemonic = "???", AddressMode am = AM_InvalidOp) {
        m_opcode = opcode;
        m_mnemonic = mnemonic;
        m_addressMode = am;
    }

    AddressMode addressMode() { return m_addressMode; }

    QString mnemonic() { return m_mnemonic; }

    quint8 opcode() { return m_opcode; }

    quint8 numArgs() {
        switch (m_addressMode) {
        case AM_Absolute:
        case AM_AbsoluteIndexedIndirect:
        case AM_AbsoluteIndexedWithX:
        case AM_AbsoluteIndexedWithY:
        case AM_AbsoluteIndirect:
            return 2;
        case AM_ProgramCounterRelative:
        case AM_ZeroPage:
        case AM_ZeroPageIndirectIndexedWithY:
        case AM_ZeroPageIndexedIndirect:
        case AM_ZeroPageIndexedWithX:
        case AM_ZeroPageIndexedWithY:
        case AM_ZeroPageIndirect:
        case AM_Immediate:
            return 1;
        case AM_InvalidOp:
        case AM_Implied:
        case AM_Accumulator:
        default:
            return 0;
        }
    }

private:
    QString m_mnemonic;
    quint8 m_opcode;
    AddressMode m_addressMode;
};

//////////////////////////////////////////////////////////////////////////////

class Disassembler
{
public:
    Disassembler(QByteArray memimage) { m_memimage = memimage, makeOpcodeTable(); }

    enum ProcessorType {
        P6502,
        P65C02
    };

    QList<QStringList> disassemble(quint16 from, quint16 to);

private:

    QStringList disassembleOp(quint16 address, quint16 *nextAddress = 0);
    void makeOpcodeTable();

    QHash<quint8,AssyInstruction> m_opcodeinfo;
    QByteArray m_memimage;

};

#endif // DISASSEMBLER_H
