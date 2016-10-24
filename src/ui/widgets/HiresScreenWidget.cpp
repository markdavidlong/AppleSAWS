#include "HiresScreenWidget.h"

#include "binaryfile.h"

#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QSettings>

#include "util.h"

#include <math.h>


HiresScreenWidget::HiresScreenWidget(QWidget *parent) :
    QWidget(parent)
{
    if (m_rawAddressToColRowList.size() == 0 ||
            m_appleAddressToColRowList.size() == 0)
    {
        makeAddressTables();
    }

    resize(561,384);
    QSettings settings;
    m_viewMode = static_cast<ViewMode>(settings.value("HiresScreenWidget.ViewMode",NTSCColor).toInt());
    m_showScanLines = settings.value("HiresScreenWidget.ShowScanLines",true).toBool();

    m_pixmap = QPixmap(561,384);
    QPainter painter(&m_pixmap);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height());

    formatGroup = new QActionGroup(this);

    m_monochromeAction = new QAction("Monochrome Display",this);
    m_monochromeAction->setCheckable(true);
    m_monochromeAction->setChecked((m_viewMode == Monochrome));
    formatGroup->addAction(m_monochromeAction);

    m_ntscAction = new QAction("NTSC Display",this);
    m_ntscAction->setCheckable(true);
    m_ntscAction->setChecked((m_viewMode == NTSCColor));
    formatGroup->addAction(m_ntscAction);

    m_perPixelColorAction= new QAction("Per-Pixel Color Display",this);
    m_perPixelColorAction->setCheckable(true);
    m_perPixelColorAction->setChecked((m_viewMode == PerPixelColor));
    formatGroup->addAction(m_perPixelColorAction);

    m_showScanLinesAction = new QAction("Show Scan Lines",this);
    m_showScanLinesAction->setCheckable(true);
    m_showScanLinesAction->setChecked(m_showScanLines);


    connect(m_ntscAction, SIGNAL(toggled(bool)), this, SLOT(handleNtscAction(bool)));
    connect(m_monochromeAction, SIGNAL(toggled(bool)), this, SLOT(handleMonochromeAction(bool)));
    connect(m_perPixelColorAction, SIGNAL(toggled(bool)), this, SLOT(handlePerPixelColorAction(bool)));

    connect(m_showScanLinesAction, SIGNAL(toggled(bool)), this, SLOT(handleShowScanLinesAction(bool)));

}

void HiresScreenWidget::handleNtscAction(bool toggled) {
    if (toggled) {
        m_viewMode = NTSCColor;
        update();
    }
    QSettings settings;
    settings.setValue("HiresScreenWidget.ViewMode",m_viewMode);
}

void HiresScreenWidget::handleMonochromeAction(bool toggled) {
    if (toggled) {
        m_viewMode = Monochrome;
        update();
    }
    QSettings settings;
    settings.setValue("HiresScreenWidget.ViewMode",m_viewMode);
}

void HiresScreenWidget::handlePerPixelColorAction(bool toggled) {
    if (toggled) {
        m_viewMode = PerPixelColor;
        update();
    }
    QSettings settings;
    settings.setValue("HiresScreenWidget.ViewMode",m_viewMode);
}

void HiresScreenWidget::handleShowScanLinesAction(bool toggled) {
    m_showScanLines = toggled;
    update();
    QSettings settings;
    settings.setValue("HiresScreenWidget.ShowScanLines",toggled);
}


void HiresScreenWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    //    if (m_pixmap.size() != this->size()) {
    //        m_pixmap = m_pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    //    }

    drawPixmap();

    QPainter painter(this);

    QPixmap tmppixmap = m_pixmap;
    //    tmppixmap = tmppixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter.drawPixmap(0,0,tmppixmap);
}

void HiresScreenWidget::resizeEvent(QResizeEvent *)
{

}

