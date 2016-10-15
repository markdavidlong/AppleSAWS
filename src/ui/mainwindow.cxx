#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hiresviewwidget.h"
#include "applesoftfileviewer.h"
#include "applesoftfile.h"
#include "textfile.h"
#include "memory.h"
#include "disassembler.h"
#include "disassemblerviewer.h"
#include "hexdumpviewer.h"
#include "mazeviewer.h"
#include "texthexdumpviewer.h"
#include "charsetviewer.h"
#include "relocatablefile.h"
#include "hrcgcontrolsinfo.h"

#include <QFileDialog>
#include <QTextDocument>
#include <QSettings>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_disk = 0;

    ui->action_Unload_Disk_Image->setEnabled(false);

    connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->action_Load_Disk_Image, SIGNAL(triggered()), SLOT(showLoadDialog()));
    connect(ui->action_Unload_Disk_Image, SIGNAL(triggered()), SLOT(unloadDiskFile()));
    connect(ui->catalogWidget,SIGNAL(openWithDefaultViewer(DiskFile*,FileDescriptiveEntry)),
            SLOT(handleDiskItemSelectedDefaultOpen(DiskFile*,FileDescriptiveEntry)));


    connect(this, SIGNAL(diskFileLoading(QString, DiskFile*)),
            ui->catalogWidget, SLOT(prepForNewDisk(QString,DiskFile*)));
    connect(this, SIGNAL(diskFileLoaded(QString,DiskFile*)),
            ui->catalogWidget, SLOT(processNewlyLoadedDisk(QString,DiskFile*)));
    connect(this, SIGNAL(diskFileUnloading(DiskFile*)),
            ui->catalogWidget, SLOT(unloadDisk(DiskFile*)));


    m_hrcgDialog = new HRCGControlsInfo(this);
    connect(ui->action_HRCG_Commands, SIGNAL(triggered()), m_hrcgDialog, SLOT(show()));

    m_hexConverter = new HexConverter(this);
    connect(ui->action_Hex_Converter, SIGNAL(triggered()), m_hexConverter, SLOT(show()));
}

MainWindow::~MainWindow()
{
    if (m_disk) {
        unloadDiskFile();
    }
    delete ui;
}

void MainWindow::loadDiskFile(QString filename)
{
    if (m_disk) {
        unloadDiskFile();
    }

    m_disk = new DiskFile();
    emit diskFileLoading(filename,m_disk);
    if (m_disk->read(filename)) {
        ui->action_Unload_Disk_Image->setEnabled(true);
        QSettings settings;
        settings.setValue("lastOpened",filename);
        emit diskFileLoaded(filename,m_disk);
    } else {
        emit diskFileLoadFailed(filename,m_disk);
        delete m_disk;
        m_disk = 0;
    }
}

void MainWindow::unloadDiskFile()
{
    emit diskFileUnloading(m_disk);
    delete m_disk;
    m_disk = 0;
    ui->action_Unload_Disk_Image->setEnabled(false);
    emit diskFileUnloaded();
}

void MainWindow::showLoadDialog()
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


void MainWindow::handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde)
{
    GenericFile *file = disk->getFile(fde);
    file->setFilename(AppleString(fde.filename).printable().trimmed());

    ViewerBase *vb = new ViewerBase();
    vb->setFile(file);
    vb->show();
}
