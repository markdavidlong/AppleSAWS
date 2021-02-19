#ifndef MAZEVIEWER_H
#define MAZEVIEWER_H

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
#include <QWidget>
#include <QImage>

#include "binaryfile.h"
#include "attributedmemory.h"
#include "fileviewerinterface.h"


/// A class for viewing maze files in "The Missing Ring" by DataMost.

class MazeViewer : public FileViewerInterface
{
    Q_OBJECT
public:
    MazeViewer(QWidget *parent = 0);

    void drawMaze();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    void setFile(GenericFile *file);

    void setFile(BinaryFile *file);

    virtual bool optionsMenuItems(QMenu *) {  return false; }

private:
    QPixmap m_maze;
    BinaryFile *m_file;
    AttributedMemory mem;

    bool cellHasWalls(int x, int y);
    bool cellHasNorthDoor(int x, int y);
    bool cellHasSouthDoor(int x, int y);
    bool cellHasEastDoor(int x, int y);
    bool cellHasWestDoor(int x, int y);

    bool cellHasSpecialNorth(int x, int y);
    bool cellHasSpecialSouth(int x, int y);
    bool cellHasSpecialEast(int x, int y);
    bool cellHasSpecialWest(int x, int y);

    quint8 getCellDesc(int x, int y);
    quint8 getCellMonsters(int x, int y);
    quint8 getCellInventory(int x, int y);

    QString inventoryToString(quint8 inv);
    QString monsterToString(quint8 mc);
};

#endif // MAZEVIEWER_H
