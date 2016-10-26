#include "binaryfilemetadata.h"

BinaryFileMetadata::BinaryFileMetadata(GenericFile *file, quint16 defaultAddress, QObject *parent)
    : QObject(parent)
{
    m_file = file;
    m_defaultAddress = defaultAddress;

    m_eps = new EntryPoints(this);
    m_as = new AssemblerSymbols(this);

    load();
}

void BinaryFileMetadata::load()
{
    EntryPoint ep;
    ep.note = "Default Entry Point";
    ep.address = m_defaultAddress;
    m_eps->addPoint(ep);
}

void BinaryFileMetadata::save()
{
}




