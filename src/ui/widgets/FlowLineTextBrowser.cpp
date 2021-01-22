#include "FlowLineTextBrowser.h"

#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>

FlowLineTextBrowser::FlowLineTextBrowser(QWidget *parent) : QTextBrowser(parent)
{
    m_lineArea = new LineArea(this);
    m_jl = Q_NULLPTR;

    updateLineAreaWidth();
}

int FlowLineTextBrowser::getFirstVisibleBlock(QTextBlock *firstBlock) const
{
    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                       this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                           this->verticalScrollBar()->sliderPosition()
                           ) ).toRect();

        if (r1.contains(r2, true) || r1.intersects(r2))
        {
   //         qDebug() << r2;
            if (firstBlock)
                *firstBlock = block;
            return i;
        }

        curs.movePosition(QTextCursor::NextBlock);
    }

    if (firstBlock)
        *firstBlock = QTextBlock();

    return 0;
}

void FlowLineTextBrowser::showEvent(QShowEvent *)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());
}

void FlowLineTextBrowser::lineAreaPaintEvent(QPaintEvent *event)
{

    QPainter painter(m_lineArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);

    QTextBlock block;
    getFirstVisibleBlock(&block);

 //   qDebug() << block.text();
    bool foundFirst = false;
    quint16 linenum;

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    bool inBlankLine = false;
    // for each visible block
    while (isBlockVisible(block) && block.next().isValid())
    {
        QString text = block.text();
        if (text.contains(QRegularExpression("[0-9A-Fa-f]{4}:")))
        {
            bool ok = true;
            quint16 addr = (quint16) text.left(4).toInt(&ok,16);
            if (ok)
            {
                //                                qDebug() << "Addr:" << uint16ToHex(addr);
                linenum = addr;
                foundFirst = true;
                inBlankLine = false;
            }
            else
            {
                //               qDebug() << "No address!";
            }
        }
        else
        {
            inBlankLine = true;
        }
        // qDebug() << "Channels:" << m_jl->channelsAtAddress(linenum);

        if (foundFirst)
        {
            QRect blockRect = getBlockGeometry(block);
            float topInY  = blockRect.top() + ((1./6.) * blockRect.height());
            float topOutY = blockRect.top() + ((2./6.) * blockRect.height());
            float botOutY = blockRect.top() + ((3./6.) * blockRect.height());
            float botInY  = blockRect.top() + ((4./6.) * blockRect.height());

            // painter.drawLine(0,blockRect.top(),m_lineArea->width(),blockRect.bottom());

            QList<JumpLine> jllist = m_jl->jumpLinesAtAddress(linenum);

            foreach (JumpLine jl, jllist)
            {
                if (jl.type == IsBranch || jl.type == IsBRA)
                {
                    painter.setPen(Qt::yellow);
                    painter.setBrush(Qt::yellow);

                }
                else if (jl.type == IsJMP)
                {
                    painter.setPen(Qt::white);
                    painter.setBrush(Qt::white);

                }
                else
                {
                    painter.setPen(Qt::red);
                    painter.setBrush(Qt::red);
                }
                int offset = getChannelOffset(jl.channel);
                if (!inBlankLine)
                {
                    if (jl.from == linenum)
                    {
                        if (jl.from == jl.to)
                        {
                            painter.drawLine(offset,              botInY,
                                             m_lineArea->width(), botInY);
                            painter.drawLine(offset, botInY, offset, topOutY);
                            painter.drawLine(offset,              topOutY,
                                             m_lineArea->width(), topOutY);
                  //          painter.drawEllipse(QPointF(m_lineArea->width(),topOutY),2,2);
                        }
                        else
                        {
                            if (jl.isUp())
                            {
                                painter.drawLine(offset, topOutY,
                                                 offset, blockRect.top());
                                painter.drawLine(offset,              topOutY,
                                                 m_lineArea->width(), topOutY);
                                painter.drawEllipse(QPointF(m_lineArea->width(),topOutY),2,2);
                            }
                            else
                            {
                                painter.drawLine(offset, botOutY,
                                                 offset, blockRect.bottom());
                                painter.drawLine(offset,              botOutY,
                                                 m_lineArea->width(), botOutY);
                        //        painter.drawEllipse(QPointF(m_lineArea->width(),botOutY),2,2);
                            }
                        }
                    }
                    else if (jl.to == linenum)
                    {
                        if (jl.isUp())
                        {
                            painter.drawLine(offset, botInY,
                                             offset, blockRect.bottom());
                            painter.drawLine(offset,              botInY,
                                             m_lineArea->width(), botInY);

                            QPolygonF varrow;
                            varrow.append(QPointF(offset,   botInY));
                            varrow.append(QPointF(offset-3, botInY+5));
                            varrow.append(QPointF(offset+3, botInY+5));
                            painter.drawConvexPolygon(varrow);


                            QPolygonF harrow;
                            harrow.append(QPointF(m_lineArea->width(),botInY));
                            harrow.append(QPointF(m_lineArea->width()-4,botInY-3));
                            harrow.append(QPointF(m_lineArea->width()-4,botInY+3));
                            painter.drawConvexPolygon(harrow);
                        }
                        else
                        {
                            painter.drawLine(offset, topInY,
                                             offset, blockRect.top());
                            painter.drawLine(offset,               topInY,
                                             m_lineArea->width(),  topInY);
                            QPolygonF varrow;
                            varrow.append(QPointF(offset,  topInY));
                            varrow.append(QPointF(offset-3, topInY-5));
                            varrow.append(QPointF(offset+3, topInY-5));
                            painter.drawConvexPolygon(varrow);

                            QPolygonF harrow;
                            harrow.append(QPointF(m_lineArea->width(),topInY));
                            harrow.append(QPointF(m_lineArea->width()-4,topInY-3));
                            harrow.append(QPointF(m_lineArea->width()-4,topInY+3));
                            painter.drawConvexPolygon(harrow);
                        }
                    }

                    if (linenum > jl.min() && linenum < jl.max())
                    {
                        painter.drawLine(offset, blockRect.top(),offset, blockRect.bottom());
                    }
                }
                else // inBlankLine
                {
                    if (jl.from == linenum)
                    {
                        if (jl.from != jl.to)
                        {
                            if (jl.isDown())
                            {
                                painter.drawLine(offset, blockRect.top(),offset, blockRect.bottom());

                            }
                        }
                    }
                    else if (jl.to == linenum)
                    {
                        if (jl.isUp())
                        {
                            painter.drawLine(offset, blockRect.top(),offset, blockRect.bottom());
                        }
                    }

                    if (linenum > jl.min() && linenum < jl.max())
                    {
                        painter.drawLine(offset, blockRect.top(),offset, blockRect.bottom());
                    }
                }
            }
        }
        block = block.next();
    }
}

