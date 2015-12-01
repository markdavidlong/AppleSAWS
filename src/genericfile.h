#ifndef GENERICFILE_H
#define GENERICFILE_H

#include <QByteArray>

class GenericFile
{
public:
    GenericFile(QByteArray data = QByteArray());
    virtual void setData(QByteArray data);

protected:
    QByteArray m_data;
};

#endif // GENERICFILE_H
