#pragma once

#include <QFlag>
#include <QFlags>
#include <QList>


enum MemoryUsage {
    Unknown            = 0x00000000,
    Data               = 0x00000001,
    Operation          = 0x00000002,
  //  OperationArgHi     = 0x00000004,
  //  OperationArgLo     = 0x00000008,
    OperationArg       = 0x00000010,

    RefAddressHi       = 0x00000100,
    RefAddressLo       = 0x00000200,
    RefAddress         = RefAddressLo | RefAddressHi,
    ZeroPageRefAddress = 0x00000400,

    InvalidOperation   = 0x00001000,
    Break              = 0x00002000,
    UndeterminedJump   = 0x00004000,
    StopsExecution     = InvalidOperation | Break | UndeterminedJump,

    EntryPointAddr     = 0x00010000,
    BranchOffsetAddr   = 0x00020000,
    Jump               = 0x00040000,
    Return             = 0x00080000,
};
Q_DECLARE_FLAGS(MemoryUsages,MemoryUsage)


class MemoryUsageMap
{
private:
    QList<MemoryUsages> m_data;
    
public:
    MemoryUsageMap()
    {
        m_data.fill(MemoryUsage::Unknown,65536);
    }

    void clearData()
    {
        for (int idx = 0; idx < m_data.size(); idx++)
        {
            m_data[idx] = MemoryUsage::Unknown;
        }
    }

    void merge(const MemoryUsageMap &other)
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

    QList<quint16> addressesWhichContain(MemoryUsage usage)
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
    inline int size() const { return m_data.size(); }
    inline bool isEmpty() const { return m_data.isEmpty(); }
    inline MemoryUsages& operator[](int i) { return m_data[i]; }
    inline const MemoryUsages& operator[](int i) const { return m_data[i]; }
    inline MemoryUsages at(int i) const { return m_data.at(i); }
    inline MemoryUsages value(int i) const { return m_data.value(i); }
    inline MemoryUsages value(int i, const MemoryUsages& defaultValue) const { return m_data.value(i, defaultValue); }

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MemoryUsages)

