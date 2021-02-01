
#include "opcodes.h"
#include "disassembler.h"


#include <QByteArray>
#include <QDebug>
#include <QList>
#include <math.h>


Disassembler::Disassembler(QByteArray memimage)
{
    m_memimage = memimage;
    m_memusagemap.clearData();
}


QList<DisassembledItem> Disassembler::disassemble(quint16 from, quint16 to,
                                                  QList<quint16> entryPoints,
                                                  bool processRecursively) {
    m_from = from;
    m_to = to;
    QList<DisassembledItem> retval;
    qDebug() << "\n\n*****************\n\nDisassemble: From"<<uint16ToHex(from)<<"to"<<uint16ToHex(to);

    MemoryUsageMap memUse;

    bool stopping = false;
    quint16 next = from;
    while (entryPoints.count())
    {
        next = entryPoints.takeFirst();
        m_stack.push(next);
    }

    while (!stopping)
    {
        //   quint8 opcode = m_memimage[next];
        //   qDebug() << "Opcode: " << uint8ToHex(opcode);
        DisassembledItem item;
        bool ok = false;

        if (next >= from && next <= to)  //TODO: Remove this to not limit disassembly to program range
            ok = disassembleOp(next,item,&memUse);

        if (ok)
        {
            retval.append(item);

            quint16 flow  = item.nextFlowAddress();
            qDebug() << uint16ToHex(next) << uint16ToHex(flow);
            if (item.isBranch())
            {
                qDebug() << "Is Branch";
                m_stack.push(item.targetAddress());
            }

            if (item.isJsr() && !item.canNotFollow())
            {
                if (item.targetAddress() <= to && item.targetAddress() >= from) //TODO: Remove this to not limit disassembly to program range
                {
                    if (m_stack.push(item.targetAddress()))
                    {
                        qDebug() << "Appending" << uint16ToHex(item.targetAddress()) << "to jump table";
                    }
                    else
                    {
                        qDebug() << "Not adding" << uint16ToHex(item.targetAddress()) << "to jump table";
                    }
                }
            }

            if (next <= to) //TODO: Remove this to not limit disassembly to program range
            {
                next = flow;
                stopping = item.stopsProcessing();
            }
            else stopping = true;
        }
        else
        {
            stopping = true; // already processed this address
        }

        if (next >= to) stopping = true;
    }

    m_memusagemap.merge(memUse);

    if (processRecursively)
        while (!m_stack.isEmpty())
        {
            quint16 num = m_stack.pop();
            if (!m_memusagemap[num].testFlag(Operation))
            {
                if (num >= from && num <= to) // TODO: remove this to not limit disassembly to program range
                {
                    qDebug() << "Calling recursively to"<<uint16ToHex(num);
                    retval.append(disassemble(from,to,QList<quint16>() << num,false));
                    qDebug() << "Return from recursive call.";
                }
            }
        }

    std::sort(retval.begin(),retval.end());

    if (processRecursively)
    {
      m_jumplines = m_jlm.buildJumpLines();
      qDebug() << "Num Channels: " << m_jlm.getNumJumpLineChannels();
    }

    return retval;
}

