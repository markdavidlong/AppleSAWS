#include "opcodes.h"

OpCodes::OpCodes()
{
}

AddressMode OpCodes::addressMode(quint8 opcode)
{
    return getAssyInstruction(opcode).addressMode();
}

QString OpCodes::mnemonic(quint8 opcode)
{
    return getAssyInstruction(opcode).mnemonic();
}

quint8 OpCodes::numArgs(quint8 opcode)
{
    return getAssyInstruction(opcode).numArgs();
}

bool OpCodes::isJump(quint8 id)
{
    if (id == 0x80) return true;  // BRA
    if (id == 0x4C) return true;  // JMP a
    if (id == 0x6C) return true;  // JMP (a)
    if (id == 0x7C) return true;  // JMP (a,x);
    return false;
}

bool OpCodes::isIndirectJump(quint8 id)
{
    if (id == 0x6C) return true;  // JMP (a)
    if (id == 0x7C) return true;  // JMP (a,x);
    return false;
}

bool OpCodes::isBranch(quint8 id)
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

bool OpCodes::isJsr(quint8 id)
{
    return (id == 0x20); // JSR
}

bool OpCodes::isReturn(quint8 id)
{
    if (id == 0x40) return true;  // RTI
    if (id == 0x60) return true;  // RTS
    return false;
}

bool OpCodes::isBreak(quint8 id)
{
    return (id = 0x00);  // BRK
}

bool OpCodes::readsMemoryLoc(quint8 opcode)
{
    return getAssyInstruction(opcode).readsMem();
}

bool OpCodes::writesMemoryLoc(quint8 opcode)
{
    return getAssyInstruction(opcode).writesMem();
}

bool OpCodes::modifiesStack(quint8 opcode)
{
    return getAssyInstruction(opcode).modifiesStack();
}


