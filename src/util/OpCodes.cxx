#include "OpCodes.h"

// AssyInstruction constructor implementation
OpCodes::AssyInstruction::AssyInstruction(quint8 opcode, const QString& mnemonic, AddressMode am, bool reads, bool writes, bool stack) noexcept
    : m_mnemonic(mnemonic), m_opcode(opcode), m_addressMode(am), m_reads(reads), m_writes(writes), m_stack(stack)
{
}

AddressMode OpCodes::addressMode(quint8 opcode) noexcept
{
    return getAssyInstruction(opcode).addressMode();
}

QString OpCodes::mnemonic(quint8 opcode)
{
    return getMnemonic(opcode);
}

quint8 OpCodes::numArgs(quint8 opcode) noexcept
{
    return getAssyInstruction(opcode).numArgs();
}

bool OpCodes::isJump(quint8 id) noexcept
{
    if (id == 0x80) return true;  // BRA
    if (id == 0x4C) return true;  // JMP a
    if (id == 0x6C) return true;  // JMP (a)
    if (id == 0x7C) return true;  // JMP (a,x);
    return false;
}

bool OpCodes::isIndirectJump(quint8 id) noexcept
{
    if (id == 0x6C) return true;  // JMP (a)
    if (id == 0x7C) return true;  // JMP (a,x);
    return false;
}

bool OpCodes::isBranch(quint8 id) noexcept
{
    if (id == 0x10) return true;  // BPL
    if (id == 0x30) return true;  // BMI
    if (id == 0x50) return true;  // BVC
    if (id == 0x70) return true;  // BVS
    if (id == 0x90) return true;  // BCC
    if (id == 0xB0) return true;  // BCS
    if (id == 0xD0) return true;  // BNE
    if (id == 0xF0) return true;  // BEQ
    return false;
}

bool OpCodes::isJsr(quint8 id) noexcept
{
    return (id == 0x20); // JSR
}

bool OpCodes::isReturn(quint8 id) noexcept
{
    if (id == 0x40) return true;  // RTI
    if (id == 0x60) return true;  // RTS
    return false;
}

bool OpCodes::isBreak(quint8 id) noexcept
{
    return (id == 0x00);  // BRK
}

bool OpCodes::readsMemoryLoc(quint8 opcode) noexcept
{
    return getAssyInstruction(opcode).readsMem();
}

bool OpCodes::writesMemoryLoc(quint8 opcode) noexcept
{
    return getAssyInstruction(opcode).writesMem();
}

bool OpCodes::modifiesStack(quint8 opcode) noexcept
{
    return getAssyInstruction(opcode).modifiesStack();
}


