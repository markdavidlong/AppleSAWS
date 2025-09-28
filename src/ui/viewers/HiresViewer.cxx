#include "HiresViewer.h"
#include "Util.h"

#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QSettings>
#include <QGridLayout>
#include <QLabel>

#include <math.h>


HiresViewer::HiresViewer(QWidget *parent) :
    FileViewerInterface(parent)
{
    QGridLayout *gv = new QGridLayout(this);
    setLayout(gv);
    hrsw = new HiresScreenWidget(this);
    gv->addWidget(hrsw);
    m_offsetLabel = new QLabel(this);
    m_offsetLabel->setText("");
    gv->addWidget(m_offsetLabel,1,0);
    gv->setRowStretch(0,10000);
    gv->setRowStretch(1,1);

    connect(hrsw, &HiresScreenWidget::newOffset,
            this, &HiresViewer::handleNewOffset);
    handleNewOffset(0);

    resize(561,384);
}

void HiresViewer::setFile(BinaryFile *file) {
    m_file = file;

    QString title = QString("Image: %1").arg(m_file->filename());
    setWindowTitle(title);

    hrsw->setData(file->data());
}

bool HiresViewer::optionsMenuItems(QMenu *menu)
{
    menu->addAction(hrsw->monochromeAction());
    menu->addAction(hrsw->ntscAction());
    menu->addAction(hrsw->perPixelColorAction());
    menu->addSeparator();
    menu->addAction(hrsw->showScanLinesAction());
    menu->addSeparator();
    menu->addAction(hrsw->prevPageAction());
    menu->addAction(hrsw->nextPageAction());

    return true;
}

void HiresViewer::setFile(GenericFile *file)
{
    BinaryFile *af = dynamic_cast<BinaryFile*>(file);
    if (af)
    {
        setFile(af);
    }
}

bool HiresViewer::canPrint() const { return true; }

void HiresViewer::doPrint()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);

    dialog.setWindowTitle(tr("Print HiRes Image"));

    if (dialog.exec() != QDialog::Accepted) { return; }

    QPainter painter(&printer);
    QPixmap pm = hrsw->getPixmap();
    if (!pm.isNull() && pm.width() != 0 && pm.height() != 0)
    {
        painter.drawPixmap(0,0,pm.width(),pm.height(),pm);
    }
    else
    {
        QMessageBox::warning(this,"Print Error","Could not print image");
    }
}

bool HiresViewer::canExport() const { return true; }

void HiresViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".png");

    QString saveName = QFileDialog::getSaveFileName(this,
        tr("Export HiRes Image"), savename.path(), tr("Png Files (*.png)"));

    if (saveName == "") return;  // User cancelled

    qDebug() << "Set filename: " << saveName;

    QFile saveFile(saveName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Save Error","Could not save "+saveName);
        return;
    }

    QPixmap pm = hrsw->getPixmap();
    pm.save(savename.path());
}

void HiresViewer::handleNewOffset(quint16 offset)
{
    QString text = QString("Offset: %1 (0x%2)").arg(offset).arg(uint16ToHex(offset));
    m_offsetLabel->setText(text);
}

