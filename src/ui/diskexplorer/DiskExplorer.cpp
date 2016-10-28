#include "DiskExplorer.h"

#include <QGridLayout>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QFileDialog>

#include "genericfile.h"
#include "viewerbase.h"

DiskExplorer::DiskExplorer(QWidget *parent) : QMainWindow(parent)
{
    m_action_Unload_Disk_Image = 0;
    m_disk = 0;

    initUi();
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

    menu = new QMenu(tr("&Misc"),this);
    menuBar->addMenu(menu);

    QAction *action_HRCG_Commands = new QAction(tr("&HRCG Commands..."),this);
    menu->addAction(action_HRCG_Commands);


    m_hrcgDialog = new HRCGControlsInfo(this);
    connect(action_HRCG_Commands, SIGNAL(triggered()), m_hrcgDialog, SLOT(show()));

    m_hexConverter = new HexConverter(this);
    connect(action_Hex_Converter, SIGNAL(triggered()), m_hexConverter, SLOT(show()));


    QWidget *widget = new QWidget(this);
    this->setCentralWidget(widget);
    QGridLayout *layout = new QGridLayout();
    layout->setVerticalSpacing(4);
    layout->setHorizontalSpacing(4);
    widget->setLayout(layout);

    m_cw = new CatalogWidget(widget);
    m_demw = new DiskExplorerMapWidget(35,16,widget);
    m_frame = new QFrame(widget);
    m_frame->setFrameStyle(QFrame::Raised);
    m_frame->setMinimumSize(200,200);
    QGridLayout *frameLayout = new QGridLayout(this);
    m_frame->setLayout(frameLayout);
    m_hdv = new HexDumpViewer(this);
    frameLayout->addWidget(m_hdv);

    layout->setColumnStretch(0,4);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,4);

    layout->addWidget(m_cw,0,0,2,1);
    layout->addWidget(m_demw,0,1,1,2);
    layout->addWidget(m_demw->makeKeyWidget(),1,1);
    layout->addWidget(m_frame,1,2);


    connect(m_cw,SIGNAL(openWithDefaultViewer(DiskFile*,FileDescriptiveEntry)),
            SLOT(handleDiskItemSelectedDefaultOpen(DiskFile*,FileDescriptiveEntry)));

    connect(m_demw, SIGNAL(showSectorData(QByteArray,int,int,QVariant)),
            SLOT(handleShowSectorData(QByteArray,int,int,QVariant)));
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
    vb->setFile(file);
    vb->show();
}
