#pragma once

#include "ApplesoftLine.h"
#include "GenericFile.h"
#include "ApplesoftToken.h"
#include "ApplesoftRetokenizer.h"

#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QVector>


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

    QList<QPair<quint16, quint16> > m_flowTargets;

};
