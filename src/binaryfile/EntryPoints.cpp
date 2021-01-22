#include "EntryPoints.h"
#include <QDebug>

EntryPoints::EntryPoints(QObject *parent) : QObject(parent)
{

}

bool EntryPoints::hasEntryPointAtAddress(quint16 address)
{
    // Assume list m_entryPoints is sorted by address (it should be)
    QListIterator<EntryPoint> it(m_entryPoints);
    while (it.hasNext())
    {
        EntryPoint ep = it.next();
        if (ep.address == address) return true;
        if (ep.address > address) return false;
    }
    return false;
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

    int idx = 0;
    for (; idx < m_entryPoints.count(); idx++)
    {
        if (ep.address < m_entryPoints[idx].address)
            break;
    }
    m_entryPoints.insert(idx,ep);
    emit pointAddedAt(idx);
    emit pointAdded(ep,idx);
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

    QListIterator<EntryPoint> it(m_entryPoints);
    while (it.hasNext())
    {
        retval.append(it.next().address);
    }
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


