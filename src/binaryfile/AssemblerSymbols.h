#ifndef ASSEMBLERSYMBOLS_H
#define ASSEMBLERSYMBOLS_H

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



#include <QObject>
#include <QDataStream>

typedef enum {
 //   SizeUnknown = 0,
    SizeByte = 1,
    SizeWord = 2
} SymbolSize;

struct AssemblerSymbol {
    quint16 address;
    QString name;
    SymbolSize symbolsize;
};


class AssemblerSymbols : public QObject
{
    Q_OBJECT
public:
    explicit AssemblerSymbols(QObject *parent = 0);

    bool hasAssemSymbolAtAddress(quint16 address);

    const AssemblerSymbol &at(int location) const { return m_assemblerSymbols.at(location); }
    AssemblerSymbol &symbolRefAt(int location) { return m_assemblerSymbols[location]; }
    AssemblerSymbol &operator[](int location) { return m_assemblerSymbols[location]; }

    void editSymbol(int at, AssemblerSymbol newSymbol);

    QDataStream &read(QDataStream &dataStream);
    QDataStream &write(QDataStream &dataStream) const;

    int numAssemblerSymbols() const { return m_assemblerSymbols.count(); }

    void doTestData();

    int locationOfSymbolAtAddress(quint16 address);
    QString getSymbolAtAddress(quint16 address);
signals:
    void symbolAdded(AssemblerSymbol &AssemblerSymbol, int location);
    void symbolAddedAt(int location);
    void symbolRemovedAt(int location);
    void symbolChangedAt(int location);

public slots:
    void addSymbol(AssemblerSymbol ep);
    void removeSymbolAt(int location);

protected:
    QList<AssemblerSymbol> m_assemblerSymbols;
};

QDataStream &operator<<(QDataStream &out, const AssemblerSymbols &model);
QDataStream &operator>>(QDataStream &in, AssemblerSymbols &model);


#endif // ASSEMBLERSYMBOLS_H