QRect FlowLineTextBrowser::getBlockGeometry(QTextBlock block) const
{
    QRect rect = this->document()->documentLayout()->blockBoundingRect(block).translated(
                     this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                         this->verticalScrollBar()->sliderPosition()
                         ) ).toRect();
    return rect;
}

bool FlowLineTextBrowser::isBlockVisible(QTextBlock block) const
{
    QRect r1 = this->viewport()->geometry();
    QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                   this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                       this->verticalScrollBar()->sliderPosition()
                       ) ).toRect();

    return (r1.contains(r2, true) || r1.intersects(r2));
}

int FlowLineTextBrowser::getChannelOffset(int channel)
{
    int width = lineAreaWidth();
    return width - (channel * 9)- 10;
}

int FlowLineTextBrowser::lineAreaWidth()
{
    if (!m_jl) return 10;
    return m_jl->m_maxChannel* 9 + 20;
}

void FlowLineTextBrowser::setJumpLines(JumpLines *jl)
{
    m_jl = jl;
    m_lineArea->setJumpLines(jl);
    updateLineAreaWidth();
}

void FlowLineTextBrowser::paintEvent(QPaintEvent *event)
{
    m_lineArea->update();
    QTextBrowser::paintEvent(event);
}

void FlowLineTextBrowser::resizeEvent(QResizeEvent *event)
{
    QTextBrowser::resizeEvent(event);

    QRect cr = contentsRect();
    m_lineArea->setGeometry(QRect(cr.left(), cr.top(), lineAreaWidth(), cr.height()));
}

void FlowLineTextBrowser::updateLineAreaWidth()
{
    setViewportMargins(lineAreaWidth(),0,0,0);
}

void FlowLineTextBrowser::updateLineArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineArea->scroll(0, dy);
    else
        m_lineArea->update(0, rect.y(), m_lineArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineAreaWidth();
}

void FlowLineTextBrowser::setLineAreaVisible(bool visible) { m_lineArea->setVisible(visible); }
