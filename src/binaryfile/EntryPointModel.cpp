#include "EntryPointModel.h"
#include "util.h"

#include <QDebug>



EntryPointModel::EntryPointModel(QObject *parent, EntryPoints *points)
    : QAbstractTableModel(parent)
{
    setEntryPointsData(points);
}

void EntryPointModel::setEntryPointsData(EntryPoints *points)
{
    entryPoints = points;

    if (entryPoints)
    {
        connect(entryPoints,SIGNAL(pointAddedAt(int)),SLOT(handlePointAddition(int)));
        connect(entryPoints,SIGNAL(pointChangedAt(int)),SLOT(handlePointChange(int)));
        connect(entryPoints,SIGNAL(pointRemovedAt(int)),SLOT(handlePointRemoval(int)));
 //       insertRows(0,entryPoints->numEntryPoints());
    }

}

QVariant EntryPointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!entryPoints) return QVariant();

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
            return "0x"+uint16ToHex(entryPoints->at(section).address);
        }
    }
    return QVariant();
}


int EntryPointModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!entryPoints) return 0;

    return entryPoints->numEntryPoints();
}

int EntryPointModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant EntryPointModel::data(const QModelIndex &index, int role) const
{
    if (!entryPoints) return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return entryPoints->at(index.row()).note;
        }
    }
    return QVariant();
}

bool EntryPointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!entryPoints) return false;

    if (data(index, role) != value) {
        if (index.column() == 0)
        {
           entryPoints->pointRefAt(index.row()).note = value.toString();
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags EntryPointModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    if (index.column() == 0)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    else
        return QAbstractTableModel::flags(index);
}

bool EntryPointModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return false;
}

bool EntryPointModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!entryPoints) return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int idx = 0; idx < count; idx++)
    {
        entryPoints->removePointAt(row);
    }
    endRemoveRows();
    return false;
}


