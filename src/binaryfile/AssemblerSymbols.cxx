#include "AssemblerSymbols.h"

#include <algorithm>
#include <iterator>


AssemblerSymbols::AssemblerSymbols(QObject *parent) : QObject(parent)
{

}

int AssemblerSymbols::locationOfSymbolAtAddress(quint16 address) const
{
    // Use modern algorithm with const reference to avoid copy
    const auto it = std::find_if(m_assemblerSymbols.cbegin(), m_assemblerSymbols.cend(),
                                 [address](const Symbol& symbol) {
                                     return symbol.address == address;
                                 });
    
    return (it != m_assemblerSymbols.cend()) ? std::distance(m_assemblerSymbols.cbegin(), it) : -1;
}

QString AssemblerSymbols::getSymbolAtAddress(quint16 address) const
{
    const int loc = locationOfSymbolAtAddress(address);
    return (loc >= 0) ? m_assemblerSymbols.at(loc).name : QString{};
}

bool AssemblerSymbols::hasAssemSymbolAtAddress(quint16 address) const
{
    // Use modern algorithm for better performance and clarity
    return std::any_of(m_assemblerSymbols.cbegin(), m_assemblerSymbols.cend(),
                       [address](const Symbol& symbol) {
                           return symbol.address == address;
                       });
}

void AssemblerSymbols::editSymbol(int location, Symbol newSymbol)
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

void AssemblerSymbols::addSymbol(Symbol ep)
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

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols::Symbol &model)
{
    out << model.address;
    out << model.name;
    out << static_cast<qint32>(model.symbolsize);
    return out;
}

QDataStream &operator>>(QDataStream &in, AssemblerSymbols::Symbol &model)
{
    in >> model.address;
    in >> model.name;
    qint32 size;
    in >> size;
    model.symbolsize = static_cast<AssemblerSymbols::SymbolSize>(size);
    return in;
}

//---------------------------------------------------------------------------
// TESTING

void AssemblerSymbols::doTestData()
{
    // Use aggregate initialization for cleaner code
    addSymbol({0x0100, QStringLiteral("Test Symbol 1"), SymbolSize::Byte});
    addSymbol({0x0200, QStringLiteral("Test Symbol 2"), SymbolSize::Word});
    addSymbol({0x0300, QStringLiteral("Test Symbol 3"), SymbolSize::Byte});
}


