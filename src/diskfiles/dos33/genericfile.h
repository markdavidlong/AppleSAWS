#ifndef GENERICFILE_H
#define GENERICFILE_H

#include <QByteArray>

class GenericFile
{
public:
    GenericFile(QByteArray data = QByteArray());
    virtual ~GenericFile() { }
    virtual void setData(QByteArray data);
    virtual QByteArray data() { return m_data; }

protected:
    QByteArray m_data;
};

#endif // GENERICFILE_H
