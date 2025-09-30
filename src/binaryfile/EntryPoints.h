#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QListIterator>
#include <QDataStream>

struct EntryPoint {
    quint16 address{0};
    QString note;
};

// QDataStream operators for EntryPoint
QDataStream &operator<<(QDataStream &out, const EntryPoint &entry);
QDataStream &operator>>(QDataStream &in, EntryPoint &entry);

class EntryPoints : public QObject
{
    Q_OBJECT
public:
    explicit EntryPoints(QObject *parent = nullptr);

    [[nodiscard]] bool hasEntryPointAtAddress(quint16 address) const noexcept;

    [[nodiscard]] const EntryPoint &at(int location) const { return m_entryPoints.at(location); }
    [[nodiscard]] EntryPoint &pointRefAt(int location) { return m_entryPoints[location]; }
    [[nodiscard]] EntryPoint &operator[](int location) { return m_entryPoints[location]; }

    void editPoint(int at, EntryPoint newPoint);

    QDataStream &read(QDataStream &dataStream);
    [[nodiscard]] QDataStream &write(QDataStream &dataStream) const;

    [[nodiscard]] int numEntryPoints() const noexcept { return m_entryPoints.count(); }

    [[nodiscard]] QList<quint16> getEntryPointAddresses() const;

    void doTestData();

signals:
    void pointAdded(EntryPoint &entryPoint, int location);
    void pointAddedAt(int location);
    void pointRemovedAt(int location);
    void pointChangedAt(int location);

public slots:
    void addPoint(EntryPoint ep);
    void removePointAt(int location);

protected:
    QList<EntryPoint> m_entryPoints;
};

QDataStream &operator<<(QDataStream &out, const EntryPoints &model);
QDataStream &operator>>(QDataStream &in, EntryPoints &model);

