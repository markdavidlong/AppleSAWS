#ifndef BINARYFILEMETADATA_H
#define BINARYFILEMETADATA_H

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



#include "entrypoints.h"
#include "assemblersymbols.h"
#include "genericfile.h"
#include "binaryfile.h"

#include <Qt>
#include <QList>
#include <QMap>
#include <QObject>




class BinaryFileMetadata : public QObject
{
    Q_OBJECT
public:
    BinaryFileMetadata(GenericFile *file, quint16 defaultAddress, QObject *parent = 0);

    QString filename() const { return m_file->filename(); }

    EntryPoints *entryPoints() { return m_eps; }
    AssemblerSymbols *assemblerSymbols() { return m_as; }

signals:
    void doDisassemble(QList<quint16>);

public slots:
    void load();
    void save();

    void requestDisassembly() { emit doDisassemble(m_eps->getEntryPointAddresses()); }

private:

    EntryPoints *m_eps;
    AssemblerSymbols *m_as;

    GenericFile *m_file;

    quint16 m_defaultAddress;
};

#endif // BINARYFILEMETADATA_H
