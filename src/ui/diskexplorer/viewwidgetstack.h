#ifndef VIEWWIDGETSTACK_H
#define VIEWWIDGETSTACK_H

#include <QTabWidget>

class Sector;
class HexDumpViewer;
class VTOCView;
class CatalogSectorView;
class TSListView;

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
    void setSector(Sector *sec, PreferredViewer viewer = PreferredViewer::DontCare);

private:
    void makeWidgets();

    HexDumpViewer *m_hdv;
    VTOCView *m_vtoc_view;
    CatalogSectorView *m_cs_view;
    TSListView *m_tsl_view;

};

#endif // VIEWWIDGETSTACK_H