void HiresScreenWidget::drawPixmap()
{
    QPainter pmpainter(&m_pixmap);

    pmpainter.setBrush(Qt::black);
    pmpainter.setPen(Qt::black);
    pmpainter.drawRect(0,0,m_pixmap.width(),m_pixmap.height());

    if (m_viewMode == Monochrome || m_viewMode == NTSCColor)
    {
        pmpainter.setPen(Qt::white);
        pmpainter.setBrush(Qt::white);

        quint8 chunkCount = 0;

        int idx = 0;
        while (idx < qMin(m_data.size(),8192)) {
            ColRow cr = getColRowFromAppleAddress(idx);

            int yoff = cr.row();

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

            if (m_viewMode == Monochrome)
            {
                drawMonoLine(pmpainter, yoff*2, bits);
                if (!m_showScanLines) drawMonoLine(pmpainter, yoff*2+1, bits);
            }
            else
            {
                drawNtscLine(pmpainter, yoff*2, bits);
                if (!m_showScanLines) drawNtscLine(pmpainter, yoff*2+1, bits);
            }

            chunkCount++;
            if (chunkCount == 3) {
                chunkCount = 0;
                idx+=8; // Skip screen hole
            }

        }
    }
    else if (m_viewMode == PerPixelColor)
    {
        pmpainter.setPen(Qt::white);
        pmpainter.setBrush(Qt::white);

        for (int idx = 0; idx <  qMin(m_data.size(),8192) ; idx++) {
            ColRow cr = getColRowFromAppleAddress(idx);

            quint8 byte = m_data[idx];

            bool highBit = byte & 0x80;

            QColor oddColor = highBit? QColor(orangeColor) : QColor(greenColor);
            QColor evenColor = highBit? QColor(blueColor) : QColor(purpleColor);

            int xoff = cr.col() * 7;
            int yoff = cr.row() * 2;

            quint8 cOffset =  highBit?1:0;

            quint8 doubleScan = 0;
            if (!m_showScanLines)
            {
                doubleScan = 1;
            }

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

    }

}

void HiresScreenWidget::setUnpackedData(QByteArray unpackedData)
{
    QByteArray packedData = packData(unpackedData);
    setData(packedData);
}

QByteArray HiresScreenWidget::packData(QByteArray unpackedData)
{
    QByteArray packedData;
    packedData.resize(8192);
    packedData.fill(0x00);

    for (int idx = 0; idx < qMin(unpackedData.count(),8192); idx++)
    {
        ColRow cr = getColRowFromRawAddress(idx);

        if (cr.isDefined())
        {
            packedData[cr.appleAddress()] = unpackedData[idx];
        }
    }

    return packedData;
}

int HiresScreenWidget::getLineAddressOffset(int line)
{
    static QList<quint16> blockOffset {
        0x0000, 0x0080, 0x0100, 0x0180, 0x0200, 0x0280, 0x0300, 0x0380,
        0x0028, 0x00A0, 0x0128, 0x01A8, 0x0228, 0x02A8, 0x0328, 0x03A8,
        0x0040, 0x00D0, 0x0150, 0x00D0, 0x0250, 0x02D0, 0x0350, 0x03D0
    };

    static QList<quint16> boxSub {
        0x0000, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00
    };

    int block = line / 8;
    int sub = line % 8;

    return blockOffset[block] + boxSub[sub];
}

void HiresScreenWidget::setData(QByteArray data) {
    m_data = data;

    repaint();
}

void HiresScreenWidget::drawMonoLine(QPainter &painter, int lineNum, QBitArray data) {
    painter.setPen(Qt::black);
    painter.drawLine(0,lineNum,data.count(),lineNum);
    painter.setPen(Qt::white);
    for (int idx = 0; idx < data.count(); idx++) {

        if (data.at(idx))
            painter.drawPoint(idx,lineNum);
    }
}

QColor HiresScreenWidget::getColorFromBits(QBitArray bits, quint8 phase)
{
    quint8 bitval = (bits[0] * 0x08) +
            (bits[1] * 0x04) +
            (bits[2] * 0x02) +
            (bits[3] * 0x01);

    phase %= 4;

    if (bitval == 0) { return blackColor; }

    if (bitval == 1  && phase == 0) return brownColor;
    if (bitval == 2  && phase == 1) return brownColor;
    if (bitval == 4  && phase == 2) return brownColor;
    if (bitval == 8  && phase == 3) return brownColor;

    if (bitval == 2  && phase == 0) return darkGreenColor;
    if (bitval == 4  && phase == 1) return darkGreenColor;
    if (bitval == 8  && phase == 2) return darkGreenColor;
    if (bitval == 1  && phase == 3) return darkGreenColor;

    if (bitval == 3  && phase == 0) return greenColor;
    if (bitval == 6  && phase == 1) return greenColor;
    if (bitval == 12 && phase == 2) return greenColor;
    if (bitval == 9  && phase == 3) return greenColor;

    if (bitval == 4  && phase == 0) return darkBlueColor;
    if (bitval == 8  && phase == 1) return darkBlueColor;
    if (bitval == 1  && phase == 2) return darkBlueColor;
    if (bitval == 2  && phase == 3) return darkBlueColor;

    if (bitval == 5  && phase == 0) return grayColor;
    if (bitval == 10 && phase == 1) return grayColor;
    if (bitval == 5  && phase == 2) return grayColor;
    if (bitval == 10 && phase == 3) return grayColor;

    if (bitval == 6  && phase == 0) return blueColor;
    if (bitval == 12 && phase == 1) return blueColor;
    if (bitval == 9  && phase == 2) return blueColor;
    if (bitval == 3  && phase == 3) return blueColor;

    if (bitval == 7  && phase == 0) return aquaColor;
    if (bitval == 14 && phase == 1) return aquaColor;
    if (bitval == 13 && phase == 2) return aquaColor;
    if (bitval == 11 && phase == 3) return aquaColor;

    if (bitval == 8  && phase == 0) return redColor;
    if (bitval == 1  && phase == 1) return redColor;
    if (bitval == 2  && phase == 2) return redColor;
    if (bitval == 4  && phase == 3) return redColor;

    if (bitval == 9  && phase == 0) return orangeColor;
    if (bitval == 3  && phase == 1) return orangeColor;
    if (bitval == 6  && phase == 2) return orangeColor;
    if (bitval == 12 && phase == 3) return orangeColor;

    if (bitval == 10 && phase == 0) return gray2Color;
    if (bitval == 5  && phase == 1) return gray2Color;
    if (bitval == 10 && phase == 2) return gray2Color;
    if (bitval == 5  && phase == 3) return gray2Color;

    if (bitval == 11 && phase == 0) return yellowColor;
    if (bitval == 7  && phase == 1) return yellowColor;
    if (bitval == 14 && phase == 2) return yellowColor;
    if (bitval == 13 && phase == 3) return yellowColor;

    if (bitval == 12 && phase == 0) return purpleColor;
    if (bitval == 9  && phase == 1) return purpleColor;
    if (bitval == 3  && phase == 2) return purpleColor;
    if (bitval == 6  && phase == 3) return purpleColor;

    if (bitval == 13 && phase == 0) return pinkColor;
    if (bitval == 11 && phase == 1) return pinkColor;
    if (bitval == 7  && phase == 2) return pinkColor;
    if (bitval == 14 && phase == 3) return pinkColor;

    if (bitval == 14 && phase == 0) return lightBlueColor;
    if (bitval == 13 && phase == 1) return lightBlueColor;
    if (bitval == 11 && phase == 2) return lightBlueColor;
    if (bitval == 7  && phase == 3) return lightBlueColor;

    return whiteColor;
}

void HiresScreenWidget::drawNtscLine(QPainter &painter, int lineNum, QBitArray data) {
    QVector<QColor> colors;
    colors.resize(data.count()+3);

    for (int idx = 0; idx < data.count(); idx++) {
        QBitArray tmp(4);
        tmp[0]=data.at(idx+0);
        if (idx < data.count()-1) tmp[1]=data.at(idx+1); else tmp[1] = false;
        if (idx < data.count()-2) tmp[2]=data.at(idx+2); else tmp[2] = false;
        if (idx < data.count()-3) tmp[3]=data.at(idx+3); else tmp[3] = false;
        colors[idx]   = getColorFromBits(tmp,idx %4);
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


QBitArray HiresScreenWidget::byteToBits(quint8 byte) {
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

void HiresScreenWidget::setMode(HiresScreenWidget::ViewMode viewmode)
{
    m_viewMode = viewmode;
    update();
}

void HiresScreenWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(m_monochromeAction);
    menu.addAction(m_ntscAction);
    menu.addAction(m_perPixelColorAction);
    menu.addSeparator();
    menu.addAction(m_showScanLinesAction);
    menu.exec(event->globalPos());
}

HiresScreenWidget::ColRow HiresScreenWidget::getColRowFromAppleAddress(quint16 address)
{
    if (address > 8191) {
        qDebug() << "Invalid apple address " << address;
        address = 8191;
    }
    return m_appleAddressToColRowList[address];
}

HiresScreenWidget::ColRow HiresScreenWidget::getColRowFromRawAddress(quint16 address)
{
    if (address > 8191) {
        qDebug() << "Invalid raw address " << address;
        address = 8191;
    }
    return m_rawAddressToColRowList[address];
}

void HiresScreenWidget::makeAddressTables()
{
    m_rawAddressToColRowList.resize(8192);
    m_appleAddressToColRowList.resize(8192);

    for (int row = 0; row < 192; row++)
    {
        for (int col = 0; col < 40; col++)
        {
            ColRow cr(col,row);

            m_rawAddressToColRowList[cr.rawAddress()] = cr;
            m_appleAddressToColRowList[cr.appleAddress()] = cr;
        }
    }

#ifdef DEBUGHOLES
    int num = 0;
    quint16 lasthole = 0;
    for (int idx = 0; idx < 8192; idx++)
    {
        if (!m_appleAddressToColRowList[idx].isDefined())
        {
            if (lasthole != 0 && lasthole != (idx-1)) { qDebug() << "\n"; }
            qDebug() << "Hole" << num << "at" << idx+0x2000 << uint16ToHex(idx+0x2000); ;
            num++;
            lasthole = idx;
        }
    }
#endif
}


QVector<HiresScreenWidget::ColRow> HiresScreenWidget::m_rawAddressToColRowList
= QVector<HiresScreenWidget::ColRow>();
QVector<HiresScreenWidget::ColRow> HiresScreenWidget::m_appleAddressToColRowList
= QVector<HiresScreenWidget::ColRow>();