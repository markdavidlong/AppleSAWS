#pragma once

#include "MemRole.h"

#include <QtGlobal>
#include <QList>
#include <QHash>

class MemoryCell
{
public:
    explicit MemoryCell(quint8 val = 0) noexcept : m_value(val) {}
    ~MemoryCell();

    void setAddress(quint16 address) noexcept { m_address = address; }
    [[nodiscard]] constexpr quint16 address() const noexcept { return m_address; }

    bool setRole(MemRole* role);
    [[nodiscard]] MemRole* getRole(int id);
    [[nodiscard]] bool hasRole(int id) const;
    bool removeRole(int id);

    void setValue(quint8 val) noexcept { m_value = val; }
    [[nodiscard]] constexpr quint8 value() const noexcept { return m_value; }
    [[nodiscard]] constexpr operator quint8() const noexcept { return m_value; }

    [[nodiscard]] QList<MemRole*> getAllRoles();

private:
    quint8 m_value{0};
    quint16 m_address{0};
    QHash<int, MemRole*> m_roles;
};

