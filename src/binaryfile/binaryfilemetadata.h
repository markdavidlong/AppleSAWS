#ifndef BINARYFILEMETADATA_H
#define BINARYFILEMETADATA_H

#include <Qt>
#include <QList>


struct AddressRange
{
public:
    quint16 start;
    quint16 end;

    quint16 length() { return end-start; }

    bool contains(quint16 address) { return (address >= start && address <= end); }

};

class BinaryFileMetadata
{
public:
    enum UseType {
        UNKNOWN = 0x00,
        ROM     = 0x01,
        IO      = 0x02,
        BASIC   = 0x04,
        OPCODE  = 0x08,
        DATA    = 0x10
    };

    BinaryFileMetadata();

    void addEntryPoint(quint16 address);
    void addDataRange(AddressRange range);

    bool load();
    bool save();

    bool containsEntryPoint(quint16 address);
    void removeEntryPoint(quint16 address);

    QList<quint16> getEntryPoints() { return m_entryPoints; }
    QList<AddressRange> getDataRanges() { return m_dataRanges; }

private:
    QList<quint16> m_entryPoints;
    QList<AddressRange> m_dataRanges;

};

#endif // BINARYFILEMETADATA_H
