#include "hiresviewwidget.h"
#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>

#include <math.h>

#define CYCLESTART (M_PI * 4.0 / 16.0)


HiresViewWidget::HiresViewWidget(QWidget *parent) :
    QWidget(parent)
{
    resize(561,384);
    m_viewMode = Color2;
    m_showScanLines = true;

    if (m_rowTable == 0) { makeOffsetTable(); }

    qDebug() << "ctor";
    m_pixmap = QPixmap(561,384);
    qDebug() << "Pixmap size: " << m_pixmap.size();
    QPainter painter(&m_pixmap);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height());

    formatGroup = new QActionGroup(this);

    monochromeAction = new QAction("Monochrome Display",this);
    monochromeAction->setCheckable(true);
    monochromeAction->setChecked(false);
    formatGroup->addAction(monochromeAction);

    ntscAction = new QAction("NTSC Display",this);
    ntscAction->setCheckable(true);
    ntscAction->setChecked(true);
    formatGroup->addAction(ntscAction);

    showScanLinesAction = new QAction("Show Scan Lines",this);
    showScanLinesAction->setCheckable(true);
    showScanLinesAction->setChecked(true);

    connect(ntscAction, SIGNAL(toggled(bool)), this, SLOT(handleNtscAction(bool)));
    connect(monochromeAction, SIGNAL(toggled(bool)), this, SLOT(handleMonochromeAction(bool)));
    connect(showScanLinesAction, SIGNAL(toggled(bool)), this, SLOT(handleShowScanLinesAction(bool)));

}

void HiresViewWidget::handleNtscAction(bool toggled) {
    if (toggled) {
        m_viewMode = Color2;
        update();
    }
}

void HiresViewWidget::handleMonochromeAction(bool toggled) {
    if (toggled) {
        m_viewMode = Monochrome;
        update();
    }
}

void HiresViewWidget::handleShowScanLinesAction(bool toggled) {
    m_showScanLines = toggled;
    update();
}

void HiresViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "paintEvent";

    //    if (m_pixmap.size() != this->size()) {
    //        m_pixmap = m_pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    //    }

    drawPixmap();

    QPainter painter(this);

    QPixmap tmppixmap = m_pixmap;
    tmppixmap = tmppixmap.scaled(this->size(), Qt::KeepAspectRatio);

    painter.drawPixmap(0,0,tmppixmap);
}

void HiresViewWidget::resizeEvent(QResizeEvent *)
{
    qDebug() << "resizeEvent";

}

