#pragma once

#include "CharSetCharacter.h"

#include <QWidget>
#include <QColor>
#include <QPixmap>

class CharacterWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterWidget(QWidget *parent = nullptr,
                    CharSetCharacter ch = CharSetCharacter());
    ~CharacterWidget() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    CharacterWidget(const CharacterWidget&) = delete;
    CharacterWidget& operator=(const CharacterWidget&) = delete;
    CharacterWidget(CharacterWidget&&) = delete;
    CharacterWidget& operator=(CharacterWidget&&) = delete;

    void doRepaint();

    [[nodiscard]] bool hasHeightForWidth() const override;
    [[nodiscard]] int heightForWidth(int w) const override;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *) override;
signals:

public slots:
    void setFgColor(const QColor& color);
    void setBgColor(const QColor& color);
    void setGridColor(const QColor& color);
    void showGrid(bool show);
    void enableBitShift(bool enable);


private:
    QPixmap m_pixmap;
    QColor m_fgcolor;
    QColor m_bgcolor;
    QColor m_gridcolor;
    bool m_showgrid{false};
    bool m_dobitshift{false};
    CharSetCharacter m_character;

};

