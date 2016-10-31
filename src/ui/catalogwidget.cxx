#include "catalogwidget.h"
#include "ui_catalogwidget.h"
#include "filedescriptiveentry.h"
#include "binaryfile.h"
#include "applesoftfile.h"
#include "genericfile.h"
#include <QUrl>
#include <QDebug>
#include <QMenu>
#include <QAction>

CatalogWidget::CatalogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogWidget)
{
    ui->setupUi(this);
    ui->catalog_list->setFont(QFont("monospace"));

    connect(ui->catalog_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            SLOT(itemDoubleClicked(QListWidgetItem*)));
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
    retval += AppleString(fde.filename).printable().trimmed() + "\n";
    retval += QString("Type: %1\n").arg(fde.fileType());
    retval += QString("Sectors: %1 (%2 bytes)\n")
              .arg(fde.lengthInSectors)
              .arg(fde.lengthInSectors*256);
    retval += QString("%1\n").arg(fde.isLocked()?"Locked":"Unlocked");

    GenericFile *file = m_disk->getFile(fde);
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

    return retval;
}

void CatalogWidget::processNewlyLoadedDisk(QString diskfilename, DiskFile *disk)
{
    if (m_disk == disk) {
        QUrl url = QUrl::fromLocalFile(diskfilename);
        QString shortfilename = url.fileName();
        QFontMetrics fm(ui->catalog_list->font());
        QRect maxrect;
        ui->volume_label->setText(shortfilename);
        int idx = 0;
        foreach(FileDescriptiveEntry fde, m_disk->getAllFDEs()) {
            QString filetype = fde.fileType();
            QString filename = AppleString(fde.filename).printable().trimmed();
            quint16 size = fde.lengthInSectors;
            qDebug() << "SIZE: " << size;
            bool locked = fde.isLocked();
            QString sizeStr = QString("%1").arg(size,5,10,QChar(' ')).toUpper();
            QString text = QString("%1 %2 %3 %4").arg(locked?"*":" ").arg(sizeStr).arg(filetype).arg(filename);
            QListWidgetItem *item = new QListWidgetItem(text);
            item->setToolTip(createToolTip(fde));
            item->setData(0x0100,idx);
            ui->catalog_list->addItem(item);
            QRect rect = fm.boundingRect(text);
            if (rect.width() > maxrect.width()) {
                maxrect = rect;
            }
            idx++;
        }
        ui->catalog_list->resize(maxrect.width(),ui->catalog_list->size().height());
    }
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
    FileDescriptiveEntry fde = m_disk->getAllFDEs()[idx];
 //   qDebug() << "Default File " << AppleString(fde.filename).printable().trimmed();
    emit openWithDefaultViewer(m_disk,fde);
}

void CatalogWidget::itemClicked(QListWidgetItem *item)
{
    int idx = item->data(0x0100).toInt();
    FileDescriptiveEntry fde = m_disk->getAllFDEs()[idx];
 //   qDebug() << "Default File " << AppleString(fde.filename).printable().trimmed();
    emit newFileSelected(m_disk,fde);
}
