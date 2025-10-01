#pragma once

#include <QPushButton>
#include <QColor>
#include <QString>
#include <QSize>

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

    [[nodiscard]] static QColor determineFgColor(const QColor& bgColor) noexcept
    {
        return (qGray(bgColor.rgb()) > 128) ? QColor(Qt::black) : QColor(Qt::white);
    }

signals:
    void checked(int track, int sec, bool isChecked);

private slots:
    void handleClick(bool isChecked) { emit checked(m_track, m_sector, isChecked); }

protected:
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