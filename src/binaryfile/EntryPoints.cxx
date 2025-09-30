#include "EntryPoints.h"

#include <QDebug>
#include <QListIterator>
#include <algorithm>
#include <iterator>


EntryPoints::EntryPoints(QObject *parent) : QObject(parent)
{

}

bool EntryPoints::hasEntryPointAtAddress(quint16 address) const noexcept
{
    // Use binary search since list is sorted by address - O(log n)
    return std::binary_search(m_entryPoints.begin(), m_entryPoints.end(),
                              EntryPoint{address, QString()},
                              [](const EntryPoint& a, const EntryPoint& b) {
                                  return a.address < b.address;
                              });
}

void EntryPoints::editPoint(int location, EntryPoint newPoint)
{

    if (m_entryPoints.at(location).address == newPoint.address)
    {
        m_entryPoints[location].note = newPoint.note;
        emit pointChangedAt(location);
    }
    else
    {
        removePointAt(location);
        addPoint(newPoint);
    }
}

void EntryPoints::addPoint(EntryPoint ep)
{
    if (hasEntryPointAtAddress(ep.address)) return;

    // Use binary search to find insertion point - O(log n)
    auto it = std::lower_bound(m_entryPoints.begin(), m_entryPoints.end(), ep,
                               [](const EntryPoint& a, const EntryPoint& b) {
                                   return a.address < b.address;
                               });
    
    int idx = std::distance(m_entryPoints.begin(), it);
    m_entryPoints.insert(it, ep);
    emit pointAddedAt(idx);
    emit pointAdded(ep, idx);
}

void EntryPoints::removePointAt(int location)
{
    m_entryPoints.removeAt(location);
}

//---------------------------------------------------------------------------
// STREAMING OPERATORS

QDataStream &EntryPoints::read(QDataStream &dataStream)
{
    quint8 version;
    dataStream >> version;
    if (version == 0)
    {
        dataStream >> m_entryPoints;
    }
    else qWarning("Unhandled version of EntryPointModel (%d) found in QDataStream",version);

    return dataStream;
}

QDataStream &EntryPoints::write(QDataStream &dataStream) const
{
    quint8 version = 0; // Increment this and update read() if new items are added
    dataStream << version;
    dataStream << m_entryPoints;
    return dataStream;
}

QList<quint16> EntryPoints::getEntryPointAddresses() const
{
    QList<quint16> retval;
    retval.reserve(m_entryPoints.size()); // Pre-allocate memory
    
    std::transform(m_entryPoints.begin(), m_entryPoints.end(),
                   std::back_inserter(retval),
                   [](const EntryPoint& ep) { return ep.address; });
    
    return retval;
}

QDataStream &operator<<(QDataStream &out, const EntryPoints &model)
{
    return model.write(out);
}

QDataStream &operator>>(QDataStream &in, EntryPoints&model)
{
    return model.read(in);
}

QDataStream &operator<<(QDataStream &out, const EntryPoint &model)
{
    out << model.address;
    out << model.note;
    return out;
}

QDataStream &operator>>(QDataStream &in, EntryPoint &model)
{
    in >> model.address;
    in >> model.note;

    return in;
}

//---------------------------------------------------------------------------
// TESTING

void EntryPoints::doTestData()
{
    EntryPoint ep;
    ep.address = 0x0010;
    ep.note = "Test Entry Point 1";
    addPoint(ep);
    ep.address = 0x0020;
    ep.note = "Test Entry Point 2";
    addPoint(ep);
    ep.address = 0x0030;
    ep.note = "Test Entry Point 3";
    addPoint(ep);

}