void HiresViewWidget::drawPixmap() {

    QPainter pmpainter(&m_pixmap);

    pmpainter.setBrush(Qt::black);
    pmpainter.setPen(Qt::black);
    pmpainter.drawRect(0,0,m_pixmap.width(),m_pixmap.height());

    if (m_viewMode == Monochrome)
    {
        pmpainter.setPen(Qt::white);
        pmpainter.setBrush(Qt::white);

        quint8 chunkCount = 0;
        //    for (int idx = 0; idx <  m_data.size() ; idx+=40) {
        int idx = 0;
        while (idx < m_data.size()) {
            int rowcol = findRowCol(idx);
            //        int xoff = (rowcol / 10000) * 7;
            int yoff = (rowcol % 10000);

            QBitArray bits(561,false);
            int bitoffset = 0;
            bool lastbit = false;

            for (int jdx = 0; jdx < 40; jdx++)
            {
                quint8 byte = m_data[idx++];
                QBitArray dataBits = byteToBits(byte);

                bool highBit = dataBits.at(0);

                if (highBit) {
                    bits[bitoffset++] = lastbit;
                }

                for (int xdx = 1; xdx < 7; xdx++) {
                    bits[bitoffset++] = dataBits.at(xdx);
                    bits[bitoffset++] = dataBits.at(xdx);
                }
                lastbit = dataBits.at(7);
                bits[bitoffset++] = dataBits.at(7);
                if (!highBit) {
                    bits[bitoffset++] = dataBits.at(7);
                }
            }


             drawMonoLine(pmpainter, yoff*2, bits);
             if (!m_showScanLines) drawMonoLine(pmpainter, yoff*2+1, bits);


            chunkCount++;
            if (chunkCount == 3) {
                chunkCount = 0;
                idx+=8;
            }

        }
    } else if (m_viewMode == Color1) {

        pmpainter.setPen(Qt::white);
        pmpainter.setBrush(Qt::white);

        for (int idx = 0; idx <  m_data.size() ; idx++) {
            int rowcol = findRowCol(idx);

            quint8 byte = m_data[idx];

            bool highBit = byte & 0x80;

            QColor oddColor = highBit? QColor(orangeColor) : QColor(greenColor);
            QColor evenColor = highBit? QColor(blueColor) : QColor(purpleColor);

            int xoff = (rowcol / 10000) * 7;
            int yoff = (rowcol % 10000)*2;

            quint8 cOffset = 0;// highBit?1:0;

            quint8 doubleScan = 0;

            pmpainter.setPen(xoff & 0x01?oddColor:evenColor);
            pmpainter.setBrush(xoff & 0x01?oddColor:evenColor);

            if (byte & 0x01) { pmpainter.drawRect((cOffset+xoff*2)+0,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?evenColor:oddColor);
            pmpainter.setBrush(xoff & 0x01?evenColor:oddColor);

            if (byte & 0x02) { pmpainter.drawRect((cOffset+xoff*2)+2,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?oddColor:evenColor);
            pmpainter.setBrush(xoff & 0x01?oddColor:evenColor);

            if (byte & 0x04) { pmpainter.drawRect((cOffset+xoff*2)+4,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?evenColor:oddColor);
            pmpainter.setBrush(xoff & 0x01?evenColor:oddColor);

            if (byte & 0x08) { pmpainter.drawRect((cOffset+xoff*2)+6,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?oddColor:evenColor);
            pmpainter.setBrush(xoff & 0x01?oddColor:evenColor);


            if (byte & 0x10) { pmpainter.drawRect((cOffset+xoff*2)+8,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?evenColor:oddColor);
            pmpainter.setBrush(xoff & 0x01?evenColor:oddColor);

            if (byte & 0x20) { pmpainter.drawRect((cOffset+xoff*2)+10,yoff,1,doubleScan); }

            pmpainter.setPen(xoff & 0x01?oddColor:evenColor);
            pmpainter.setBrush(xoff & 0x01?oddColor:evenColor);


            if (byte & 0x40) { pmpainter.drawRect((cOffset+xoff*2)+12,yoff,cOffset?0:1,doubleScan); }



            if (idx >= (280*192)) break;
        }
    } else if (m_viewMode == Color2) {

        pmpainter.setPen(Qt::white);
        pmpainter.setBrush(Qt::white);

        quint8 chunkCount = 0;
        //    for (int idx = 0; idx <  m_data.size() ; idx+=40) {
        int idx = 0;
        while (idx < m_data.size()) {
            int rowcol = findRowCol(idx);
            //        int xoff = (rowcol / 10000) * 7;
            int yoff = (rowcol % 10000);

            QBitArray bits(561,false);
            int bitoffset = 0;
            bool lastbit = false;

            for (int jdx = 0; jdx < 40; jdx++)
            {
                quint8 byte = m_data[idx++];
                QBitArray dataBits = byteToBits(byte);

                bool highBit = dataBits.at(0);

                if (highBit) {
                    bits[bitoffset++] = lastbit;
                }

                for (int xdx = 1; xdx < 7; xdx++) {
                    bits[bitoffset++] = dataBits.at(xdx);
                    bits[bitoffset++] = dataBits.at(xdx);
                }
                lastbit = dataBits.at(7);
                bits[bitoffset++] = dataBits.at(7);
                if (!highBit) {
                    bits[bitoffset++] = dataBits.at(7);
                }
            }

            drawNtscLine(pmpainter, yoff*2, bits);
            if (!m_showScanLines) {
                drawNtscLine(pmpainter, yoff*2+1, bits);
            }

            chunkCount++;
            if (chunkCount == 3) {
                chunkCount = 0;
                idx+=8;
            }
        }
    }
}

void HiresViewWidget::setData(QByteArray data) {
    qDebug() << "setData";
    m_data = data;

    drawPixmap();
}

void HiresViewWidget::drawMonoLine(QPainter &painter, int lineNum, QBitArray data) {
    for (int idx = 0; idx < data.count(); idx++) {

        if (data.at(idx))
        {
            painter.setPen(Qt::white);
        } else {
            painter.setPen(Qt::black);
        }


        painter.drawPoint(idx,lineNum);
    }
}

QColor HiresViewWidget::getColorFromBits(QBitArray bits, quint8 phase)
{
    quint8 bitval = (bits[0] * 0x08) +
            (bits[1] * 0x04) +
            (bits[2] * 0x02) +
            (bits[3] * 0x01);

    phase %= 4;

    //qDebug() << bits << phase;


    if (bitval == 0) { return blackColor; }

    if (bitval == 1 && phase == 0) return brownColor;
    if (bitval == 2 && phase == 1) return brownColor;
    if (bitval == 4 && phase == 2) return brownColor;
    if (bitval == 8 && phase == 3) return brownColor;

    if (bitval == 2 && phase == 0) return darkGreenColor;
    if (bitval == 4 && phase == 1) return darkGreenColor;
    if (bitval == 8 && phase == 2) return darkGreenColor;
    if (bitval == 1 && phase == 3) return darkGreenColor;

    if (bitval == 3 && phase == 0) return greenColor;
    if (bitval == 6 && phase == 1) return greenColor;
    if (bitval == 12 && phase == 2) return greenColor;
    if (bitval == 9 && phase == 3) return greenColor;

    if (bitval == 4 && phase == 0) return darkBlueColor;
    if (bitval == 8 && phase == 1) return darkBlueColor;
    if (bitval == 1 && phase == 2) return darkBlueColor;
    if (bitval == 2 && phase == 3) return darkBlueColor;

    if (bitval == 5 && phase == 0) return grayColor;
    if (bitval == 10 && phase == 1) return grayColor;
    if (bitval == 5 && phase == 2) return grayColor;
    if (bitval == 10 && phase == 3) return grayColor;

    if (bitval == 6 && phase == 0) return blueColor;
    if (bitval == 12 && phase == 1) return blueColor;
    if (bitval == 9 && phase == 2) return blueColor;
    if (bitval == 3 && phase == 3) return blueColor;

    if (bitval == 7 && phase == 0) return aquaColor;
    if (bitval == 14 && phase == 1) return aquaColor;
    if (bitval == 13 && phase == 2) return aquaColor;
    if (bitval == 11 && phase == 3) return aquaColor;

    if (bitval == 8 && phase == 0) return redColor;
    if (bitval == 1 && phase == 1) return redColor;
    if (bitval == 2 && phase == 2) return redColor;
    if (bitval == 4 && phase == 3) return redColor;

    if (bitval == 9 && phase == 0) return orangeColor;
    if (bitval == 3 && phase == 1) return orangeColor;
    if (bitval == 6 && phase == 2) return orangeColor;
    if (bitval == 12 && phase == 3) return orangeColor;

    if (bitval == 10 && phase == 0) return gray2Color;
    if (bitval == 5 && phase == 1) return gray2Color;
    if (bitval == 10 && phase == 2) return gray2Color;
    if (bitval == 5 && phase == 3) return gray2Color;

    if (bitval == 11 && phase == 0) return yellowColor;
    if (bitval == 7 && phase == 1) return yellowColor;
    if (bitval == 14 && phase == 2) return yellowColor;
    if (bitval == 13 && phase == 3) return yellowColor;

    if (bitval == 12 && phase == 0) return purpleColor;
    if (bitval == 9 && phase == 1) return purpleColor;
    if (bitval == 3 && phase == 2) return purpleColor;
    if (bitval == 6 && phase == 3) return purpleColor;

    if (bitval == 13 && phase == 0) return pinkColor;
    if (bitval == 11 && phase == 1) return pinkColor;
    if (bitval == 7 && phase == 2) return pinkColor;
    if (bitval == 14 && phase == 3) return pinkColor;

    if (bitval == 14 && phase == 0) return lightBlueColor;
    if (bitval == 13 && phase == 1) return lightBlueColor;
    if (bitval == 11 && phase == 2) return lightBlueColor;
    if (bitval == 7 && phase == 3) return lightBlueColor;

    return whiteColor;

}

void HiresViewWidget::drawNtscLine(QPainter &painter, int lineNum, QBitArray data) {
    QVector<QColor> colors;
    colors.resize(data.count()+4);

    for (int idx = 0; idx < data.count(); idx++) {
        QBitArray tmp(4);
        tmp[0]=data.at(idx+0);
        if (idx < data.count()-1) tmp[1]=data.at(idx+1); else tmp[1] = false;
        if (idx < data.count()-2) tmp[2]=data.at(idx+2); else tmp[2] = false;
        if (idx < data.count()-3) tmp[3]=data.at(idx+3); else tmp[3] = false;
        colors[idx] = getColorFromBits(tmp,idx %4);
        colors[idx+1] = getColorFromBits(tmp,idx %4);
        colors[idx+2] = getColorFromBits(tmp,idx %4);
        colors[idx+3] = getColorFromBits(tmp,idx %4);
    }


    for (int idx = 0; idx < colors.count(); idx++)
    {
        painter.setPen(colors.at(idx));
        painter.setBrush(colors.at(idx));
        painter.drawPoint(idx,lineNum);
    }
}


QBitArray HiresViewWidget::byteToBits(quint8 byte) {
    QBitArray bits(8);
    bits.setBit(0,byte & 0x80);
    bits.setBit(7,byte & 0x40);
    bits.setBit(6,byte & 0x20);
    bits.setBit(5,byte & 0x10);
    bits.setBit(4,byte & 0x08);
    bits.setBit(3,byte & 0x04);
    bits.setBit(2,byte & 0x02);
    bits.setBit(1,byte & 0x01);
    return bits;
}

void HiresViewWidget::setMode(HiresViewWidget::ViewMode viewmode)
{
    m_viewMode = viewmode;
    update();
}

int HiresViewWidget::findRowCol(int offset) {
    int retval = 0;
    retval = (*m_rowTable)[offset];
    return retval;
}

void HiresViewWidget::makeOffsetTable() {
    m_rowTable = new QMap<int,int>();

    for (int idx = 0; idx < 8192; idx++)
    {
        (*m_rowTable)[idx] = -1; // Fill the memory holes.  Brute force, but it works.
    }

    int count = 0;

    int outer = 0x0000;
    for (int idx = 0; idx < 8; idx++) {
        int inner = 0x0000;
        for (int jdx = 0; jdx < 8; jdx++) {
            for (int kdx = 0; kdx<40;kdx++) {
                (*m_rowTable)[outer+inner+kdx] = count + (kdx*10000);
            }
            count++;
            inner += 0x0400;
        }
        outer += 0x0080;
    }


    outer = 0x0028;
    for (int idx = 0; idx < 8; idx++) {
        int inner = 0x0000;
        for (int jdx = 0; jdx < 8; jdx++) {
            for (int kdx = 0; kdx<40;kdx++) {
                (*m_rowTable)[outer+inner+kdx] =  count + (kdx*10000);
            }
            count++;
            inner += 0x0400;
        }
        outer += 0x0080;
    }

    outer = 0x0050;
    for (int idx = 0; idx < 8; idx++) {
        int inner = 0x0000;
        for (int jdx = 0; jdx < 8; jdx++) {
            for (int kdx = 0; kdx<40;kdx++) {
                (*m_rowTable)[outer+inner+kdx] =  count + (kdx*10000);
            }
            count++;
            inner += 0x0400;
        }
        outer += 0x0080;
    }


}

void HiresViewWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(monochromeAction);
    menu.addAction(ntscAction);
    menu.addSeparator();
    menu.addAction(showScanLinesAction);
    menu.exec(event->globalPos());
}

QMap<int,int> *HiresViewWidget::m_rowTable = 0;

