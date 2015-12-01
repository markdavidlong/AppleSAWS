#ifndef APPLESOFTFILE_H
#define APPLESOFTFILE_H

#include <QByteArray>
#include <QStringList>
#include <QMap>

#include "genericfile.h"

struct ApplesoftLine {
    quint16 next_address;
    quint16 linenum;
    QByteArray tokens;
    QString detokenized_line;
};


class ApplesoftFile : public GenericFile
{
public:
    ApplesoftFile(QByteArray data = QByteArray());
    void setData(QByteArray data);
    QByteArray extraData();
    QList<ApplesoftLine> detokenized() { return m_detokenized; }
    void list();
    QStringList extraDataHexValues();

private:
    void makeTokens();
    QList<ApplesoftLine> detokenize();

    int m_data_end;
    QMap<quint8,QString> m_tokens;
    quint16 m_length;
    QList<ApplesoftLine> m_detokenized;
};

#endif // APPLESOFTFILE_H
