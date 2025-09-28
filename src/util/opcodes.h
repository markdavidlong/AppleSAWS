#pragma once

#include "util.h"

#include <QString>
#include <QHash>

enum AddressMode
{
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

class OpCodes
{

    struct AssyInstruction
    {
    public:
        AssyInstruction(quint8 opcode = 0x00, QString mnemonic = "???", AddressMode am = AM_InvalidOp, bool reads = false, bool writes = false, bool stack = false);

        AddressMode addressMode() { return m_addressMode; }

        QString mnemonic() const { return m_mnemonic; }

        quint8 opcode() const { return m_opcode; }

        quint8 numArgs() const;

        bool readsMem() const { return m_reads; }
        bool writesMem() const { return m_writes; }
        bool modifiesStack() const { return m_stack; }

        //  QString debugStr() { return QString("%1 %2 %3").arg(uint8ToHex(m_opcode)).arg(m_mnemonic).arg(m_addressMode); }

    private:
        QString m_mnemonic;
        quint8 m_opcode;
        AddressMode m_addressMode;
        bool m_reads;
        bool m_writes;
        bool m_stack;
    };

public:
    static QString getMnemonic(quint8 opcode)
    {
        return getAssyInstruction(opcode).mnemonic();
    }

    static AssyInstruction &getAssyInstruction(quint8 id)
    {
        static QHash<quint8, AssyInstruction> m_opcodeinfo;
        if (m_opcodeinfo.size() == 0)
        {
            makeOpcodeTable(&m_opcodeinfo);
        }
        return m_opcodeinfo[id];
    }

    static AddressMode addressMode(quint8 opcode);
    static QString mnemonic(quint8 opcode);
    static quint8 numArgs(quint8 opcode);

    static bool isJump(quint8 id);
    static bool isIndirectJump(quint8 id);
    static bool isBranch(quint8 id);
    static bool isJsr(quint8 id);
    static bool isReturn(quint8 id);
    static bool isBreak(quint8 id);

    static bool readsMemoryLoc(quint8 id);
    static bool writesMemoryLoc(quint8 id);
    static bool modifiesStack(quint8 id);

protected:
    OpCodes();
    static void makeOpcodeTable(QHash<quint8, AssyInstruction> *m_opcodeinfo);
};
