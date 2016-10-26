#ifndef BINARYFILEMETADATA_H
#define BINARYFILEMETADATA_H

#include <Qt>
#include <QList>
#include <QMap>




class BinaryFileMetadata
{
public:

    BinaryFileMetadata(QString filename);

    void setFilename(QString filename) { m_filename = filename; }
    QString filename() const { return m_filename; }

    bool load();
    bool save();

//    void setEntryPoint(quint16 address, QString note = "");
//    void setEntryPoint(EntryPoint ep);

//    bool hasEntryPointAtAddress(EntryPoint ep);
//    bool hasEntryPointAtAddress(quint16 address);

//    void removeEntryPoint(quint16 address);

//    QList<EntryPoint> getEntryPointList() const { return m_entryPoints.values(); }
//    QMap<quint16,EntryPoint> getEntryPointMap() const { return m_entryPoints; }

//    void setSymbol(quint16 address, QString name);
//    void setSymbol(AssemSymbol Symbol);
//    bool hasSymbol(AssemSymbol Symbol) const { return hasSymbolAtAddress(Symbol.address); }
//    bool hasSymbolAtAddress(quint16 address) const { return m_Symbols.contains(address); }
//    AssemSymbol getSymbolAtAddress(quint16 address);
//    void removeSymbol(AssemSymbol Symbol);
//    void removeSymbol(quint16 address);

//    QList<AssemSymbol> getSymbolList() const { return m_Symbols.values(); }
//    QMap<quint16, AssemSymbol> getSymbolMap() const { return m_Symbols; }

private:

//    QMap<quint16,AssemSymbol> m_Symbols;
    QString m_filename;
};

#endif // BINARYFILEMETADATA_H
