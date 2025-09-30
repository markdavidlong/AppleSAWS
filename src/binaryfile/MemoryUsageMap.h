#pragma once

#include <QFlag>
#include <QFlags>
#include <QList>


enum class MemoryUsage : quint32 {
    Unknown            = 0x00000000,
    Data               = 0x00000001,
    Operation          = 0x00000002,
  //  OperationArgHi     = 0x00000004,
  //  OperationArgLo     = 0x00000008,
    OperationArg       = 0x00000010,

    RefAddressHi       = 0x00000100,
    RefAddressLo       = 0x00000200,
    RefAddress         = static_cast<quint32>(RefAddressLo) | static_cast<quint32>(RefAddressHi),
    ZeroPageRefAddress = 0x00000400,

    InvalidOperation   = 0x00001000,
    Break              = 0x00002000,
    UndeterminedJump   = 0x00004000,
    StopsExecution     = static_cast<quint32>(InvalidOperation) | static_cast<quint32>(Break) | static_cast<quint32>(UndeterminedJump),

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
    MemoryUsageMap() : m_data(65536, MemoryUsage::Unknown)
    {
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

    [[nodiscard]] QList<quint16> addressesWhichContain(MemoryUsage usage) const
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
    [[nodiscard]] inline int size() const noexcept { return m_data.size(); }
    [[nodiscard]] inline bool isEmpty() const noexcept { return m_data.isEmpty(); }
    [[nodiscard]] inline MemoryUsages& operator[](int i) { return m_data[i]; }
    [[nodiscard]] inline const MemoryUsages& operator[](int i) const { return m_data[i]; }
    [[nodiscard]] inline MemoryUsages at(int i) const { return m_data.at(i); }
    [[nodiscard]] inline MemoryUsages value(int i) const { return m_data.value(i); }
    [[nodiscard]] inline MemoryUsages value(int i, const MemoryUsages& defaultValue) const { return m_data.value(i, defaultValue); }

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MemoryUsages)

