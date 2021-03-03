#ifndef HEXDUMPVIEWER_H
#define HEXDUMPVIEWER_H

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
class HexDumpViewer;
}

class HexDumpViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit HexDumpViewer(QWidget *parent = 0, int defaultFontSize = -1);
    ~HexDumpViewer();

    void setFile(GenericFile *file) { setFile(file,0); }
    void setFile(GenericFile *file, quint16 offset);
    void setRawData(QByteArray data, quint16 offset = 0);
    virtual bool optionsMenuItems(QMenu *menu);

    bool canPrint() const;
    bool canExport() const { return false; }

public slots:
    void showHexAndAsciiValues();
    void toggleWordWrap(bool enabled);
    void doPrint();

private:
    void setText(QString text);
    void setData(QByteArray data);

    int m_defaultFontSize;
    Ui::HexDumpViewer *ui;

    quint16 m_offset;
    QByteArray m_data;

    GenericFile *m_file;
};

#endif // HEXDUMPVIEWER_H
