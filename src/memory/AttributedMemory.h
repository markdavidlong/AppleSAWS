#pragma once

#include "MemoryCell.h"


class AttributedMemory
{
public:
    explicit AttributedMemory(quint16 expectedBottom = 0x0000,
                              quint16 expectedTop = 0xffff);

    void setExpectedRange(quint16 bottom, quint16 top) noexcept;

    [[nodiscard]] constexpr quint16 getExpectedBottom() const noexcept { return m_expected_bottom; }
    [[nodiscard]] quint16 getExpectedTop() const noexcept;
    [[nodiscard]] constexpr quint16 getExpectedSize() const noexcept { return m_expected_top - m_expected_bottom; }

    bool setValueAt(quint16 address, quint8 withValue, MemRole* andRole = nullptr);
    [[nodiscard]] quint8 valueAt(quint16 address) const { return m_cells[address].value(); }
    bool setRoleAt(quint16 address, MemRole* withRole);
    [[nodiscard]] bool hasRoleAt(quint16 address, int withId) const;
    [[nodiscard]] MemRole* getRoleAt(quint16 address, int withId);
    [[nodiscard]] QList<MemRole*> getAllRolesAt(quint16 address);
    bool removeRoleAt(quint16 address, int withId);
    bool replaceRoleAt(quint16 address, MemRole* withRole);

    [[nodiscard]] QByteArray getAllValues() const;
    [[nodiscard]] QByteArray getAllValuesInExpectedRange() const;
    [[nodiscard]] QByteArray getAllValuesInRange(quint16 bottom, quint16 top) const;

    [[nodiscard]] quint8 at(quint16 address) const { return m_cells[address].value(); }

    bool addFile(const QByteArray& data, quint16 start);  // From Memory.h.  Should be replaced?

protected:
    quint16 m_expected_bottom{0x0000};
    quint16 m_expected_top{0xffff};
    QList<MemoryCell> m_cells;
};

