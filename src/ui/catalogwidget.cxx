#include "catalogwidget.h"
#include "ui_catalogwidget.h"
#include "filedescriptiveentry.h"
#include <QUrl>

CatalogWidget::CatalogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogWidget)
{
    ui->setupUi(this);
    ui->catalog_list->setFont(QFont("monospace"));

    connect(ui->catalog_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            SLOT(itemClicked(QListWidgetItem*)));

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

void CatalogWidget::processNewlyLoadedDisk(QString diskfilename, DiskFile *disk)
{
    if (m_disk == disk) {
        QUrl url = QUrl::fromLocalFile(diskfilename);
        QString shortfilename = url.fileName();
        QFontMetrics *fm = new QFontMetrics(ui->catalog_list->font());
        QRect maxrect;
        ui->volume_label->setText(shortfilename);
        int idx = 0;
        foreach(FileDescriptiveEntry fde, m_disk->getAllFDEs()) {
            QString filetype = fde.fileType();
            QString filename = AppleString(fde.filename).printable().trimmed();
            int size = fde.lengthInSectors;
            bool locked = fde.isLocked();
            QString sizeStr = QString("%1").arg(size,5,10,QChar(' ')).toUpper();
            QString text = QString("%1 %2 %3 %4").arg(locked?"*":" ").arg(sizeStr).arg(filetype).arg(filename);
            QListWidgetItem *item = new QListWidgetItem(text);
            item->setData(0x0100,idx);
            ui->catalog_list->addItem(item);
            QRect rect = fm->boundingRect(text);
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

void CatalogWidget::itemClicked(QListWidgetItem *item)
{
    int idx = item->data(0x0100).toInt();
    FileDescriptiveEntry fde = m_disk->getAllFDEs()[idx];
    qDebug() << "File " << AppleString(fde.filename).printable().trimmed();
    emit itemSelected(m_disk,fde);
}


