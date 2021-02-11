#include "sequenceoutputview.h"

#include <QRect>
#include <QPainter>
#include <QTextBlock>

SequenceOutputView::SequenceOutputView(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new SOVNumberPanel(this);
    m_curdoc = nullptr;
    setFont(QFont("Courier",12));
    connect(this, &SequenceOutputView::blockCountChanged, this, &SequenceOutputView::updateLineNumberAreaWidth);
    connect(this, &SequenceOutputView::updateRequest, this, &SequenceOutputView::updateLineNumberArea);
    connect(this, &SequenceOutputView::cursorPositionChanged, this, &SequenceOutputView::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    auto hl = new TestHighlighter(document());
}

void SequenceOutputView::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
     int bottom = top + qRound(blockBoundingRect(block).height());
     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {
             QString number = QString::number(blockNumber + 1);
             painter.drawText(0, top, lineNumberArea->width()-10,
                              fontMetrics().height(),
                              Qt::AlignRight | Qt::AlignHCenter, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + qRound(blockBoundingRect(block).height());
         ++blockNumber;
     }
}

int SequenceOutputView::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    digits = qMax(digits,5); // Reserve at least 5 digits worth. More if needed.

    int space = 13 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void SequenceOutputView::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

}


void SequenceOutputView::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void SequenceOutputView::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(0x303030);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void SequenceOutputView::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);}
