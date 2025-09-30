
#include "Disassembler.h"
#include "OpCodes.h"
#include "AttributedMemory.h"
#include "RoleAsmOpcode.h"
#include "RoleAsmOperand.h"

#include <QDebug>
#include <math.h>


Disassembler::Disassembler(AttributedMemory &mem)
{
    m_mem = &mem;
    m_memusagemap.clearData();
}

QList<DisassembledItem> Disassembler::disassemble(quint16 from, quint16 to,
                                                  QList<quint16> entryPoints,
                                                  bool processRecursively) {
    m_from = from;
    m_to = to;
    QList<DisassembledItem> retval;

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
        DisassembledItem item;
        bool ok = false;

        if (next >= from && next <= to)  //TODO: Remove this to not limit disassembly to program range
        {
            ok = disassembleOp(next,item,&memUse);
        }   

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
                    if (!m_stack.contains(item.targetAddress()))
                    {
                        m_stack.push(item.targetAddress());
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
            if (!m_memusagemap[num].testFlag(MemoryUsage::Operation))
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
        if ((*memuse)[address].testFlag(MemoryUsage::Operation)) return false;
    }

    quint8 opcode = m_mem->at(address);

    retval.setOpcode(opcode);


    if (!m_mem->hasRoleAt(address,RoleAsmOpcode::RoleID))
    {
        auto opRole = new RoleAsmOpcode();
        if (!m_mem->setRoleAt(address,opRole))
        {
            qWarning("Cannot set role %s at address %s for opcode %s",
                     qPrintable(opRole->name()),
                     qPrintable(uint16ToHex(address)),
                     qPrintable(uint8ToHex(opcode)));
            delete opRole;
        }
    }

    if (OpCodes::isIndirectJump(opcode)) // Indirect jumps
    {
        m_jlm.addJump(address,address,IsUnknownJump,m_from,m_to);
        retval.setCanNotFollow(true);
    }

    QString disassemblyLine;
    QString hexValueString;
    QByteArray hexValues;
    hexValues.append(opcode);

    // Prepare Op Code arguments

    auto numArgs =  OpCodes::numArgs(opcode);
    for (int idx = 1; idx < numArgs+1; idx++) {
        qDebug() << "Opcode at " << uint16ToHex(idx + address);
        auto oprndRole = new RoleAsmOperand();
        if (numArgs == 1)
        {
            oprndRole->setOperandType(RoleAsmOperand::Type::Byte);
        }
        else {
            if (idx == 1)
            {
                oprndRole->setOperandType(RoleAsmOperand::Type::WordLo);
            }
            else // idx == 2
            {
                oprndRole->setOperandType(RoleAsmOperand::Type::WordHi);
            }
        }
        if (!m_mem->setRoleAt(address+idx,oprndRole))
        {
            qWarning(">> Cannot set role %s at address %s",
                     qPrintable(oprndRole->name()),
                     qPrintable(uint16ToHex(address)));
            delete oprndRole;
        }

        quint8 val = m_mem->at(address+idx);
        hexValues.append(val);
    }

    if (memuse)
    {
        (*memuse)[address].setFlag(MemoryUsage::Operation);
        for (int idx = 1; idx < OpCodes::numArgs(opcode)+1; idx++)
        {
            (*memuse)[address+idx].setFlag(MemoryUsage::OperationArg);
        }
    }

    quint16 argval = 0;
    if (numArgs == 1)
    {
        argval = (quint8) hexValues[1];
    }
    else if (numArgs == 2)
    {
        argval = makeWord(hexValues[1],hexValues[2]);
    }

    for (int idx = 0; idx < hexValues.length(); idx++) {
        hexValueString.append(QString("%1 ").arg(uint8ToHex(hexValues[idx])));
    }

    retval.setNextContiguousAddress(address+1+numArgs);
    retval.setNextFlowAddress(address+1+numArgs);

    QString mnemonic = OpCodes::mnemonic(opcode);
    // Disassemble instruction
    switch (OpCodes::addressMode(opcode)) {
    case AM_InvalidOp: {
        disassemblyLine = OpCodes::mnemonic(opcode);
        retval.setIsInvalidOp(true);
        break;
    }
    case AM_Absolute:{
        disassemblyLine = QString("%1 _ARG16_").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_AbsoluteIndexedIndirect:{
        disassemblyLine = QString("%1 (_ARG16_,x)").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_AbsoluteIndexedWithX:{
        disassemblyLine = QString("%1 _ARG16_,x").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_AbsoluteIndexedWithY:{
        disassemblyLine = QString("%1 _ARG16_,y").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_AbsoluteIndirect:{
        disassemblyLine = QString("%1 (_ARG16_)").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_Immediate:{
        disassemblyLine = QString("%1 #%2").arg(mnemonic).arg((quint8) hexValues[1],2,16,QChar('0')).toUpper();
        retval.setRawArgument(argval);
        break;
    }
    case AM_Implied:{
        disassemblyLine = mnemonic;
        break;
    }
    case AM_Accumulator:{
        disassemblyLine = mnemonic;
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

        disassemblyLine = QString("%1 _ARG16_ {%2%3}").arg(mnemonic)
                .arg((offset<0)?"-":"+")
                .arg(abs(offset));
        retval.setRawArgument(offsetAddress);
        break;
    }
    case AM_ZeroPage:{
        disassemblyLine = QString("%1 _ARG8_").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_ZeroPageIndirectIndexedWithY:{
        disassemblyLine = QString("%1 (_ARG8_),Y").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_ZeroPageIndexedIndirect:{
        disassemblyLine = QString("%1 (_ARG8_,x)").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_ZeroPageIndexedWithX:{
        disassemblyLine = QString("%1 _ARG8_,x").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_ZeroPageIndexedWithY:{
        disassemblyLine = QString("%1 _ARG8_,y").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    case AM_ZeroPageIndirect:{
        disassemblyLine = QString("%1 (_ARG8_)").arg(mnemonic);
        retval.setRawArgument(argval);
        break;
    }
    default:{
        disassemblyLine = mnemonic;
        retval.setIsInvalidOp(true);
        qDebug() << "Unhandled Address Mode: " << mnemonic;
        break;
    }
    }

    if (opcode == 0x20 || opcode == 0x4c) // JSR / JMP
    {
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
        if (m_memusagemap[idx].testFlag(MemoryUsage::Unknown))
        {
            m_memusagemap[idx].setFlag(MemoryUsage::Data);
        }
    }
}
