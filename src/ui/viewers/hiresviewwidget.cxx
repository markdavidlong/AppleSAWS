#include "hiresviewwidget.h"
#include "binaryfile.h"

#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QSettings>
#include <QGridLayout>

#include <math.h>


HiresViewWidget::HiresViewWidget(QWidget *parent) :
    FileViewerInterface(parent)
{
    QGridLayout *gv = new QGridLayout(this);
    setLayout(gv);
    hrsw = new HiresScreenWidget(this);
    gv->addWidget(hrsw);

    resize(561,384);
}

void HiresViewWidget::setFile(BinaryFile *file) {
    m_file = file;

    QString title = QString("Image: %1").arg(m_file->filename());
    setWindowTitle(title);

    hrsw->setData(file->data());
}

bool HiresViewWidget::optionsMenuItems(QMenu *menu)
{
    menu->addAction(hrsw->monochromeAction());
    menu->addAction(hrsw->ntscAction());
    menu->addAction(hrsw->perPixelColorAction());
    menu->addSeparator();
    menu->addAction(hrsw->showScanLinesAction());
    return true;
}

void HiresViewWidget::setFile(GenericFile *file)
{
    BinaryFile *af = dynamic_cast<BinaryFile*>(file);
    if (af)
    {
        setFile(af);
    }
}

bool HiresViewWidget::canPrint() const { return true; }

void HiresViewWidget::doPrint()
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

bool HiresViewWidget::canExport() const { return true; }

void HiresViewWidget::doExport()
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

