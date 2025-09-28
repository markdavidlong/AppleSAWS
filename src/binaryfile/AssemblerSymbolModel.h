#pragma once

#include "AssemblerSymbols.h"

#include <QDataStream>
#include <QAbstractTableModel>




class AssemblerSymbolModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AssemblerSymbolModel(QObject *parent = 0, AssemblerSymbols *symbols = Q_NULLPTR);
    void setAssemblerSymbolsData(AssemblerSymbols *symbols);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;//DONE

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
    void handleSymbolAddition(int location) { insertRows(location,1); }
    void handleSymbolRemoval(int location) { removeRows(location, 1); }
    void handleSymbolChange(int location)
    {
        QModelIndex ind = createIndex(location,0);
        emit dataChanged(ind,ind,QVector<int>() << Qt::DisplayRole);
    }


private:
    AssemblerSymbols *assemblerSymbols;
};

