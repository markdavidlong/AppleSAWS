#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include "hiresviewwidget.h"
#include "applesoftfileviewer.h"
#include <QMdiSubWindow>
#include "applesoftfile.h"
#include "memory.h"
#include "disassembler.h"
#include "disassemblerviewer.h"

#include <QTextDocument>

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
    connect(ui->catalogWidget,SIGNAL(itemSelected(DiskFile*,FileDescriptiveEntry)),
            SLOT(handleDiskItemSelectedDefaultOpen(DiskFile*,FileDescriptiveEntry)));


    connect(this, SIGNAL(diskFileLoading(QString, DiskFile*)),
            ui->catalogWidget, SLOT(prepForNewDisk(QString,DiskFile*)));
    connect(this, SIGNAL(diskFileLoaded(QString,DiskFile*)),
            ui->catalogWidget, SLOT(processNewlyLoadedDisk(QString,DiskFile*)));
    connect(this, SIGNAL(diskFileUnloading(DiskFile*)),
            ui->catalogWidget, SLOT(unloadDisk(DiskFile*)));


}

MainWindow::~MainWindow()
{
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
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Disk Image"),
                                                    "/home/mlong/Desktop",
                                                    "Disk Images (*.do *.dsk)");
    if (!filename.isEmpty()) {
        loadDiskFile(filename);
    }
}

void MainWindow::openInHiresViewWidget(BinaryFile *file, QString filename) {
    HiresViewWidget *hvwma = new HiresViewWidget(0);

    QString title = QString("Image: %1").arg(filename);
    hvwma->setWindowTitle(title);
    hvwma->show();
    hvwma->setData(file->data());
}

void MainWindow::openInDisassemblerViewer(BinaryFile *file) {
    DisassemblerViewer *hvwma = new DisassemblerViewer(0);
    hvwma->show();
    hvwma->setFile(file);
}



void MainWindow::handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde)
{
    GenericFile *file = disk->getFile(fde);

    if (dynamic_cast<BinaryFile*>(file)) {
        BinaryFile *binfile = dynamic_cast<BinaryFile*>(file);
        binfile->setFilename(AppleString(fde.filename).printable().trimmed());

        if (fde.lengthInSectors == 34 && (binfile->address() == 0x2000 || binfile->address() == 0x4000))
        {
            openInHiresViewWidget(binfile, AppleString(fde.filename).printable().trimmed());
        }
        else
        {
            openInDisassemblerViewer(binfile);

        }
    }
    else if (dynamic_cast<ApplesoftFile *>(file))
    {
        ApplesoftFile *abf = dynamic_cast<ApplesoftFile *>(file);
        abf->setFilename(AppleString(fde.filename).printable().trimmed());

        ApplesoftFileViewer *afv = new ApplesoftFileViewer(0);
        afv->setFile(abf);
        afv->show();
    }

}
