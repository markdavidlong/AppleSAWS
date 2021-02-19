#ifndef TEXTHEXDUMPVIEWER_H
#define TEXTHEXDUMPVIEWER_H

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

#include "genericfile.h"
#include "fileviewerinterface.h"

#include <QString>
#include <QByteArray>
#include <QWidget>


namespace Ui {
class TextHexDumpViewer;
}

class TextHexDumpViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit TextHexDumpViewer(QWidget *parent = 0);
    ~TextHexDumpViewer();

    void setFile(GenericFile *file) { setFile(file,0); }
    void setFile(GenericFile *file, quint16 offset);
    void setData(QByteArray data);
    void setText(QString text);

    virtual bool optionsMenuItems(QMenu *menu);

    bool canPrint() const;
    bool canExport() const;

public slots:
    void toggleWordWrap(bool enabled);
    void doPrint();
    void doExport();

protected:
    QString makeTextStr(QByteArray data);
    QString makeHexStr(QByteArray data);

private:
    Ui::TextHexDumpViewer *ui;

    quint16 m_offset;

    GenericFile *m_file;
};

#endif // TEXTHEXDUMPVIEWER_H
