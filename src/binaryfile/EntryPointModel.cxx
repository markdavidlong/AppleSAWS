#include "EntryPointModel.h"
#include "Util.h"

#include <QDebug>



EntryPointModel::EntryPointModel(QObject *parent, EntryPoints *points)
    : QAbstractTableModel(parent), m_entryPoints{nullptr}
{
    setEntryPointsData(points);
}

void EntryPointModel::setEntryPointsData(EntryPoints *points) noexcept
{
    m_entryPoints = points;

    if (m_entryPoints)
    {
        connect(m_entryPoints, &EntryPoints::pointAddedAt,   this, &EntryPointModel::handlePointAddition);
        connect(m_entryPoints, &EntryPoints::pointChangedAt, this, &EntryPointModel::handlePointChange);

    }

}

QVariant EntryPointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_entryPoints) return QVariant();

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            // Column headers
            switch (section)
            {
                case ADDRESS_COLUMN: return QStringLiteral("Address");
                case NOTE_COLUMN: return QStringLiteral("Note");
                default: return QVariant();
            }
        }
        else // Orientation == Qt::Vertical
        {
            // Row headers showing addresses
            return QStringLiteral("0x") + uint16ToHex(m_entryPoints->at(section).address);
        }
    }
    return QVariant();
}


int EntryPointModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!m_entryPoints) return 0;

    return m_entryPoints->numEntryPoints();
}

int EntryPointModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant EntryPointModel::data(const QModelIndex &index, int role) const
{
    if (!m_entryPoints) return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case ADDRESS_COLUMN:
                return QStringLiteral("0x") + uint16ToHex(m_entryPoints->at(index.row()).address);
            case NOTE_COLUMN:
                return m_entryPoints->at(index.row()).note;
            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool EntryPointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_entryPoints) return false;

    if (data(index, role) != value) {
        if (index.column() == NOTE_COLUMN)
        {
           m_entryPoints->pointRefAt(index.row()).note = value.toString();
        }
        emit dataChanged(index, index, QList<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags EntryPointModel::flags(const QModelIndex &index) const
{
    if (index.column() == NOTE_COLUMN)
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
    if (!m_entryPoints) return false;
    bool success = false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int idx = 0; idx < count; idx++)
    {
        m_entryPoints->removePointAt(row);
        success = true;
    }
    endRemoveRows();
    return success;
}


