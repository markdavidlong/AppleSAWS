#pragma once

#include <QFlag>
#include <QFlags>
#include <QVector>
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


class MemoryUsageMap : public QVector<MemoryUsages>
{

public:
    MemoryUsageMap()
    {
        fill(MemoryUsage::Unknown,65536);
    }

    void clearData()
    {
        for (int idx = 0; idx < size(); idx++)
        {
            (*this)[idx] = MemoryUsage::Unknown;
        }
    }

    void merge(const MemoryUsageMap &other)
    {
        if (other.size() != size())
        {
            qWarning("Mismatched size of MemoryMaps!");
            return;
        }
        for (int idx = 0; idx < size(); idx++)
        {
            (*this)[idx] |= other[idx];
        }
    }

    QList<quint16> addressesWhichContain(MemoryUsage usage)
    {
        QList<quint16> retval;
        for (int idx = 0; idx < size(); idx++)
        {
            if (value(idx).testFlag(usage))
            {
                retval.append((quint16) idx);
            }
        }
        return retval;
    }

private:
    void clear();
    void append(const MemoryUsages &);
    void append(MemoryUsages &&);
    void append(const QVector<MemoryUsages>);
    void insert(int,const MemoryUsages &);
    void insert(int,int,const MemoryUsages &);
    void move(int,int);
    void prepend(const MemoryUsages&);
    void push_back(const MemoryUsages&);
    void push_back(MemoryUsages&&);
    void push_front(const MemoryUsages&);
    void remove(int);
    void remove(int,int);
    void removeAll(const MemoryUsages&);
    void removeAt(int);
    void removeFirst();
    void removeLast();
    bool removeOne(const MemoryUsages&);
    void reserve(int);
    void resize(int);
    void swap(QVector<MemoryUsages>&);
    MemoryUsages &takeAt(int);
    MemoryUsages &takeFirst();
    MemoryUsages &takeLast();

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MemoryUsages)

