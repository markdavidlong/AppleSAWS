#include "hiresviewwidget.h"
#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>

HiresViewWidget::HiresViewWidget(QWidget *parent) :
    QWidget(parent)
{
    if (m_rowTable == 0) { makeOffsetTable(); }

    qDebug() << "ctor";
    m_pixmap = QPixmap(280,192);
    qDebug() << "Pixmap size: " << m_pixmap.size();
    QPainter painter(&m_pixmap);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height());
}

void HiresViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "paintEvent";

    //    if (m_pixmap.size() != this->size()) {
    //        m_pixmap = m_pixmap.scaled(this->size(),Qt::KeepAspectRatio);
    //    }
    QPainter painter(this);

    QPixmap tmppixmap = m_pixmap;
    tmppixmap = tmppixmap.scaled(this->size(), Qt::KeepAspectRatio);

    painter.drawPixmap(0,0,tmppixmap);
}

void HiresViewWidget::resizeEvent(QResizeEvent *)
{
    qDebug() << "resizeEvent";

}

void HiresViewWidget::setData(QByteArray data) {
    qDebug() << "setData";
    m_data = data;
    QPainter pmpainter(&m_pixmap);
    pmpainter.setPen(Qt::white);
    pmpainter.setBrush(Qt::white);

    for (int idx = 0; idx <  m_data.size() ; idx++) {
        int rowcol = findRowCol(idx);

        quint8 byte = m_data[idx];

        //     qDebug() << "idx: " << idx << (rowcol / 1000) << (rowcol % 1000);
        int xoff = (rowcol / 10000) * 7;
        int yoff = (rowcol % 10000);

        if (byte & 0x01) { pmpainter.drawPoint(xoff+0,yoff); }
        if (byte & 0x02) { pmpainter.drawPoint(xoff+1,yoff); }
        if (byte & 0x04) { pmpainter.drawPoint(xoff+2,yoff); }
        if (byte & 0x08) { pmpainter.drawPoint(xoff+3,yoff); }
        if (byte & 0x10) { pmpainter.drawPoint(xoff+4,yoff); }
        if (byte & 0x20) { pmpainter.drawPoint(xoff+5,yoff); }
        if (byte & 0x40) { pmpainter.drawPoint(xoff+6,yoff); }

        if (idx >= (280*192)) break;
    }

}

int HiresViewWidget::findRowCol(int offset) {
    int retval = 0;

    retval = (*m_rowTable)[offset];

    // qDebug() << offset << " = " << row << col << retval;
    return retval;
}

void HiresViewWidget::makeOffsetTable() {
    m_rowTable = new QMap<int,int>();

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

QMap<int,int> *HiresViewWidget::m_rowTable = 0;

