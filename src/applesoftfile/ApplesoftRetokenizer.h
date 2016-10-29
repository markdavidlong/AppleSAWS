#ifndef APPLESOFTRETOKENIZER_H
#define APPLESOFTRETOKENIZER_H

#include "applesoftline.h"

#include <QByteArray>
#include <QVector>
#include <QList>
#include <QPair>

class ApplesoftRetokenizer
{
public:
    ApplesoftRetokenizer();

    void setData(QByteArray data);
    void parse(quint16 start_address = 0x0801);

    quint16 getEndOfDataOffset() const { return m_data_end; }

    QVector<ApplesoftLine> getRetokenizedLines() { return m_retokenized_lines; }

    QList<QPair<quint16, quint16> >  getFlowTargets() const { return m_flowTargets; }

private:
    void retokenizeLinesForFormatting();


    void retokenizeLine(ApplesoftLine &line);
    QVector<ApplesoftToken> retokenizeRems(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeStrings(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeDataStatements(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeDataPayload(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeVariables(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeNumbers(QVector<ApplesoftToken> &datatokens);
    QVector<ApplesoftToken> retokenizeNegativeNumbers(QVector<ApplesoftToken> &datatokens);


    QVector<ApplesoftLine> m_retokenized_lines;
    QByteArray m_data;

    quint16 m_data_end;

    bool m_isParsed;

    QList<QPair<quint16, quint16> > m_flowTargets;

};


#endif // APPLESOFTRETOKENIZER_H

