#include "mazeviewer.h"
#include <QPainter>
#include <QResizeEvent>

#include "memory.h"

MazeViewer::MazeViewer(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(480,600);
    m_maze = QPixmap(width(),height());
    drawMaze();
}

void MazeViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_maze = QPixmap(event->size().width(),
                     event->size().height());
    drawMaze();
}

void MazeViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    drawMaze();
    QPainter painter(this);
    painter.drawPixmap(0,0,m_maze);
}

void MazeViewer::setFile(BinaryFile *file)
{
    m_file = file;

    QString title = QString("Maze Viewer: %1").arg(m_file->filename());
    setWindowTitle(title);

    quint16 address = file->address();
    mem.addFile(file->data(), address);

}


void MazeViewer::drawMaze()
{
    QPainter painter(&m_maze);

    painter.setBrush(QColor(0xff,0xff,0xff));
    painter.setPen(QColor(0xff,0xff,0xff));
    painter.drawRect(0,0,m_maze.width(),m_maze.height());

    double cellWidth = width()/8;
    double cellHeight = height()/10;

    painter.setBrush(QColor(0xff,0xff,0xff));

    QPen lockedDoorPen(Qt::red);
    lockedDoorPen.setStyle(Qt::DashLine);

    QPen hiddenDoorPen(Qt::blue);
    hiddenDoorPen.setStyle(Qt::DashLine);
    hiddenDoorPen.setWidth(2);

    for (int idx = 0; idx < 8; idx++)
    {
        for (int jdx = 0; jdx < 10; jdx++)
        {
            double cellTop = jdx * cellHeight + 1;
            double cellBot = (jdx+1) * cellHeight - 1;

            double cellTopQuarter = cellTop + (cellHeight/4);
            double cellBotQuarter = cellBot - (cellHeight/4);

            double cellLeft = idx * cellWidth + 1;
            double cellRight = (idx+1) * cellWidth - 1;

            double cellLeftQuarter = cellLeft + (cellWidth/4);
            double cellRightQuarter = cellRight - (cellWidth/4);


            if (cellHasWalls(idx,jdx))
            {
                painter.setPen(QColor(0xff,0xff,0xff));
                painter.setBrush(QColor(0xff,0xff,0xff));
                painter.drawRect(cellLeft, cellTop, cellWidth-2, cellHeight-2);
                painter.setPen(Qt::black);
                if (cellHasEastDoor(idx,jdx))
                {
                    painter.drawLine(QPointF(cellRight,cellTop),QPointF(cellRight,cellTopQuarter));
                    painter.drawLine(QPointF(cellRight,cellBotQuarter),QPointF(cellRight,cellBot));

                    if (cellHasSpecialEast(idx,jdx)) { // Locked Door
                        QPen oldpen = painter.pen();

                        painter.setPen(lockedDoorPen);
                        painter.drawLine(QPointF(cellRight,cellTopQuarter),QPointF(cellRight,cellBotQuarter));
                        painter.setPen(oldpen);
                    }
                }
                else
                {
                    painter.drawLine(QPointF(cellRight,cellTop),QPointF(cellRight,cellBot));
                    if (cellHasSpecialEast(idx,jdx)) { // Hidden Door
                        QPen oldpen = painter.pen();

                        painter.setPen(hiddenDoorPen);
                        painter.drawLine(QPointF(cellRight,cellTopQuarter),QPointF(cellRight,cellBotQuarter));
                        painter.setPen(oldpen);
                    }
                }

                if (cellHasWestDoor(idx,jdx))
                {
                    painter.drawLine(QPointF(cellLeft,cellTop),QPointF(cellLeft,cellTopQuarter));
                    painter.drawLine(QPointF(cellLeft,cellBotQuarter),QPointF(cellLeft,cellBot));

                    if (cellHasSpecialWest(idx,jdx)) { // Locked Door
                        QPen oldpen = painter.pen();

                        painter.setPen(lockedDoorPen);
                        painter.drawLine(QPointF(cellLeft,cellTopQuarter),QPointF(cellLeft,cellBotQuarter));
                        painter.setPen(oldpen);
                    }
                }
                else
                {
                    painter.drawLine(QPointF(cellLeft,cellTop),QPointF(cellLeft,cellBot));
                    if (cellHasSpecialWest(idx,jdx)) { // Hidden Door
                        QPen oldpen = painter.pen();

                        painter.setPen(hiddenDoorPen);
                        painter.drawLine(QPointF(cellLeft+1,cellTopQuarter),QPointF(cellLeft+1,cellBotQuarter));
                        painter.setPen(oldpen);
                    }
                }

                if (cellHasNorthDoor(idx,jdx))
                {
                    painter.drawLine(QPointF(cellLeft,cellTop),QPointF(cellLeftQuarter,cellTop));
                    painter.drawLine(QPointF(cellRight,cellTop),QPointF(cellRightQuarter,cellTop));


                    if (cellHasSpecialNorth(idx,jdx)) { // Locked Door
                        QPen oldpen = painter.pen();

                        painter.setPen(lockedDoorPen);
                        painter.drawLine(QPointF(cellLeftQuarter,cellTop),QPointF(cellRightQuarter,cellTop));
                        painter.setPen(oldpen);
                    }
                }
                else
                {
                    painter.drawLine(QPointF(cellLeft,cellTop),QPointF(cellRight,cellTop));
                    if (cellHasSpecialNorth(idx,jdx)) { // Hidden Door
                        QPen oldpen = painter.pen();

                        painter.setPen(hiddenDoorPen);
                        painter.drawLine(QPointF(cellLeftQuarter,cellTop+1),QPointF(cellRightQuarter,cellTop+1));
                        painter.setPen(oldpen);
                    }
                }

                if (cellHasSouthDoor(idx,jdx))
                {
                    painter.drawLine(QPointF(cellLeft,cellBot),QPointF(cellLeftQuarter,cellBot));
                    painter.drawLine(QPointF(cellRightQuarter,cellBot),QPointF(cellRight,cellBot));


                    if (cellHasSpecialSouth(idx,jdx)) { // Locked Door
                        QPen oldpen = painter.pen();

                        painter.setPen(lockedDoorPen);
                        painter.drawLine(QPointF(cellLeftQuarter,cellBot),QPointF(cellRightQuarter,cellBot));
                        painter.setPen(oldpen);
                    }
                }
                else
                {
                    painter.drawLine(QPointF(cellLeft,cellBot),QPointF(cellRight,cellBot));
                    if (cellHasSpecialSouth(idx,jdx)) { // Hidden Door
                        QPen oldpen = painter.pen();

                        painter.setPen(hiddenDoorPen);
                        painter.drawLine(QPointF(cellLeftQuarter,cellBot),QPointF(cellRightQuarter,cellBot));
                        painter.setPen(oldpen);
                    }

                }
            }
            else
            {
                painter.setPen(QColor(0xef,0xef,0xef));
                painter.setBrush(QColor(0xef,0xef,0xef));
                painter.drawRect(cellLeft, cellTop, cellWidth-2, cellHeight-2);
            }

            painter.setPen(Qt::black);
            quint8 cv = getCellDesc(idx,jdx);
            quint8 tr = getCellInventory(idx,jdx);
            QString trs = inventoryToString(tr);
            quint8 mo = getCellMonsters(idx,jdx);
            QString mos = monsterToString(mo);

            //QString str = QString("%1\nInv:0x%3\nMo:0x%4")
            //        .arg(idx+(jdx*8))
            //        .arg(cv,2,16,QChar('0'))
            //        .arg(tr,2,16,QChar('0'))
            //        .arg(mo,2,16,QChar('0'));

            QString str = QString("%1\n%2\n%3")
                    .arg(idx+(jdx*8))
                    .arg(mos)
                    .arg(trs);

            painter.drawText(QRect(cellLeft+2,cellTop+2,cellWidth-2,cellHeight-2),str);
        }
    }

}

