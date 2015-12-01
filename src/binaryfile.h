#ifndef BINARYFILE_H
#define BINARYFILE_H

#include "genericfile.h"

class BinaryFile : public GenericFile
{
public:
    BinaryFile(QByteArray data = QByteArray());
    void setData(QByteArray data);

    quint16 address() { return m_address; }
    quint16 length() { return m_length; }

    QByteArray data() { return m_data; }
    void dump();

protected:
    quint16 m_address;
    quint16 m_length;
};

#endif // BINARYFILE_H
