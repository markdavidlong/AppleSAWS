#pragma once

#include "../../binaryfile/JumpLineManager.h"

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

    void proxyWheelEvent(QWheelEvent *ev)
    {
        if (ev)
        {
            wheelEvent(ev);
        }
    }

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
            return QSize(m_browser->lineAreaWidth(), 0);
        }

    protected:
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
        {
            m_browser->lineAreaPaintEvent(event);
        }

        void wheelEvent(QWheelEvent *ev) Q_DECL_OVERRIDE { m_browser->proxyWheelEvent(ev); }

        bool event(QEvent *event) Q_DECL_OVERRIDE
        {
            if (event->type() == QEvent::ToolTip)
            {
                QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
                int index = getChannelForXPos(helpEvent->pos().x());
                if (index != -1)
                {
                    QToolTip::showText(helpEvent->globalPos(), QString("Channel %1").arg(index));
                }
                else
                {
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
