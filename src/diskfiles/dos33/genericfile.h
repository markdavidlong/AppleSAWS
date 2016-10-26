#ifndef GENERICFILE_H
#define GENERICFILE_H

#include <QByteArray>
#include <QString>

class GenericFile
{
public:
    GenericFile(QByteArray data = QByteArray());
    virtual ~GenericFile() { }
    virtual void setData(QByteArray data);

    virtual QByteArray data() { return m_data; }
    void setFilename(QString filename) { m_filename = filename; }
    QString filename() const { return m_filename; }
    virtual void setAddress(quint16 location) { m_address = location; }
    virtual quint16 address() { return m_address; }
    virtual QByteArray rawData() { return m_data; }

    virtual void setLength(quint16 length) { m_length = length; }
    virtual quint16 length() { return m_length; }


protected:
    QByteArray m_data;
    QString m_filename;
    quint16 m_address;
    qint16 m_length;

};

#endif // GENERICFILE_H
