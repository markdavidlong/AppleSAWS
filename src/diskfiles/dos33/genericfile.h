#ifndef GENERICFILE_H
#define GENERICFILE_H

#include "dos33diskimage.h"

#include <QByteArray>
#include <QString>


class GenericFile
{
    friend class Dos33DiskImage;

public:
    virtual ~GenericFile() { }
  
    virtual QByteArray data() { return m_data.mid(m_ignore_offset); }
    quint8 dataAt(int  offset) const;
    quint16 dataWordAt(int offset) const;

    virtual void setFilename(QString filename) { m_filename = filename; }
    QString filename() const { return m_filename; }
    
    virtual void setAddress(quint16 location) { m_address = location; }
    virtual quint16 address() { return m_address; }
   
    virtual int length() const { return m_length; }

    Dos33DiskImage *diskFile() const { return m_diskfile; }
    void setDiskFile(Dos33DiskImage *diskfile) { m_diskfile = diskfile; }

    void setFileType(QString type) { m_file_type = type; }
    QString getFileType() const;

    QByteArray rawData() const { return m_data; }

    void setIgnoreOffset(int offset) { m_ignore_offset = offset; }
    int ignoreOffset() const { return m_ignore_offset; }

protected:
    GenericFile(Dos33DiskImage *image, QList<TSPair> pairs);

    GenericFile(QByteArray data = QByteArray());
    virtual void setLength(quint16 length) { m_length = length; }
    virtual void setData(QByteArray data);

private:
    int m_ignore_offset;
    QByteArray m_data;
    QString m_filename;
    quint16 m_address;
    qint16 m_length;
    Dos33DiskImage * m_diskfile;
    bool m_data_loaded;
    TSPairList m_ts_pairs;

    QString m_file_type;

};

#endif // GENERICFILE_H
