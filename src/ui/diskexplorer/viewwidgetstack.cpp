#include "viewwidgetstack.h"
#include "hexdumpviewer.h"
#include "vtocview.h"
#include "catalogsectorview.h"
#include "tslistview.h"

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
