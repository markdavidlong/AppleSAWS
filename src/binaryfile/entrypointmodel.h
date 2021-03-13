#ifndef ENTRYPOINTMODEL_H
#define ENTRYPOINTMODEL_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/



#include "entrypoints.h"

#include <QDataStream>
#include <QAbstractTableModel>




class EntryPointModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit EntryPointModel(QObject *parent = 0, EntryPoints *points = Q_NULLPTR);
    void setEntryPointsData(EntryPoints *points);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void doTestData();


protected slots:
    void handlePointAddition(int location) { insertRows(location,1); }
//    void handlePointRemoval(int location) { removeRows(location, 1); }
    void handlePointChange(int location)
    {
        QModelIndex ind = createIndex(location,0);
        emit dataChanged(ind,ind,QVector<int>() << Qt::DisplayRole);
    }


private:
    EntryPoints *entryPoints;
};




#endif // ENTRYPOINTMODEL_H