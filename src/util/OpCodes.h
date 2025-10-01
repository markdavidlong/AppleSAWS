#pragma once

#include "Util.h"

#include <QString>
#include <QHash>

/**
 * @brief 6502/65C02 addressing modes for assembly instructions
 */
enum class AddressMode : quint8
{
    InvalidOp,
    Absolute,                    // a
    AbsoluteIndexedIndirect,     // (a,x)
    AbsoluteIndexedWithX,        // a,x
    AbsoluteIndexedWithY,        // a,y
    AbsoluteIndirect,            // (a)
    Immediate,                   // #
    Implied,                     // i
    Accumulator,                 // A
    ProgramCounterRelative,      // r
    ZeroPage,                    // zp
    ZeroPageIndexedIndirect,     // (zp,x)
    ZeroPageIndexedWithX,        // zp,x
    ZeroPageIndexedWithY,        // zp,y
    ZeroPageIndirect,            // (zp)
    ZeroPageIndirectIndexedWithY // (zp),y
};

/**
 * @brief Utility class for 6502/65C02 opcode analysis and instruction information
 * 
 * This class provides static methods for analyzing 6502/65C02 opcodes, determining
 * addressing modes, instruction properties, and control flow characteristics.
 */
class OpCodes
{
    /**
     * @brief Represents a 6502/65C02 assembly instruction with its properties
     */
    struct AssyInstruction final
    {
    public:
        /**
         * @brief Construct an assembly instruction
         * @param opcode The instruction opcode (0x00-0xFF)
         * @param mnemonic The instruction mnemonic (e.g., "LDA", "STA")
         * @param am The addressing mode for this instruction
         * @param reads Whether instruction reads from memory
         * @param writes Whether instruction writes to memory
         * @param stack Whether instruction modifies the stack
         */
        explicit AssyInstruction(quint8 opcode = 0x00, 
                               const QString& mnemonic = QStringLiteral("???"), 
                               AddressMode am = AddressMode::InvalidOp, 
                               bool reads = false, 
                               bool writes = false, 
                               bool stack = false) noexcept;

        /**
         * @brief Get the addressing mode for this instruction
         * @return The addressing mode
         */
        [[nodiscard]] constexpr AddressMode addressMode() const noexcept { return m_addressMode; }

        /**
         * @brief Get the instruction mnemonic
         * @return The mnemonic string (e.g., "LDA", "STA")
         */
        [[nodiscard]] const QString& mnemonic() const noexcept { return m_mnemonic; }

        /**
         * @brief Get the instruction opcode
         * @return The opcode value (0x00-0xFF)
         */
        [[nodiscard]] constexpr quint8 opcode() const noexcept { return m_opcode; }

        /**
         * @brief Get the number of argument bytes for this instruction
         * @return Number of bytes (0, 1, or 2)
         */
        [[nodiscard]] quint8 numArgs() const noexcept;

        /**
         * @brief Check if instruction reads from memory
         * @return True if instruction reads memory
         */
        [[nodiscard]] constexpr bool readsMem() const noexcept { return m_reads; }
        
        /**
         * @brief Check if instruction writes to memory
         * @return True if instruction writes memory
         */
        [[nodiscard]] constexpr bool writesMem() const noexcept { return m_writes; }
        
        /**
         * @brief Check if instruction modifies the stack
         * @return True if instruction affects stack
         */
        [[nodiscard]] constexpr bool modifiesStack() const noexcept { return m_stack; }

    private:
        QString m_mnemonic{QStringLiteral("???")}; ///< Instruction mnemonic
        quint8 m_opcode{0x00};                    ///< Instruction opcode
        AddressMode m_addressMode{AddressMode::InvalidOp}; ///< Addressing mode
        bool m_reads{false};                      ///< Reads from memory
        bool m_writes{false};                     ///< Writes to memory
        bool m_stack{false};                      ///< Modifies stack
    };

public:
    /**
     * @brief Get the mnemonic for an opcode
     * @param opcode The instruction opcode
     * @return The mnemonic string
     */
    [[nodiscard]] static QString getMnemonic(quint8 opcode)
    {
        return getAssyInstruction(opcode).mnemonic();
    }

    /**
     * @brief Get the complete instruction information for an opcode
     * @param id The instruction opcode
     * @return Reference to the AssyInstruction
     */
    [[nodiscard]] static AssyInstruction& getAssyInstruction(quint8 id)
    {
        static QHash<quint8, AssyInstruction> opcodeInfo;
        if (opcodeInfo.isEmpty())
        {
            makeOpcodeTable(&opcodeInfo);
        }
        return opcodeInfo[id];
    }

    [[nodiscard]] static AddressMode addressMode(quint8 opcode) noexcept;
    [[nodiscard]] static QString mnemonic(quint8 opcode);
    [[nodiscard]] static quint8 numArgs(quint8 opcode) noexcept;

    [[nodiscard]] static bool isJump(quint8 id) noexcept;
    [[nodiscard]] static bool isIndirectJump(quint8 id) noexcept;
    [[nodiscard]] static bool isBranch(quint8 id) noexcept;
    [[nodiscard]] static bool isJsr(quint8 id) noexcept;
    [[nodiscard]] static bool isReturn(quint8 id) noexcept;
    [[nodiscard]] static bool isBreak(quint8 id) noexcept;

    [[nodiscard]] static bool readsMemoryLoc(quint8 id) noexcept;
    [[nodiscard]] static bool writesMemoryLoc(quint8 id) noexcept;
    [[nodiscard]] static bool modifiesStack(quint8 id) noexcept;

protected:
    /**
     * @brief Default constructor (protected - utility class)
     */
    OpCodes() = default;
    
    /**
     * @brief Initialize the opcode lookup table
     * @param opcodeTable Pointer to the hash table to populate
     */
    static void makeOpcodeTable(QHash<quint8, AssyInstruction>* opcodeTable);
};
