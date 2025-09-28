#pragma once

#include "MemRole.h"

#include <QtGlobal>
#include <QList>
#include <QHash>

class MemoryCell
{
public:
    MemoryCell(quint8 val = 0);
    virtual ~MemoryCell();

    void setAddress(quint16 address) { m_address = address; }
    quint16 address() const { return m_address; }

    bool setRole(MemRole *role);
    MemRole *getRole(int id);
    bool hasRole(int id) const;
    bool removeRole(int id);

    void setValue(quint8 val) { m_value = val; }
    quint8 value() const { return m_value; }
    operator quint8() const { return m_value; }

    QList<MemRole *> getAllRoles();

private:
    quint8 m_value;
    quint16 m_address;

    QHash<int,MemRole *> m_roles;
};

