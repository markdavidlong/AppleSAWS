#ifndef ENTRYPOINTMODEL_H
#define ENTRYPOINTMODEL_H

#include <QAbstractTableModel>

struct EntryPoint {
    quint16 address;
    QString note;
};

class EntryPointModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit EntryPointModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;//DONE

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override; //DONE
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;//DONE

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;//DONE

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void doTestData();

    void addEntryPoint(EntryPoint ep);

    void removeSelection(QModelIndexList selection);

private:
    QList<EntryPoint> m_entryPoints;

};

#endif // ENTRYPOINTMODEL_H
