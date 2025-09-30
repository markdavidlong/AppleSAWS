#pragma once

#include "ApplesoftLine.h"

#include <QByteArray>
#include <QList>
#include <QPair>

class ApplesoftToken;

class ApplesoftRetokenizer
{
public:
    ApplesoftRetokenizer();

    void setData(const QByteArray& data);
    void parse(quint16 start_address = 0x0801);

    [[nodiscard]] quint16 getEndOfDataOffset() const noexcept { return m_data_end; }

    [[nodiscard]] const QList<ApplesoftLine>& getRetokenizedLines() const noexcept { return m_retokenized_lines; }

    [[nodiscard]] const QList<QPair<quint16, quint16>>& getFlowTargets() const noexcept { return m_flowTargets; }

private:
    void retokenizeLinesForFormatting();

    void retokenizeLine(ApplesoftLine &line);
    QList<ApplesoftToken> retokenizeRems(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeStrings(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeDataStatements(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeDataPayload(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeVariables(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeNumbers(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeNegativeNumbers(QList<ApplesoftToken> &datatokens);


    QList<ApplesoftLine> m_retokenized_lines;
    QByteArray m_data;

    quint16 m_data_end{0};

    bool m_isParsed{false};

    QList<QPair<quint16, quint16> > m_flowTargets;

};



