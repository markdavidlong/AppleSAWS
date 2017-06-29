#ifndef FLOWLINETEXTBROWSER_H
#define FLOWLINETEXTBROWSER_H

#include "JumpLineManager.h"

#include <QTextBrowser>
#include <QObject>
#include <QResizeEvent>
#include <QDebug>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>
#include <QScrollEvent>

class LineArea;

class FlowLineTextBrowser : public QTextBrowser
{
public:
    FlowLineTextBrowser(QWidget *parent = Q_NULLPTR);

    void lineAreaPaintEvent(QPaintEvent *event);
    int lineAreaWidth();

    void setJumpLines(JumpLines *jl);

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
    LineArea *m_lineArea;

    JumpLines *m_jl;
};

class LineArea : public QWidget
{
public:
    LineArea(FlowLineTextBrowser *browser) : QWidget(browser)
    {
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

private:
    FlowLineTextBrowser *m_browser;
    JumpLines *m_jl;
};


#endif // FLOWLINETEXTBROWSER_H
