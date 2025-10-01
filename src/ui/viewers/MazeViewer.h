#pragma once

#include "AttributedMemory.h"
#include "FileViewerInterface.h"

#include <QObject>
#include <QWidget>
#include <QImage>

class BinaryFile;

/// A class for viewing maze files in "The Missing Ring" by DataMost.

class MazeViewer final : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit MazeViewer(QWidget *parent = nullptr);
    ~MazeViewer() override = default;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    MazeViewer(const MazeViewer&) = delete;
    MazeViewer& operator=(const MazeViewer&) = delete;
    MazeViewer(MazeViewer&&) = delete;
    MazeViewer& operator=(MazeViewer&&) = delete;

    void drawMaze();
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void setFile(GenericFile *file) override;
    void setFile(BinaryFile *file);

    [[nodiscard]] bool optionsMenuItems(QMenu *) override { return false; }

private:
    QPixmap m_maze;
    BinaryFile *m_file{nullptr};
    AttributedMemory mem;

    [[nodiscard]] bool cellHasWalls(int x, int y);
    [[nodiscard]] bool cellHasNorthDoor(int x, int y);
    [[nodiscard]] bool cellHasSouthDoor(int x, int y);
    [[nodiscard]] bool cellHasEastDoor(int x, int y);
    [[nodiscard]] bool cellHasWestDoor(int x, int y);

    [[nodiscard]] bool cellHasSpecialNorth(int x, int y);
    [[nodiscard]] bool cellHasSpecialSouth(int x, int y);
    [[nodiscard]] bool cellHasSpecialEast(int x, int y);
    [[nodiscard]] bool cellHasSpecialWest(int x, int y);

    [[nodiscard]] quint8 getCellDesc(int x, int y);
    [[nodiscard]] quint8 getCellMonsters(int x, int y);
    [[nodiscard]] quint8 getCellInventory(int x, int y);

    [[nodiscard]] QString inventoryToString(quint8 inv);
    [[nodiscard]] QString monsterToString(quint8 mc);
};
