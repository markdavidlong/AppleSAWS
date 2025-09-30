#pragma once

#include "Util.h"
#include "JumpLine.h"
#include "JumpLines.h"

#include <QPair>
#include <QMap>
#include <QMapIterator>
#include <QDebug>
#include <QList>

//////////////////

class JumpLineManager
{
public:
    explicit JumpLineManager(quint16 from = 0x0000, quint16 to = 0xffff);

    void addJump(quint16 src, quint16 dest, JumpLine::JumpType type, quint16 from = 0, quint16 to = 0xffff);
    void dumpJumps() const;

    [[nodiscard]] JumpLines buildJumpLines();
    [[nodiscard]] JumpLines getJumpLines() const noexcept { return m_jumplines; }

    void clear() noexcept { m_jumpmap.clear(); }
    void dumpJumpLines() const;

    [[nodiscard]] int getNumJumpLineChannels() const noexcept { return m_jumplines.m_maxChannel; }

protected:
    [[nodiscard]] int findBestChannel(JumpLine& jl);
    void setChannelForJumpLine(int channel, JumpLine& jl);

private:
    quint16 m_start;
    quint16 m_end;
    JumpLine::JumpMap m_jumpmap;
    JumpLines m_jumplines;
    QMap<quint16, QList<int>> m_channelsAtAddress;

    [[nodiscard]] bool doJumpsIntersect(JumpLine::TJump& A, JumpLine::TJump& B) const noexcept;
    [[nodiscard]] bool isLineWithinRange(quint16 line, JumpLine::TJump& jm) const noexcept;
    void dumpJumps(JumpLine::JumpMap map) const;
};
