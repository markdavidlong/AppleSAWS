#include "DiskExplorer.h"

#include <QGridLayout>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QFileDialog>
#include <QShowEvent>
#include <QDebug>
#include <QStatusBar>

#include "genericfile.h"
#include "viewerbase.h"

DiskExplorer::DiskExplorer(QWidget *parent) : QMainWindow(parent)
{
    m_action_Unload_Disk_Image = 0;
    m_disk = 0;
    m_horizSizePref = -1;

    resize(300,800);
    initUi();
}

DiskExplorer::~DiskExplorer()
{
}

void DiskExplorer::initUi()
{


    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu *menu = new QMenu(tr("&File"),this);
    menuBar->addMenu(menu);

    QAction *action_Load_Disk_Image = new QAction(tr("&Load Disk Image..."),this);
    menu->addAction(action_Load_Disk_Image);
    connect(action_Load_Disk_Image, SIGNAL(triggered()), SLOT(showLoadDialog()));

    m_action_Unload_Disk_Image = new QAction(tr("&Unload Disk Image"),this);
    m_action_Unload_Disk_Image->setEnabled(false);
    menu->addAction(m_action_Unload_Disk_Image);
    connect(m_action_Unload_Disk_Image, SIGNAL(triggered()), SLOT(unloadDiskFile()));

    menu->addSeparator();

    QAction *action_Quit = new QAction(tr("&Quit"),this);
    menu->addAction(action_Quit);
    connect(action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    menu = new QMenu(tr("&Util"),this);
    menuBar->addMenu(menu);

    QAction *action_Hex_Converter = new QAction(tr("&Hex Converter..."),this);
    menu->addAction(action_Hex_Converter);

    menu->addSeparator();

    m_setDiskToolsVisibleAction = new QAction(tr("Show &Disk tools"),this);
    m_setDiskToolsVisibleAction->setCheckable(true);
    m_setDiskToolsVisibleAction->setChecked(false);
    connect(m_setDiskToolsVisibleAction, SIGNAL(toggled(bool)),
            SLOT(setDiskToolsVisible(bool)));
    menu->addAction(m_setDiskToolsVisibleAction);


    menu = new QMenu(tr("&Misc"),this);
    menuBar->addMenu(menu);

    QAction *action_HRCG_Commands = new QAction(tr("&HRCG Commands..."),this);
    menu->addAction(action_HRCG_Commands);


    m_hrcgDialog = new HRCGControlsInfo(this);
    connect(action_HRCG_Commands, SIGNAL(triggered()), m_hrcgDialog, SLOT(show()));

    m_hexConverter = new HexConverter(this);
    connect(action_Hex_Converter, SIGNAL(triggered()), m_hexConverter, SLOT(show()));

    QWidget *widget = new QWidget(0);
    m_gridLayout = new QGridLayout();
    m_gridLayout->setVerticalSpacing(4);
    m_gridLayout->setHorizontalSpacing(4);
    widget->setLayout(m_gridLayout);
    m_cw = new CatalogWidget(widget);
    m_demw = new DiskExplorerMapWidget(35,16,widget);
    m_frame = new QFrame(widget);
    m_frame->setFrameStyle(QFrame::Raised);
    m_frame->setMinimumSize(200,200);
    QGridLayout *frameLayout = new QGridLayout(0);
    m_frame->setLayout(frameLayout);
    m_hdv = new HexDumpViewer(this);
    frameLayout->addWidget(m_hdv);

    m_gridLayout->setColumnStretch(0,4);
    m_gridLayout->setColumnStretch(1,1);
    m_gridLayout->setColumnStretch(2,4);

    m_gridLayout->addWidget(m_cw,0,0,2,1);
    m_gridLayout->addWidget(m_demw,0,1,1,2);
    m_key = m_demw->makeKeyWidget();
    m_gridLayout->addWidget(m_key,1,1);
    m_gridLayout->addWidget(m_frame,1,2);
    this->setCentralWidget(widget);

    connect(m_cw,SIGNAL(openWithDefaultViewer(DiskFile*,FileDescriptiveEntry)),
            SLOT(handleDiskItemSelectedDefaultOpen(DiskFile*,FileDescriptiveEntry)));

    connect(m_demw, SIGNAL(showSectorData(QByteArray,int,int,QVariant)),
            SLOT(handleShowSectorData(QByteArray,int,int,QVariant)));


    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    m_demwStatusWidget = m_demw->getStatusWidget();
    statusBar->addPermanentWidget(m_demwStatusWidget);


    setDiskToolsVisible(false);
}


void DiskExplorer::unloadDiskFile()
{
    if (m_disk)
    {
        m_cw->unloadDisk(m_disk);
        m_demw->unloadDisk();
        m_hdv->setRawData(QByteArray(),0);
    }
}

void DiskExplorer::loadDiskFile(QString filename)
{
    if (m_disk) {
        unloadDiskFile();
    }

    m_disk = new DiskFile();
    m_cw->prepForNewDisk(filename,m_disk);
    if (m_disk->read(filename)) {
        m_action_Unload_Disk_Image->setEnabled(true);
        QSettings settings;
        settings.setValue("lastOpened",filename);
        m_cw->processNewlyLoadedDisk(filename,m_disk);
        m_demw->setDisk(m_disk);
    } else {
        emit diskFileLoadFailed(filename,m_disk);
        delete m_disk;
        m_disk = 0;
    }
}

void DiskExplorer::handleShowSectorData(QByteArray data, int track, int sector, QVariant metadata)
{
    Q_UNUSED(track)
    Q_UNUSED(sector)
    Q_UNUSED(metadata)
    m_hdv->setRawData(data,0);
}

void DiskExplorer::showLoadDialog()
{
    QSettings settings;
    QString last = settings.value("lastOpened",".").toString();
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Disk Image"),
                                                    last,
                                                    "Disk Images (*.do *.dsk)");
    if (!filename.isEmpty()) {
        loadDiskFile(filename);
    }
}

