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

protected:
    QByteArray m_data;
    QString m_filename;
};

#endif // GENERICFILE_H
