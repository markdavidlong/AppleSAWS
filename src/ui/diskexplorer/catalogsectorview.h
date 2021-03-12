#ifndef CATALOGSECTORVIEW_H
#define CATALOGSECTORVIEW_H

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
class CatalogSectorView;
}

class CatalogSectorView : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogSectorView(QWidget *parent = nullptr);
    ~CatalogSectorView();

    void setSector(QSharedPointer<Sector> sec);

private:
    Ui::CatalogSectorView *ui;

    QSharedPointer<Sector> m_sector;
};

#endif // CATALOGSECTORVIEW_H
