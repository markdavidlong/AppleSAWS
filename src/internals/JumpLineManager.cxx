#include "JumpLineManager.h"

JumpLineManager::JumpLineManager(quint16 from, quint16 to)
    : m_start(from),
      m_end(to)
{
    //qDebug() << "JumpLineManager(from:"<<uint16ToHex(m_start)<<", to:"<<uint16ToHex(m_end) << ")";

}

void JumpLineManager::addJump(quint16 src, quint16 dest, JumpType type, quint16 from, quint16 to)
{
    TJump jump(src,dest);
    if (src >= from && src <= to && dest >= from && dest <= to)
    {
        if (!m_jumpmap.contains(jump))
        {
            //       qDebug() << "JumpLineManager::addJump: Added Jump from" << uint16ToHex(src) << "to" << uint16ToHex(dest) << ", Type:" << type;
            m_jumpmap.insert(jump,type);
        }
        else
        {
            //qDebug() << "JumpLineManager::addJump: Not adding duplicate jump:" << uint16ToHex(src) << "," << uint16ToHex(dest);
        }
    }
    else
    {
   //     qDebug() << "JumpLineManager::addJump: Address range is out of bounds";

    }
}

void JumpLineManager::dumpJumps() const {
    dumpJumps(m_jumpmap);
}

void JumpLineManager::dumpJumps(JumpMap map) const
{
    //qDebug() << "JumpLineManager::dumpJumps()\n  JumpTable:";
    QMapIterator<TJump,JumpType> it(map);
    while (it.hasNext())
    {
        it.next();
        QString jumptypelabel;
        if (it.value() == IsUnknownJump) { jumptypelabel = "Unknown Jump"; }
        if (it.value() == IsJMP) { jumptypelabel = "JMP"; }
        if (it.value() == IsBranch) { jumptypelabel = "Branch"; }
        if (it.value() == IsJSR) { jumptypelabel = "JSR"; }
        if (it.value() == IsBRA) { jumptypelabel = "BRA"; }
        //qDebug() << "    Jump from" << uint16ToHex(it.key().first) << "to"
         //        << uint16ToHex(it.key().second) << jumptypelabel;
    }
}

JumpLines JumpLineManager::buildJumpLines()
{
//qDebug() << "A";

    m_channelsAtAddress.clear();
    m_jumplines.m_maxChannel = 0;
    QMapIterator<TJump, JumpType> it(m_jumpmap);
    while (it.hasNext())
    {
        it.next();
        TJump range = it.key();

        JumpLine jl;
        jl.type = it.value();
        jl.from = range.first;
        jl.to = range.second;
        int channel = findBestChannel(jl);
        setChannelForJumpLine(channel,jl);

        m_jumplines.jumpLines.append(jl);
        m_jumplines.m_maxChannel = qMax(m_jumplines.m_maxChannel, channel);
    }
    //qDebug() << "A";


    return m_jumplines;

}



int JumpLineManager::findBestChannel(JumpLine &jl)
{
    //qDebug() << "findBestChannel()";
    if (m_jumplines.jumpLines.count() == 0)
    {
        return 0;
    }

    int potentialChannel = 0;
    bool foundChannel = false;
    while (!foundChannel)
    {
        //qDebug() << "Tryning potential channel" << potentialChannel;
        bool matched = false;
        for (quint16 addr = jl.min(); addr <= jl.max(); addr++)
        {
            // If any of these addresses contain the potential channel, move on
            if (m_channelsAtAddress[addr].contains(potentialChannel))
            {
                matched = true;
            }
        }
        if (matched)
        {
            potentialChannel++;
            matched = false;
        }
        else
        {
            foundChannel = true;
        }
    }
    return potentialChannel;
}

void JumpLineManager::setChannelForJumpLine(int channel, JumpLine &jl)
{
    jl.channel = channel;
    for (quint16 addr = jl.min(); addr <= jl.max(); addr++)
    {
        m_channelsAtAddress[addr].append(channel);
    }
}

void JumpLineManager::dumpJumpLines() const
{
    //foreach (JumpLine jl, m_jumplines.jumpLines)
   // {
        //qDebug() << " JumpLine from:" << uint16ToHex(jl.from)
         //        << " to:" << uint16ToHex(jl.to)
          //       << "channel: " << jl.channel << " type:" << jl.type;
   // }
}

bool JumpLineManager::doJumpsIntersect(TJump &A, TJump &B) const
{

    if (A == B) return false;
    if (isLineWithinRange(A.first,B) || isLineWithinRange(A.second,B))
        return true;

    if (isLineWithinRange(B.first,A) || isLineWithinRange(B.second,A))
        return true;

    return false;

}

bool JumpLineManager::isLineWithinRange(quint16 line, TJump &jm) const
{
    quint16 min = qMin(jm.first,jm.second);
    quint16 max = qMax(jm.first,jm.second);

    return (line > min && line < max);
}

QList<JumpLine> JumpLines::jumpLinesAtAddress(quint16 addrs)
{
    QList<JumpLine> list;
    foreach (JumpLine jl, jumpLines)
    {
        if (addrs >= jl.min() && addrs <= jl.max())
        {
            list.append(jl);
        }
    }
    return list;
}
