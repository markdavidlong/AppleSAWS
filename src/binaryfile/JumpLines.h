#pragma once

#include "JumpLine.h"

#include <QList>
#include <QMap>

class JumpLines
{
public:
    // Type aliases for convenience and compatibility
    using TJump = JumpLine::TJump;
    using JumpType = JumpLine::JumpType;
    using JumpMap = JumpLine::JumpMap;

    constexpr JumpLines() noexcept : m_maxChannel(0) {}

    [[nodiscard]] QList<int> channelsAtAddress(quint16 address) const {
        return m_channelsAtAddress.value(address);
    }

    [[nodiscard]] QList<JumpLine> jumpLinesAtAddress(quint16 addrs) const;

    // Public data members (maintaining existing interface)
    QList<JumpLine> jumpLines;
    int m_maxChannel;
    QMap<quint16, QList<int>> m_channelsAtAddress;

    // Additional utility methods
    [[nodiscard]] bool isEmpty() const noexcept { return jumpLines.isEmpty(); }
    [[nodiscard]] int count() const noexcept { return jumpLines.count(); }
    void clear() noexcept { 
        jumpLines.clear(); 
        m_channelsAtAddress.clear(); 
        m_maxChannel = 0; 
    }
};