void DiskExplorer::handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde)
{
    GenericFile *file = disk->getFile(fde);
    file->setFilename(AppleString(fde.filename).printable().trimmed());

    ViewerBase *vb = new ViewerBase();
    qDebug() << "Adding viewer" << vb;
    m_viewerList.append(vb);
    connect(vb,SIGNAL(viewerClosing(ViewerBase*)), SLOT(handleViewerClosing(ViewerBase*)));
    vb->setFile(file);
    vb->show();
}

void DiskExplorer::setDiskToolsVisible(bool visible)
{
    if (visible)
    {
        m_gridLayout->setColumnStretch(0,4);
        m_gridLayout->setColumnStretch(1,1);
        m_gridLayout->setColumnStretch(2,4);
    }
    else
    {
        m_gridLayout->setColumnStretch(0,0);
        m_gridLayout->setColumnStretch(1,0);
        m_gridLayout->setColumnStretch(2,0);
    }

    if (m_demwStatusWidget) { m_demwStatusWidget->setVisible(visible); }

    m_demw->setVisible(visible);
    m_frame->setVisible(visible);
    m_hdv->setVisible(visible);
    m_key->setVisible(visible);

    if (!visible)
    {
        QTimer::singleShot(100, this, SLOT(doResize()));
    }
}

void DiskExplorer::handleViewerClosing(ViewerBase *viewer)
{
    qDebug() << "Removing viewer " << viewer;
    m_viewerList.removeAll(viewer);
}

void DiskExplorer::doResize()
{
    resize(m_horizSizePref, this->height());
}

void DiskExplorer::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    if (m_horizSizePref == -1)
    {
        m_horizSizePref = this->width();
        qDebug() << "New Horiz Size Pref = " << m_horizSizePref;
    }
}

void DiskExplorer::closeEvent(QCloseEvent *)
{
    foreach (ViewerBase *viewer, m_viewerList)
    {
        if (viewer)
        {
            viewer->deleteLater();
        }
    }
}



