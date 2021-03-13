#ifndef APPLESOFTFILE_H
#define APPLESOFTFILE_H

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



#include <QByteArray>
#include <QStringList>
#include <QMap>
#include <QVector>

#include "applesoftline.h"
#include "genericfile.h"
#include "applesofttoken.h"
#include "applesoftretokenizer.h"


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
    ApplesoftFile(Dos33DiskImage *image, FileDescriptiveEntry &fde);

    QVector<ApplesoftLine> m_lines;
    int m_data_end;
    quint16 m_length;

    ApplesoftRetokenizer *m_retokenizer;

    QList<QPair<quint16, quint16> > m_flowTargets;

};



#endif // APPLESOFTFILE_H
