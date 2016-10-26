#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "MemoryUsageMap.h"
#include "util.h"

#include <QByteArray>
#include <QStringList>
#include <QHash>
#include <QDebug>

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
//////////////////////////////////////////////////////////////////////////////

class DisassembledItem {
public:
    DisassembledItem() { init(); }

    DisassembledItem(AssyInstruction instr);

    bool operator<(const DisassembledItem &other) const { return (address() < other.address()); }

    void setInstruction(AssyInstruction instr);

    void setAddress(quint16 add) { m_address = add; }
    void setDisassembledString(QString ds) { m_disassembly_text = ds; }
    void setHexValues(QByteArray hv) { m_hexvalues = hv; }
    void setHexString(QString hs) { m_hexstring = hs; }
    void setBranch(bool branch) { m_is_branch = branch; }
    void setJump(bool jump) { m_is_jump = jump; }
    void setJsr(bool jsr) { m_is_jsr = jsr; }
    void setTargetAddress(quint16 ta) { m_unknown_ta = false; m_target_address = ta; }
    void setRawArgument(quint16 arg) { m_has_arg = true; m_raw_arg = arg; }
    void setCanNotFollow(bool canNotFollow) { m_canNotFollow = canNotFollow; }
    void setIsInvalidOp(bool isInvalid) { m_isInvalidOp = isInvalid; }

    AssyInstruction assyInstruction() const { return m_instruction; }
    QString rawDisassembledString() const { return m_disassembly_text; }
    QString disassembledString();
    quint16 address()  const { return m_address; }
    QString hexAddress() const { return QString("%1").arg(m_address,4,16,QChar('0')).toUpper(); }
    QByteArray hexValues() const { return m_hexvalues; }
    QString hexString() const { return m_hexstring; }
    bool isBranch() const { return m_is_branch; }
    bool isJump() const { return m_is_jump; }
    bool isJsr() const { return m_is_jsr; }
    bool isReturn() { return (m_instruction.opcode() == 0x60) || m_instruction.opcode() == 0x40; }
    bool isBreak() { return (m_instruction.opcode() == 0x00); }
    bool isInvalidOp() { return m_isInvalidOp; }

    bool canNotFollow() { return m_canNotFollow; }

    bool stopsProcessing() {
        if (isReturn()) qDebug() << "Is Return";
        if (isInvalidOp()) qDebug() << "Is Invalid Op" << uint8ToHex(m_instruction.opcode());
        if (canNotFollow()) qDebug() << "Can not follow indirect jump";
        if (isBreak()) qDebug() << "Is Break";
        return isBreak() || isInvalidOp() || isReturn() || canNotFollow(); }

    quint16 nextContiguousAddress() { return m_nextContiguousAddress; }
    quint16 nextFlowAddress() { return m_nextFlowAddress; }

    void setNextContiguousAddress(quint16 addr) { m_nextContiguousAddress = addr; }
    void setNextFlowAddress(quint16 addr) { m_nextFlowAddress = addr; }

    quint16 targetAddress() const { return m_target_address; }
    bool hasArg() const { return m_has_arg; }
    quint16 arg16() { return m_raw_arg; }
    quint8 arg8() { return m_raw_arg % 256; }
    QString arg16Str() { return QString("%1").arg(arg16(),4,16,QChar('0')).toUpper(); }
    QString arg8Str() { return QString("%1").arg(arg8(),2,16,QChar('0')).toUpper(); }

private:
    void init();

    quint16 m_address;
    quint16 m_nextContiguousAddress;
    quint16 m_nextFlowAddress;
    quint16 m_target_address;

    QByteArray m_hexvalues;
    QString m_disassembly_text;
    QString m_hexstring;
    bool m_is_branch;
    bool m_is_jump;
    bool m_is_jsr;
    AssyInstruction m_instruction;
    bool m_unknown_ta;
    quint16 m_raw_arg;
    bool m_has_arg;
    bool m_isInvalidOp;
    bool m_canNotFollow;
};


//////////////////////////////////////////////////////////////////////////////

class Disassembler
{
public:
    Disassembler(QByteArray memimage);

    enum ProcessorType {
        P6502,
        P65C02
    };

    QList<DisassembledItem> disassemble(quint16 from, quint16 to,bool processRecursively = true);

    MemoryUsageMap *memoryUsageMap() { return &m_memusagemap; }

    void setUnknownToData(quint16 from, quint16 to);

    QString getMnemonicForOp(quint8 opcode)
    {
        return m_opcodeinfo[opcode].mnemonic();
    }

private:

    bool disassembleOp(quint16 address, DisassembledItem &retval, MemoryUsageMap *memuse = Q_NULLPTR);
    void makeOpcodeTable();


    QHash<quint8,AssyInstruction> m_opcodeinfo;
    QByteArray m_memimage;

    QList<quint16> m_jumps;

    MemoryUsageMap m_memusagemap;

};

#endif // DISASSEMBLER_H
