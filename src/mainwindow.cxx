#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include "hiresviewwidget.h"
#include <QMdiSubWindow>

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
            SLOT(handleDiskItemSelected(DiskFile*,FileDescriptiveEntry)));


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

void MainWindow::handleDiskItemSelected(DiskFile *disk, FileDescriptiveEntry fde)
{
    HiresViewWidget *hvwma = new HiresViewWidget(0);
    hvwma->resize(280,192);
    QString title = QString("Image: %1").arg(AppleString(fde.filename).printable().trimmed());
    hvwma->setWindowTitle(title);
    hvwma->show();

    QByteArray data = disk->getFile(fde).mid(4);
    hvwma->setData(data);

}
