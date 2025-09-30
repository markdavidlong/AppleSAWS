#pragma once

#include "Util.h"

#include <QList>
#include <QChar>
#include <QString>
#include <QByteArray>

class AppleChar final {
public:
    constexpr AppleChar() noexcept = default;
    explicit constexpr AppleChar(quint8 ch) noexcept : m_val(ch) {}

    [[nodiscard]] QChar printable() const { return printable(m_val); }

    [[nodiscard]] TextAttribute getAttribute() const { return getAttribute(m_val); }
    [[nodiscard]] TextSet getTextSet() const { return getTextSet(m_val); }

    [[nodiscard]] static TextAttribute getAttribute(quint8 val);
    [[nodiscard]] static TextSet getTextSet(quint8 val);
    [[nodiscard]] static QChar printable(quint8 val);
    [[nodiscard]] static QString getHtmlString(quint8 val);

    [[nodiscard]] constexpr quint8 value() const noexcept { return m_val; }
    
    // Comparison operators
    [[nodiscard]] constexpr auto operator<=>(const AppleChar& other) const noexcept = default;

private:
    quint8 m_val{0};
};

class AppleString final : public QByteArray {
public:
    using QByteArray::QByteArray;
    
    void setData(const QByteArray& data) { insert(0, data); }
    
    [[nodiscard]] QString printable() const;
    [[nodiscard]] QList<TextAttribute> attributes() const;
};


