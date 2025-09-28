#include "CharacterWidget.h"

#include <QResizeEvent>
#include <QSize>
#include <QPainter>
#include <QBitArray>

CharacterWidget::CharacterWidget(QWidget *parent, CharSetCharacter ch)
    : QWidget(parent), m_character(ch)
{

    m_dobitshift = true;
    m_showgrid = true;
  //  setMaximumSize(this->size());
  //  setMinimumSize(this->size());
    m_pixmap = QPixmap(this->size());
    setFgColor(Qt::black);
    setBgColor(Qt::white);
    setGridColor(Qt::red);
    QString name = QChar(ch.asciiVal());
    if (ch.asciiVal() == ' ') { name = "<Space>"; }
    if (ch.asciiVal() == 0x7f) { name = "<Delete>"; }
    QString ttstring = QString("Ascii: %1\nCharacter: %2").arg(ch.asciiVal()).arg(name);
    setToolTip(ttstring);
    doRepaint();
}

bool CharacterWidget::hasHeightForWidth() const { return true; }
int CharacterWidget::heightForWidth(int w) const { return w * 9 / 8; }

void CharacterWidget::doRepaint()
{
    m_pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&m_pixmap);
    float hscale = width() / 15;
    float vscale = height() / 8;

    painter.setPen(m_bgcolor);
    painter.setBrush(m_bgcolor);
    painter.drawRect(0,0, hscale * 15, vscale * 8);

    painter.setPen(m_fgcolor);
    painter.setBrush(m_fgcolor);

    QByteArray chardata = m_character.data();

    for (quint8 yval = 0; yval < 8; yval++)
    {
        int ypos = yval * vscale;

        quint8 line = chardata[yval];

        QBitArray bits(7);
        bits.setBit(0,(line & 0x01));
        bits.setBit(1,(line & 0x02));
        bits.setBit(2,(line & 0x04));
        bits.setBit(3,(line & 0x08));
        bits.setBit(4,(line & 0x10));
        bits.setBit(5,(line & 0x20));
        bits.setBit(6,(line & 0x40));

        int shiftval = 0;
        painter.setBrush(m_fgcolor);
        if (m_dobitshift && (line & 0x80)) {
            shiftval = 1;
            painter.setBrush(Qt::gray);
        }

        for (int jdx = 0; jdx < 7; jdx++)
        {
            if (bits.testBit(jdx))
            {
                painter.drawRect((jdx*2+shiftval)*hscale, ypos,
                                 hscale*2,                vscale);
            }
        }
    }

    if (m_showgrid)
    {
        painter.setPen(QPen(m_gridcolor,1,Qt::DotLine));
        painter.setBrush(Qt::NoBrush);
        for (int idx = 0; idx < 9; idx++)
        {
            painter.drawLine(0,           idx*vscale,
                             hscale * 15, idx*vscale);
        }
        for (int idx = 0; idx < 8; idx++)
        {
            painter.drawLine(idx*hscale*2, 0,
                             idx*hscale*2, vscale * 8);
        }
        painter.setPen(QPen(m_gridcolor,2,Qt::SolidLine));
        painter.drawLine(0,0, 0, vscale * 8);
        painter.drawLine(0,vscale * 8, hscale * 15, vscale * 8);
        painter.drawLine(hscale * 15, vscale * 8, hscale * 15,0);
        painter.drawLine(hscale * 15,0, 0,0);
    }

    repaint();
}

void CharacterWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,m_pixmap);
}

void CharacterWidget::resizeEvent(QResizeEvent *event)
{
    m_pixmap = m_pixmap.scaled(event->size().width(),event->size().height());
    doRepaint();
}

void CharacterWidget::setFgColor(QColor color)
{
    m_fgcolor = color;
    doRepaint();
}

void CharacterWidget::setBgColor(QColor color)
{
    m_bgcolor = color;
    doRepaint();
}

void CharacterWidget::setGridColor(QColor color)
{
    m_gridcolor = color;
    doRepaint();
}

void CharacterWidget::showGrid(bool show)
{
    m_showgrid = show;
    doRepaint();
}

void CharacterWidget::enableBitShift(bool enable)
{
    m_dobitshift = enable;
    doRepaint();
}
