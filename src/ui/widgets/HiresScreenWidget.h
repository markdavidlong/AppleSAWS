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

class HiresScreenWidget final : public QWidget
{
    Q_OBJECT
public:

    class ColRow final {
    public:
        constexpr ColRow() noexcept : m_col(0), m_row(0), m_undefined(true) {}
        explicit ColRow(quint8 col, quint8 row) { setColRow(col,row); }
        
        void setColRow(quint8 col, quint8 row) { m_col = col; m_row = row; calc(); }

        [[nodiscard]] constexpr quint8 col() const noexcept { return m_col; }
        [[nodiscard]] constexpr quint8 row() const noexcept { return m_row; }

        [[nodiscard]] constexpr bool isDefined() const noexcept { return !m_undefined; }
        [[nodiscard]] constexpr bool isUndefined() const noexcept { return m_undefined; }

        [[nodiscard]] quint16 appleAddress() const noexcept { return m_appleAddress; }
        [[nodiscard]] quint16 rawAddress() const noexcept { return m_rawAddress; }

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

    enum class ViewMode {
         Monochrome,
         PerPixelColor,
         NTSCColor
     };

     explicit HiresScreenWidget(QWidget *parent = nullptr);
     ~HiresScreenWidget() override;
     
     // Rule of Five - Qt QObject classes cannot be copied or moved
     HiresScreenWidget(const HiresScreenWidget&) = delete;
     HiresScreenWidget& operator=(const HiresScreenWidget&) = delete;
     HiresScreenWidget(HiresScreenWidget&&) = delete;
     HiresScreenWidget& operator=(HiresScreenWidget&&) = delete;

     void paintEvent(QPaintEvent *event) override;

     void resizeEvent(QResizeEvent *event) override;

     [[nodiscard]] static QBitArray byteToBits(quint8 byte);
     void contextMenuEvent(QContextMenuEvent *) override;

     [[nodiscard]] QAction *monochromeAction() const noexcept { return m_monochromeAction; }
     [[nodiscard]] QAction *ntscAction() const noexcept { return m_ntscAction; }
     [[nodiscard]] QAction *perPixelColorAction() const noexcept { return m_perPixelColorAction; }
     [[nodiscard]] QAction *showScanLinesAction() const noexcept { return m_showScanLinesAction; }
     [[nodiscard]] QAction *prevPageAction() const noexcept { return m_prevPageAction; }
     [[nodiscard]] QAction *nextPageAction() const noexcept { return m_nextPageAction; }

     [[nodiscard]] ColRow getColRowFromAppleAddress(quint16 address) const;
     [[nodiscard]] ColRow getColRowFromRawAddress(quint16 address) const;

     [[nodiscard]] QPixmap getPixmap() const noexcept { return m_pixmap; }

     [[nodiscard]] quint16 offset() const noexcept;
signals:
    void newOffset(quint16 offset);

public slots:
    void setData(const QByteArray& data);
    void setMode(ViewMode mode);
    void setUnpackedData(const QByteArray& unpackedData);
    void setOffset(quint16 offset);
protected:
    [[nodiscard]] int getLineAddressOffset(int line) const;
    [[nodiscard]] QByteArray packData(const QByteArray& unpackedData) const;

protected slots:
    void handleNtscAction(bool toggled);
    void handleMonochromeAction(bool toggled);
    void handlePerPixelColorAction(bool toggled);
    void handleShowScanLinesAction(bool toggled);

    void handlePrevPageAction(bool);
    void handleNextPageAction(bool);

private:
    void makeAddressTables();
    [[nodiscard]] QColor getColorFromBits(const QBitArray& bits, quint8 phase) const;
    void drawNtscLine(QPainter &painter, int linenum, const QBitArray& data) const;
    void drawMonoLine(QPainter &painter, int lineNum, const QBitArray& data) const;
    void drawPerPositionColorLine(QPainter &painter, int lineNum, const QBitArray& data) const;
    void drawPixmap();

    QPixmap m_pixmap;
    QByteArray m_data;
    ViewMode m_viewMode{ViewMode::Monochrome};
    QAction *m_monochromeAction{nullptr};
    QAction *m_ntscAction{nullptr};
    QAction *m_perPixelColorAction{nullptr};
    QAction *m_showScanLinesAction{nullptr};
    QAction *m_prevPageAction{nullptr};
    QAction *m_nextPageAction{nullptr};
    QActionGroup *formatGroup{nullptr};
    bool m_showScanLines{false};
    static QList<ColRow> m_rawAddressToColRowList;
    static QList<ColRow> m_appleAddressToColRowList;
    quint16 m_offset{0};
};