bool MazeViewer::cellHasWalls(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc != 0xFF);
}

bool MazeViewer::cellHasNorthDoor(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x04);
}

bool MazeViewer::cellHasSpecialNorth(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x40);
}

bool MazeViewer::cellHasEastDoor(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x08);
}

bool MazeViewer::cellHasSpecialEast(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x80);
}

bool MazeViewer::cellHasSouthDoor(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x01);
}

bool MazeViewer::cellHasSpecialSouth(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x10);
}

bool MazeViewer::cellHasWestDoor(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x02);
}

bool MazeViewer::cellHasSpecialWest(int x, int y)
{
    quint8 cellDesc = getCellDesc(x,y);
    return (cellDesc & 0x20);
}

quint8 MazeViewer::getCellDesc(int x, int y)
{
    return mem.at(0x8100+x+(y*8));
}

quint8 MazeViewer::getCellMonsters(int x, int y)
{
    return mem.at(0x81A0+x+(y*8));
}

quint8 MazeViewer::getCellInventory(int x, int y)
{
    return mem.at(0x8150+x+(y*8));
}

QString MazeViewer::inventoryToString(quint8 inv)
{
    quint8 gems = (inv & 0xf0) >> 4;
    quint8 gold_or_special = (inv & 0x0f);

    if (gems == 0)  // Special item;
    {
        if (gold_or_special == 0x00) { return ""; }
        if (gold_or_special == 0x01) { return "Key1"; }
        if (gold_or_special == 0x02) { return "Key2"; }
        if (gold_or_special == 0x03) { return "Key3"; }
        if (gold_or_special == 0x04) { return "Mirror"; }
        if (gold_or_special == 0x05) { return "Goblet"; }
        if (gold_or_special == 0x06) { return "Stauette"; }
        if (gold_or_special == 0x07) { return "UNKN 7"; }
        if (gold_or_special == 0x08) { return "UNKN 8"; }
        if (gold_or_special == 0x09) { return "UNKN 9"; }
        if (gold_or_special == 0x0A) { return "UNKN 10"; }
        if (gold_or_special == 0x0B) { return "UNKN 11"; }
        if (gold_or_special == 0x0C) { return "UNKN 12"; }
        if (gold_or_special == 0x0D) { return "UNKN 13"; }
        if (gold_or_special == 0x0E) { return "UNKN 14"; }
        if (gold_or_special == 0x0F) { return "UNKN 15"; }
        return "";
    }
    else
    {
        return QString("%1Gm,%2Au").arg(gems).arg(gold_or_special * 4);
    }
}

