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

class FlowLineTextBrowser final : public QTextBrowser
{
    class LineArea;

public:
    explicit FlowLineTextBrowser(QWidget *parent = nullptr);
    ~FlowLineTextBrowser() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    FlowLineTextBrowser(const FlowLineTextBrowser&) = delete;
    FlowLineTextBrowser& operator=(const FlowLineTextBrowser&) = delete;
    FlowLineTextBrowser(FlowLineTextBrowser&&) = delete;
    FlowLineTextBrowser& operator=(FlowLineTextBrowser&&) = delete;

    void lineAreaPaintEvent(QPaintEvent *event);
    [[nodiscard]] int lineAreaWidth() const;

    void setJumpLines(JumpLines *jl);

    void proxyWheelEvent(QWheelEvent *ev)
    {
        if (ev)
        {
            wheelEvent(ev);
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    [[nodiscard]] int getFirstVisibleBlock(QTextBlock *firstBlock) const;
    [[nodiscard]] int getChannelOffset(int channel) const;
    [[nodiscard]] QRect getBlockGeometry(QTextBlock block) const;
    [[nodiscard]] bool isBlockVisible(QTextBlock block) const;
    void showEvent(QShowEvent *) override;

private slots:
    void updateLineAreaWidth();
    void updateLineArea(const QRect &, int);

public slots:
    void setLineAreaVisible(bool visible);

private:
    FlowLineTextBrowser::LineArea *m_lineArea{nullptr};
    JumpLines *m_jl{nullptr};

    class LineArea final : public QWidget
    {
    public:
        explicit LineArea(FlowLineTextBrowser *browser) : QWidget(browser), m_browser(browser)
        {
            setMouseTracking(true);
        }

        void setJumpLines(JumpLines *jl) noexcept { m_jl = jl; }

        [[nodiscard]] QSize sizeHint() const override
        {
            return QSize(m_browser->lineAreaWidth(), 0);
        }

    protected:
        void paintEvent(QPaintEvent *event) override
        {
            m_browser->lineAreaPaintEvent(event);
        }

        void wheelEvent(QWheelEvent *ev) override { m_browser->proxyWheelEvent(ev); }

        bool event(QEvent *event) override
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

        [[nodiscard]] int getChannelForXPos(int xpos) const
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
        JumpLines *m_jl{nullptr};
    };
};