bool Disassembler::disassembleOp(quint16 address, DisassembledItem &retval, MemoryUsageMap *memuse)
{

    if (memuse)
    {
        if ((*memuse)[address].testFlag(Operation)) return false;
    }

    quint8 opcode = m_memimage[address];
    AssyInstruction op = OpCodes::getAssyInstruction(opcode);
    retval.setInstruction(op);

    if (opcode == 0x6C || opcode == 0x7c) // Indirect jumps
    {
        m_jlm.addJump(address,address,IsUnknownJump,m_from,m_to);
        retval.setCanNotFollow(true);
    }

    QString disassemblyLine;
    QString hexValueString;
    QByteArray hexValues;
    hexValues.append(opcode);

    // Prepare Op Code arguments

    for (int idx = 1; idx < op.numArgs()+1; idx++) {
        quint8 val = m_memimage[address+idx];
        hexValues.append(val);
    }
    if (memuse)
    {
        (*memuse)[address].setFlag(Operation);
        for (int idx = 1; idx < op.numArgs()+1; idx++)
        {
            (*memuse)[address+idx].setFlag(OperationArg);
        }
    }

    quint16 argval = 0;
    if (op.numArgs() == 1)
        argval = (quint8) hexValues[1];
    else if (op.numArgs() == 2)
    {
        argval = makeWord(hexValues[1],hexValues[2]);
    }



    for (int idx = 0; idx < hexValues.length(); idx++) {
        hexValueString.append(QString("%1 ").arg((quint8) hexValues[idx],2,16,QChar('0')));
    }
    
    retval.setNextContiguousAddress(address+1+op.numArgs());
    retval.setNextFlowAddress(address+1+op.numArgs());

    // Disassemble instruction
    switch (op.addressMode()) {
        case AM_InvalidOp: {
            disassemblyLine = op.mnemonic();
            retval.setIsInvalidOp(true);
            break;
        }
        case AM_Absolute:{
            disassemblyLine = QString("%1 _ARG16_").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_AbsoluteIndexedIndirect:{
            disassemblyLine = QString("%1 (_ARG16_,x)").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_AbsoluteIndexedWithX:{
            disassemblyLine = QString("%1 _ARG16_,x").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_AbsoluteIndexedWithY:{
            disassemblyLine = QString("%1 _ARG16_,y").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_AbsoluteIndirect:{
            disassemblyLine = QString("%1 (_ARG16_)").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_Immediate:{
            disassemblyLine = QString("%1 #%2").arg(op.mnemonic()).arg((quint8) hexValues[1],2,16,QChar('0')).toUpper();
            retval.setRawArgument(argval);
            break;
        }
        case AM_Implied:{
            disassemblyLine = op.mnemonic();
            break;
        }
        case AM_Accumulator:{
            disassemblyLine = op.mnemonic();
            break;
        }
        case AM_ProgramCounterRelative:{
            qint8 offset = (qint8) hexValues[1];
            quint16 offsetAddress = address+2+offset;

            retval.setTargetAddress(offsetAddress);
            if (opcode == 0x80) // BRA
                m_jlm.addJump(address,offsetAddress,IsBRA,m_from,m_to);
            else
                m_jlm.addJump(address,offsetAddress,IsBranch,m_from,m_to);

            disassemblyLine = QString("%1 _ARG16_ {%2%3}").arg(op.mnemonic())
                              .arg((offset<0)?"-":"+")
                              .arg(abs(offset));
            retval.setRawArgument(offsetAddress);
            break;
        }
        case AM_ZeroPage:{
            disassemblyLine = QString("%1 _ARG8_").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_ZeroPageIndirectIndexedWithY:{
            disassemblyLine = QString("%1 (_ARG8_),Y").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_ZeroPageIndexedIndirect:{
            disassemblyLine = QString("%1 (_ARG8_,x)").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_ZeroPageIndexedWithX:{
            disassemblyLine = QString("%1 _ARG8_,x").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_ZeroPageIndexedWithY:{
            disassemblyLine = QString("%1 _ARG8_,y").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        case AM_ZeroPageIndirect:{
            disassemblyLine = QString("%1 (_ARG8_)").arg(op.mnemonic());
            retval.setRawArgument(argval);
            break;
        }
        default:{
            disassemblyLine = op.mnemonic();
            retval.setIsInvalidOp(true);
            qDebug() << "Unhandled Address Mode: " << op.addressMode();
            break;
        }
    }

    if (opcode == 0x20 || opcode == 0x4c) {
        retval.setTargetAddress(makeWord(hexValues[1],hexValues[2]));

        if (opcode == 0x4c) // JMP
        {
            qDebug() << "JMP: Adding flow address "
                     << uint16ToHex(makeWord(hexValues[1],hexValues[2])) << "to jump table";
            m_jlm.addJump(address,argval,IsJMP,m_from,m_to);
            m_stack.push(argval);
            retval.setCanNotFollow(true);
        }
        else // JSR
        {
            m_jlm.addJump(address,argval,IsJSR,m_from,m_to);
        }
    }

    retval.setAddress(address);
    retval.setDisassembledString(disassemblyLine.toUpper());
    retval.setHexString(hexValueString.trimmed().toUpper().leftJustified(12,' '));
    retval.setHexValues(hexValues);

    return true;
}


void Disassembler::setUnknownToData(quint16 from, quint16 to)
{
    for (int idx = from; idx <= to; idx++)
    {
        if (m_memusagemap[idx].testFlag(Unknown))
        {
            m_memusagemap[idx].setFlag(Data);
        }
    }
}

DisassembledItem::DisassembledItem(AssyInstruction instr) {
    m_canNotFollow = false;
    setInstruction(instr);
}

void DisassembledItem::setInstruction(AssyInstruction instr) {
    m_instruction = instr;
    //    qDebug() << "Set instruction: " << uint8ToHex(instr.opcode());
    //    qDebug() << " Copied instr:" << m_instruction.debugStr();
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

QString DisassembledItem::disassembledString() {
    QString retval = rawDisassembledString();
    if (hasArg()) {
        if (retval.contains("_ARG16_")) {
            retval.replace("_ARG16_","$"+arg16Str());
        } else if (retval.contains("_ARG8_")) {
            retval.replace("_ARG8_","$"+arg8Str());
        }
    }
    return retval;
}

void DisassembledItem::init() {
    m_address = m_target_address = 0;
    m_nextContiguousAddress = 0;
    m_nextFlowAddress = 0;
    m_is_jump = m_is_branch = m_is_jsr = false;
    m_unknown_ta = true;
    m_raw_arg = 0;
    m_has_arg = false;
    m_canNotFollow = false;
    m_isInvalidOp = false;

}
