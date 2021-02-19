#ifndef APPLESOFTRETOKENIZER_H
#define APPLESOFTRETOKENIZER_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/



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
    QList<ApplesoftToken> retokenizeRems(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeStrings(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeDataStatements(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeDataPayload(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeVariables(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeNumbers(QList<ApplesoftToken> &datatokens);
    QList<ApplesoftToken> retokenizeNegativeNumbers(QList<ApplesoftToken> &datatokens);


    QVector<ApplesoftLine> m_retokenized_lines;
    QByteArray m_data;

    quint16 m_data_end;

    bool m_isParsed;

    QList<QPair<quint16, quint16> > m_flowTargets;

};


#endif // APPLESOFTRETOKENIZER_H

