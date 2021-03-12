#ifndef VIEWWIDGETSTACK_H
#define VIEWWIDGETSTACK_H

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



#include <QTabWidget>

class Sector;
class HexDumpViewer;
class VTOCView;
class CatalogSectorView;
class TSListView;
enum class DiskSectorRole;

class ViewWidgetStack : public QTabWidget
{
    Q_OBJECT

public:

    enum class PreferredViewer {
        DontCare = 0,
        HexDump,
        VTOC,
        CatalogSector,
        TSList
    };

    explicit ViewWidgetStack(QWidget *parent = nullptr);

public slots:
    void setSector(QSharedPointer<Sector> sec, PreferredViewer viewer = PreferredViewer::DontCare);

    void handleShowSectorData(QSharedPointer<Sector> data, int track, int sector, DiskSectorRole role);



private:
    void makeWidgets();

    HexDumpViewer *m_hdv;
    VTOCView *m_vtoc_view;
    CatalogSectorView *m_cs_view;
    TSListView *m_tsl_view;

};

#endif // VIEWWIDGETSTACK_H
