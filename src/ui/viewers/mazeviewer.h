#pragma once

#include "attributedmemory.h"
#include "fileviewerinterface.h"

#include <QObject>
#include <QWidget>
#include <QImage>

class BinaryFile;

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