QString MazeViewer::monsterToString(quint8 mc)
{
    quint8 count = mc & 0x0f;
    quint8 monster = (mc & 0xf0) >> 4;

    QString name;
    switch (monster) {
    case 0x00:
        name = "";
        break;
    case 0x01:
        name = "LgSpdr";
        break;
    case 0x02:
        name = "GntRat";
        break;
    case 0x03:
        name = "Orc";
        break;
    case 0x04:
        name = "FrBtle";
        break;
    case 0x05:
        name = "Goblin";
        break;
    case 0x06:
        name = "Sklton";
        break;
    case 0x07:
        name = "Zombie";
        break;
    case 0x08:
        name = "Grgyle";
        break;
    case 0x09:
        name = "DgOfWr";
        break;
    case 0x0a:
        name = "HlHnd";
        break;
    case 0x0b:
        name = "Wrght";
        break;
    case 0x0c:
        name = "HgSpdr";
        break;
    case 0x0d:
        name = "Hmncls";
        break;
    case 0x0e:
        name = "UNKN";
        break;
    case 0x0f:
        name = "EvlMage";
        break;
    default:
        name = "UNK";
        break;
    }

    if (count > 0 && monster > 0)
    {
        return QString("%1 %2").arg(count).arg(name);
    }
    return "";
}
