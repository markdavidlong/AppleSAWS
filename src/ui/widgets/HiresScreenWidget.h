#pragma once

#include "AppleColors.h"

#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QPair>
#include <QMap>
#include <QBitArray>
#include <QAction>
#include <QMenu>
#include <QList>
#include <QDebug>

class HiresScreenWidget : public QWidget
{
    Q_OBJECT
public:

    class ColRow {
    public:
        ColRow() { m_col = 0; m_row = 0; m_undefined = true; }
        ColRow(quint8 col, quint8 row) { setColRow(col,row); }
        void setColRow(quint8 col, quint8 row) { m_col = col; m_row = row; calc(); }

        quint8 col() const { return m_col; }
        quint8 row() const { return m_row; }

        bool isDefined() const { return !m_undefined; }
        bool isUndefined() const { return m_undefined; }

        quint16 appleAddress() { return m_appleAddress; }
        quint16 rawAddress() { return m_rawAddress; }

    private:
        quint8 m_col;
        quint8 m_row;

        quint16 m_appleAddress;
        quint16 m_rawAddress;
        bool m_undefined;

        void calc() {
            if (m_col > 39) { qDebug() << "Col out of range: " << m_col; m_col = 39; }
            if (m_row > 191) { qDebug() << "Row out of range: " << m_row; m_row = 191; }

            m_rawAddress = (m_row*40) + m_col;

            unsigned short blockOffset[] = {
                    0x0000, 0x0080, 0x0100, 0x0180, 0x0200, 0x0280, 0x0300, 0x0380,
                    0x0028, 0x00A8, 0x0128, 0x01A8, 0x0228, 0x02A8, 0x0328, 0x03A8,
                    0x0050, 0x00D0, 0x0150, 0x01D0, 0x0250, 0x02D0, 0x0350, 0x03D0
                };

            unsigned short boxSub[] = {
                    0x0000, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00
                };

            m_appleAddress = blockOffset[m_row/8]+boxSub[m_row%8] + m_col;
            m_undefined = false;
        }

    };

    enum ViewMode {
         Monochrome,
         PerPixelColor,
         NTSCColor
     };

     explicit HiresScreenWidget(QWidget *parent = 0);
     void paintEvent(QPaintEvent *event);

     void resizeEvent(QResizeEvent *event);

     static QBitArray byteToBits(quint8 byte);
     void contextMenuEvent(QContextMenuEvent *);

     QAction *monochromeAction()    { return m_monochromeAction; }
     QAction *ntscAction()          { return m_ntscAction; }
     QAction *perPixelColorAction() { return m_perPixelColorAction; }
     QAction *showScanLinesAction() { return m_showScanLinesAction; }
     QAction *prevPageAction() { return m_prevPageAction; }
     QAction *nextPageAction() { return m_nextPageAction; }

     ColRow getColRowFromAppleAddress(quint16 address);
     ColRow getColRowFromRawAddress(quint16 address);

     QPixmap getPixmap() const { return m_pixmap; }

     quint16 offset() const;
signals:
    void newOffset(quint16 offset);

public slots:
    void setData(QByteArray data);
    void setMode(ViewMode);
    void setUnpackedData(QByteArray unpackedData);
    void setOffset(quint16 offset);
protected:
    int getLineAddressOffset(int line);
    QByteArray packData(QByteArray unpackedData);

protected slots:
    void handleNtscAction(bool toggled);
    void handleMonochromeAction(bool toggled);
    void handlePerPixelColorAction(bool toggled);
    void handleShowScanLinesAction(bool toggled);

    void handlePrevPageAction(bool);
    void handleNextPageAction(bool);

private:
    void makeAddressTables();
    QColor getColorFromBits(QBitArray bits, quint8 phase);
    void drawNtscLine(QPainter &painter,int linenum, QBitArray data);
    void drawMonoLine(QPainter &painter, int lineNum, QBitArray data);
    void drawPerPositionColorLine(QPainter &painter, int lineNum, QBitArray data);
    void drawPixmap();


    QPixmap m_pixmap;
    QByteArray m_data;

    ViewMode m_viewMode;

    QAction *m_monochromeAction;
    QAction *m_ntscAction;
    QAction *m_perPixelColorAction;
    QAction *m_showScanLinesAction;
    QAction *m_prevPageAction;
    QAction *m_nextPageAction;
    QActionGroup *formatGroup;

    bool m_showScanLines;

    static QList<ColRow> m_rawAddressToColRowList;
    static QList<ColRow> m_appleAddressToColRowList;

    quint16 m_offset;
};

