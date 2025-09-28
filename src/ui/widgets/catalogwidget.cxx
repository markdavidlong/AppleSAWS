#include "catalogwidget.h"
#include "ui_catalogwidget.h"
#include "filedescriptiveentry.h"
#include "binaryfile.h"
#include "ApplesoftFile.h"
#include "genericfile.h"
#include "IntBasicFile.h"
#include <QUrl>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QPalette>

CatalogWidget::CatalogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogWidget)
{
    ui->setupUi(this);
    QFont catalogFont("PR Number 3",12);
    //catalogFont.setStyleHint(QFont::Monospace);

    ui->catalog_list->setFont(catalogFont);
    ui->noteButton->setText(QChar(0x270d));
    ui->noteButton->setFont(QFont("sans",16,QFont::Bold));

    connect(ui->catalog_list, &QListWidget::itemDoubleClicked,
            this, &CatalogWidget::itemDoubleClicked);
}

CatalogWidget::~CatalogWidget()
{
    delete ui;
}

void CatalogWidget::prepForNewDisk(QString filename, DiskFile *disk)
{
    m_disk = disk;
    m_diskname = filename;
}

QString CatalogWidget::createToolTip(FileDescriptiveEntry &fde) {
    QString retval;
    retval += AppleString(fde.filename).printable().trimmed() +
            (fde.deleted?"(Deleted)":"") + "\n";
    retval += QString("Type: %1\n").arg(fde.fileType());
    retval += QString("Sectors: %1 (%2 bytes)\n")
            .arg(fde.lengthInSectors)
            .arg(fde.lengthInSectors*256);
    retval += QString("%1\n").arg(fde.isLocked()?"Locked":"Unlocked");
    GenericFile *file = m_disk->getFile(fde);
    if (!file) { return ""; }
    quint16 address = file->address();
    retval += QString("Address: $%1 (%2)\n").arg((quint16) (address),4,16,QChar('0'))
            .arg(address);
    if (dynamic_cast<BinaryFile*>(file)) {
        BinaryFile *binfile = dynamic_cast<BinaryFile*>(file);
        quint16 length = binfile->length();
        retval += QString("Length: $%1 (%2)\n").arg((quint16) (length),4,16,QChar('0'))
                .arg(length);
    } else if (dynamic_cast<ApplesoftFile*>(file)) {
        ApplesoftFile *asfile = dynamic_cast<ApplesoftFile*>(file);
        quint16 length = asfile->length();
        retval += QString("Length: $%1 (%2)\n").arg((quint16) (length),4,16,QChar('0'))
                .arg(length);
        quint16 uabytes = asfile->extraData().length();
        if (uabytes) {
            retval += QString("Unaccounted Bytes: $%1 (%2)\n").arg((quint16) (uabytes),4,16,QChar('0'))
                    .arg(uabytes);
        }
    } else {
        retval += QString("Data Length: $%1 (%2)\n").arg((quint16) (file->data().length()),4,16,QChar('0'))
                .arg(file->data().length());
    }
//    delete file;
    return retval;
}

void CatalogWidget::processNewlyLoadedDisk(QString diskfilename, DiskFile *disk)
{
   // qDebug() << "### Start processNewlyLoadedDisk";
    if (m_disk == disk) {
        QUrl url = QUrl::fromLocalFile(diskfilename);
        QString shortfilename = url.fileName();
        QFontMetrics fm(ui->catalog_list->font());
        QRect maxrect;
        ui->volume_label->setText(shortfilename);
        int idx = 0;
        foreach(FileDescriptiveEntry fde, m_disk->getAllFDEs()) {
    //        qDebug() << "    Processing FDE# " << idx;
            QString filetype = fde.fileType();
            QString filename = AppleString(fde.filename).printable().trimmed();
            quint16 size = fde.lengthInSectors;
            bool locked = fde.isLocked();
            QString sizeStr = QString("%1").arg(size,5,10,QChar(' ')).toUpper();
            QString text = QString("%1 %2 %3 %4").arg(locked?"*":" ").arg(sizeStr).arg(filetype).arg(filename);
            QListWidgetItem *item = new QListWidgetItem(text);
            if (filetype == "A")      { item->setForeground(Qt::blue); }
            else if (filetype == "I") { item->setForeground(Qt::darkYellow); }
            else if (filetype == "B") { item->setForeground(Qt::darkGreen); }
            else if (filetype == "T") { item->setForeground(Qt::red); }
            else if (filetype == "R") { item->setForeground(Qt::darkRed); }
            else if (filetype == "S") { item->setForeground(Qt::magenta); }
            else if (filetype == "a") { item->setForeground(Qt::darkBlue); }
            else if (filetype == "b") { item->setForeground(Qt::darkMagenta); }
            else { item->setForeground(Qt::black); }
            item->setToolTip(createToolTip(fde));
            item->setData(0x0100,idx);

            if (!fde.deleted) ui->catalog_list->addItem(item);

            QRect rect = fm.boundingRect(text);
            if (rect.width() > maxrect.width()) {
                maxrect = rect;
            }
            idx++;
      //      qDebug() << "    Done Processing FDE# " << idx-1;
        }
        //        QFont italfont = ui->catalog_list->font();
        //        italfont.setItalic(true);
        //        QListWidgetItem *item = new QListWidgetItem("Boot Sector");
        //        item->setForeground(Qt::black);
        //        item->setFont(italfont);
        //        item->setData(0x0100,-1);
        //        ui->catalog_list->addItem(item);

        //        item = new QListWidgetItem("DOS Image");
        //        item->setForeground(Qt::black);
        //        item->setFont(italfont);
        //        item->setData(0x0100,-2);
        //        ui->catalog_list->addItem(item);

        ui->catalog_list->resize(maxrect.width(),ui->catalog_list->size().height());
    }
  //  qDebug() << "### End processNewlyLoadedDisk";
}

void CatalogWidget::unloadDisk(DiskFile *disk)
{
    if (m_disk == disk) {
        m_disk = 0;
    }
    ui->catalog_list->clear();
    ui->volume_label->clear();
}

void CatalogWidget::itemDoubleClicked(QListWidgetItem *item)
{
    int idx = item->data(0x0100).toInt();
    if (idx >= 0)
    {
        FileDescriptiveEntry fde = m_disk->getAllFDEs()[idx];
        //   qDebug() << "Default File " << AppleString(fde.filename).printable().trimmed();
        emit openWithDefaultViewer(m_disk,fde);
    }
    else
    {
        if (idx == -1) // Boot Sector
        {

        }
        else if (idx == -2) // DOS Image
        {

        }
    }
}

void CatalogWidget::itemClicked(QListWidgetItem *item)
{
    int idx = item->data(0x0100).toInt();
    FileDescriptiveEntry fde = m_disk->getAllFDEs()[idx];
    //   qDebug() << "Default File " << AppleString(fde.filename).printable().trimmed();
    emit newFileSelected(m_disk,fde);
}
