#pragma once

#include <QtGlobal>
#include <QPair>
#include <QMap>

class JumpLine
{
public:
    enum class JumpType
    {
        Unknown,
        JMP,
        Branch,
        JSR,
        BRA
    };

    using TJump = QPair<quint16, quint16>;
    using JumpMap = QMap<TJump, JumpType>;

    constexpr JumpLine() noexcept = default;
    
    explicit JumpLine(quint16 fromAddr, quint16 toAddr, JumpType jumpType = JumpType::Unknown, int ch = -1) noexcept
        : from(fromAddr), to(toAddr), channel(ch), type(jumpType) {}

    quint16 from{0};
    quint16 to{0};
    int channel{-1};
    JumpType type{JumpType::Unknown};

    [[nodiscard]] constexpr quint16 min() const noexcept { return qMin(from, to); }
    [[nodiscard]] constexpr quint16 max() const noexcept { return qMax(from, to); }
    [[nodiscard]] constexpr bool isUp() const noexcept { return (from > to); }
    [[nodiscard]] constexpr bool isDown() const noexcept { return !isUp(); }
    
    // Comparison operators for container usage
    [[nodiscard]] constexpr bool operator==(const JumpLine& other) const noexcept {
        return from == other.from && to == other.to && type == other.type;
    }
    
    [[nodiscard]] constexpr bool operator<(const JumpLine& other) const noexcept {
        if (from != other.from) return from < other.from;
        if (to != other.to) return to < other.to;
        return static_cast<int>(type) < static_cast<int>(other.type);
    }
};