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
//////////////////////////////////////////////////////////////////////////////

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

class DisassembledItem {
public:
    DisassembledItem() { init(); }

    DisassembledItem(AssyInstruction instr) {
        setInstruction(instr);
    }


    void setInstruction(AssyInstruction instr) {
        m_instruction = instr;
        if (instr.opcode() == 0x20) { m_is_jsr = true; }
        if (instr.opcode() == 0x10) { m_is_branch = true; } // BPL
        if (instr.opcode() == 0x30) { m_is_branch = true; } // BMI
        if (instr.opcode() == 0x50) { m_is_branch = true; } // BVC
        if (instr.opcode() == 0x70) { m_is_branch = true; } // BVS
        if (instr.opcode() == 0x90) { m_is_branch = true; } // BCC
        if (instr.opcode() == 0xB0) { m_is_branch = true; } // BCS
        if (instr.opcode() == 0xD0) { m_is_branch = true; } // BNE
        if (instr.opcode() == 0xF0) { m_is_branch = true; } // BEQ
        if (instr.opcode() == 0x80) { m_is_jump = true; }   // BRA
        if (instr.opcode() == 0x4C) { m_is_jump = true; }   // JMP a
        if (instr.opcode() == 0x6C) { m_is_jump = true; }   // JMP (a)
        if (instr.opcode() == 0x7C) { m_is_jump = true; }   // JMP (a,x)
    }

    void setAddress(quint16 add) { m_address = add; }
    void setDisassembledString(QString ds) { m_disassembly_text = ds; }
    void setHexValues(QByteArray hv) { m_hexvalues = hv; }
    void setHexString(QString hs) { m_hexstring = hs; }
    void setBranch(bool branch) { m_is_branch = branch; }
    void setJump(bool jump) { m_is_jump = jump; }
    void setJsr(bool jsr) { m_is_jsr = jsr; }
    void setTargetAddress(quint16 ta) { m_unknown_ta = false; m_target_address = ta; }

    AssyInstruction assyInstruction() const { return m_instruction; }
    QString disassembledString() const { return m_disassembly_text; }
    quint16 address()  const { return m_address; }
    QString hexAddress() const { return QString("%1").arg(m_address,4,16,QChar('0')).toUpper(); }
    QByteArray hexValues() const { return m_hexvalues; }
    QString hexString() const { return m_hexstring; }
    bool isBranch() const { return m_is_branch; }
    bool isJump() const { return m_is_jump; }
    bool isJsr() const { return m_is_jsr; }
    quint16 targetAddress() const { return m_target_address; }

private:
    void init() {
        m_address = m_target_address = 0;
        m_is_jump = m_is_branch = m_is_jsr = false;
        m_unknown_ta = true;
    }

    quint16 m_address;
    QByteArray m_hexvalues;
    QString m_disassembly_text;
    QString m_hexstring;
    bool m_is_branch;
    bool m_is_jump;
    bool m_is_jsr;
    quint16 m_target_address;
    AssyInstruction m_instruction;
    bool m_unknown_ta;
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

    QList<DisassembledItem> disassemble(quint16 from, quint16 to);

private:

    DisassembledItem disassembleOp(quint16 address, quint16 *nextAddress = 0);
    void makeOpcodeTable();

    QHash<quint8,AssyInstruction> m_opcodeinfo;
    QByteArray m_memimage;

};

#endif // DISASSEMBLER_H
