#include "binaryfilemetadata.h"

BinaryFileMetadata::BinaryFileMetadata()
{

}

void BinaryFileMetadata::addEntryPoint(quint16 address)
{
    if (!containsEntryPoint(address))
    {
        m_entryPoints.append(address);
        qSort(m_entryPoints);
    }
}

void BinaryFileMetadata::addDataRange(AddressRange range)
{
    m_dataRanges.append(range);
}

bool BinaryFileMetadata::load()
{
    return false;
}

bool BinaryFileMetadata::save()
{
    return false;
}

bool BinaryFileMetadata::containsEntryPoint(quint16 address)
{
    return m_entryPoints.contains(address);
}

void BinaryFileMetadata::removeEntryPoint(quint16 address)
{
    if (containsEntryPoint(address))
    {
        m_entryPoints.removeAll(address);
    }
}
