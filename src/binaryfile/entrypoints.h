#ifndef ENTRYPOINTS_H
#define ENTRYPOINTS_H

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
