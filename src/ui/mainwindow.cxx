#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include "hiresviewwidget.h"
#include "applesoftfileviewer.h"
#include <QMdiSubWindow>
#include "binaryfile.h"
#include "applesoftfile.h"
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

void MainWindow::handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde)
{
    GenericFile *file = disk->getFile(fde);

    if (dynamic_cast<BinaryFile*>(file)) {
        if (fde.lengthInSectors == 34)
        {
            HiresViewWidget *hvwma = new HiresViewWidget(0);
            hvwma->resize(280,192);
            QString title = QString("Image: %1").arg(AppleString(fde.filename).printable().trimmed());
            hvwma->setWindowTitle(title);
            hvwma->show();
            hvwma->setData(file->data());
        }
    }
    else if (dynamic_cast<ApplesoftFile *>(file))
    {
        ApplesoftFile *abf = dynamic_cast<ApplesoftFile *>(file);
        ApplesoftFileViewer *afv = new ApplesoftFileViewer(0);
        QString title = QString("AppleSoft Viewer: %1").arg(AppleString(fde.filename).printable().trimmed());
        afv->setWindowTitle(title);
        QStringList strings;
        foreach (ApplesoftLine line, abf->detokenized())
        {
            QString linestring = QString("%1  %2").arg(line.linenum,5,10,QChar(' ')).arg(line.detokenized_line);
            strings.append(linestring);
        }
        QString fulltext = Qt::convertFromPlainText(strings.join("\n"));
 //       fulltext.replace("{kwd}","<font color=\"red\">{kwd}</font>");
  //      fulltext.replace("{num}","<font color=\"red\">{num}</font>");
   //     fulltext.replace("{string}","<font color=\"red\">{string}</font>");
        fulltext.replace("GOTO","<b><font color=\"green\">GOTO</font></b>");
        fulltext.replace("GOSUB","<b><font color=\"green\">GOSUB</font></b>");
//        fulltext.replace("0","<font color=\"orange\">0</font>");
//        fulltext.replace("1","<font color=\"orange\">1</font>");
//        fulltext.replace("2","<font color=\"orange\">2</font>");
//        fulltext.replace("3","<font color=\"orange\">3</font>");
//        fulltext.replace("4","<font color=\"orange\">4</font>");
//        fulltext.replace("5","<font color=\"orange\">5</font>");
//        fulltext.replace("6","<font color=\"orange\">6</font>");
//        fulltext.replace("7","<font color=\"orange\">7</font>");
//        fulltext.replace("8","<font color=\"orange\">8</font>");
//        fulltext.replace("9","<font color=\"orange\">9</font>");
        fulltext.replace("RETURN","RETURN<p>");

        afv->setText(fulltext);
        afv->show();
    }


}
