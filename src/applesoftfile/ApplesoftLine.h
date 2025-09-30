#pragma once

#include "ApplesoftToken.h"

#include <QtGlobal>
#include <QList>

struct ApplesoftLine {
    qint16 address{0};
    quint16 next_address{0};
    quint16 linenum{0};
    QList<ApplesoftToken> tokens{};
    
    // Applesoft BASIC maximum line number
    static constexpr quint16 MAX_LINE_NUMBER = 63999;
    
    // Utility methods for modern C++
    [[nodiscard]] constexpr bool isValid() const noexcept { return linenum <= MAX_LINE_NUMBER; }
    [[nodiscard]] bool isEmpty() const noexcept { return tokens.isEmpty(); }
};


