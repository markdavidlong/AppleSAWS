#ifndef APPLESOFTFILE_H
#define APPLESOFTFILE_H

#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QVector>

#include "genericfile.h"
#include "applesofttoken.h"

struct ApplesoftLine {
    qint16 address;
    quint16 next_address;
    quint16 linenum;
    QVector<ApplesoftToken> tokens;
 //   QByteArray raw_tokens;
 //   QByteArray advanced_tokens;
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
    QList<ApplesoftLine> detokenize(quint16 start_address = 0x0801);

    int m_data_end;
    quint16 m_length;
    QList<ApplesoftLine> m_detokenized;

    friend class Retokenizer;
};

class Retokenizer {
public:
    static void retokenize(ApplesoftLine &line);
private:
    QByteArray retokenizePart(QByteArray part);
};

#endif // APPLESOFTFILE_H
