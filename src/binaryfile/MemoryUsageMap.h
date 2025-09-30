#pragma once

#include <QFlag>
#include <QFlags>
#include <QList>

class MemoryUsageMap
{
public:
    enum class UsageRole : quint32 {
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

    using UsageRoles = QFlags<UsageRole>;

    MemoryUsageMap();

    void clearData();
    void merge(const MemoryUsageMap &other);

    [[nodiscard]] QList<quint16> addressesWhichContain(UsageRole usage) const;

    // Essential interface methods for accessing the data
    [[nodiscard]] int size() const noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] UsageRoles& operator[](int i);
    [[nodiscard]] const UsageRoles& operator[](int i) const;
    [[nodiscard]] UsageRoles at(int i) const;
    [[nodiscard]] UsageRoles value(int i) const;
    [[nodiscard]] UsageRoles value(int i, const UsageRoles& defaultValue) const;

private:
    QList<UsageRoles> m_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MemoryUsageMap::UsageRoles)