void OpCodes::makeOpcodeTable(QHash<quint8,AssyInstruction>* opcode_table)
{
    opcode_table->insert(0x00,AssyInstruction(0x00, "BRK", AM_Implied));
    opcode_table->insert(0x10,AssyInstruction(0x10, "BPL", AM_ProgramCounterRelative));
    opcode_table->insert(0x20,AssyInstruction(0x20, "JSR", AM_Absolute, false, false, true));
    opcode_table->insert(0x30,AssyInstruction(0x30, "BMI", AM_ProgramCounterRelative));
    opcode_table->insert(0x40,AssyInstruction(0x40, "RTI", AM_Implied, false, false, true));
    opcode_table->insert(0x50,AssyInstruction(0x50, "BVC", AM_ProgramCounterRelative));
    opcode_table->insert(0x60,AssyInstruction(0x60, "RTS", AM_Implied, false, false, true));
    opcode_table->insert(0x70,AssyInstruction(0x70, "BVS", AM_ProgramCounterRelative));
    opcode_table->insert(0x80,AssyInstruction(0x80, "BRA", AM_ProgramCounterRelative)); //65C02
    opcode_table->insert(0x90,AssyInstruction(0x90, "BCC", AM_ProgramCounterRelative));
    opcode_table->insert(0xa0,AssyInstruction(0xa0, "LDY", AM_Immediate));
    opcode_table->insert(0xb0,AssyInstruction(0xb0, "BCS", AM_ProgramCounterRelative));
    opcode_table->insert(0xc0,AssyInstruction(0xc0, "CPY", AM_Immediate));
    opcode_table->insert(0xd0,AssyInstruction(0xd0, "BNE", AM_ProgramCounterRelative));
    opcode_table->insert(0xe0,AssyInstruction(0xe0, "CPX", AM_Immediate));
    opcode_table->insert(0xf0,AssyInstruction(0xf0, "BEQ", AM_ProgramCounterRelative));

    opcode_table->insert(0x01,AssyInstruction(0x01, "ORA", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x11,AssyInstruction(0x11, "ORA", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x21,AssyInstruction(0x21, "AND", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x31,AssyInstruction(0x31, "AND", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x41,AssyInstruction(0x41, "EOR", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x51,AssyInstruction(0x51, "EOR", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x61,AssyInstruction(0x61, "ADC", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0x71,AssyInstruction(0x71, "ADC", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0x81,AssyInstruction(0x81, "STA", AM_ZeroPageIndexedIndirect, false, true, false));
    opcode_table->insert(0x91,AssyInstruction(0x91, "STA", AM_ZeroPageIndirectIndexedWithY, false, true, false));
    opcode_table->insert(0xa1,AssyInstruction(0xa1, "LDA", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xb1,AssyInstruction(0xb1, "LDA", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0xc1,AssyInstruction(0xc1, "CMP", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xd1,AssyInstruction(0xd1, "CMP", AM_ZeroPageIndirectIndexedWithY, true, false, false));
    opcode_table->insert(0xe1,AssyInstruction(0xe1, "SBC", AM_ZeroPageIndexedIndirect, true, false, false));
    opcode_table->insert(0xf1,AssyInstruction(0xff, "SBC", AM_ZeroPageIndirectIndexedWithY, true, false, false));

    opcode_table->insert(0x02,AssyInstruction(0x02, "???", AM_InvalidOp));
    opcode_table->insert(0x12,AssyInstruction(0x12, "ORA", AM_ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x22,AssyInstruction(0x22, "???", AM_InvalidOp));
    opcode_table->insert(0x32,AssyInstruction(0x32, "AND", AM_ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x42,AssyInstruction(0x42, "???", AM_InvalidOp));
    opcode_table->insert(0x52,AssyInstruction(0x52, "EOR", AM_ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x62,AssyInstruction(0x62, "???", AM_InvalidOp));
    opcode_table->insert(0x72,AssyInstruction(0x72, "ADC", AM_ZeroPageIndirect, true, false, false));
    opcode_table->insert(0x82,AssyInstruction(0x82, "???", AM_InvalidOp));
    opcode_table->insert(0x92,AssyInstruction(0x92, "STA", AM_ZeroPageIndirect, false, true, false)); //65C02
    opcode_table->insert(0xa2,AssyInstruction(0xa2, "LDX", AM_Immediate));
    opcode_table->insert(0xb2,AssyInstruction(0xb2, "LDA", AM_ZeroPageIndirect, true, false, false)); //65C02
    opcode_table->insert(0xc2,AssyInstruction(0xc2, "???", AM_InvalidOp));
    opcode_table->insert(0xd2,AssyInstruction(0xd2, "CMP", AM_ZeroPageIndirect, true, false, false));
    opcode_table->insert(0xe2,AssyInstruction(0xe2, "???", AM_InvalidOp));
    opcode_table->insert(0xf2,AssyInstruction(0xf2, "SBC", AM_ZeroPageIndirect, true, false, false));

    opcode_table->insert(0x03,AssyInstruction(0x03, "???", AM_InvalidOp));
    opcode_table->insert(0x13,AssyInstruction(0x13, "???", AM_InvalidOp));
    opcode_table->insert(0x23,AssyInstruction(0x23, "???", AM_InvalidOp));
    opcode_table->insert(0x33,AssyInstruction(0x33, "???", AM_InvalidOp));
    opcode_table->insert(0x43,AssyInstruction(0x43, "???", AM_InvalidOp));
    opcode_table->insert(0x53,AssyInstruction(0x53, "???", AM_InvalidOp));
    opcode_table->insert(0x63,AssyInstruction(0x63, "???", AM_InvalidOp));
    opcode_table->insert(0x73,AssyInstruction(0x73, "???", AM_InvalidOp));
    opcode_table->insert(0x83,AssyInstruction(0x83, "???", AM_InvalidOp));
    opcode_table->insert(0x93,AssyInstruction(0x93, "???", AM_InvalidOp));
    opcode_table->insert(0xa3,AssyInstruction(0xa3, "???", AM_InvalidOp));
    opcode_table->insert(0xb3,AssyInstruction(0xb3, "???", AM_InvalidOp));
    opcode_table->insert(0xc3,AssyInstruction(0xc3, "???", AM_InvalidOp));
    opcode_table->insert(0xd3,AssyInstruction(0xd3, "???", AM_InvalidOp));
    opcode_table->insert(0xe3,AssyInstruction(0xe3, "???", AM_InvalidOp));
    opcode_table->insert(0xf3,AssyInstruction(0xf3, "???", AM_InvalidOp));

    opcode_table->insert(0x04,AssyInstruction(0x04, "TSB", AM_ZeroPage, true, false, false)); //65C02
    opcode_table->insert(0x14,AssyInstruction(0x14, "TRB", AM_ZeroPage, true, false, false)); //65C02
    opcode_table->insert(0x24,AssyInstruction(0x24, "BIT", AM_ZeroPage, true, false, false));
    opcode_table->insert(0x34,AssyInstruction(0x34, "BIT", AM_ZeroPageIndexedWithX, true, false, false)); //65C02
    opcode_table->insert(0x44,AssyInstruction(0x44, "???", AM_InvalidOp));
    opcode_table->insert(0x54,AssyInstruction(0x54, "???", AM_InvalidOp));
    opcode_table->insert(0x64,AssyInstruction(0x64, "STZ", AM_ZeroPage, false, true, false));  //65C02
    opcode_table->insert(0x74,AssyInstruction(0x74, "STZ", AM_ZeroPageIndexedWithX, false, true, false)); //65C02
    opcode_table->insert(0x84,AssyInstruction(0x84, "STY", AM_ZeroPage, false, true, false));
    opcode_table->insert(0x94,AssyInstruction(0x94, "STY", AM_ZeroPageIndexedWithX, false, true, false));
    opcode_table->insert(0xa4,AssyInstruction(0xa4, "LDY", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xb4,AssyInstruction(0xb4, "LDY", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xc4,AssyInstruction(0xc4, "CPY", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xd4,AssyInstruction(0xd4, "???", AM_InvalidOp));
    opcode_table->insert(0xe4,AssyInstruction(0xe4, "CPX", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xf4,AssyInstruction(0xf4, "???", AM_InvalidOp));

    opcode_table->insert(0x05,AssyInstruction(0x05, "ORA", AM_ZeroPage, true, false, false));
    opcode_table->insert(0x15,AssyInstruction(0x15, "ORA", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x25,AssyInstruction(0x25, "AND", AM_ZeroPage, true, false, false));
    opcode_table->insert(0x35,AssyInstruction(0x35, "AND", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x45,AssyInstruction(0x45, "EOR", AM_ZeroPage, true, false, false));
    opcode_table->insert(0x55,AssyInstruction(0x55, "EOR", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x65,AssyInstruction(0x65, "ADC", AM_ZeroPage, true, false, false));
    opcode_table->insert(0x75,AssyInstruction(0x75, "ADC", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0x85,AssyInstruction(0x85, "STA", AM_ZeroPage, false, true, false));
    opcode_table->insert(0x95,AssyInstruction(0x95, "STA", AM_ZeroPageIndexedWithX, false, true, false));
    opcode_table->insert(0xa5,AssyInstruction(0xa5, "LDA", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xb5,AssyInstruction(0xb5, "LDA", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xc5,AssyInstruction(0xc5, "CMP", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xd5,AssyInstruction(0xd5, "CMP", AM_ZeroPageIndexedWithX, true, false, false));
    opcode_table->insert(0xe5,AssyInstruction(0xe5, "SBC", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xf5,AssyInstruction(0xf5, "SBC", AM_ZeroPageIndexedWithX, true, false, false));

    opcode_table->insert(0x06,AssyInstruction(0x06, "ASL", AM_ZeroPage, true, true, false));
    opcode_table->insert(0x16,AssyInstruction(0x16, "ASL", AM_ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x26,AssyInstruction(0x26, "ROL", AM_ZeroPage, true, true, false));
    opcode_table->insert(0x36,AssyInstruction(0x36, "ROL", AM_ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x46,AssyInstruction(0x46, "LSR", AM_ZeroPage, true, true, false));
    opcode_table->insert(0x56,AssyInstruction(0x56, "LSR", AM_ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x66,AssyInstruction(0x66, "ROR", AM_ZeroPage, true, true, false));
    opcode_table->insert(0x76,AssyInstruction(0x76, "ROR", AM_ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0x86,AssyInstruction(0x86, "STX", AM_ZeroPage, false, true, false));
    opcode_table->insert(0x96,AssyInstruction(0x96, "STX", AM_ZeroPageIndexedWithY, false, true, false));
    opcode_table->insert(0xa6,AssyInstruction(0xa6, "LDX", AM_ZeroPage, true, false, false));
    opcode_table->insert(0xb6,AssyInstruction(0xb6, "LDX", AM_ZeroPageIndexedWithY, true, false, false));
    opcode_table->insert(0xc6,AssyInstruction(0xc6, "DEC", AM_ZeroPage, true, true, false));
    opcode_table->insert(0xd6,AssyInstruction(0xd6, "DEC", AM_ZeroPageIndexedWithX, true, true, false));
    opcode_table->insert(0xe6,AssyInstruction(0xe6, "INC", AM_ZeroPage, true, true, false));
    opcode_table->insert(0xf6,AssyInstruction(0xf6, "INC", AM_ZeroPageIndexedWithX, true, true, false));

    opcode_table->insert(0x07,AssyInstruction(0x07, "???", AM_InvalidOp));
    opcode_table->insert(0x17,AssyInstruction(0x17, "???", AM_InvalidOp));
    opcode_table->insert(0x27,AssyInstruction(0x27, "???", AM_InvalidOp));
    opcode_table->insert(0x37,AssyInstruction(0x37, "???", AM_InvalidOp));
    opcode_table->insert(0x47,AssyInstruction(0x47, "???", AM_InvalidOp));
    opcode_table->insert(0x57,AssyInstruction(0x57, "???", AM_InvalidOp));
    opcode_table->insert(0x67,AssyInstruction(0x67, "???", AM_InvalidOp));
    opcode_table->insert(0x77,AssyInstruction(0x77, "???", AM_InvalidOp));
    opcode_table->insert(0x87,AssyInstruction(0x87, "???", AM_InvalidOp));
    opcode_table->insert(0x97,AssyInstruction(0x97, "???", AM_InvalidOp));
    opcode_table->insert(0xa7,AssyInstruction(0xa7, "???", AM_InvalidOp));
    opcode_table->insert(0xb7,AssyInstruction(0xb7, "???", AM_InvalidOp));
    opcode_table->insert(0xc7,AssyInstruction(0xc7, "???", AM_InvalidOp));
    opcode_table->insert(0xd7,AssyInstruction(0xd7, "???", AM_InvalidOp));
    opcode_table->insert(0xe7,AssyInstruction(0xe7, "???", AM_InvalidOp));
    opcode_table->insert(0xf7,AssyInstruction(0xf7, "???", AM_InvalidOp));

    opcode_table->insert(0x08,AssyInstruction(0x08, "PHP", AM_Implied));
    opcode_table->insert(0x18,AssyInstruction(0x18, "CLC", AM_Implied));
    opcode_table->insert(0x28,AssyInstruction(0x28, "PLP", AM_Implied));
    opcode_table->insert(0x38,AssyInstruction(0x38, "SEC", AM_Implied));
    opcode_table->insert(0x48,AssyInstruction(0x48, "PHA", AM_Implied));
    opcode_table->insert(0x58,AssyInstruction(0x58, "CLI", AM_Implied));
    opcode_table->insert(0x68,AssyInstruction(0x68, "PLA", AM_Implied));
    opcode_table->insert(0x78,AssyInstruction(0x78, "SEI", AM_Implied));
    opcode_table->insert(0x88,AssyInstruction(0x88, "DEY", AM_Implied));
    opcode_table->insert(0x98,AssyInstruction(0x98, "TYA", AM_Implied));
    opcode_table->insert(0xa8,AssyInstruction(0xa8, "TAY", AM_Implied));
    opcode_table->insert(0xb8,AssyInstruction(0xb8, "CLV", AM_Implied));
    opcode_table->insert(0xc8,AssyInstruction(0xc8, "INY", AM_Implied));
    opcode_table->insert(0xd8,AssyInstruction(0xd8, "CLD", AM_Implied));
    opcode_table->insert(0xe8,AssyInstruction(0xe8, "INX", AM_Implied));
    opcode_table->insert(0xf8,AssyInstruction(0xf8, "SED", AM_Implied));

    opcode_table->insert(0x09,AssyInstruction(0x09, "ORA", AM_Immediate));
    opcode_table->insert(0x19,AssyInstruction(0x19, "ORA", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x29,AssyInstruction(0x29, "AND", AM_Immediate));
    opcode_table->insert(0x39,AssyInstruction(0x39, "AND", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x49,AssyInstruction(0x49, "EOR", AM_Immediate));
    opcode_table->insert(0x59,AssyInstruction(0x59, "EOR", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x69,AssyInstruction(0x69, "ADC", AM_Immediate));
    opcode_table->insert(0x79,AssyInstruction(0x79, "ADC", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0x89,AssyInstruction(0x89, "BIT", AM_Immediate)); //65C02
    opcode_table->insert(0x99,AssyInstruction(0x99, "STA", AM_AbsoluteIndexedWithY, false, true, false));
    opcode_table->insert(0xa9,AssyInstruction(0xa9, "LDA", AM_Immediate));
    opcode_table->insert(0xb9,AssyInstruction(0xb9, "LDA", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xc9,AssyInstruction(0xc9, "CMP", AM_Immediate));
    opcode_table->insert(0xd9,AssyInstruction(0xd9, "CMP", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xe9,AssyInstruction(0xe9, "SBC", AM_Immediate));
    opcode_table->insert(0xf9,AssyInstruction(0xf9, "SBC", AM_AbsoluteIndexedWithY, true, false, false));

    opcode_table->insert(0x0a,AssyInstruction(0x0a, "ASL", AM_Accumulator));
    opcode_table->insert(0x1a,AssyInstruction(0x1a, "INA", AM_Accumulator)); //65C02
    opcode_table->insert(0x2a,AssyInstruction(0x2a, "ROL", AM_Accumulator));
    opcode_table->insert(0x3a,AssyInstruction(0x3a, "DEC", AM_Accumulator)); //65C02
    opcode_table->insert(0x4a,AssyInstruction(0x4a, "LSR", AM_Accumulator));
    opcode_table->insert(0x5a,AssyInstruction(0x5a, "PHY", AM_Implied));//65C02
    opcode_table->insert(0x6a,AssyInstruction(0x6a, "ROR", AM_Accumulator));
    opcode_table->insert(0x7a,AssyInstruction(0x7a, "PLY", AM_Implied));//65C02
    opcode_table->insert(0x8a,AssyInstruction(0x8a, "TXA", AM_Implied));
    opcode_table->insert(0x9a,AssyInstruction(0x9a, "TXS", AM_Implied));
    opcode_table->insert(0xaa,AssyInstruction(0xaa, "TAX", AM_Implied));
    opcode_table->insert(0xba,AssyInstruction(0xba, "TSX", AM_Implied));
    opcode_table->insert(0xca,AssyInstruction(0xca, "DEX", AM_Implied));
    opcode_table->insert(0xda,AssyInstruction(0xda, "PHX", AM_Implied));//65C02
    opcode_table->insert(0xea,AssyInstruction(0xea, "NOP", AM_Implied));
    opcode_table->insert(0xfa,AssyInstruction(0xfa, "PLX", AM_Implied));//65C02

    opcode_table->insert(0x0b,AssyInstruction(0x0b, "???", AM_InvalidOp));
    opcode_table->insert(0x1b,AssyInstruction(0x1b, "???", AM_InvalidOp));
    opcode_table->insert(0x2b,AssyInstruction(0x2b, "???", AM_InvalidOp));
    opcode_table->insert(0x3b,AssyInstruction(0x3b, "???", AM_InvalidOp));
    opcode_table->insert(0x4b,AssyInstruction(0x4b, "???", AM_InvalidOp));
    opcode_table->insert(0x5b,AssyInstruction(0x5b, "???", AM_InvalidOp));
    opcode_table->insert(0x6b,AssyInstruction(0x6b, "???", AM_InvalidOp));
    opcode_table->insert(0x7b,AssyInstruction(0x7b, "???", AM_InvalidOp));
    opcode_table->insert(0x8b,AssyInstruction(0x8b, "???", AM_InvalidOp));
    opcode_table->insert(0x9b,AssyInstruction(0x9b, "???", AM_InvalidOp));
    opcode_table->insert(0xab,AssyInstruction(0xab, "???", AM_InvalidOp));
    opcode_table->insert(0xbb,AssyInstruction(0xbb, "???", AM_InvalidOp));
    opcode_table->insert(0xcb,AssyInstruction(0xcb, "???", AM_InvalidOp));
    opcode_table->insert(0xdb,AssyInstruction(0xdb, "???", AM_InvalidOp));
    opcode_table->insert(0xeb,AssyInstruction(0xeb, "???", AM_InvalidOp));
    opcode_table->insert(0xfb,AssyInstruction(0xfb, "???", AM_InvalidOp));

    opcode_table->insert(0x0c,AssyInstruction(0x0c, "TSB", AM_Absolute, true, true, false)); //65C02
    opcode_table->insert(0x1c,AssyInstruction(0x1c, "TRB", AM_Absolute, true, true, false)); //65C02
    opcode_table->insert(0x2c,AssyInstruction(0x2c, "BIT", AM_Absolute, true, false, false));
    opcode_table->insert(0x3c,AssyInstruction(0x3c, "BIT", AM_AbsoluteIndexedWithX, true, false, false)); //65C02
    opcode_table->insert(0x4c,AssyInstruction(0x4c, "JMP", AM_Absolute));
    opcode_table->insert(0x5c,AssyInstruction(0x5c, "???", AM_InvalidOp));
    opcode_table->insert(0x6c,AssyInstruction(0x6c, "JMP", AM_AbsoluteIndirect, true, false, false));
    opcode_table->insert(0x7c,AssyInstruction(0x7c, "JMP", AM_AbsoluteIndexedIndirect, true, false, false)); //65C02
    opcode_table->insert(0x8c,AssyInstruction(0x8c, "STY", AM_Absolute, false, true, false));
    opcode_table->insert(0x9c,AssyInstruction(0x9c, "STZ", AM_Absolute, false, true, false));
    opcode_table->insert(0xac,AssyInstruction(0xac, "LDY", AM_Absolute, true, false, false));
    opcode_table->insert(0xbc,AssyInstruction(0xbc, "LDY", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xcc,AssyInstruction(0xcc, "CPY", AM_Absolute, true, false, false));
    opcode_table->insert(0xdc,AssyInstruction(0xdc, "???", AM_InvalidOp));
    opcode_table->insert(0xec,AssyInstruction(0xec, "CPX", AM_Absolute, true, false, false));
    opcode_table->insert(0xfc,AssyInstruction(0xfc, "???", AM_InvalidOp));

    opcode_table->insert(0x0d,AssyInstruction(0x0d, "ORA", AM_Absolute, true, false, false));
    opcode_table->insert(0x1d,AssyInstruction(0x1d, "ORA", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x2d,AssyInstruction(0x2d, "AND", AM_Absolute, true, false, false));
    opcode_table->insert(0x3d,AssyInstruction(0x3d, "AND", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x4d,AssyInstruction(0x4d, "EOR", AM_Absolute, true, false, false));
    opcode_table->insert(0x5d,AssyInstruction(0x5d, "EOR", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x6d,AssyInstruction(0x6d, "ADC", AM_Absolute, true, false, false));
    opcode_table->insert(0x7d,AssyInstruction(0x7d, "ADC", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0x8d,AssyInstruction(0x8d, "STA", AM_Absolute, false, true, false));
    opcode_table->insert(0x9d,AssyInstruction(0x9d, "STA", AM_AbsoluteIndexedWithX, false, true, false));
    opcode_table->insert(0xad,AssyInstruction(0xad, "LDA", AM_Absolute, true, false, false));
    opcode_table->insert(0xbd,AssyInstruction(0xbd, "LDA", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xcd,AssyInstruction(0xcd, "CMP", AM_Absolute, true, false, false));
    opcode_table->insert(0xdd,AssyInstruction(0xdd, "CMP", AM_AbsoluteIndexedWithX, true, false, false));
    opcode_table->insert(0xed,AssyInstruction(0xed, "SBC", AM_Absolute, true, false, false));
    opcode_table->insert(0xfd,AssyInstruction(0xfd, "SBC", AM_AbsoluteIndexedWithX, true, false, false));

    opcode_table->insert(0x0e,AssyInstruction(0x0e, "ASL", AM_Absolute, true, true, false));
    opcode_table->insert(0x1e,AssyInstruction(0x1e, "ASL", AM_AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x2e,AssyInstruction(0x2e, "ROL", AM_Absolute, true, true, false));
    opcode_table->insert(0x3e,AssyInstruction(0x3e, "ROL", AM_AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x4e,AssyInstruction(0x4e, "LSR", AM_Absolute, true, true, false));
    opcode_table->insert(0x5e,AssyInstruction(0x5e, "LSR", AM_AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x6e,AssyInstruction(0x6e, "ROR", AM_Absolute, true, true, false));
    opcode_table->insert(0x7e,AssyInstruction(0x7e, "ROR", AM_AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0x8e,AssyInstruction(0x8e, "STX", AM_Absolute, false, true, false));
    opcode_table->insert(0x9e,AssyInstruction(0x9e, "STZ", AM_AbsoluteIndexedWithX, false, true, false)); //65C02
    opcode_table->insert(0xae,AssyInstruction(0xae, "LDX", AM_Absolute, true, false, false));
    opcode_table->insert(0xbe,AssyInstruction(0xbe, "LDX", AM_AbsoluteIndexedWithY, true, false, false));
    opcode_table->insert(0xce,AssyInstruction(0xce, "DEC", AM_Absolute, true, true, false));
    opcode_table->insert(0xde,AssyInstruction(0xde, "DEC", AM_AbsoluteIndexedWithX, true, true, false));
    opcode_table->insert(0xee,AssyInstruction(0xee, "INC", AM_Absolute, true, true, false));
    opcode_table->insert(0xfe,AssyInstruction(0xfe, "INC", AM_AbsoluteIndexedWithX, true, true, false));

    opcode_table->insert(0x0f,AssyInstruction(0x0f, "???", AM_InvalidOp));
    opcode_table->insert(0x1f,AssyInstruction(0x1f, "???", AM_InvalidOp));
    opcode_table->insert(0x2f,AssyInstruction(0x2f, "???", AM_InvalidOp));
    opcode_table->insert(0x3f,AssyInstruction(0x3f, "???", AM_InvalidOp));
    opcode_table->insert(0x4f,AssyInstruction(0x4f, "???", AM_InvalidOp));
    opcode_table->insert(0x5f,AssyInstruction(0x5f, "???", AM_InvalidOp));
    opcode_table->insert(0x6f,AssyInstruction(0x6f, "???", AM_InvalidOp));
    opcode_table->insert(0x7f,AssyInstruction(0x7f, "???", AM_InvalidOp));
    opcode_table->insert(0x8f,AssyInstruction(0x8f, "???", AM_InvalidOp));
    opcode_table->insert(0x9f,AssyInstruction(0x9f, "???", AM_InvalidOp));
    opcode_table->insert(0xaf,AssyInstruction(0xaf, "???", AM_InvalidOp));
    opcode_table->insert(0xbf,AssyInstruction(0xbf, "???", AM_InvalidOp));
    opcode_table->insert(0xcf,AssyInstruction(0xcf, "???", AM_InvalidOp));
    opcode_table->insert(0xdf,AssyInstruction(0xdf, "???", AM_InvalidOp));
    opcode_table->insert(0xef,AssyInstruction(0xef, "???", AM_InvalidOp));
    opcode_table->insert(0xff,AssyInstruction(0xff, "???", AM_InvalidOp));

#ifdef UNUSED_OPCODES
    m_opcodeinfo->insert(0x00,AssyInstruction(0x00, "BRK", AM_Implied));
    m_opcodeinfo->insert(0x10,AssyInstruction(0x10, "BPL", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0x20,AssyInstruction(0x20, "JSR", AM_Absolute));
    m_opcodeinfo->insert(0x30,AssyInstruction(0x30, "BMI", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0x40,AssyInstruction(0x40, "RTI", AM_Implied));
    m_opcodeinfo->insert(0x50,AssyInstruction(0x50, "BVC", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0x60,AssyInstruction(0x60, "RTS", AM_Implied));
    m_opcodeinfo->insert(0x70,AssyInstruction(0x70, "BVS", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0x80,AssyInstruction(0x80, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x90,AssyInstruction(0x90, "BCC", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0xa0,AssyInstruction(0xa0, "LDY", AM_Immediate));
    m_opcodeinfo->insert(0xb0,AssyInstruction(0xb0, "BCC", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0xc0,AssyInstruction(0xc0, "LDY", AM_Immediate));
    m_opcodeinfo->insert(0xd0,AssyInstruction(0xd0, "BCS", AM_ProgramCounterRelative);
    m_opcodeinfo->insert(0xe0,AssyInstruction(0xe0, "CPX", AM_Immediate));
    m_opcodeinfo->insert(0xf0,AssyInstruction(0xf0, "BEQ", AM_ProgramCounterRelative);

    m_opcodeinfo->insert(0x01,AssyInstruction(0x01, "ORA", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x11,AssyInstruction(0x11, "ORA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x21,AssyInstruction(0x21, "AND", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x31,AssyInstruction(0x31, "AND", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x41,AssyInstruction(0x41, "EOR", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x51,AssyInstruction(0x51, "EOR", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x61,AssyInstruction(0x61, "ADC", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x71,AssyInstruction(0x71, "ADC", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x81,AssyInstruction(0x81, "STA", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x91,AssyInstruction(0x91, "STA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa1,AssyInstruction(0xa1, "LDA", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xb1,AssyInstruction(0xb1, "LDA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc1,AssyInstruction(0xc1, "CMP", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xd1,AssyInstruction(0xd1, "CMP", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe1,AssyInstruction(0xe1, "SBC", AM_AbsoluteIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xf1,AssyInstruction(0xff, "SBC", AM_AbsoluteIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x02,AssyInstruction(0x02, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x12,AssyInstruction(0x12, "asl-ora", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x22,AssyInstruction(0x22, "and", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x32,AssyInstruction(0x32, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x42,AssyInstruction(0x42, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x52,AssyInstruction(0x52, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x62,AssyInstruction(0x62, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x72,AssyInstruction(0x72, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x82,AssyInstruction(0x82, "halt", AM_Immediate));
    m_opcodeinfo->insert(0x92,AssyInstruction(0x92, "halt", AM_Immediate));
    m_opcodeinfo->insert(0xa2,AssyInstruction(0xa2, "LDX", AM_Immediate));
    m_opcodeinfo->insert(0xb2,AssyInstruction(0xb2, "halt", AM_Immediate));
    m_opcodeinfo->insert(0xc2,AssyInstruction(0xc2, "halt", AM_Immediate));
    m_opcodeinfo->insert(0xd2,AssyInstruction(0xd2, "halt", AM_Immediate));
    m_opcodeinfo->insert(0xe2,AssyInstruction(0xe2, "halt", AM_Immediate));
    m_opcodeinfo->insert(0xf2,AssyInstruction(0xf2, "halt", AM_Immediate));

    m_opcodeinfo->insert(0x03,AssyInstruction(0x03, "asl/ora", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x13,AssyInstruction(0x13, "asl/ora", AM_ZeroPageIndirect, false, false, false));
    m_opcodeinfo->insert(0x23,AssyInstruction(0x23, "rol/and", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x33,AssyInstruction(0x33, "rol/and", AM_ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x43,AssyInstruction(0x43, "lsr/eor", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x53,AssyInstruction(0x53, "lsr/eor", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x63,AssyInstruction(0x63, "ror/adc", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x73,AssyInstruction(0x73, "ror/adc", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x83,AssyInstruction(0x83, "sta/stx", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0x93,AssyInstruction(0x93, "sta/stx", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa3,AssyInstruction(0xa3, "lda/ldx", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xb3,AssyInstruction(0xb3, "lda/ldx", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc3,AssyInstruction(0xc3, "dec/cmp", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xd3,AssyInstruction(0xd3, "dec/cmp", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe3,AssyInstruction(0xe3, "inc/sbc", AM_ZeroPageIndexedIndirect, false, false, false));
    m_opcodeinfo->insert(0xf3,AssyInstruction(0xf3, "inc/sbc", AM_ZeroPageIndirectIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x04,AssyInstruction(0x04, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x14,AssyInstruction(0x14, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x24,AssyInstruction(0x24, "BIT", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x34,AssyInstruction(0x34, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x44,AssyInstruction(0x44, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x54,AssyInstruction(0x54, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x64,AssyInstruction(0x64, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x74,AssyInstruction(0x74, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x84,AssyInstruction(0x84, "STY", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x94,AssyInstruction(0x94, "STY", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xa4,AssyInstruction(0xa4, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb4,AssyInstruction(0xb4, "LDY", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xc4,AssyInstruction(0xc4, "CPY", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd4,AssyInstruction(0xd4, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xe4,AssyInstruction(0xe4, "CPX", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf4,AssyInstruction(0xf4, "nop", AM_ZeroPage, false, false, false));

    m_opcodeinfo->insert(0x05,AssyInstruction(0x05, "ORA", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x15,AssyInstruction(0x15, "ORA", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x25,AssyInstruction(0x25, "AND", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x35,AssyInstruction(0x35, "AND", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x45,AssyInstruction(0x45, "EOR", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x55,AssyInstruction(0x55, "EOR", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x65,AssyInstruction(0x65, "ADC", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x75,AssyInstruction(0x75, "ADC", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x85,AssyInstruction(0x85, "STA", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x95,AssyInstruction(0x95, "STA", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xa5,AssyInstruction(0xa5, "LDA", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb5,AssyInstruction(0xb5, "LDA", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xc5,AssyInstruction(0xc5, "CMP", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd5,AssyInstruction(0xd5, "CMP", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe5,AssyInstruction(0xe5, "SEC", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf5,AssyInstruction(0xf5, "SEC", AM_ZeroPageIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x06,AssyInstruction(0x06, "ASL", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x16,AssyInstruction(0x16, "ASL", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x26,AssyInstruction(0x26, "ROL", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x36,AssyInstruction(0x36, "ROL", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x46,AssyInstruction(0x46, "LSR", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x56,AssyInstruction(0x56, "LSR", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x66,AssyInstruction(0x66, "ROR", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x76,AssyInstruction(0x76, "ROR", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x86,AssyInstruction(0x86, "STX", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x96,AssyInstruction(0x96, "STX", AM_ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa6,AssyInstruction(0xa6, "LDX", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb6,AssyInstruction(0xb6, "LDX", AM_ZeroPageIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc6,AssyInstruction(0xc6, "DEC", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd6,AssyInstruction(0xd6, "DEC", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe6,AssyInstruction(0xe6, "INC", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf6,AssyInstruction(0xf6, "INC", AM_ZeroPageIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x07,AssyInstruction(0x07, "asl/ora", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x17,AssyInstruction(0x17, "asl/ora", AM_ZeroPageIndirectIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x27,AssyInstruction(0x27, "rol/and", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x37,AssyInstruction(0x37, "rol/and", AM_ZeroPageIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x47,AssyInstruction(0x47, "lsr/eor", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x57,AssyInstruction(0x57, "lsr/eor", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x67,AssyInstruction(0x67, "ror/adc", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x77,AssyInstruction(0x77, "ror/adc", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x87,AssyInstruction(0x87, "sta/stx", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x97,AssyInstruction(0x97, "sta/stx", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa7,AssyInstruction(0xa7, "lda/ldx", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xb7,AssyInstruction(0xb7, "ldx/ldx", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc7,AssyInstruction(0xc7, "dec/cmp", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xd7,AssyInstruction(0xd7, "dec/cmp", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xe7,AssyInstruction(0xe7, "inc/sbc", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0xf7,AssyInstruction(0xf7, "inc/sbc", AM_AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x08,AssyInstruction(0x08, "PHP", AM_Implied));
    m_opcodeinfo->insert(0x18,AssyInstruction(0x18, "CLC", AM_Implied));
    m_opcodeinfo->insert(0x28,AssyInstruction(0x28, "PLP", AM_Implied));
    m_opcodeinfo->insert(0x38,AssyInstruction(0x38, "SEC", AM_Implied));
    m_opcodeinfo->insert(0x48,AssyInstruction(0x48, "PHA", AM_Implied));
    m_opcodeinfo->insert(0x58,AssyInstruction(0x58, "CLI", AM_Implied));
    m_opcodeinfo->insert(0x68,AssyInstruction(0x68, "PLA", AM_Implied));
    m_opcodeinfo->insert(0x78,AssyInstruction(0x78, "SEI", AM_Implied));
    m_opcodeinfo->insert(0x88,AssyInstruction(0x88, "DEY", AM_Implied));
    m_opcodeinfo->insert(0x98,AssyInstruction(0x98, "TYA", AM_Implied));
    m_opcodeinfo->insert(0xa8,AssyInstruction(0xa8, "TAY", AM_Implied));
    m_opcodeinfo->insert(0xb8,AssyInstruction(0xb8, "CLV", AM_Implied));
    m_opcodeinfo->insert(0xc8,AssyInstruction(0xc8, "INY", AM_Implied));
    m_opcodeinfo->insert(0xd8,AssyInstruction(0xd8, "CLD", AM_Implied));
    m_opcodeinfo->insert(0xe8,AssyInstruction(0xe8, "INX", AM_Implied));
    m_opcodeinfo->insert(0xf8,AssyInstruction(0xf8, "SED", AM_Implied));

    m_opcodeinfo->insert(0x09,AssyInstruction(0x09, "ORA", AM_Immediate));
    m_opcodeinfo->insert(0x19,AssyInstruction(0x19, "ORA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x29,AssyInstruction(0x29, "AND", AM_Immediate));
    m_opcodeinfo->insert(0x39,AssyInstruction(0x39, "AND", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x49,AssyInstruction(0x49, "EOR", AM_Immediate));
    m_opcodeinfo->insert(0x59,AssyInstruction(0x59, "EOR", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x69,AssyInstruction(0x69, "ADC", AM_Immediate));
    m_opcodeinfo->insert(0x79,AssyInstruction(0x79, "ADC", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x89,AssyInstruction(0x89, "nop", AM_ZeroPage, false, false, false));
    m_opcodeinfo->insert(0x99,AssyInstruction(0x99, "STA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xa9,AssyInstruction(0xa9, "LDA", AM_Immediate));
    m_opcodeinfo->insert(0xb9,AssyInstruction(0xb9, "LDA", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xc9,AssyInstruction(0xc9, "CMP", AM_Immediate));
    m_opcodeinfo->insert(0xd9,AssyInstruction(0xd9, "CMP", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xe9,AssyInstruction(0xe9, "SBC", AM_Immediate));
    m_opcodeinfo->insert(0xf9,AssyInstruction(0xf9, "SBC", AM_AbsoluteIndexedWithY, false, false, false));

    m_opcodeinfo->insert(0x0a,AssyInstruction(0x0a, "ASL", AM_Accumulator));
    m_opcodeinfo->insert(0x1a,AssyInstruction(0x1a, "nop", AM_Implied));
    m_opcodeinfo->insert(0x2a,AssyInstruction(0x2a, "ROL", AM_Accumulator));
    m_opcodeinfo->insert(0x3a,AssyInstruction(0x3a, "nop", AM_Implied));
    m_opcodeinfo->insert(0x4a,AssyInstruction(0x4a, "LSR", AM_Accumulator));
    m_opcodeinfo->insert(0x5a,AssyInstruction(0x5a, "nop", AM_Implied));
    m_opcodeinfo->insert(0x6a,AssyInstruction(0x6a, "ROR", AM_Accumulator));
    m_opcodeinfo->insert(0x7a,AssyInstruction(0x7a, "nop", AM_Implied));
    m_opcodeinfo->insert(0x8a,AssyInstruction(0x8a, "TXA", AM_Implied));
    m_opcodeinfo->insert(0x9a,AssyInstruction(0x9a, "TXS", AM_Implied));
    m_opcodeinfo->insert(0xaa,AssyInstruction(0xaa, "TAX", AM_Implied));
    m_opcodeinfo->insert(0xba,AssyInstruction(0xba, "TSX", AM_Implied));
    m_opcodeinfo->insert(0xca,AssyInstruction(0xca, "DEX", AM_Implied));
    m_opcodeinfo->insert(0xda,AssyInstruction(0xda, "nop", AM_Implied));
    m_opcodeinfo->insert(0xea,AssyInstruction(0xea, "NOP", AM_Implied));
    m_opcodeinfo->insert(0xfa,AssyInstruction(0xfa, "nop", AM_Implied));

    m_opcodeinfo->insert(0x0b,AssyInstruction(0x0b, "and/mov bit7->Cy", AM_Immediate));
    m_opcodeinfo->insert(0x1b,AssyInstruction(0x1b, "asl/ora", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x2b,AssyInstruction(0x2b, "and/mov bit7->Cy", AM_Immediate));
    m_opcodeinfo->insert(0x3b,AssyInstruction(0x3b, "asl/ora", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x4b,AssyInstruction(0x4b, "and/lsr A", AM_Immediate));
    m_opcodeinfo->insert(0x5b,AssyInstruction(0x5b, "lsr/eor", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6b,AssyInstruction(0x6b, "and/ror A", AM_Immediate));
    m_opcodeinfo->insert(0x7b,AssyInstruction(0x7b, "ror/adc", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0x8b,AssyInstruction(0x8b, "txa/and", AM_Immediate));
    m_opcodeinfo->insert(0x9b,AssyInstruction(0x9b, "sta/stx", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xab,AssyInstruction(0xab, "lda/ldx", AM_Implied));
    m_opcodeinfo->insert(0xbb,AssyInstruction(0xbb, "lda/ldx", AM_Implied));
    m_opcodeinfo->insert(0xcb,AssyInstruction(0xcb, "sbx", AM_Immediate));
    m_opcodeinfo->insert(0xdb,AssyInstruction(0xdb, "dec/cmp", AM_Absolute));
    m_opcodeinfo->insert(0xeb,AssyInstruction(0xeb, "sbc", AM_Immediate));
    m_opcodeinfo->insert(0xfb,AssyInstruction(0xfb, "inc/sbc", AM_Absolute));

    m_opcodeinfo->insert(0x0c,AssyInstruction(0x0c, "nop", AM_Absolute));
    m_opcodeinfo->insert(0x1c,AssyInstruction(0x1c, "nop", AM_Absolute));
    m_opcodeinfo->insert(0x2c,AssyInstruction(0x2c, "BIT", AM_Absolute));
    m_opcodeinfo->insert(0x3c,AssyInstruction(0x3c, "nop", AM_Absolute));
    m_opcodeinfo->insert(0x4c,AssyInstruction(0x4c, "JMP", AM_Absolute));
    m_opcodeinfo->insert(0x5c,AssyInstruction(0x5c, "nop", AM_Absolute));
    m_opcodeinfo->insert(0x6c,AssyInstruction(0x6c, "JMP", AM_AbsoluteIndirect, false, false, false));
    m_opcodeinfo->insert(0x7c,AssyInstruction(0x7c, "nop", AM_Absolute));
    m_opcodeinfo->insert(0x8c,AssyInstruction(0x8c, "STY", AM_Absolute));
    m_opcodeinfo->insert(0x9c,AssyInstruction(0x9c, "sta/stx", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xac,AssyInstruction(0xac, "LDY", AM_Absolute));
    m_opcodeinfo->insert(0xbc,AssyInstruction(0xbc, "LDY", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xcc,AssyInstruction(0xcc, "CPY", AM_Absolute));
    m_opcodeinfo->insert(0xdc,AssyInstruction(0xdc, "???", AM_InvalidOp));
    m_opcodeinfo->insert(0xec,AssyInstruction(0xec, "CPX", AM_Absolute));
    m_opcodeinfo->insert(0xfc,AssyInstruction(0xfc, "???", AM_InvalidOp));

    m_opcodeinfo->insert(0x0d,AssyInstruction(0x0d, "ORA", AM_Absolute));
    m_opcodeinfo->insert(0x1d,AssyInstruction(0x1d, "ORA", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2d,AssyInstruction(0x2d, "AND", AM_Absolute));
    m_opcodeinfo->insert(0x3d,AssyInstruction(0x3d, "AND", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4d,AssyInstruction(0x4d, "EOR", AM_Absolute));
    m_opcodeinfo->insert(0x5d,AssyInstruction(0x5d, "EOR", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6d,AssyInstruction(0x6d, "ADC", AM_Absolute));
    m_opcodeinfo->insert(0x7d,AssyInstruction(0x7d, "ADC", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8d,AssyInstruction(0x8d, "STA", AM_Absolute));
    m_opcodeinfo->insert(0x9d,AssyInstruction(0x9d, "STA", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xad,AssyInstruction(0xad, "LDA", AM_Absolute));
    m_opcodeinfo->insert(0xbd,AssyInstruction(0xbd, "LDA", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xcd,AssyInstruction(0xcd, "CMP", AM_Absolute));
    m_opcodeinfo->insert(0xdd,AssyInstruction(0xdd, "CMP", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xed,AssyInstruction(0xed, "SBC", AM_Absolute));
    m_opcodeinfo->insert(0xfd,AssyInstruction(0xfd, "SBC", AM_AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x0e,AssyInstruction(0x0e, "ASL", AM_Absolute));
    m_opcodeinfo->insert(0x1e,AssyInstruction(0x1e, "ASL", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2e,AssyInstruction(0x2e, "ROL", AM_Absolute));
    m_opcodeinfo->insert(0x3e,AssyInstruction(0x3e, "ROL", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4e,AssyInstruction(0x4e, "LSR", AM_Absolute));
    m_opcodeinfo->insert(0x5e,AssyInstruction(0x5e, "LSR", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6e,AssyInstruction(0x6e, "ROR", AM_Absolute));
    m_opcodeinfo->insert(0x7e,AssyInstruction(0x7e, "ROR", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8e,AssyInstruction(0x8e, "STX", AM_Absolute));
    m_opcodeinfo->insert(0x9e,AssyInstruction(0x9e, "sta/stx", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xae,AssyInstruction(0xae, "LDX", AM_Absolute));
    m_opcodeinfo->insert(0xbe,AssyInstruction(0xbe, "LDX", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xce,AssyInstruction(0xce, "DEC", AM_Absolute));
    m_opcodeinfo->insert(0xde,AssyInstruction(0xde, "DEC", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xee,AssyInstruction(0xee, "INC", AM_Absolute));
    m_opcodeinfo->insert(0xfe,AssyInstruction(0xfe, "INC", AM_AbsoluteIndexedWithX, false, false, false));

    m_opcodeinfo->insert(0x0f,AssyInstruction(0x0f, "asl/ora", AM_Absolute));
    m_opcodeinfo->insert(0x1f,AssyInstruction(0x1f, "asl/ora", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x2f,AssyInstruction(0x2f, "rol/and", AM_Absolute));
    m_opcodeinfo->insert(0x3f,AssyInstruction(0x3f, "rol/and", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x4f,AssyInstruction(0x4f, "lsr/eor", AM_Absolute));
    m_opcodeinfo->insert(0x5f,AssyInstruction(0x5f, "lsr/eor", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x6f,AssyInstruction(0x6f, "ror/adc", AM_Absolute));
    m_opcodeinfo->insert(0x7f,AssyInstruction(0x7f, "ror/adc", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0x8f,AssyInstruction(0x8f, "sta/stx", AM_Absolute));
    m_opcodeinfo->insert(0x9f,AssyInstruction(0x9f, "sta/stx", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xaf,AssyInstruction(0xaf, "lda/ldx", AM_Absolute));
    m_opcodeinfo->insert(0xbf,AssyInstruction(0xbf, "ldx/ldx", AM_AbsoluteIndexedWithY, false, false, false));
    m_opcodeinfo->insert(0xcf,AssyInstruction(0xcf, "dec/cmp", AM_Absolute));
    m_opcodeinfo->insert(0xdf,AssyInstruction(0xdf, "dec/cmp", AM_AbsoluteIndexedWithX, false, false, false));
    m_opcodeinfo->insert(0xef,AssyInstruction(0xef, "inc/sbc", AM_Absolute));
    m_opcodeinfo->insert(0xff,AssyInstruction(0xff, "inc/sbc", AM_AbsoluteIndexedWithX, false, false, false));
#endif
}



OpCodes::AssyInstruction::AssyInstruction(quint8 opcode, QString mnemonic, AddressMode am, bool reads, bool writes, bool stack)
{
    m_opcode = opcode;
    m_mnemonic = mnemonic;
    m_addressMode = am;
    m_reads = reads;
    m_writes = writes;
    m_stack = stack;
}

quint8 OpCodes::AssyInstruction::numArgs() const
{
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
