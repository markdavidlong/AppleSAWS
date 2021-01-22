#ifndef JUMPLINEMANAGER_H
#define JUMPLINEMANAGER_H

#include <QPair>
#include <QMap>
#include <QMapIterator>
#include <QDebug>

#include "util.h"

typedef QPair<quint16,quint16> TJump;
typedef enum {
    IsUnknownJump,
    IsJMP,
    IsBranch,
    IsJSR,
    IsBRA
} JumpType;
typedef QMap<TJump,JumpType> JumpMap;

class JumpLine {
public:
    JumpLine() : from(0), to(0), channel(-1), type(IsUnknownJump) { }
    quint16 from;
    quint16 to;
    int channel;
    JumpType type;

    inline quint16 min() const { return qMin(from,to); }
    inline quint16 max() const { return qMax(from,to); }
    inline bool isUp() const { return (from > to); }
    inline bool isDown() const { return !isUp(); }
};

//////////////////

class JumpLines
{
public:
    JumpLines() : m_maxChannel(0)
    {

    }

    QList<int> channelsAtAddress(quint16 address)
    {
       return m_channelsAtAddress[address];
    }

    QList<JumpLine> jumpLinesAtAddress(quint16 addrs);

    QList<JumpLine> jumpLines;

    int m_maxChannel;

    QMap<quint16, QList<int> > m_channelsAtAddress;
};

//////////////////

class JumpLineManager
{
public:
    JumpLineManager(quint16 from = 0x0000, quint16 to = 0xffff);

    void addJump(quint16 src, quint16 dest, JumpType type,quint16 from = 0, quint16 to = 0xffff);
    void dumpJumps() const;

    JumpLines buildJumpLines();
    JumpLines getJumpLines() const { return m_jumplines; }



    void clear() { m_jumpmap.clear(); }
    void dumpJumpLines() const;

    int getNumJumpLineChannels() const { return m_jumplines.m_maxChannel; }

protected:
    int findBestChannel(JumpLine &jl);

    void setChannelForJumpLine(int channel, JumpLine &jl);

private:
    quint16 m_start;
    quint16 m_end;

    JumpMap m_jumpmap;

    JumpLines m_jumplines;

    QMap<quint16, QList<int>> m_channelsAtAddress;


    bool doJumpsIntersect(TJump &A, TJump &B) const;
    bool isLineWithinRange(quint16 line, TJump &jm) const;

    void dumpJumps(JumpMap map) const;

};

#endif // JUMPLINEMANAGER_H
