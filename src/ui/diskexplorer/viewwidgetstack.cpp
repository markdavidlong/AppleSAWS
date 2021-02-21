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

#include "viewwidgetstack.h"
#include "hexdumpviewer.h"
#include "vtocview.h"
#include "catalogsectorview.h"
#include "tslistview.h"
#include "DiskExplorerMapWidget.h"

ViewWidgetStack::ViewWidgetStack(QWidget *parent) : QTabWidget(parent)
{
    makeWidgets();
    setSector(nullptr);
}

void ViewWidgetStack::setSector(Sector *sec, PreferredViewer viewer)
{
    if (sec)
    {
        m_hdv->setRawData(*(sec->rawData()));
    }
    else
    {
        m_hdv->setRawData(QByteArray(),0);
    }
    m_cs_view->setSector(sec);
    m_vtoc_view->setSector(sec);
    m_tsl_view->setSector(sec);

    if (viewer == PreferredViewer::VTOC)
    {
        setCurrentWidget(m_vtoc_view);
    }
    else if (viewer == PreferredViewer::TSList)
    {
        setCurrentWidget(m_tsl_view);
    }
    else if (viewer == PreferredViewer::CatalogSector)
    {
        setCurrentWidget(m_cs_view);
    }
    else if (viewer == PreferredViewer::HexDump)
    {
        setCurrentWidget(m_hdv);
    }

}

void ViewWidgetStack::makeWidgets()
{
    m_hdv = new HexDumpViewer(this,10);
    addTab(m_hdv,"View Raw Data");

    m_vtoc_view = new VTOCView(this);
    addTab(m_vtoc_view,"View as VTOC");

    m_cs_view = new CatalogSectorView(this);
    addTab(m_cs_view,"View as Catalog");

    m_tsl_view = new TSListView(this);
    addTab(m_tsl_view,"View as T/S List");

    setCurrentWidget(m_hdv);
}

void ViewWidgetStack::handleShowSectorData(Sector *data, int track,
                                           int sector, DiskSectorRole role)
{

    ViewWidgetStack::PreferredViewer viewer =
            ViewWidgetStack::PreferredViewer::DontCare;

    bool USE_SPECIFIC_VIEWER = true;  // Hook for later allowing the disuse of
    if (USE_SPECIFIC_VIEWER)          //   default viewer switching
    {
        if (role == DiskSectorRole::VTOC)
        {
            viewer = ViewWidgetStack::PreferredViewer::VTOC;
        }
        else if (role == DiskSectorRole::CatalogSector)
        {
            viewer = ViewWidgetStack::PreferredViewer::CatalogSector;
        }
        else if (role == DiskSectorRole::TSList)
        {
            viewer = ViewWidgetStack::PreferredViewer::TSList;
        }
        else
        {
            viewer = ViewWidgetStack::PreferredViewer::HexDump;
        }
    }

    setSector(data, viewer);
}
