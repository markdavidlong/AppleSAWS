#include "AssemblerSymbolModel.h"
#include "Util.h"

#include <QDebug>

AssemblerSymbolModel::AssemblerSymbolModel(QObject *parent, AssemblerSymbols *symbols)
    : QAbstractTableModel(parent), m_assemblerSymbols{nullptr}
{
    setAssemblerSymbolsData(symbols);
}

void AssemblerSymbolModel::setAssemblerSymbolsData(AssemblerSymbols *symbols) noexcept
{
    m_assemblerSymbols = symbols;

    if (m_assemblerSymbols)
    {
        connect(m_assemblerSymbols, &AssemblerSymbols::symbolAddedAt, this, &AssemblerSymbolModel::handleSymbolAddition);
        connect(m_assemblerSymbols, &AssemblerSymbols::symbolChangedAt, this, &AssemblerSymbolModel::handleSymbolChange);
        connect(m_assemblerSymbols, &AssemblerSymbols::symbolRemovedAt, this, &AssemblerSymbolModel::handleSymbolRemoval);
    }

}

QVariant AssemblerSymbolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_assemblerSymbols) return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            // Column headers
            switch (section)
            {
                case TYPE_COLUMN: return QStringLiteral("Type");
                case NAME_COLUMN: return QStringLiteral("Symbol");
                default: return QVariant();
            }
        }
        else // Orientation == Qt::Vertical
        {
            // Row headers showing addresses
            return QStringLiteral("0x") + uint16ToHex(m_assemblerSymbols->at(section).address);
        }
    }
    return QVariant();
}


int AssemblerSymbolModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!m_assemblerSymbols) return 0;

    return m_assemblerSymbols->numAssemblerSymbols();
}

int AssemblerSymbolModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant AssemblerSymbolModel::data(const QModelIndex &index, int role) const
{
    if (!m_assemblerSymbols) return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == TYPE_COLUMN)
        {
            // Use switch for better performance and readability
            switch (m_assemblerSymbols->at(index.row()).symbolsize)
            {
                case SymbolSize::Word: return QStringLiteral("WORD");
                case SymbolSize::Byte: return QStringLiteral("BYTE");
                default: return QVariant();
            }
        }
        else if (index.column() == NAME_COLUMN)
        {
            return m_assemblerSymbols->at(index.row()).name;
        }
    }
    return QVariant();
}

bool AssemblerSymbolModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_assemblerSymbols) return false;

    if (data(index, role) != value) {
        if (index.column() == NAME_COLUMN)
        {
            m_assemblerSymbols->symbolRefAt(index.row()).name = value.toString();
        }
        emit dataChanged(index, index, QList<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags AssemblerSymbolModel::flags(const QModelIndex &index) const
{
    if (index.column() == NAME_COLUMN)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    else
        return QAbstractTableModel::flags(index);
}

bool AssemblerSymbolModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return false;
}

bool AssemblerSymbolModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!m_assemblerSymbols) return false;
    bool success = false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int idx = 0; idx < count; idx++)
    {
        m_assemblerSymbols->removeSymbolAt(row);
        success = true;
    }
    endRemoveRows();
    return success;
}


