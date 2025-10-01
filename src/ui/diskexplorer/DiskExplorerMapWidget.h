#pragma once

#include "DiskFile.h"

#include <QWidget>
#include <QPair>
#include <QMap>
#include <QPushButton>
#include <QColor>
#include <QGroupBox>
#include <QLabel>


class DEButton final : public QPushButton
{
    Q_OBJECT

public:
    explicit DEButton(QWidget* parent, int track = -1, int sec = -1) 
        : QPushButton(parent), m_track(track), m_sector(sec)
    {
        connect(this, &DEButton::clicked, this, &DEButton::handleClick);
    }
    void setTrack(int track) noexcept { m_track = track; }
    void setSector(int sector) noexcept { m_sector = sector; }

    [[nodiscard]] constexpr int track() const noexcept { return m_track; }
    [[nodiscard]] constexpr int sector() const noexcept { return m_sector; }

    void clearBgColor() { 
        m_backgroundColor.clear(); 
        setText(QString{}); 
        setStyleSheet(makeStyleSheet());
    }

    void setBgColor(const QColor& color) {
        m_fgColor = determineFgColor(color).name();
        m_backgroundColor = color.name();
        m_hlColor = color.lighter(155).name();
        setStyleSheet(makeStyleSheet());
    }

    [[nodiscard]] constexpr bool highlighted() const noexcept { return m_isHighlighted; }
    void setHighlighted(bool highlighted) { 
        m_isHighlighted = highlighted;
        setStyleSheet(makeStyleSheet()); 
    }

    void reset() { 
        setHighlighted(false); 
        setChecked(false); 
        setStyleSheet(makeStyleSheet()); 
    }

    void resetToDefault() { 
        clearBgColor(); 
        reset(); 
    }

    QColor determineFgColor(QColor bgColor)
    {
        if (qGray(bgColor.rgb()) > 128)
        {
            return QColor(Qt::black);
        }
        return Qt::white;
    }

signals:
    void checked(int track, int sec,bool );

private slots:
    void handleClick(bool isChecked) { emit checked(m_track, m_sector, isChecked); }

    [[nodiscard]] QSize minimumSizeHint() const noexcept override { return QSize(24, 24); }
    [[nodiscard]] QSize sizeHint() const noexcept override { return QSize(24, 24); }
    [[nodiscard]] bool hasHeightForWidth() const noexcept override { return true; }
    [[nodiscard]] int heightForWidth(int width) const noexcept override { return width; }

private:
    [[nodiscard]] QString makeStyleSheet() const {
        return QStringLiteral(" QPushButton { font: 10px; border-width: 1px; color: %1; background-color: %2} "
                              " QPushButton:checked { font: bold italic 11px; } ")
                .arg(m_fgColor, m_backgroundColor);
    }

    int m_track{-1};
    int m_sector{-1};
    bool m_isHighlighted{false};

    QString m_fgColor;
    QString m_backgroundColor;
    QString m_hlColor;
};



using DETSPair = QPair<int, int>;

class DiskExplorerMapWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit DiskExplorerMapWidget(int numtracks, int numsectors, QWidget* parent = nullptr);

    void setButtonBgColor(int track, int sector, const QColor& color);
    void setButtonText(int track, int sector, const QString& text) {
        buttonAt(track, sector)->setText(text);
    }

    void setDisk(DiskFile* disk);
    void unloadDisk();

    void setAllButtonsEnabled(bool enabled);

    [[nodiscard]] QGroupBox* makeKeyWidget();
    [[nodiscard]] QWidget* getStatusWidget() const noexcept { return m_statusWidget; }

signals:
    void showSectorData(const QByteArray& data, int track, int sector, const QVariant& metadata);

public slots:
    void handleButtonCheck(int track, int sector, bool checked);

protected:
    void mapDiskToButtons();
    [[nodiscard]] DEButton* buttonAt(int track, int sector);

    void initColors();

    [[nodiscard]] static QColor determineFgColor(const QColor& bgColor) noexcept
    {
        return (qGray(bgColor.rgb()) > 128) ? QColor(Qt::black) : QColor(Qt::white);
    }

    [[nodiscard]] QLabel* makeKeyLabel(QWidget* parent, const QString& name, const QColor& color);
    void showEvent(QShowEvent* event) override;
    void makeStatusWidget();
    [[nodiscard]] QString getSectorDescription(int track, int sector) const;

private:
    QMap<int, QMap<int, DEButton*>> m_buttons;
    DEButton* m_currentChecked{nullptr};

    int m_numtracks{0};
    int m_numsectors{0};

    QColor m_defaultColor{Qt::lightGray};
    QColor m_bootSectorColor{Qt::red};
    QColor m_dosImageColor{Qt::blue};
    QColor m_vtocColor{Qt::yellow};
    QColor m_dirEntryColor{Qt::green};
    QColor m_tsListColor{Qt::cyan};
    QColor m_applesoftFileColor{Qt::magenta};
    QColor m_intBasicFileColor{Qt::darkRed};
    QColor m_binaryFileColor{Qt::darkBlue};
    QColor m_textFileColor{Qt::darkGreen};
    QColor m_reloFileColor{Qt::darkCyan};
    QColor m_typeAFileColor{Qt::darkMagenta};
    QColor m_typeBFileColor{Qt::darkYellow};
    QColor m_typeSFileColor{Qt::gray};

    QButtonGroup* m_bgroup{nullptr};
    DiskFile* m_disk{nullptr};

    bool m_deferredSetup{false};

    QLabel* m_diskLabel{nullptr};
    QLabel* m_trackSectorLabel{nullptr};
    QWidget* m_statusWidget{nullptr};

    QMap<DETSPair, QString> m_sectorDescriptions;
};

