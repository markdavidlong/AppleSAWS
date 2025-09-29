#pragma once

#include "Util.h"
#include "MemoryUsageMap.h"
#include "../internals/JumpLineManager.h"

#include <QByteArray>
#include <QStringList>
#include <QHash>
#include <QStack>
#include <QList>

// Forward declarations
class AttributedMemory;
struct JumpLines;


class AddressStack
{
    public:
    AddressStack() { }

    bool push(quint16 address, bool force = false) {
        if (force || (!m_stack.contains(address)))
        {
  //          qDebug() << "  PUSH: " << uint16ToHex(address);
            m_stack.push(address);
            return true;
        }
        return false;
    }

    bool isEmpty() { return m_stack.isEmpty(); }
    quint16 pop() { return m_stack.pop(); }

    private:
    QStack<quint16> m_stack;
};


class DisassembledItem {
public:
    DisassembledItem() { init(); }

    DisassembledItem(quint8 opcode);

    bool operator<(const DisassembledItem &other) const { return (address() < other.address()); }

    void setOpcode(quint8 opcode) { m_opcode = opcode; }
    void setAddress(quint16 add) { m_address = add; }
    void setDisassembledString(QString ds) { m_disassembly_text = ds; }
    void setHexValues(QByteArray hv) { m_hexvalues = hv; }
    void setHexString(QString hs) { m_hexstring = hs; }
    void setTargetAddress(quint16 ta) { m_unknown_ta = false; m_target_address = ta; }
    void setRawArgument(quint16 arg) { m_has_arg = true; m_raw_arg = arg; }
    void setCanNotFollow(bool canNotFollow) { m_canNotFollow = canNotFollow; }
    void setIsInvalidOp(bool isInvalid) { m_isInvalidOp = isInvalid; }

    quint8 opcode() const { return m_opcode; }
    quint16 address()  const { return m_address; }
    QString disassembledString();
    QString rawDisassembledString() const { return m_disassembly_text; }
    QString hexAddress() const;
    QByteArray hexValues() const { return m_hexvalues; }
    QString hexString() const { return m_hexstring; }

    bool isBranch() const;
    bool isJump() const;
    bool isJsr() const;
    bool isReturn();
    bool isBreak();
    bool isInvalidOp() { return m_isInvalidOp; }

    bool canNotFollow() { return m_canNotFollow; }

    bool stopsProcessing()
    {
        return isBreak() || isInvalidOp() || isReturn() || canNotFollow();
    }

    quint16 nextContiguousAddress() { return m_nextContiguousAddress; }
    quint16 nextFlowAddress() { return m_nextFlowAddress; }

    void setNextContiguousAddress(quint16 addr) { m_nextContiguousAddress = addr; }
    void setNextFlowAddress(quint16 addr) { m_nextFlowAddress = addr; }

    quint16 targetAddress() const { return m_target_address; }
    bool hasArg() const { return m_has_arg; }

    quint8 arg8() { return m_raw_arg % 256; }
    QString arg8Str();

    quint16 arg16() { return m_raw_arg; }
    QString arg16Str();

private:
    void init();

    quint8 m_opcode;

    quint16 m_address;
    quint16 m_nextContiguousAddress;
    quint16 m_nextFlowAddress;
    quint16 m_target_address;

    QByteArray m_hexvalues;
    QString m_disassembly_text;
    QString m_hexstring;

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
    Disassembler(AttributedMemory &mem);

    enum ProcessorType {
        MOS6502,
        MOS65C02
    };


    QList<DisassembledItem> disassemble(quint16 from,
                                        quint16 to,
                                        QList<quint16> entryPoints,
                                        bool processRecursively = true);

    MemoryUsageMap *memoryUsageMap() { return &m_memusagemap; }

    void setUnknownToData(quint16 from, quint16 to);


    JumpLines getJumpLines() const { return m_jumplines; }

private:

    bool disassembleOp(quint16 address,
                       DisassembledItem &retval,
                       MemoryUsageMap *memuse = Q_NULLPTR);

    quint16 m_from;
    quint16 m_to;

    AttributedMemory *m_mem;

    AddressStack m_stack;

    MemoryUsageMap m_memusagemap;

    JumpLineManager m_jlm;

    JumpLines m_jumplines;

};