void OpCodes::makeOpcodeTable(QHash<quint8,AssyInstruction>* opcode_table)
{
    opcode_table->insert(0x00,AssyInstruction(0x00, "BRK", AddressMode::Implied));
    opcode_table->insert(0x10,AssyInstruction(0x10, "BPL", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0x20,AssyInstruction(0x20, "JSR", AddressMode::Absolute, false, false, true));
    opcode_table->insert(0x30,AssyInstruction(0x30, "BMI", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0x40,AssyInstruction(0x40, "RTI", AddressMode::Implied, false, false, true));
    opcode_table->insert(0x50,AssyInstruction(0x50, "BVC", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0x60,AssyInstruction(0x60, "RTS", AddressMode::Implied, false, false, true));
    opcode_table->insert(0x70,AssyInstruction(0x70, "BVS", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0x80,AssyInstruction(0x80, "BRA", AddressMode::ProgramCounterRelative)); //65C02
    opcode_table->insert(0x90,AssyInstruction(0x90, "BCC", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0xa0,AssyInstruction(0xa0, "LDY", AddressMode::Immediate));
    opcode_table->insert(0xb0,AssyInstruction(0xb0, "BCS", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0xc0,AssyInstruction(0xc0, "CPY", AddressMode::Immediate));
    opcode_table->insert(0xd0,AssyInstruction(0xd0, "BNE", AddressMode::ProgramCounterRelative));
    opcode_table->insert(0xe0,AssyInstruction(0xe0, "CPX", AddressMode::Immediate));
    opcode_table->insert(0xf0,AssyInstruction(0xf0, "BEQ", AddressMode::ProgramCounterRelative));

    opcode_table->insert(0x01,AssyInstruction(0x01, "ORA", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x11,AssyInstruction(0x11, "ORA", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x21,AssyInstruction(0x21, "AND", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x31,AssyInstruction(0x31, "AND", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x41,AssyInstruction(0x41, "EOR", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x51,AssyInstruction(0x51, "EOR", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x61,AssyInstruction(0x61, "ADC", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x71,AssyInstruction(0x71, "ADC", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x81,AssyInstruction(0x81, "STA", AddressMode::ZeroPageIndexedIndirect, false, true, false));
    opcode_table->insert(0x91,AssyInstruction(0x91, "STA", AddressMode::ZeroPageIndirectIndexedWithY, false, true, false));
    opcode_table->insert(0xa1,AssyInstruction(0xa1, "LDA", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xb1,AssyInstruction(0xb1, "LDA", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0xc1,AssyInstruction(0xc1, "CMP", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xd1,AssyInstruction(0xd1, "CMP", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0xe1,AssyInstruction(0xe1, "SBC", AddressMode::ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xf1,AssyInstruction(0xff, "SBC", AddressMode::ZeroPageIndirectIndexedWithY, true, false, false));

    opcode_table->insert(0x02,AssyInstruction(0x02, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x12,AssyInstruction(0x12, "ORA", AddressMode::ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x22,AssyInstruction(0x22, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x32,AssyInstruction(0x32, "AND", AddressMode::ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x42,AssyInstruction(0x42, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x52,AssyInstruction(0x52, "EOR", AddressMode::ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x62,AssyInstruction(0x62, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x72,AssyInstruction(0x72, "ADC", AddressMode::ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x82,AssyInstruction(0x82, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x92,AssyInstruction(0x92, "STA", AddressMode::ZeroPageIndirect, false, true, false)); //65C02
    opcode_table->insert(0xa2,AssyInstruction(0xa2, "LDX", AddressMode::Immediate));
    opcode_table->insert(0xb2,AssyInstruction(0xb2, "LDA", AddressMode::ZeroPageIndirect, true, false, false)); //65C02
    opcode_table->insert(0xc2,AssyInstruction(0xc2, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xd2,AssyInstruction(0xd2, "CMP", AddressMode::ZeroPageIndirect, true, false, false));
    opcode_table->insert(0xe2,AssyInstruction(0xe2, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xf2,AssyInstruction(0xf2, "SBC", AddressMode::ZeroPageIndirect, true, false, false));

    opcode_table->insert(0x03,AssyInstruction(0x03, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x13,AssyInstruction(0x13, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x23,AssyInstruction(0x23, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x33,AssyInstruction(0x33, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x43,AssyInstruction(0x43, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x53,AssyInstruction(0x53, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x63,AssyInstruction(0x63, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x73,AssyInstruction(0x73, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x83,AssyInstruction(0x83, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x93,AssyInstruction(0x93, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xa3,AssyInstruction(0xa3, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xb3,AssyInstruction(0xb3, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xc3,AssyInstruction(0xc3, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xd3,AssyInstruction(0xd3, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xe3,AssyInstruction(0xe3, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xf3,AssyInstruction(0xf3, "???", AddressMode::InvalidOp));

    opcode_table->insert(0x04,AssyInstruction(0x04, "TSB", AddressMode::ZeroPage, true, false, false)); //65C02
    opcode_table->insert(0x14,AssyInstruction(0x14, "TRB", AddressMode::ZeroPage, true, false, false)); //65C02
    opcode_table->insert(0x24,AssyInstruction(0x24, "BIT", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0x34,AssyInstruction(0x34, "BIT", AddressMode::ZeroPageIndexedWithX, true, false, false)); //65C02
    opcode_table->insert(0x44,AssyInstruction(0x44, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x54,AssyInstruction(0x54, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x64,AssyInstruction(0x64, "STZ", AddressMode::ZeroPage, false, true, false));  //65C02
    opcode_table->insert(0x74,AssyInstruction(0x74, "STZ", AddressMode::ZeroPageIndexedWithX, false, true, false)); //65C02
    opcode_table->insert(0x84,AssyInstruction(0x84, "STY", AddressMode::ZeroPage, false, true, false));
    opcode_table->insert(0x94,AssyInstruction(0x94, "STY", AddressMode::ZeroPageIndexedWithX, false, true, false));
    opcode_table->insert(0xa4,AssyInstruction(0xa4, "LDY", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xb4,AssyInstruction(0xb4, "LDY", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xc4,AssyInstruction(0xc4, "CPY", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xd4,AssyInstruction(0xd4, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xe4,AssyInstruction(0xe4, "CPX", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xf4,AssyInstruction(0xf4, "???", AddressMode::InvalidOp));

    opcode_table->insert(0x05,AssyInstruction(0x05, "ORA", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0x15,AssyInstruction(0x15, "ORA", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x25,AssyInstruction(0x25, "AND", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0x35,AssyInstruction(0x35, "AND", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x45,AssyInstruction(0x45, "EOR", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0x55,AssyInstruction(0x55, "EOR", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x65,AssyInstruction(0x65, "ADC", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0x75,AssyInstruction(0x75, "ADC", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x85,AssyInstruction(0x85, "STA", AddressMode::ZeroPage, false, true, false));
    opcode_table->insert(0x95,AssyInstruction(0x95, "STA", AddressMode::ZeroPageIndexedWithX, false, true, false));
    opcode_table->insert(0xa5,AssyInstruction(0xa5, "LDA", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xb5,AssyInstruction(0xb5, "LDA", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xc5,AssyInstruction(0xc5, "CMP", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xd5,AssyInstruction(0xd5, "CMP", AddressMode::ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xe5,AssyInstruction(0xe5, "SBC", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xf5,AssyInstruction(0xf5, "SBC", AddressMode::ZeroPageIndexedWithX, true, false, false));

    opcode_table->insert(0x06,AssyInstruction(0x06, "ASL", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0x16,AssyInstruction(0x16, "ASL", AddressMode::ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x26,AssyInstruction(0x26, "ROL", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0x36,AssyInstruction(0x36, "ROL", AddressMode::ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x46,AssyInstruction(0x46, "LSR", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0x56,AssyInstruction(0x56, "LSR", AddressMode::ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x66,AssyInstruction(0x66, "ROR", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0x76,AssyInstruction(0x76, "ROR", AddressMode::ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x86,AssyInstruction(0x86, "STX", AddressMode::ZeroPage, false, true, false));
    opcode_table->insert(0x96,AssyInstruction(0x96, "STX", AddressMode::ZeroPageIndexedWithY, false, true, false));
    opcode_table->insert(0xa6,AssyInstruction(0xa6, "LDX", AddressMode::ZeroPage, true, false, false));
    opcode_table->insert(0xb6,AssyInstruction(0xb6, "LDX", AddressMode::ZeroPageIndexedWithY, true, false, false));
    opcode_table->insert(0xc6,AssyInstruction(0xc6, "DEC", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0xd6,AssyInstruction(0xd6, "DEC", AddressMode::ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0xe6,AssyInstruction(0xe6, "INC", AddressMode::ZeroPage, true, true, false));
    opcode_table->insert(0xf6,AssyInstruction(0xf6, "INC", AddressMode::ZeroPageIndexedWithX, true, true, false));

    opcode_table->insert(0x07,AssyInstruction(0x07, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x17,AssyInstruction(0x17, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x27,AssyInstruction(0x27, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x37,AssyInstruction(0x37, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x47,AssyInstruction(0x47, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x57,AssyInstruction(0x57, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x67,AssyInstruction(0x67, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x77,AssyInstruction(0x77, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x87,AssyInstruction(0x87, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x97,AssyInstruction(0x97, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xa7,AssyInstruction(0xa7, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xb7,AssyInstruction(0xb7, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xc7,AssyInstruction(0xc7, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xd7,AssyInstruction(0xd7, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xe7,AssyInstruction(0xe7, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xf7,AssyInstruction(0xf7, "???", AddressMode::InvalidOp));

    opcode_table->insert(0x08,AssyInstruction(0x08, "PHP", AddressMode::Implied));
    opcode_table->insert(0x18,AssyInstruction(0x18, "CLC", AddressMode::Implied));
    opcode_table->insert(0x28,AssyInstruction(0x28, "PLP", AddressMode::Implied));
    opcode_table->insert(0x38,AssyInstruction(0x38, "SEC", AddressMode::Implied));
    opcode_table->insert(0x48,AssyInstruction(0x48, "PHA", AddressMode::Implied));
    opcode_table->insert(0x58,AssyInstruction(0x58, "CLI", AddressMode::Implied));
    opcode_table->insert(0x68,AssyInstruction(0x68, "PLA", AddressMode::Implied));
    opcode_table->insert(0x78,AssyInstruction(0x78, "SEI", AddressMode::Implied));
    opcode_table->insert(0x88,AssyInstruction(0x88, "DEY", AddressMode::Implied));
    opcode_table->insert(0x98,AssyInstruction(0x98, "TYA", AddressMode::Implied));
    opcode_table->insert(0xa8,AssyInstruction(0xa8, "TAY", AddressMode::Implied));
    opcode_table->insert(0xb8,AssyInstruction(0xb8, "CLV", AddressMode::Implied));
    opcode_table->insert(0xc8,AssyInstruction(0xc8, "INY", AddressMode::Implied));
    opcode_table->insert(0xd8,AssyInstruction(0xd8, "CLD", AddressMode::Implied));
    opcode_table->insert(0xe8,AssyInstruction(0xe8, "INX", AddressMode::Implied));
    opcode_table->insert(0xf8,AssyInstruction(0xf8, "SED", AddressMode::Implied));

    opcode_table->insert(0x09,AssyInstruction(0x09, "ORA", AddressMode::Immediate));
    opcode_table->insert(0x19,AssyInstruction(0x19, "ORA", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x29,AssyInstruction(0x29, "AND", AddressMode::Immediate));
    opcode_table->insert(0x39,AssyInstruction(0x39, "AND", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x49,AssyInstruction(0x49, "EOR", AddressMode::Immediate));
    opcode_table->insert(0x59,AssyInstruction(0x59, "EOR", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x69,AssyInstruction(0x69, "ADC", AddressMode::Immediate));
    opcode_table->insert(0x79,AssyInstruction(0x79, "ADC", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x89,AssyInstruction(0x89, "BIT", AddressMode::Immediate)); //65C02
    opcode_table->insert(0x99,AssyInstruction(0x99, "STA", AddressMode::AbsoluteIndexedWithY, false, true, false));
    opcode_table->insert(0xa9,AssyInstruction(0xa9, "LDA", AddressMode::Immediate));
    opcode_table->insert(0xb9,AssyInstruction(0xb9, "LDA", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xc9,AssyInstruction(0xc9, "CMP", AddressMode::Immediate));
    opcode_table->insert(0xd9,AssyInstruction(0xd9, "CMP", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xe9,AssyInstruction(0xe9, "SBC", AddressMode::Immediate));
    opcode_table->insert(0xf9,AssyInstruction(0xf9, "SBC", AddressMode::AbsoluteIndexedWithY, true, false, false));

    opcode_table->insert(0x0a,AssyInstruction(0x0a, "ASL", AddressMode::Accumulator));
    opcode_table->insert(0x1a,AssyInstruction(0x1a, "INA", AddressMode::Accumulator)); //65C02
    opcode_table->insert(0x2a,AssyInstruction(0x2a, "ROL", AddressMode::Accumulator));
    opcode_table->insert(0x3a,AssyInstruction(0x3a, "DEC", AddressMode::Accumulator)); //65C02
    opcode_table->insert(0x4a,AssyInstruction(0x4a, "LSR", AddressMode::Accumulator));
    opcode_table->insert(0x5a,AssyInstruction(0x5a, "PHY", AddressMode::Implied));//65C02
    opcode_table->insert(0x6a,AssyInstruction(0x6a, "ROR", AddressMode::Accumulator));
    opcode_table->insert(0x7a,AssyInstruction(0x7a, "PLY", AddressMode::Implied));//65C02
    opcode_table->insert(0x8a,AssyInstruction(0x8a, "TXA", AddressMode::Implied));
    opcode_table->insert(0x9a,AssyInstruction(0x9a, "TXS", AddressMode::Implied));
    opcode_table->insert(0xaa,AssyInstruction(0xaa, "TAX", AddressMode::Implied));
    opcode_table->insert(0xba,AssyInstruction(0xba, "TSX", AddressMode::Implied));
    opcode_table->insert(0xca,AssyInstruction(0xca, "DEX", AddressMode::Implied));
    opcode_table->insert(0xda,AssyInstruction(0xda, "PHX", AddressMode::Implied));//65C02
    opcode_table->insert(0xea,AssyInstruction(0xea, "NOP", AddressMode::Implied));
    opcode_table->insert(0xfa,AssyInstruction(0xfa, "PLX", AddressMode::Implied));//65C02

    opcode_table->insert(0x0b,AssyInstruction(0x0b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x1b,AssyInstruction(0x1b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x2b,AssyInstruction(0x2b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x3b,AssyInstruction(0x3b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x4b,AssyInstruction(0x4b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x5b,AssyInstruction(0x5b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x6b,AssyInstruction(0x6b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x7b,AssyInstruction(0x7b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x8b,AssyInstruction(0x8b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x9b,AssyInstruction(0x9b, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xab,AssyInstruction(0xab, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xbb,AssyInstruction(0xbb, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xcb,AssyInstruction(0xcb, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xdb,AssyInstruction(0xdb, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xeb,AssyInstruction(0xeb, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xfb,AssyInstruction(0xfb, "???", AddressMode::InvalidOp));

    opcode_table->insert(0x0c,AssyInstruction(0x0c, "TSB", AddressMode::Absolute, true, true, false)); //65C02
    opcode_table->insert(0x1c,AssyInstruction(0x1c, "TRB", AddressMode::Absolute, true, true, false)); //65C02
    opcode_table->insert(0x2c,AssyInstruction(0x2c, "BIT", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0x3c,AssyInstruction(0x3c, "BIT", AddressMode::AbsoluteIndexedWithX, true, false, false)); //65C02
    opcode_table->insert(0x4c,AssyInstruction(0x4c, "JMP", AddressMode::Absolute));
    opcode_table->insert(0x5c,AssyInstruction(0x5c, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x6c,AssyInstruction(0x6c, "JMP", AddressMode::AbsoluteIndirect, true, false, false));
    opcode_table->insert(0x7c,AssyInstruction(0x7c, "JMP", AddressMode::AbsoluteIndexedIndirect, true, false, false)); //65C02
    opcode_table->insert(0x8c,AssyInstruction(0x8c, "STY", AddressMode::Absolute, false, true, false));
    opcode_table->insert(0x9c,AssyInstruction(0x9c, "STZ", AddressMode::Absolute, false, true, false));
    opcode_table->insert(0xac,AssyInstruction(0xac, "LDY", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xbc,AssyInstruction(0xbc, "LDY", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xcc,AssyInstruction(0xcc, "CPY", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xdc,AssyInstruction(0xdc, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xec,AssyInstruction(0xec, "CPX", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xfc,AssyInstruction(0xfc, "???", AddressMode::InvalidOp));

    opcode_table->insert(0x0d,AssyInstruction(0x0d, "ORA", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0x1d,AssyInstruction(0x1d, "ORA", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x2d,AssyInstruction(0x2d, "AND", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0x3d,AssyInstruction(0x3d, "AND", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x4d,AssyInstruction(0x4d, "EOR", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0x5d,AssyInstruction(0x5d, "EOR", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x6d,AssyInstruction(0x6d, "ADC", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0x7d,AssyInstruction(0x7d, "ADC", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x8d,AssyInstruction(0x8d, "STA", AddressMode::Absolute, false, true, false));
    opcode_table->insert(0x9d,AssyInstruction(0x9d, "STA", AddressMode::AbsoluteIndexedWithX, false, true, false));
    opcode_table->insert(0xad,AssyInstruction(0xad, "LDA", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xbd,AssyInstruction(0xbd, "LDA", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xcd,AssyInstruction(0xcd, "CMP", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xdd,AssyInstruction(0xdd, "CMP", AddressMode::AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xed,AssyInstruction(0xed, "SBC", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xfd,AssyInstruction(0xfd, "SBC", AddressMode::AbsoluteIndexedWithX, true, false, false));

    opcode_table->insert(0x0e,AssyInstruction(0x0e, "ASL", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0x1e,AssyInstruction(0x1e, "ASL", AddressMode::AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x2e,AssyInstruction(0x2e, "ROL", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0x3e,AssyInstruction(0x3e, "ROL", AddressMode::AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x4e,AssyInstruction(0x4e, "LSR", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0x5e,AssyInstruction(0x5e, "LSR", AddressMode::AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x6e,AssyInstruction(0x6e, "ROR", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0x7e,AssyInstruction(0x7e, "ROR", AddressMode::AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x8e,AssyInstruction(0x8e, "STX", AddressMode::Absolute, false, true, false));
    opcode_table->insert(0x9e,AssyInstruction(0x9e, "STZ", AddressMode::AbsoluteIndexedWithX, false, true, false)); //65C02
    opcode_table->insert(0xae,AssyInstruction(0xae, "LDX", AddressMode::Absolute, true, false, false));
    opcode_table->insert(0xbe,AssyInstruction(0xbe, "LDX", AddressMode::AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xce,AssyInstruction(0xce, "DEC", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0xde,AssyInstruction(0xde, "DEC", AddressMode::AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0xee,AssyInstruction(0xee, "INC", AddressMode::Absolute, true, true, false));
    opcode_table->insert(0xfe,AssyInstruction(0xfe, "INC", AddressMode::AbsoluteIndexedWithX, true, true, false));

    opcode_table->insert(0x0f,AssyInstruction(0x0f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x1f,AssyInstruction(0x1f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x2f,AssyInstruction(0x2f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x3f,AssyInstruction(0x3f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x4f,AssyInstruction(0x4f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x5f,AssyInstruction(0x5f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x6f,AssyInstruction(0x6f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x7f,AssyInstruction(0x7f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x8f,AssyInstruction(0x8f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0x9f,AssyInstruction(0x9f, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xaf,AssyInstruction(0xaf, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xbf,AssyInstruction(0xbf, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xcf,AssyInstruction(0xcf, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xdf,AssyInstruction(0xdf, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xef,AssyInstruction(0xef, "???", AddressMode::InvalidOp));
    opcode_table->insert(0xff,AssyInstruction(0xff, "???", AddressMode::InvalidOp));

#ifdef UNUSED_OPCODES
    m_opcodeinfo->insert(0x00,AssyInstruction(0x00, "BRK", AddressMode::Implied));
    m_opcodeinfo->insert(0x10,AssyInstruction(0x10, "BPL", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0x20,AssyInstruction(0x20, "JSR", AddressMode::Absolute));
    m_opcodeinfo->insert(0x30,AssyInstruction(0x30, "BMI", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0x40,AssyInstruction(0x40, "RTI", AddressMode::Implied));
    m_opcodeinfo->insert(0x50,AssyInstruction(0x50, "BVC", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0x60,AssyInstruction(0x60, "RTS", AddressMode::Implied));
    m_opcodeinfo->insert(0x70,AssyInstruction(0x70, "BVS", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0x80,AssyInstruction(0x80, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x90,AssyInstruction(0x90, "BCC", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0xa0,AssyInstruction(0xa0, "LDY", AddressMode::Immediate));
    m_opcodeinfo->insert(0xb0,AssyInstruction(0xb0, "BCC", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0xc0,AssyInstruction(0xc0, "LDY", AddressMode::Immediate));
    m_opcodeinfo->insert(0xd0,AssyInstruction(0xd0, "BCS", AddressMode::ProgramCounterRelative);
    m_opcodeinfo->insert(0xe0,AssyInstruction(0xe0, "CPX", AddressMode::Immediate));
    m_opcodeinfo->insert(0xf0,AssyInstruction(0xf0, "BEQ", AddressMode::ProgramCounterRelative);

    m_opcodeinfo->insert(0x01,AssyInstruction(0x01, "ORA", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x11,AssyInstruction(0x11, "ORA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x21,AssyInstruction(0x21, "AND", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x31,AssyInstruction(0x31, "AND", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x41,AssyInstruction(0x41, "EOR", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x51,AssyInstruction(0x51, "EOR", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x61,AssyInstruction(0x61, "ADC", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x71,AssyInstruction(0x71, "ADC", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x81,AssyInstruction(0x81, "STA", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x91,AssyInstruction(0x91, "STA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa1,AssyInstruction(0xa1, "LDA", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xb1,AssyInstruction(0xb1, "LDA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc1,AssyInstruction(0xc1, "CMP", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xd1,AssyInstruction(0xd1, "CMP", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe1,AssyInstruction(0xe1, "SBC", AddressMode::AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xf1,AssyInstruction(0xff, "SBC", AddressMode::AbsoluteIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x02,AssyInstruction(0x02, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x12,AssyInstruction(0x12, "asl-ora", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x22,AssyInstruction(0x22, "and", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x32,AssyInstruction(0x32, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x42,AssyInstruction(0x42, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x52,AssyInstruction(0x52, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x62,AssyInstruction(0x62, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x72,AssyInstruction(0x72, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x82,AssyInstruction(0x82, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0x92,AssyInstruction(0x92, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0xa2,AssyInstruction(0xa2, "LDX", AddressMode::Immediate));
    m_opcodeinfo->insert(0xb2,AssyInstruction(0xb2, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0xc2,AssyInstruction(0xc2, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0xd2,AssyInstruction(0xd2, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0xe2,AssyInstruction(0xe2, "halt", AddressMode::Immediate));
    m_opcodeinfo->insert(0xf2,AssyInstruction(0xf2, "halt", AddressMode::Immediate));

    m_opcodeinfo->insert(0x03,AssyInstruction(0x03, "asl/ora", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x13,AssyInstruction(0x13, "asl/ora", AddressMode::ZeroPageIndirect, false, false, false));
    m_opcodeinfo->insert(0x23,AssyInstruction(0x23, "rol/and", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x33,AssyInstruction(0x33, "rol/and", AddressMode::ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x43,AssyInstruction(0x43, "lsr/eor", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x53,AssyInstruction(0x53, "lsr/eor", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x63,AssyInstruction(0x63, "ror/adc", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x73,AssyInstruction(0x73, "ror/adc", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x83,AssyInstruction(0x83, "sta/stx", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x93,AssyInstruction(0x93, "sta/stx", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa3,AssyInstruction(0xa3, "lda/ldx", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xb3,AssyInstruction(0xb3, "lda/ldx", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc3,AssyInstruction(0xc3, "dec/cmp", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xd3,AssyInstruction(0xd3, "dec/cmp", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe3,AssyInstruction(0xe3, "inc/sbc", AddressMode::ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xf3,AssyInstruction(0xf3, "inc/sbc", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x04,AssyInstruction(0x04, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x14,AssyInstruction(0x14, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x24,AssyInstruction(0x24, "BIT", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x34,AssyInstruction(0x34, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x44,AssyInstruction(0x44, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x54,AssyInstruction(0x54, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x64,AssyInstruction(0x64, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x74,AssyInstruction(0x74, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x84,AssyInstruction(0x84, "STY", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x94,AssyInstruction(0x94, "STY", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xa4,AssyInstruction(0xa4, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb4,AssyInstruction(0xb4, "LDY", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xc4,AssyInstruction(0xc4, "CPY", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd4,AssyInstruction(0xd4, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xe4,AssyInstruction(0xe4, "CPX", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf4,AssyInstruction(0xf4, "nop", AddressMode::ZeroPage, false, false, false));

    m_opcodeinfo->insert(0x05,AssyInstruction(0x05, "ORA", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x15,AssyInstruction(0x15, "ORA", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x25,AssyInstruction(0x25, "AND", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x35,AssyInstruction(0x35, "AND", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x45,AssyInstruction(0x45, "EOR", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x55,AssyInstruction(0x55, "EOR", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x65,AssyInstruction(0x65, "ADC", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x75,AssyInstruction(0x75, "ADC", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x85,AssyInstruction(0x85, "STA", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x95,AssyInstruction(0x95, "STA", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xa5,AssyInstruction(0xa5, "LDA", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb5,AssyInstruction(0xb5, "LDA", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xc5,AssyInstruction(0xc5, "CMP", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd5,AssyInstruction(0xd5, "CMP", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe5,AssyInstruction(0xe5, "SEC", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf5,AssyInstruction(0xf5, "SEC", AddressMode::ZeroPageIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x06,AssyInstruction(0x06, "ASL", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x16,AssyInstruction(0x16, "ASL", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x26,AssyInstruction(0x26, "ROL", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x36,AssyInstruction(0x36, "ROL", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x46,AssyInstruction(0x46, "LSR", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x56,AssyInstruction(0x56, "LSR", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x66,AssyInstruction(0x66, "ROR", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x76,AssyInstruction(0x76, "ROR", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x86,AssyInstruction(0x86, "STX", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x96,AssyInstruction(0x96, "STX", AddressMode::ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa6,AssyInstruction(0xa6, "LDX", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb6,AssyInstruction(0xb6, "LDX", AddressMode::ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc6,AssyInstruction(0xc6, "DEC", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd6,AssyInstruction(0xd6, "DEC", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe6,AssyInstruction(0xe6, "INC", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf6,AssyInstruction(0xf6, "INC", AddressMode::ZeroPageIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x07,AssyInstruction(0x07, "asl/ora", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x17,AssyInstruction(0x17, "asl/ora", AddressMode::ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x27,AssyInstruction(0x27, "rol/and", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x37,AssyInstruction(0x37, "rol/and", AddressMode::ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x47,AssyInstruction(0x47, "lsr/eor", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x57,AssyInstruction(0x57, "lsr/eor", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x67,AssyInstruction(0x67, "ror/adc", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x77,AssyInstruction(0x77, "ror/adc", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x87,AssyInstruction(0x87, "sta/stx", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x97,AssyInstruction(0x97, "sta/stx", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa7,AssyInstruction(0xa7, "lda/ldx", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb7,AssyInstruction(0xb7, "ldx/ldx", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc7,AssyInstruction(0xc7, "dec/cmp", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd7,AssyInstruction(0xd7, "dec/cmp", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe7,AssyInstruction(0xe7, "inc/sbc", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf7,AssyInstruction(0xf7, "inc/sbc", AddressMode::AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x08,AssyInstruction(0x08, "PHP", AddressMode::Implied));
    m_opcodeinfo->insert(0x18,AssyInstruction(0x18, "CLC", AddressMode::Implied));
    m_opcodeinfo->insert(0x28,AssyInstruction(0x28, "PLP", AddressMode::Implied));
    m_opcodeinfo->insert(0x38,AssyInstruction(0x38, "SEC", AddressMode::Implied));
    m_opcodeinfo->insert(0x48,AssyInstruction(0x48, "PHA", AddressMode::Implied));
    m_opcodeinfo->insert(0x58,AssyInstruction(0x58, "CLI", AddressMode::Implied));
    m_opcodeinfo->insert(0x68,AssyInstruction(0x68, "PLA", AddressMode::Implied));
    m_opcodeinfo->insert(0x78,AssyInstruction(0x78, "SEI", AddressMode::Implied));
    m_opcodeinfo->insert(0x88,AssyInstruction(0x88, "DEY", AddressMode::Implied));
    m_opcodeinfo->insert(0x98,AssyInstruction(0x98, "TYA", AddressMode::Implied));
    m_opcodeinfo->insert(0xa8,AssyInstruction(0xa8, "TAY", AddressMode::Implied));
    m_opcodeinfo->insert(0xb8,AssyInstruction(0xb8, "CLV", AddressMode::Implied));
    m_opcodeinfo->insert(0xc8,AssyInstruction(0xc8, "INY", AddressMode::Implied));
    m_opcodeinfo->insert(0xd8,AssyInstruction(0xd8, "CLD", AddressMode::Implied));
    m_opcodeinfo->insert(0xe8,AssyInstruction(0xe8, "INX", AddressMode::Implied));
    m_opcodeinfo->insert(0xf8,AssyInstruction(0xf8, "SED", AddressMode::Implied));

    m_opcodeinfo->insert(0x09,AssyInstruction(0x09, "ORA", AddressMode::Immediate));
    m_opcodeinfo->insert(0x19,AssyInstruction(0x19, "ORA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x29,AssyInstruction(0x29, "AND", AddressMode::Immediate));
    m_opcodeinfo->insert(0x39,AssyInstruction(0x39, "AND", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x49,AssyInstruction(0x49, "EOR", AddressMode::Immediate));
    m_opcodeinfo->insert(0x59,AssyInstruction(0x59, "EOR", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x69,AssyInstruction(0x69, "ADC", AddressMode::Immediate));
    m_opcodeinfo->insert(0x79,AssyInstruction(0x79, "ADC", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x89,AssyInstruction(0x89, "nop", AddressMode::ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x99,AssyInstruction(0x99, "STA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa9,AssyInstruction(0xa9, "LDA", AddressMode::Immediate));
    m_opcodeinfo->insert(0xb9,AssyInstruction(0xb9, "LDA", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc9,AssyInstruction(0xc9, "CMP", AddressMode::Immediate));
    m_opcodeinfo->insert(0xd9,AssyInstruction(0xd9, "CMP", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe9,AssyInstruction(0xe9, "SBC", AddressMode::Immediate));
    m_opcodeinfo->insert(0xf9,AssyInstruction(0xf9, "SBC", AddressMode::AbsoluteIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x0a,AssyInstruction(0x0a, "ASL", AddressMode::Accumulator));
    m_opcodeinfo->insert(0x1a,AssyInstruction(0x1a, "nop", AddressMode::Implied));
    m_opcodeinfo->insert(0x2a,AssyInstruction(0x2a, "ROL", AddressMode::Accumulator));
    m_opcodeinfo->insert(0x3a,AssyInstruction(0x3a, "nop", AddressMode::Implied));
    m_opcodeinfo->insert(0x4a,AssyInstruction(0x4a, "LSR", AddressMode::Accumulator));
    m_opcodeinfo->insert(0x5a,AssyInstruction(0x5a, "nop", AddressMode::Implied));
    m_opcodeinfo->insert(0x6a,AssyInstruction(0x6a, "ROR", AddressMode::Accumulator));
    m_opcodeinfo->insert(0x7a,AssyInstruction(0x7a, "nop", AddressMode::Implied));
    m_opcodeinfo->insert(0x8a,AssyInstruction(0x8a, "TXA", AddressMode::Implied));
    m_opcodeinfo->insert(0x9a,AssyInstruction(0x9a, "TXS", AddressMode::Implied));
    m_opcodeinfo->insert(0xaa,AssyInstruction(0xaa, "TAX", AddressMode::Implied));
    m_opcodeinfo->insert(0xba,AssyInstruction(0xba, "TSX", AddressMode::Implied));
    m_opcodeinfo->insert(0xca,AssyInstruction(0xca, "DEX", AddressMode::Implied));
    m_opcodeinfo->insert(0xda,AssyInstruction(0xda, "nop", AddressMode::Implied));
    m_opcodeinfo->insert(0xea,AssyInstruction(0xea, "NOP", AddressMode::Implied));
    m_opcodeinfo->insert(0xfa,AssyInstruction(0xfa, "nop", AddressMode::Implied));

    m_opcodeinfo->insert(0x0b,AssyInstruction(0x0b, "and/mov bit7->Cy", AddressMode::Immediate));
    m_opcodeinfo->insert(0x1b,AssyInstruction(0x1b, "asl/ora", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x2b,AssyInstruction(0x2b, "and/mov bit7->Cy", AddressMode::Immediate));
    m_opcodeinfo->insert(0x3b,AssyInstruction(0x3b, "asl/ora", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x4b,AssyInstruction(0x4b, "and/lsr A", AddressMode::Immediate));
    m_opcodeinfo->insert(0x5b,AssyInstruction(0x5b, "lsr/eor", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6b,AssyInstruction(0x6b, "and/ror A", AddressMode::Immediate));
    m_opcodeinfo->insert(0x7b,AssyInstruction(0x7b, "ror/adc", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x8b,AssyInstruction(0x8b, "txa/and", AddressMode::Immediate));
    m_opcodeinfo->insert(0x9b,AssyInstruction(0x9b, "sta/stx", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xab,AssyInstruction(0xab, "lda/ldx", AddressMode::Implied));
    m_opcodeinfo->insert(0xbb,AssyInstruction(0xbb, "lda/ldx", AddressMode::Implied));
    m_opcodeinfo->insert(0xcb,AssyInstruction(0xcb, "sbx", AddressMode::Immediate));
    m_opcodeinfo->insert(0xdb,AssyInstruction(0xdb, "dec/cmp", AddressMode::Absolute));
    m_opcodeinfo->insert(0xeb,AssyInstruction(0xeb, "sbc", AddressMode::Immediate));
    m_opcodeinfo->insert(0xfb,AssyInstruction(0xfb, "inc/sbc", AddressMode::Absolute));

    m_opcodeinfo->insert(0x0c,AssyInstruction(0x0c, "nop", AddressMode::Absolute));
    m_opcodeinfo->insert(0x1c,AssyInstruction(0x1c, "nop", AddressMode::Absolute));
    m_opcodeinfo->insert(0x2c,AssyInstruction(0x2c, "BIT", AddressMode::Absolute));
    m_opcodeinfo->insert(0x3c,AssyInstruction(0x3c, "nop", AddressMode::Absolute));
    m_opcodeinfo->insert(0x4c,AssyInstruction(0x4c, "JMP", AddressMode::Absolute));
    m_opcodeinfo->insert(0x5c,AssyInstruction(0x5c, "nop", AddressMode::Absolute));
    m_opcodeinfo->insert(0x6c,AssyInstruction(0x6c, "JMP", AddressMode::AbsoluteIndirect, false, false, false));
    m_opcodeinfo->insert(0x7c,AssyInstruction(0x7c, "nop", AddressMode::Absolute));
    m_opcodeinfo->insert(0x8c,AssyInstruction(0x8c, "STY", AddressMode::Absolute));
    m_opcodeinfo->insert(0x9c,AssyInstruction(0x9c, "sta/stx", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xac,AssyInstruction(0xac, "LDY", AddressMode::Absolute));
    m_opcodeinfo->insert(0xbc,AssyInstruction(0xbc, "LDY", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xcc,AssyInstruction(0xcc, "CPY", AddressMode::Absolute));
    m_opcodeinfo->insert(0xdc,AssyInstruction(0xdc, "???", AddressMode::InvalidOp));
    m_opcodeinfo->insert(0xec,AssyInstruction(0xec, "CPX", AddressMode::Absolute));
    m_opcodeinfo->insert(0xfc,AssyInstruction(0xfc, "???", AddressMode::InvalidOp));

    m_opcodeinfo->insert(0x0d,AssyInstruction(0x0d, "ORA", AddressMode::Absolute));
    m_opcodeinfo->insert(0x1d,AssyInstruction(0x1d, "ORA", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2d,AssyInstruction(0x2d, "AND", AddressMode::Absolute));
    m_opcodeinfo->insert(0x3d,AssyInstruction(0x3d, "AND", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4d,AssyInstruction(0x4d, "EOR", AddressMode::Absolute));
    m_opcodeinfo->insert(0x5d,AssyInstruction(0x5d, "EOR", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6d,AssyInstruction(0x6d, "ADC", AddressMode::Absolute));
    m_opcodeinfo->insert(0x7d,AssyInstruction(0x7d, "ADC", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8d,AssyInstruction(0x8d, "STA", AddressMode::Absolute));
    m_opcodeinfo->insert(0x9d,AssyInstruction(0x9d, "STA", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xad,AssyInstruction(0xad, "LDA", AddressMode::Absolute));
    m_opcodeinfo->insert(0xbd,AssyInstruction(0xbd, "LDA", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xcd,AssyInstruction(0xcd, "CMP", AddressMode::Absolute));
    m_opcodeinfo->insert(0xdd,AssyInstruction(0xdd, "CMP", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xed,AssyInstruction(0xed, "SBC", AddressMode::Absolute));
    m_opcodeinfo->insert(0xfd,AssyInstruction(0xfd, "SBC", AddressMode::AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x0e,AssyInstruction(0x0e, "ASL", AddressMode::Absolute));
    m_opcodeinfo->insert(0x1e,AssyInstruction(0x1e, "ASL", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2e,AssyInstruction(0x2e, "ROL", AddressMode::Absolute));
    m_opcodeinfo->insert(0x3e,AssyInstruction(0x3e, "ROL", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4e,AssyInstruction(0x4e, "LSR", AddressMode::Absolute));
    m_opcodeinfo->insert(0x5e,AssyInstruction(0x5e, "LSR", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6e,AssyInstruction(0x6e, "ROR", AddressMode::Absolute));
    m_opcodeinfo->insert(0x7e,AssyInstruction(0x7e, "ROR", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8e,AssyInstruction(0x8e, "STX", AddressMode::Absolute));
    m_opcodeinfo->insert(0x9e,AssyInstruction(0x9e, "sta/stx", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xae,AssyInstruction(0xae, "LDX", AddressMode::Absolute));
    m_opcodeinfo->insert(0xbe,AssyInstruction(0xbe, "LDX", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xce,AssyInstruction(0xce, "DEC", AddressMode::Absolute));
    m_opcodeinfo->insert(0xde,AssyInstruction(0xde, "DEC", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xee,AssyInstruction(0xee, "INC", AddressMode::Absolute));
    m_opcodeinfo->insert(0xfe,AssyInstruction(0xfe, "INC", AddressMode::AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x0f,AssyInstruction(0x0f, "asl/ora", AddressMode::Absolute));
    m_opcodeinfo->insert(0x1f,AssyInstruction(0x1f, "asl/ora", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2f,AssyInstruction(0x2f, "rol/and", AddressMode::Absolute));
    m_opcodeinfo->insert(0x3f,AssyInstruction(0x3f, "rol/and", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4f,AssyInstruction(0x4f, "lsr/eor", AddressMode::Absolute));
    m_opcodeinfo->insert(0x5f,AssyInstruction(0x5f, "lsr/eor", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6f,AssyInstruction(0x6f, "ror/adc", AddressMode::Absolute));
    m_opcodeinfo->insert(0x7f,AssyInstruction(0x7f, "ror/adc", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8f,AssyInstruction(0x8f, "sta/stx", AddressMode::Absolute));
    m_opcodeinfo->insert(0x9f,AssyInstruction(0x9f, "sta/stx", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xaf,AssyInstruction(0xaf, "lda/ldx", AddressMode::Absolute));
    m_opcodeinfo->insert(0xbf,AssyInstruction(0xbf, "ldx/ldx", AddressMode::AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xcf,AssyInstruction(0xcf, "dec/cmp", AddressMode::Absolute));
    m_opcodeinfo->insert(0xdf,AssyInstruction(0xdf, "dec/cmp", AddressMode::AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xef,AssyInstruction(0xef, "inc/sbc", AddressMode::Absolute));
    m_opcodeinfo->insert(0xff,AssyInstruction(0xff, "inc/sbc", AddressMode::AbsoluteIndexedWithX, false, false, false));
#endif
}



quint8 OpCodes::AssyInstruction::numArgs() const noexcept
{
    switch (m_addressMode) {
        case AddressMode::Absolute:
        case AddressMode::AbsoluteIndexedIndirect:
        case AddressMode::AbsoluteIndexedWithX:
        case AddressMode::AbsoluteIndexedWithY:
        case AddressMode::AbsoluteIndirect:
            return 2;
        case AddressMode::ProgramCounterRelative:
        case AddressMode::ZeroPage:
        case AddressMode::ZeroPageIndirectIndexedWithY:
        case AddressMode::ZeroPageIndexedIndirect:
        case AddressMode::ZeroPageIndexedWithX:
        case AddressMode::ZeroPageIndexedWithY:
        case AddressMode::ZeroPageIndirect:
        case AddressMode::Immediate:
            return 1;
        case AddressMode::InvalidOp:
        case AddressMode::Implied:
        case AddressMode::Accumulator:
        default:
            return 0;
    }
}
