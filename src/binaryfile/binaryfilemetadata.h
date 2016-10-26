#ifndef BINARYFILEMETADATA_H
#define BINARYFILEMETADATA_H

#include <Qt>
#include <QList>
#include <QMap>



struct AssemSymbol {
    quint16 address;
    QString name;
};

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

    void setSymbol(quint16 address, QString name);
    void setSymbol(AssemSymbol symbol);
    bool hasSymbol(AssemSymbol symbol) const { return hasSymbolAtAddress(symbol.address); }
    bool hasSymbolAtAddress(quint16 address) const { return m_symbols.contains(address); }
    AssemSymbol getSymbolAtAddress(quint16 address);
    void removeSymbol(AssemSymbol symbol);
    void removeSymbol(quint16 address);

    QList<AssemSymbol> getSymbolList() const { return m_symbols.values(); }
    QMap<quint16, AssemSymbol> getSymbolMap() const { return m_symbols; }

private:

    QMap<quint16,AssemSymbol> m_symbols;
    QString m_filename;
};

#endif // BINARYFILEMETADATA_H
