#ifndef HIRESVIEWWIDGET_H
#define HIRESVIEWWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QPair>
#include <QMap>
#include <QBitArray>

static const QColor blackColor = QColor(0,0,0);
static const QColor brownColor = QColor();
static const QColor darkGreenColor = QColor(96,114,3);
static const QColor greenColor  = QColor(20,245,60);
static const QColor darkBlueColor = QColor(96,78,189);
static const QColor grayColor = QColor(156,156,156);
static const QColor blueColor   = QColor(20,207,253);
static const QColor aquaColor = QColor(114,255,208);
static const QColor redColor = QColor(227,30,96);
static const QColor orangeColor = QColor(255,106,60);
static const QColor gray2Color = QColor(156,156,156);
static const QColor yellowColor = QColor(208,221,141);
static const QColor purpleColor = QColor(255,68,253);
static const QColor pinkColor = QColor(255,160,208);
static const QColor lightBlueColor = QColor(208,195,255);
static const QColor whiteColor = QColor(255,255,255);


class HiresViewWidget : public QWidget
{
    Q_OBJECT
public:


   enum ViewMode {
        Monochrome,
        Color1,
        Color2
    };

    explicit HiresViewWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);

    static QBitArray byteToBits(quint8 byte);
signals:

public slots:
    void setData(QByteArray data);
    void setMode(ViewMode);

private:
    QPixmap m_pixmap;
    QByteArray m_data;


    int findRowCol(int offset);
    static QMap<int,int> *m_rowTable;

    void makeOffsetTable();

    ViewMode m_viewMode;
    void drawNtscLine(QPainter &painter,int linenum, QBitArray data);
    void drawMonoLine(QPainter &painter, int lineNum, QBitArray data);

    QColor getColorFromBits(QBitArray bits, quint8 phase);
};


#endif // HIRESVIEWWIDGET_H
