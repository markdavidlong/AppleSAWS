#ifndef CHARSETVIEWER_H
#define CHARSETVIEWER_H

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

#include "binaryfile.h"
#include "characterwidget.h"
#include "fileviewerinterface.h"
#include "charactersetexplorer.h"

#include <QWidget>

class CharSetViewer : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit CharSetViewer(QWidget *parent = 0);
    virtual ~CharSetViewer();

    void setFile(GenericFile *file);
    void setFile(BinaryFile *file);

    virtual bool optionsMenuItems(QMenu *menu);

public slots:
    void setFgColor(QColor color);
    void setBgColor(QColor color);
    void setGridColor(QColor color);

    void showGrid(bool show);
    void enableBitShift(bool enable);

    void showExplorer();

signals:

protected:
    QList<CharacterWidget*> getChildren();

protected slots:
    void cleanupExplorer();

private:
    BinaryFile *m_file;

    QByteArray m_data;

    CharacterSet m_charset;

    CharacterSetExplorer *m_cse;

    QAction *m_showGridAction;
    QAction *m_enableBitShiftAction;
    QAction *m_charSetEncoderAction;
};

#endif // CHARSETVIEWER_H
