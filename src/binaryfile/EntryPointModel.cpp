#include "EntryPointModel.h"
#include "util.h"

#include <QDebug>

EntryPointModel::EntryPointModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant EntryPointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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
            return "0x"+uint16ToHex(m_entryPoints[section].address);
        }
    }
    return QVariant();
}


int EntryPointModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
   // if (parent.isValid())
        return m_entryPoints.count();
  //  else
  //      return 0;
}

int EntryPointModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    //if (parent.isValid())
        return 1;
   // else
   //     return 0;
}

QVariant EntryPointModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        EntryPoint ep = m_entryPoints[index.row()];
        if (index.column() == 0)
        {
            return ep.note;
        }

    }
    return QVariant();
}

bool EntryPointModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (data(index, role) != value) {
        if (index.column() == 0)
        {
            m_entryPoints[index.row()].note = value.toString();
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
    // FIXME: Implement me!
    endInsertRows();
    return false;
}

bool EntryPointModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int idx = 0; idx < count; idx++)
    {
        m_entryPoints.removeAt(row);
    }
    endRemoveRows();
    return false;
}

void EntryPointModel::doTestData()
{
    EntryPoint ep;
    ep.address = 0x0010;
    ep.note = "Test Entry Point 1";
    addEntryPoint(ep);
    ep.address = 0x0020;
    ep.note = "Test Entry Point 2";
    addEntryPoint(ep);
    ep.address = 0x0030;
    ep.note = "Test Entry Point 3";
    addEntryPoint(ep);

}

void EntryPointModel::addEntryPoint(EntryPoint ep)
{
    if (!m_entryPoints.count())
    {
        m_entryPoints.append(ep);
        insertRows(m_entryPoints.count(),1);
    }
    else
    {
        int idx = 0;
        for (; idx < m_entryPoints.count(); idx++)
        {
            if (ep.address < m_entryPoints[idx].address)
                break;
        }
        m_entryPoints.insert(idx,ep);
        insertRows(idx,1);
    }
}

void EntryPointModel::removeSelection(QModelIndexList selection)
{
    if (selection.count() > 1)
    {
        qWarning() << "Unexpected number of items to remove.  Only removing the first.";
    }
    if (selection.count())
    {
        removeRow(selection[1].row());
    }
}


