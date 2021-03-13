#pragma once

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

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

