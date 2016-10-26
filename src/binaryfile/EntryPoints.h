#ifndef ENTRYPOINTS_H
#define ENTRYPOINTS_H

#include <QObject>
#include <QDataStream>

struct EntryPoint {
    quint16 address;
    QString note;
};

class EntryPoints : public QObject
{
    Q_OBJECT
public:
    explicit EntryPoints(QObject *parent = 0);

    bool hasEntryPointAtAddress(quint16 address);

    const EntryPoint &at(int location) const { return m_entryPoints.at(location); }
    EntryPoint &pointRefAt(int location) { return m_entryPoints[location]; }
    EntryPoint &operator[](int location) { return m_entryPoints[location]; }

    void editPoint(int at, EntryPoint newPoint);

    QDataStream &read(QDataStream &dataStream);
    QDataStream &write(QDataStream &dataStream) const;

    int numEntryPoints() const { return m_entryPoints.count(); }

    QList<quint16> getEntryPointAddresses() const;

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

#endif // ENTRYPOINTS_H
