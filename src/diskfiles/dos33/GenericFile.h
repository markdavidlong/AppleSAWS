#pragma once

#include "DiskFile.h"

#include <QByteArray>
#include <QString>


class GenericFile
{
public:
    GenericFile(const QByteArray& data = QByteArray());
    virtual ~GenericFile() { }
  
    virtual void setData(const QByteArray& data);
    virtual QByteArray data() const { return m_data; }
    
    inline void setFilename(const QString& filename) { m_filename = filename; }
    inline QString filename() const { return m_filename; }
    
    virtual void setAddress(quint16 location) { m_address = location; }
    virtual quint16 address() const { return m_address; }
   
    virtual QByteArray rawData() const { return m_data; }

    virtual void setLength(quint16 length) { m_length = length; }
    virtual quint16 length() const { return m_length; }

    inline DiskFile *diskFile() const { return m_diskfile; }
    inline void setDiskFile(DiskFile *diskfile) { m_diskfile = diskfile; }

protected:
    QByteArray m_data{};
    QString m_filename{};
    quint16 m_address{0};
    quint16 m_length{0};
    DiskFile *m_diskfile{nullptr};

};
