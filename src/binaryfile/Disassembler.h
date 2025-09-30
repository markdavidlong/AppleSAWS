#pragma once

#include "Util.h"
#include "MemoryUsageMap.h"
#include "DisassembledItem.h"
#include "JumpLineManager.h"

#include <QByteArray>
#include <QStringList>
#include <QHash>
#include <QStack>
#include <QList>

// Forward declarations
class AttributedMemory;
struct JumpLines;

class Disassembler
{
public:
    explicit Disassembler(AttributedMemory &mem);

    enum class ProcessorType
    {
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

    quint16 m_from{0};
    quint16 m_to{0};

    AttributedMemory &m_mem;

    QStack<quint16> m_stack;

    MemoryUsageMap m_memusagemap;

    JumpLineManager m_jlm;

    JumpLines m_jumplines;
};
