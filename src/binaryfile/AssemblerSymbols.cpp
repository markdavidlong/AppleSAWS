#include "AssemblerSymbols.h"

AssemblerSymbols::AssemblerSymbols(QObject *parent) : QObject(parent)
{

}

bool AssemblerSymbols::hasAssemSymbolAtAddress(quint16 address)
{
    // Assume list m_assemblerSymbols is sorted by address (it should be)
    QListIterator<AssemSymbol> it(m_assemblerSymbols);
    while (it.hasNext())
    {
        AssemSymbol ep = it.next();
        if (ep.address == address) return true;
        if (ep.address > address) return false;
    }
    return false;
}

void AssemblerSymbols::editSymbol(int location, AssemSymbol newSymbol)
{

    if (m_assemblerSymbols.at(location).address == newSymbol.address)
    {
        m_assemblerSymbols[location].name = newSymbol.name;
        emit symbolChangedAt(location);
    }
    else
    {
        removeSymbolAt(location);
        addSymbol(newSymbol);
    }
}

void AssemblerSymbols::addSymbol(AssemSymbol ep)
{
    if (hasAssemSymbolAtAddress(ep.address)) return;

    int idx = 0;
    for (; idx < m_assemblerSymbols.count(); idx++)
    {
        if (ep.address < m_assemblerSymbols[idx].address)
            break;
    }
    m_assemblerSymbols.insert(idx,ep);
    emit symbolAddedAt(idx);
    emit symbolAdded(ep,idx);
}

void AssemblerSymbols::removeSymbolAt(int location)
{
    m_assemblerSymbols.removeAt(location);
    emit symbolRemovedAt(location);
}

//---------------------------------------------------------------------------
// STREAMING OPERATORS

QDataStream &AssemblerSymbols::read(QDataStream &dataStream)
{
    quint8 version;
    dataStream >> version;
    if (version == 0)
    {
        dataStream >> m_assemblerSymbols;
    }
    else qWarning("Unhandled version of AssemSymbolModel (%d) found in QDataStream",version);

    return dataStream;
}

QDataStream &AssemblerSymbols::write(QDataStream &dataStream) const
{
    quint8 version = 0; // Increment this and update read() if new items are added
    dataStream << version;
    dataStream << m_assemblerSymbols;
    return dataStream;
}

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols &model)
{
    return model.write(out);
}

QDataStream &operator>>(QDataStream &in, AssemblerSymbols&model)
{
    return model.read(in);
}

QDataStream &operator<<(QDataStream &out, const AssemSymbol &model)
{
    out << model.address;
    out << model.name;
    return out;
}

QDataStream &operator>>(QDataStream &in, AssemSymbol &model)
{
    in >> model.address;
    in >> model.name;

    return in;
}

//---------------------------------------------------------------------------
// TESTING

void AssemblerSymbols::doTestData()
{
    AssemSymbol ep;
    ep.address = 0x0010;
    ep.name = "Test Entry Symbol 1";
    addSymbol(ep);
    ep.address = 0x0020;
    ep.name = "Test Entry Symbol 2";
    addSymbol(ep);
    ep.address = 0x0030;
    ep.name = "Test Entry Symbol 3";
    addSymbol(ep);

}


