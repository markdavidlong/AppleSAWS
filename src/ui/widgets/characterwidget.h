#pragma once

#include <QWidget>
#include <QColor>
#include <QPixmap>

#include "charset.h"

class CharacterWidget : public QWidget
{
    Q_OBJECT

public:
    CharacterWidget(QWidget *parent = 0,
                    CharSetCharacter ch = CharSetCharacter());

    void doRepaint();

    bool hasHeightForWidth() const;
    int heightForWidth(int w) const;
protected:
    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *);
signals:

public slots:
    void setFgColor(QColor color);
    void setBgColor(QColor color);
    void setGridColor(QColor color);

    void showGrid(bool show);
    void enableBitShift(bool enable);


private:
    QPixmap m_pixmap;

    QColor m_fgcolor;
    QColor m_bgcolor;
    QColor m_gridcolor;
    bool m_showgrid;
    bool m_dobitshift;

    CharSetCharacter m_character;

};

