#include "MemoryUsageMap.h"

#include <QDebug>

MemoryUsageMap::MemoryUsageMap() : m_data(65536, UsageRole::Unknown)
{
}

void MemoryUsageMap::clearData()
{
    for (int idx = 0; idx < m_data.size(); idx++)
    {
        m_data[idx] = UsageRole::Unknown;
    }
}

void MemoryUsageMap::merge(const MemoryUsageMap &other)
{
    if (other.m_data.size() != m_data.size())
    {
        qWarning("Mismatched size of MemoryMaps!");
        return;
    }
    for (int idx = 0; idx < m_data.size(); idx++)
    {
        m_data[idx] |= other.m_data[idx];
    }
}

QList<quint16> MemoryUsageMap::addressesWhichContain(UsageRole usage) const
{
    QList<quint16> retval;
    for (int idx = 0; idx < m_data.size(); idx++)
    {
        if (m_data.value(idx).testFlag(usage))
        {
            retval.append(static_cast<quint16>(idx));
        }
    }
    return retval;
}

// Essential interface methods for accessing the data
int MemoryUsageMap::size() const noexcept
{
    return m_data.size();
}

bool MemoryUsageMap::isEmpty() const noexcept
{
    return m_data.isEmpty();
}

MemoryUsageMap::UsageRoles& MemoryUsageMap::operator[](int i)
{
    return m_data[i];
}

const MemoryUsageMap::UsageRoles& MemoryUsageMap::operator[](int i) const
{
    return m_data[i];
}

MemoryUsageMap::UsageRoles MemoryUsageMap::at(int i) const
{
    return m_data.at(i);
}

MemoryUsageMap::UsageRoles MemoryUsageMap::value(int i) const
{
    return m_data.value(i);
}

MemoryUsageMap::UsageRoles MemoryUsageMap::value(int i, const UsageRoles& defaultValue) const
{
    return m_data.value(i, defaultValue);
}