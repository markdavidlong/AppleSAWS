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
    AddressStack() = default;

    bool push(quint16 address, bool force = false) {
        if (force || (!m_stack.contains(address)))
        {
  //          qDebug() << "  PUSH: " << uint16ToHex(address);
            m_stack.push(address);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool isEmpty() const noexcept { return m_stack.isEmpty(); }
    [[nodiscard]] quint16 pop() { return m_stack.pop(); }

    private:
    QStack<quint16> m_stack;
};


class DisassembledItem {
public:
    DisassembledItem() { init(); }

    explicit DisassembledItem(quint8 opcode);

    [[nodiscard]] bool operator<(const DisassembledItem &other) const noexcept { return (address() < other.address()); }

    void setOpcode(quint8 opcode) { m_opcode = opcode; }
    void setAddress(quint16 add) { m_address = add; }
    void setDisassembledString(QString ds) { m_disassembly_text = ds; }
    void setHexValues(QByteArray hv) { m_hexvalues = hv; }
    void setHexString(QString hs) { m_hexstring = hs; }
    void setTargetAddress(quint16 ta) { m_unknown_ta = false; m_target_address = ta; }
    void setRawArgument(quint16 arg) { m_has_arg = true; m_raw_arg = arg; }
    void setCanNotFollow(bool canNotFollow) { m_canNotFollow = canNotFollow; }
    void setIsInvalidOp(bool isInvalid) { m_isInvalidOp = isInvalid; }

    [[nodiscard]] quint8 opcode() const noexcept { return m_opcode; }
    [[nodiscard]] quint16 address() const noexcept { return m_address; }
    [[nodiscard]] QString disassembledString();
    [[nodiscard]] QString rawDisassembledString() const { return m_disassembly_text; }
    [[nodiscard]] QString hexAddress() const;
    [[nodiscard]] QByteArray hexValues() const { return m_hexvalues; }
    [[nodiscard]] QString hexString() const { return m_hexstring; }

    [[nodiscard]] bool isBranch() const;
    [[nodiscard]] bool isJump() const;
    [[nodiscard]] bool isJsr() const;
    [[nodiscard]] bool isReturn() const;
    [[nodiscard]] bool isBreak() const;
    [[nodiscard]] bool isInvalidOp() const noexcept { return m_isInvalidOp; }

    [[nodiscard]] bool canNotFollow() const noexcept { return m_canNotFollow; }

    [[nodiscard]] bool stopsProcessing() const
    {
        return isBreak() || isInvalidOp() || isReturn() || canNotFollow();
    }

    [[nodiscard]] quint16 nextContiguousAddress() const noexcept { return m_nextContiguousAddress; }
    [[nodiscard]] quint16 nextFlowAddress() const noexcept { return m_nextFlowAddress; }

    void setNextContiguousAddress(quint16 addr) { m_nextContiguousAddress = addr; }
    void setNextFlowAddress(quint16 addr) { m_nextFlowAddress = addr; }

    [[nodiscard]] quint16 targetAddress() const noexcept { return m_target_address; }
    [[nodiscard]] bool hasArg() const noexcept { return m_has_arg; }

    [[nodiscard]] quint8 arg8() const noexcept { return static_cast<quint8>(m_raw_arg); }
    [[nodiscard]] QString arg8Str();

    [[nodiscard]] quint16 arg16() const noexcept { return m_raw_arg; }
    [[nodiscard]] QString arg16Str();

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
    explicit Disassembler(AttributedMemory &mem);

    enum class ProcessorType {
        MOS6502,
        MOS65C02
    };


    [[nodiscard]] QList<DisassembledItem> disassemble(quint16 from,
                                        quint16 to,
                                        QList<quint16> entryPoints,
                                        bool processRecursively = true);

    [[nodiscard]] MemoryUsageMap *memoryUsageMap() noexcept { return &m_memusagemap; }

    void setUnknownToData(quint16 from, quint16 to);


    [[nodiscard]] JumpLines getJumpLines() const { return m_jumplines; }

private:

    bool disassembleOp(quint16 address,
                       DisassembledItem &retval,
                       MemoryUsageMap *memuse = nullptr);

    quint16 m_from;
    quint16 m_to;

    AttributedMemory *m_mem;

    AddressStack m_stack;

    MemoryUsageMap m_memusagemap;

    JumpLineManager m_jlm;

    JumpLines m_jumplines;

};


