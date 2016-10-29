#ifndef APPLESOFTFILE_H
#define APPLESOFTFILE_H

#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QVector>

#include "applesoftline.h"
#include "genericfile.h"
#include "applesofttoken.h"
#include "ApplesoftRetokenizer.h"


class ApplesoftFile : public GenericFile
{
public:
    ApplesoftFile(QByteArray data = QByteArray());
    void setData(QByteArray data);
    QByteArray extraData();
    QStringList extraDataHexValues();

    QVector<ApplesoftLine> getLines() const { return m_lines; }

    quint16 length() const { return m_length; }

    QByteArray rawData();

private:

    QVector<ApplesoftLine> m_lines;
    int m_data_end;
    quint16 m_length;

    ApplesoftRetokenizer *m_retokenizer;
};



#endif // APPLESOFTFILE_H
