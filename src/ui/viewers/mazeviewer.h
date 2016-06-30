#ifndef MAZEVIEWER_H
#define MAZEVIEWER_H

#include <QObject>
#include <QWidget>
#include <QImage>

#include "binaryfile.h"
#include "memory.h"


/// A class for viewing maze files in "The Missing Ring" by DataMost.

class MazeViewer : public QWidget
{
    Q_OBJECT
public:
    MazeViewer(QWidget *parent = 0);

    void drawMaze();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    void setFile(BinaryFile *file);

private:
    QPixmap m_maze;
    BinaryFile *m_file;
    Memory mem;

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
