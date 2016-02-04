#include "disassembler.h"


QStringList Disassembler::disassemble(quint16 address)
{
    quint8 opcode = m_memimage[address];
    AssyInstruction op = m_opcodeinfo.at(opcode);



}

void Disassembler::makeOpcodeTable()
{
    m_opcodeinfo[0x00] = AssyInstruction(0x00, "BRK", AM_Implied);
    m_opcodeinfo[0x10] = AssyInstruction(0x10, "BPL", AM_ProgramCounterRelative);
    m_opcodeinfo[0x20] = AssyInstruction(0x20, "JSR", AM_Absolute);
    m_opcodeinfo[0x30] = AssyInstruction(0x30, "BMI", AM_ProgramCounterRelative);
    m_opcodeinfo[0x40] = AssyInstruction(0x40, "RTI", AM_Implied);
    m_opcodeinfo[0x50] = AssyInstruction(0x50, "BVC", AM_ProgramCounterRelative);
    m_opcodeinfo[0x60] = AssyInstruction(0x60, "RTS", AM_Implied);
    m_opcodeinfo[0x70] = AssyInstruction(0x70, "BVS", AM_ProgramCounterRelative);
    m_opcodeinfo[0x80] = AssyInstruction(0x80, "???", AM_InvalidOp);
    m_opcodeinfo[0x90] = AssyInstruction(0x90, "BCC", AM_ProgramCounterRelative);
    m_opcodeinfo[0xa0] = AssyInstruction(0xa0, "LDY", AM_Immediate);
    m_opcodeinfo[0xb0] = AssyInstruction(0xb0, "BCC", AM_ProgramCounterRelative);
    m_opcodeinfo[0xc0] = AssyInstruction(0xc0, "LDY", AM_Immediate);
    m_opcodeinfo[0xd0] = AssyInstruction(0xd0, "BCS", AM_ProgramCounterRelative);
    m_opcodeinfo[0xe0] = AssyInstruction(0xe0, "CPX", AM_Immediate);
    m_opcodeinfo[0xf0] = AssyInstruction(0xf0, "BEQ", AM_ProgramCounterRelative);

    m_opcodeinfo[0x01] = AssyInstruction(0x01, "ORA", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0x11] = AssyInstruction(0x11, "ORA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x21] = AssyInstruction(0x21, "AND", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0x31] = AssyInstruction(0x31, "AND", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x41] = AssyInstruction(0x41, "EOR", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0x51] = AssyInstruction(0x51, "EOR", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x61] = AssyInstruction(0x61, "ADC", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0x71] = AssyInstruction(0x71, "ADC", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x81] = AssyInstruction(0x81, "STA", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0x91] = AssyInstruction(0x91, "STA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xa1] = AssyInstruction(0xa1, "LDA", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0xb1] = AssyInstruction(0xb1, "LDA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xc1] = AssyInstruction(0xc1, "CMP", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0xd1] = AssyInstruction(0xd1, "CMP", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xe1] = AssyInstruction(0xe1, "SBC", AM_AbsoluteIndexedIndirect);
    m_opcodeinfo[0xf1] = AssyInstruction(0xff, "SBC", AM_AbsoluteIndexedWithY);

    m_opcodeinfo[0x02] = AssyInstruction(0x02, "???", AM_InvalidOp);
    m_opcodeinfo[0x12] = AssyInstruction(0x12, "???", AM_InvalidOp);
    m_opcodeinfo[0x22] = AssyInstruction(0x22, "???", AM_InvalidOp);
    m_opcodeinfo[0x32] = AssyInstruction(0x32, "???", AM_InvalidOp);
    m_opcodeinfo[0x42] = AssyInstruction(0x42, "???", AM_InvalidOp);
    m_opcodeinfo[0x52] = AssyInstruction(0x52, "???", AM_InvalidOp);
    m_opcodeinfo[0x62] = AssyInstruction(0x62, "???", AM_InvalidOp);
    m_opcodeinfo[0x72] = AssyInstruction(0x72, "???", AM_InvalidOp);
    m_opcodeinfo[0x82] = AssyInstruction(0x82, "???", AM_InvalidOp);
    m_opcodeinfo[0x92] = AssyInstruction(0x92, "???", AM_InvalidOp);
    m_opcodeinfo[0xa2] = AssyInstruction(0xa2, "LDX", AM_Immediate);
    m_opcodeinfo[0xb2] = AssyInstruction(0xb2, "???", AM_InvalidOp);
    m_opcodeinfo[0xc2] = AssyInstruction(0xc2, "???", AM_InvalidOp);
    m_opcodeinfo[0xd2] = AssyInstruction(0xd2, "???", AM_InvalidOp);
    m_opcodeinfo[0xe2] = AssyInstruction(0xe2, "???", AM_InvalidOp);
    m_opcodeinfo[0xf2] = AssyInstruction(0xf2, "???", AM_InvalidOp);

    m_opcodeinfo[0x03] = AssyInstruction(0x03, "???", AM_InvalidOp);
    m_opcodeinfo[0x13] = AssyInstruction(0x13, "???", AM_InvalidOp);
    m_opcodeinfo[0x23] = AssyInstruction(0x23, "???", AM_InvalidOp);
    m_opcodeinfo[0x33] = AssyInstruction(0x33, "???", AM_InvalidOp);
    m_opcodeinfo[0x43] = AssyInstruction(0x43, "???", AM_InvalidOp);
    m_opcodeinfo[0x53] = AssyInstruction(0x53, "???", AM_InvalidOp);
    m_opcodeinfo[0x63] = AssyInstruction(0x63, "???", AM_InvalidOp);
    m_opcodeinfo[0x73] = AssyInstruction(0x73, "???", AM_InvalidOp);
    m_opcodeinfo[0x83] = AssyInstruction(0x83, "???", AM_InvalidOp);
    m_opcodeinfo[0x93] = AssyInstruction(0x93, "???", AM_InvalidOp);
    m_opcodeinfo[0xa3] = AssyInstruction(0xa3, "???", AM_InvalidOp);
    m_opcodeinfo[0xb3] = AssyInstruction(0xb3, "???", AM_InvalidOp);
    m_opcodeinfo[0xc3] = AssyInstruction(0xc3, "???", AM_InvalidOp);
    m_opcodeinfo[0xd3] = AssyInstruction(0xd3, "???", AM_InvalidOp);
    m_opcodeinfo[0xe3] = AssyInstruction(0xe3, "???", AM_InvalidOp);
    m_opcodeinfo[0xf3] = AssyInstruction(0xf3, "???", AM_InvalidOp);

    m_opcodeinfo[0x04] = AssyInstruction(0x04, "???", AM_InvalidOp);
    m_opcodeinfo[0x14] = AssyInstruction(0x14, "???", AM_InvalidOp);
    m_opcodeinfo[0x24] = AssyInstruction(0x24, "BIT", AM_ZeroPage);
    m_opcodeinfo[0x34] = AssyInstruction(0x34, "???", AM_InvalidOp);
    m_opcodeinfo[0x44] = AssyInstruction(0x44, "???", AM_InvalidOp);
    m_opcodeinfo[0x54] = AssyInstruction(0x54, "???", AM_InvalidOp);
    m_opcodeinfo[0x64] = AssyInstruction(0x64, "???", AM_InvalidOp);
    m_opcodeinfo[0x74] = AssyInstruction(0x74, "???", AM_InvalidOp);
    m_opcodeinfo[0x84] = AssyInstruction(0x84, "STY", AM_ZeroPage);
    m_opcodeinfo[0x94] = AssyInstruction(0x94, "STY", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xa4] = AssyInstruction(0xa4, "LDY", AM_ZeroPage);
    m_opcodeinfo[0xb4] = AssyInstruction(0xb4, "LDY", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xc4] = AssyInstruction(0xc4, "CPY", AM_ZeroPage);
    m_opcodeinfo[0xd4] = AssyInstruction(0xd4, "???", AM_InvalidOp);
    m_opcodeinfo[0xe4] = AssyInstruction(0xe4, "CPX", AM_ZeroPage);
    m_opcodeinfo[0xf4] = AssyInstruction(0xf4, "???", AM_InvalidOp);

    m_opcodeinfo[0x05] = AssyInstruction(0x05, "ORA", AM_ZeroPage);
    m_opcodeinfo[0x15] = AssyInstruction(0x15, "ORA", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x25] = AssyInstruction(0x25, "AND", AM_ZeroPage);
    m_opcodeinfo[0x35] = AssyInstruction(0x35, "AND", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x45] = AssyInstruction(0x45, "EOR", AM_ZeroPage);
    m_opcodeinfo[0x55] = AssyInstruction(0x55, "EOR", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x65] = AssyInstruction(0x65, "ADC", AM_ZeroPage);
    m_opcodeinfo[0x75] = AssyInstruction(0x75, "ADC", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x85] = AssyInstruction(0x85, "STA", AM_ZeroPage);
    m_opcodeinfo[0x95] = AssyInstruction(0x95, "STA", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xa5] = AssyInstruction(0xa5, "LDA", AM_ZeroPage);
    m_opcodeinfo[0xb5] = AssyInstruction(0xb5, "LDA", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xc5] = AssyInstruction(0xc5, "CMP", AM_ZeroPage);
    m_opcodeinfo[0xd5] = AssyInstruction(0xd5, "CMP", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xe5] = AssyInstruction(0xe5, "SEC", AM_ZeroPage);
    m_opcodeinfo[0xf5] = AssyInstruction(0xf5, "SEC", AM_ZeroPageIndexedWithX);

    m_opcodeinfo[0x06] = AssyInstruction(0x06, "ASL", AM_ZeroPage);
    m_opcodeinfo[0x16] = AssyInstruction(0x16, "ASL", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x26] = AssyInstruction(0x26, "ROL", AM_ZeroPage);
    m_opcodeinfo[0x36] = AssyInstruction(0x36, "ROL", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x46] = AssyInstruction(0x46, "LSR", AM_ZeroPage);
    m_opcodeinfo[0x56] = AssyInstruction(0x56, "LSR", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x66] = AssyInstruction(0x66, "ROR", AM_ZeroPage);
    m_opcodeinfo[0x76] = AssyInstruction(0x76, "ROR", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0x86] = AssyInstruction(0x86, "STX", AM_ZeroPage);
    m_opcodeinfo[0x96] = AssyInstruction(0x96, "STX", AM_ZeroPageIndexedWithY);
    m_opcodeinfo[0xa6] = AssyInstruction(0xa6, "LDX", AM_ZeroPage);
    m_opcodeinfo[0xb6] = AssyInstruction(0xb6, "LDX", AM_ZeroPageIndexedWithY);
    m_opcodeinfo[0xc6] = AssyInstruction(0xc6, "DEC", AM_ZeroPage);
    m_opcodeinfo[0xd6] = AssyInstruction(0xd6, "DEC", AM_ZeroPageIndexedWithX);
    m_opcodeinfo[0xe6] = AssyInstruction(0xe6, "INC", AM_ZeroPage);
    m_opcodeinfo[0xf6] = AssyInstruction(0xf6, "INC", AM_ZeroPageIndexedWithX);

    m_opcodeinfo[0x07] = AssyInstruction(0x07, "???", AM_InvalidOp);
    m_opcodeinfo[0x17] = AssyInstruction(0x17, "???", AM_InvalidOp);
    m_opcodeinfo[0x27] = AssyInstruction(0x27, "???", AM_InvalidOp);
    m_opcodeinfo[0x37] = AssyInstruction(0x37, "???", AM_InvalidOp);
    m_opcodeinfo[0x47] = AssyInstruction(0x47, "???", AM_InvalidOp);
    m_opcodeinfo[0x57] = AssyInstruction(0x57, "???", AM_InvalidOp);
    m_opcodeinfo[0x67] = AssyInstruction(0x67, "???", AM_InvalidOp);
    m_opcodeinfo[0x77] = AssyInstruction(0x77, "???", AM_InvalidOp);
    m_opcodeinfo[0x87] = AssyInstruction(0x87, "???", AM_InvalidOp);
    m_opcodeinfo[0x97] = AssyInstruction(0x97, "???", AM_InvalidOp);
    m_opcodeinfo[0xa7] = AssyInstruction(0xa7, "???", AM_InvalidOp);
    m_opcodeinfo[0xb7] = AssyInstruction(0xb7, "???", AM_InvalidOp);
    m_opcodeinfo[0xc7] = AssyInstruction(0xc7, "???", AM_InvalidOp);
    m_opcodeinfo[0xd7] = AssyInstruction(0xd7, "???", AM_InvalidOp);
    m_opcodeinfo[0xe7] = AssyInstruction(0xe7, "???", AM_InvalidOp);
    m_opcodeinfo[0xf7] = AssyInstruction(0xf7, "???", AM_InvalidOp);

    m_opcodeinfo[0x08] = AssyInstruction(0x08, "PHP", AM_Implied);
    m_opcodeinfo[0x18] = AssyInstruction(0x18, "CLC", AM_Implied);
    m_opcodeinfo[0x28] = AssyInstruction(0x28, "PLP", AM_Implied);
    m_opcodeinfo[0x38] = AssyInstruction(0x38, "SEC", AM_Implied);
    m_opcodeinfo[0x48] = AssyInstruction(0x48, "PHA", AM_Implied);
    m_opcodeinfo[0x58] = AssyInstruction(0x58, "CLI", AM_Implied);
    m_opcodeinfo[0x68] = AssyInstruction(0x68, "PLA", AM_Implied);
    m_opcodeinfo[0x78] = AssyInstruction(0x78, "SEI", AM_Implied);
    m_opcodeinfo[0x88] = AssyInstruction(0x88, "DEY", AM_Implied);
    m_opcodeinfo[0x98] = AssyInstruction(0x98, "TYA", AM_Implied);
    m_opcodeinfo[0xa8] = AssyInstruction(0xa8, "TAY", AM_Implied);
    m_opcodeinfo[0xb8] = AssyInstruction(0xb8, "CLV", AM_Implied);
    m_opcodeinfo[0xc8] = AssyInstruction(0xc8, "INY", AM_Implied);
    m_opcodeinfo[0xd8] = AssyInstruction(0xd8, "CLD", AM_Implied);
    m_opcodeinfo[0xe8] = AssyInstruction(0xe8, "INX", AM_Implied);
    m_opcodeinfo[0xf8] = AssyInstruction(0xf8, "SED", AM_Implied);

    m_opcodeinfo[0x09] = AssyInstruction(0x09, "ORA", AM_Immediate);
    m_opcodeinfo[0x19] = AssyInstruction(0x19, "ORA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x29] = AssyInstruction(0x29, "AND", AM_Immediate);
    m_opcodeinfo[0x39] = AssyInstruction(0x39, "AND", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x49] = AssyInstruction(0x49, "EOR", AM_Immediate);
    m_opcodeinfo[0x59] = AssyInstruction(0x59, "EOR", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x69] = AssyInstruction(0x69, "ADC", AM_Immediate);
    m_opcodeinfo[0x79] = AssyInstruction(0x79, "ADC", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0x89] = AssyInstruction(0x89, "???", AM_InvalidOp);
    m_opcodeinfo[0x99] = AssyInstruction(0x99, "STA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xa9] = AssyInstruction(0xa9, "LDA", AM_Immediate);
    m_opcodeinfo[0xb9] = AssyInstruction(0xb9, "LDA", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xc9] = AssyInstruction(0xc9, "CMP", AM_Immediate);
    m_opcodeinfo[0xd9] = AssyInstruction(0xd9, "CMP", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xe9] = AssyInstruction(0xe9, "SBC", AM_Immediate);
    m_opcodeinfo[0xf9] = AssyInstruction(0xf9, "SBC", AM_AbsoluteIndexedWithY);

    m_opcodeinfo[0x0a] = AssyInstruction(0x0a, "ASL", AM_Accumulator);
    m_opcodeinfo[0x1a] = AssyInstruction(0x1a, "???", AM_InvalidOp);
    m_opcodeinfo[0x2a] = AssyInstruction(0x2a, "ROL", AM_Accumulator);
    m_opcodeinfo[0x3a] = AssyInstruction(0x3a, "???", AM_InvalidOp);
    m_opcodeinfo[0x4a] = AssyInstruction(0x4a, "LSR", AM_Accumulator);
    m_opcodeinfo[0x5a] = AssyInstruction(0x5a, "???", AM_InvalidOp);
    m_opcodeinfo[0x6a] = AssyInstruction(0x6a, "ROR", AM_Accumulator);
    m_opcodeinfo[0x7a] = AssyInstruction(0x7a, "???", AM_InvalidOp);
    m_opcodeinfo[0x8a] = AssyInstruction(0x8a, "TXA", AM_Implied);
    m_opcodeinfo[0x9a] = AssyInstruction(0x9a, "TXS", AM_Implied);
    m_opcodeinfo[0xaa] = AssyInstruction(0xaa, "TAX", AM_Implied);
    m_opcodeinfo[0xba] = AssyInstruction(0xba, "TSX", AM_Implied);
    m_opcodeinfo[0xca] = AssyInstruction(0xca, "DEX", AM_Implied);
    m_opcodeinfo[0xda] = AssyInstruction(0xda, "???", AM_InvalidOp);
    m_opcodeinfo[0xea] = AssyInstruction(0xea, "NOP", AM_Implied);
    m_opcodeinfo[0xfa] = AssyInstruction(0xfa, "???", AM_InvalidOp);

    m_opcodeinfo[0x0b] = AssyInstruction(0x0b, "???", AM_InvalidOp);
    m_opcodeinfo[0x1b] = AssyInstruction(0x1b, "???", AM_InvalidOp);
    m_opcodeinfo[0x2b] = AssyInstruction(0x2b, "???", AM_InvalidOp);
    m_opcodeinfo[0x3b] = AssyInstruction(0x3b, "???", AM_InvalidOp);
    m_opcodeinfo[0x4b] = AssyInstruction(0x4b, "???", AM_InvalidOp);
    m_opcodeinfo[0x5b] = AssyInstruction(0x5b, "???", AM_InvalidOp);
    m_opcodeinfo[0x6b] = AssyInstruction(0x6b, "???", AM_InvalidOp);
    m_opcodeinfo[0x7b] = AssyInstruction(0x7b, "???", AM_InvalidOp);
    m_opcodeinfo[0x8b] = AssyInstruction(0x8b, "???", AM_InvalidOp);
    m_opcodeinfo[0x9b] = AssyInstruction(0x9b, "???", AM_InvalidOp);
    m_opcodeinfo[0xab] = AssyInstruction(0xab, "???", AM_InvalidOp);
    m_opcodeinfo[0xbb] = AssyInstruction(0xbb, "???", AM_InvalidOp);
    m_opcodeinfo[0xcb] = AssyInstruction(0xcb, "???", AM_InvalidOp);
    m_opcodeinfo[0xdb] = AssyInstruction(0xdb, "???", AM_InvalidOp);
    m_opcodeinfo[0xeb] = AssyInstruction(0xeb, "???", AM_InvalidOp);
    m_opcodeinfo[0xfb] = AssyInstruction(0xfb, "???", AM_InvalidOp);

    m_opcodeinfo[0x0c] = AssyInstruction(0x0c, "???", AM_InvalidOp);
    m_opcodeinfo[0x1c] = AssyInstruction(0x1c, "???", AM_InvalidOp);
    m_opcodeinfo[0x2c] = AssyInstruction(0x2c, "BIT", AM_Absolute);
    m_opcodeinfo[0x3c] = AssyInstruction(0x3c, "???", AM_InvalidOp);
    m_opcodeinfo[0x4c] = AssyInstruction(0x4c, "JMP", AM_Absolute);
    m_opcodeinfo[0x5c] = AssyInstruction(0x5c, "???", AM_InvalidOp);
    m_opcodeinfo[0x6c] = AssyInstruction(0x6c, "JMP", AM_AbsoluteIndirect);
    m_opcodeinfo[0x7c] = AssyInstruction(0x7c, "???", AM_InvalidOp);
    m_opcodeinfo[0x8c] = AssyInstruction(0x8c, "STY", AM_Absolute);
    m_opcodeinfo[0x9c] = AssyInstruction(0x9c, "???", AM_InvalidOp);
    m_opcodeinfo[0xac] = AssyInstruction(0xac, "LDY", AM_Absolute);
    m_opcodeinfo[0xbc] = AssyInstruction(0xbc, "LDY", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0xcc] = AssyInstruction(0xcc, "CPY", AM_Absolute);
    m_opcodeinfo[0xdc] = AssyInstruction(0xdc, "???", AM_InvalidOp);
    m_opcodeinfo[0xec] = AssyInstruction(0xec, "CPX", AM_Absolute);
    m_opcodeinfo[0xfc] = AssyInstruction(0xfc, "???", AM_InvalidOp);

    m_opcodeinfo[0x0d] = AssyInstruction(0x0d, "ORA", AM_Absolute);
    m_opcodeinfo[0x1d] = AssyInstruction(0x1d, "ORA", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x2d] = AssyInstruction(0x2d, "AND", AM_Absolute);
    m_opcodeinfo[0x3d] = AssyInstruction(0x3d, "AND", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x4d] = AssyInstruction(0x4d, "EOR", AM_Absolute);
    m_opcodeinfo[0x5d] = AssyInstruction(0x5d, "EOR", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x6d] = AssyInstruction(0x6d, "ADC", AM_Absolute);
    m_opcodeinfo[0x7d] = AssyInstruction(0x7d, "ADC", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x8d] = AssyInstruction(0x8d, "STA", AM_Absolute);
    m_opcodeinfo[0x9d] = AssyInstruction(0x9d, "STA", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0xad] = AssyInstruction(0xad, "LDA", AM_Absolute);
    m_opcodeinfo[0xbd] = AssyInstruction(0xbd, "LDA", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0xcd] = AssyInstruction(0xcd, "CMP", AM_Absolute);
    m_opcodeinfo[0xdd] = AssyInstruction(0xdd, "CMP", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0xed] = AssyInstruction(0xed, "SBC", AM_Absolute);
    m_opcodeinfo[0xfd] = AssyInstruction(0xfd, "SBC", AM_AbsoluteIndexedWithX);

    m_opcodeinfo[0x0e] = AssyInstruction(0x0e, "ASL", AM_Absolute);
    m_opcodeinfo[0x1e] = AssyInstruction(0x1e, "ASL", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x2e] = AssyInstruction(0x2e, "ROL", AM_Absolute);
    m_opcodeinfo[0x3e] = AssyInstruction(0x3e, "ROL", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x4e] = AssyInstruction(0x4e, "LSR", AM_Absolute);
    m_opcodeinfo[0x5e] = AssyInstruction(0x5e, "LSR", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x6e] = AssyInstruction(0x6e, "ROR", AM_Absolute);
    m_opcodeinfo[0x7e] = AssyInstruction(0x7e, "ROR", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0x8e] = AssyInstruction(0x8e, "STX", AM_Absolute);
    m_opcodeinfo[0x9e] = AssyInstruction(0x9e, "???", AM_InvalidOp);
    m_opcodeinfo[0xae] = AssyInstruction(0xae, "LDX", AM_Absolute);
    m_opcodeinfo[0xbe] = AssyInstruction(0xbe, "LDX", AM_AbsoluteIndexedWithY);
    m_opcodeinfo[0xce] = AssyInstruction(0xce, "DEC", AM_Absolute);
    m_opcodeinfo[0xde] = AssyInstruction(0xde, "DEC", AM_AbsoluteIndexedWithX);
    m_opcodeinfo[0xee] = AssyInstruction(0xee, "INC", AM_Absolute);
    m_opcodeinfo[0xfe] = AssyInstruction(0xfe, "INC", AM_AbsoluteIndexedWithX);

    m_opcodeinfo[0x0f] = AssyInstruction(0x0f, "???", AM_InvalidOp);
    m_opcodeinfo[0x1f] = AssyInstruction(0x1f, "???", AM_InvalidOp);
    m_opcodeinfo[0x2f] = AssyInstruction(0x2f, "???", AM_InvalidOp);
    m_opcodeinfo[0x3f] = AssyInstruction(0x3f, "???", AM_InvalidOp);
    m_opcodeinfo[0x4f] = AssyInstruction(0x4f, "???", AM_InvalidOp);
    m_opcodeinfo[0x5f] = AssyInstruction(0x5f, "???", AM_InvalidOp);
    m_opcodeinfo[0x6f] = AssyInstruction(0x6f, "???", AM_InvalidOp);
    m_opcodeinfo[0x7f] = AssyInstruction(0x7f, "???", AM_InvalidOp);
    m_opcodeinfo[0x8f] = AssyInstruction(0x8f, "???", AM_InvalidOp);
    m_opcodeinfo[0x9f] = AssyInstruction(0x9f, "???", AM_InvalidOp);
    m_opcodeinfo[0xaf] = AssyInstruction(0xaf, "???", AM_InvalidOp);
    m_opcodeinfo[0xbf] = AssyInstruction(0xbf, "???", AM_InvalidOp);
    m_opcodeinfo[0xcf] = AssyInstruction(0xcf, "???", AM_InvalidOp);
    m_opcodeinfo[0xdf] = AssyInstruction(0xdf, "???", AM_InvalidOp);
    m_opcodeinfo[0xef] = AssyInstruction(0xef, "???", AM_InvalidOp);
    m_opcodeinfo[0xff] = AssyInstruction(0xff, "???", AM_InvalidOp);


}
