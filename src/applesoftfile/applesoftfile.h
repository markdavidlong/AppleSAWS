#ifndef APPLESOFTFILE_H
#define APPLESOFTFILE_H

#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QVector>

#include "applesoftline.h"
#include "genericfile.h"
#include "applesofttoken.h"


class ApplesoftFile : public GenericFile
{
public:
    ApplesoftFile(QByteArray data = QByteArray());
    void setData(QByteArray data);
    void setFilename(QString filename) { m_filename = filename; }
    QByteArray extraData();
    QStringList extraDataHexValues();

    QVector<ApplesoftLine> getLines() const { return m_lines; }

    QString filename() const { return m_filename; }

private:
    void parse(quint16 start_address = 0x0801);

    QVector<ApplesoftLine> m_lines;
    int m_data_end;
    quint16 m_length;
    QString m_filename;

    friend class Retokenizer;
};

class Retokenizer {
public:
    static void retokenize(ApplesoftLine &line);
private:
    QByteArray retokenizePart(QByteArray part);
};

#endif // APPLESOFTFILE_H
