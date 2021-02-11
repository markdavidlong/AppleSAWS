#ifndef SEQUENCEOUTPUTVIEW_H
#define SEQUENCEOUTPUTVIEW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTextBlock>
#include <QRegularExpression>
#include <QStyle>

class SOVNumberPanel;


class SequenceOutputView : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit SequenceOutputView(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;

protected:
    void resizeEvent(QResizeEvent *event) override;


private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    SOVNumberPanel *lineNumberArea;
    QTextDocument *m_curdoc;
};


class SOVNumberPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SOVNumberPanel(SequenceOutputView *sov) : QWidget(sov), m_sov(sov) {
        setAttribute(Qt::WA_StyledBackground, true);

    }
    QSize sizeHint() const override { return QSize(m_sov->lineNumberAreaWidth(), 0); }

protected:
    void paintEvent(QPaintEvent *event) override {
        m_sov->lineNumberAreaPaintEvent(event);
    }
    SequenceOutputView *m_sov;
};


class TestHighlighter : public QSyntaxHighlighter
{
public:
    explicit TestHighlighter(QTextDocument *doc):QSyntaxHighlighter(doc) { }
    void highlightBlock(const QString &text) {
        for (int idx = 0; idx< text.length(); idx++)
        {
            QTextCharFormat format;
         //   auto block = currentBlock();
            format.setForeground(QColor("green"));
            setFormat(0,5,format);
            QTextCharFormat format2;
            format2.setForeground(QColor("yellow"));
            setFormat(5,10,format2);
            QTextCharFormat format3;
            format3.setForeground(QColor("magenta"));
            setFormat(15,text.length()-15,format3);
            QRegularExpression re("'.'");
            auto match = re.match(text);
            if (match.hasMatch())
            {
                auto y = match.capturedStart();
                y++;

                QRegularExpression re2("; *\\d+");
                auto m = re2.match(text);
                auto x = match.capturedStart();
                x++;

                QTextCharFormat atformat;
                if (m.hasMatch())
                {
                    auto txt = m.captured().right(m.capturedLength()-2);
                    int num = txt.toInt();

//                    qDebug("Number: %s %d", qPrintable(txt), num);

                    if (num > 0x7f)
                    {
                        atformat.setForeground(QColor("white"));
                        atformat.setBackground(QColor("black"));
                    }
                    else
                    {
                        atformat.setForeground(QColor("black"));
                        atformat.setBackground(QColor("white"));
                    }

                }
                else
                {
                    atformat.setForeground(QColor("white"));
                    atformat.setForeground(QColor("black"));
                }

                atformat.setFontFamily("Print Char 21");
                setFormat(y,1,atformat);
            }
        }
    }
};


#endif // SEQUENCEOUTPUTVIEW_H
