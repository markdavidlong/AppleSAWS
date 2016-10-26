#include "AssemblerSymbolModel.h"
#include "util.h"

#include <QDebug>

AssemblerSymbolModel::AssemblerSymbolModel(QObject *parent, AssemblerSymbols *symbols)
    :QAbstractTableModel(parent)
{
    setAssemblerSymbolsData(symbols);
}

void AssemblerSymbolModel::setAssemblerSymbolsData(AssemblerSymbols *symbols)
{
    assemblerSymbols = symbols;

    if (assemblerSymbols)
    {
        connect(assemblerSymbols,SIGNAL(symbolAddedAt(int)),SLOT(handleSymbolAddition(int)));
        connect(assemblerSymbols,SIGNAL(symbolChangedAt(int)),SLOT(handleSymbolChange(int)));
        connect(assemblerSymbols,SIGNAL(symbolRemovedAt(int)),SLOT(handleSymbolRemoval(int)));
    }

}

QVariant AssemblerSymbolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!assemblerSymbols) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            if (section == 0)
                return "";

        }
    }
    else // Orientation == Qt::Vertical
    {
        if (role == Qt::DisplayRole)
        {
            return "0x"+uint16ToHex(assemblerSymbols->at(section).address);
        }
    }
    return QVariant();
}


int AssemblerSymbolModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!assemblerSymbols) return 0;

    return assemblerSymbols->numAssemblerSymbols();
}

int AssemblerSymbolModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant AssemblerSymbolModel::data(const QModelIndex &index, int role) const
{
    if (!assemblerSymbols) return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return assemblerSymbols->at(index.row()).name;
        }
    }
    return QVariant();
}

bool AssemblerSymbolModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!assemblerSymbols) return false;

    if (data(index, role) != value) {
        if (index.column() == 0)
        {
           assemblerSymbols->symbolRefAt(index.row()).name = value.toString();
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags AssemblerSymbolModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    if (index.column() == 0)
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
    if (!assemblerSymbols) return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int idx = 0; idx < count; idx++)
    {
        assemblerSymbols->removeSymbolAt(row);
    }
    endRemoveRows();
    return false;
}


