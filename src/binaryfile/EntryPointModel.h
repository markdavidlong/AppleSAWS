#pragma once

#include "EntryPoints.h"

#include <QDataStream>
#include <QAbstractTableModel>




class EntryPointModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit EntryPointModel(QObject *parent = nullptr, EntryPoints *points = nullptr);
    void setEntryPointsData(EntryPoints *points) noexcept;

    // Header:
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // Basic functionality:
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Constants for better maintainability
    static constexpr int COLUMN_COUNT = 2;
    static constexpr int ADDRESS_COLUMN = 0;
    static constexpr int NOTE_COLUMN = 1;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

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
        emit dataChanged(ind,ind,QList<int>() << Qt::DisplayRole);
    }


private:
    EntryPoints *m_entryPoints{nullptr};
};


