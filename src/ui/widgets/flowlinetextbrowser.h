#ifndef FLOWLINETEXTBROWSER_H
#define FLOWLINETEXTBROWSER_H

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

#include "jumplinemanager.h"

#include <QTextBrowser>
#include <QObject>
#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>
#include <QScrollEvent>
#include <QToolTip>


class FlowLineTextBrowser : public QTextBrowser
{
    class LineArea;
public:
    FlowLineTextBrowser(QWidget *parent = Q_NULLPTR);

    void lineAreaPaintEvent(QPaintEvent *event);
    int lineAreaWidth();

    void setJumpLines(JumpLines *jl);

    void proxyWheelEvent(QWheelEvent *ev) { if (ev)  { wheelEvent(ev); } }
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    int getFirstVisibleBlock(QTextBlock *firstBlock) const;

    int getChannelOffset(int channel);
    QRect getBlockGeometry(QTextBlock block) const;
    bool isBlockVisible(QTextBlock block) const;
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;

private slots:
    void updateLineAreaWidth();
    void updateLineArea(const QRect &, int);

public slots:
    void setLineAreaVisible(bool visible);

private:
    FlowLineTextBrowser::LineArea *m_lineArea;

    JumpLines *m_jl;


class LineArea : public QWidget
{
public:
    LineArea(FlowLineTextBrowser *browser) : QWidget(browser)
    {
        setMouseTracking(true);
        m_browser = browser;
    }

    void setJumpLines(JumpLines *jl) { m_jl = jl; }

    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(m_browser->lineAreaWidth(),0);
    }
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {
        m_browser->lineAreaPaintEvent(event);
    }

    void wheelEvent(QWheelEvent *ev)  Q_DECL_OVERRIDE { m_browser->proxyWheelEvent(ev); }

    bool event(QEvent *event) Q_DECL_OVERRIDE
    {
        if (event->type() == QEvent::ToolTip) {
               QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
               int index = getChannelForXPos(helpEvent->pos().x());
               if (index != -1) {
                   QToolTip::showText(helpEvent->globalPos(), QString("Channel %1").arg(index));
               } else {
                   QToolTip::hideText();
                   event->ignore();
               }

               return true;
           }
           return QWidget::event(event);
    }

    int getChannelForXPos(int xpos)
    {
        for (int idx = 0; idx < m_browser->m_jl->m_maxChannel; idx++)
        {
            int channeloffset = m_browser->getChannelOffset(idx);
            if (abs(channeloffset - xpos) < 4)
            {
                return idx;
            }
        }
        return -1;
    }


private:
    FlowLineTextBrowser *m_browser;
    JumpLines *m_jl;
};

};
#endif // FLOWLINETEXTBROWSER_H
