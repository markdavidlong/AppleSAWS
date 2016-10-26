#include "binaryfilemetadata.h"

BinaryFileMetadata::BinaryFileMetadata(QString filename)
{
    m_filename = filename;
}

//void BinaryFileMetadata::setEntryPoint(quint16 address, QString note)
//{
//    EntryPoint ep;
//    ep.address = address;
//    ep.note = note;
//    setEntryPoint(ep);
//}

//void BinaryFileMetadata::setEntryPoint(EntryPoint ep)
//{
//    m_entryPoints.insert(ep.address,ep);
//}


//bool BinaryFileMetadata::hasEntryPointAtAddress(quint16 address)
//{
//    return m_entryPoints.contains(address);
//}

//void BinaryFileMetadata::removeEntryPoint(quint16 address)
//{
//    if (hasEntryPointAtAddress(address))
//    {
//        m_entryPoints.remove(address);
//    }
//}

bool BinaryFileMetadata::load()
{
 //   setSymbol(0x0000,"Test Symbol 1");
  //  setSymbol(0x0006,"Test Symbol 2");
  //  setEntryPoint(0x0010,"Test Entry Point 1");
  //  setEntryPoint(0x0020,"Test Entry Point 2");
    return true;
}

bool BinaryFileMetadata::save()
{
    return false;
}

//void BinaryFileMetadata::setSymbol(quint16 address, QString name)
//{
//    AssemSymbol Symbol;
//    Symbol.address = address;
//    Symbol.name = name;
//    setSymbol(Symbol);
//}

//void BinaryFileMetadata::setSymbol(AssemSymbol Symbol)
//{
//    m_Symbols.insert(Symbol.address, Symbol);
//}

//AssemSymbol BinaryFileMetadata::getSymbolAtAddress(quint16 address)
//{
//    if (hasSymbolAtAddress(address))
//    {
//        return m_Symbols[address];
//    }
//    return AssemSymbol();
//}

//void BinaryFileMetadata::removeSymbol(AssemSymbol Symbol)
//{
//    removeSymbol(Symbol.address);
//}

//void BinaryFileMetadata::removeSymbol(quint16 address)
//{
//    if (hasSymbolAtAddress(address))
//    {
//        m_Symbols.remove(address);
//    }
//}


