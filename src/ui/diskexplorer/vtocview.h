#ifndef VTOCVIEW_H
#define VTOCVIEW_H

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

class Sector;

namespace Ui {
class VTOCView;
}

class VTOCView : public QWidget
{
    Q_OBJECT

public:
    explicit VTOCView(QWidget *parent = nullptr);
    ~VTOCView();

    void setSector(Sector *sec);

protected:
    QString makeUsage();

private:
    Ui::VTOCView *ui;
    Sector *m_sector;

};

#endif // VTOCVIEW_H
