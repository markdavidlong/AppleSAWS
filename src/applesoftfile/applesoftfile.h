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
    friend class Dos33DiskImage;

public:
    QByteArray extraData();
    QStringList extraDataHexValues();

    QVector<ApplesoftLine> getLines() const { return m_lines; }

    int length() const override { return m_length; }


protected:
    void processData();

private:
    ApplesoftFile(QByteArray data = QByteArray());
    QVector<ApplesoftLine> m_lines;
    int m_data_end;
    quint16 m_length;

    ApplesoftRetokenizer *m_retokenizer;

    QList<QPair<quint16, quint16> > m_flowTargets;

};



#endif // APPLESOFTFILE_H
