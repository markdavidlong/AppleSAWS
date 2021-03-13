/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "hiresviewwidget.h"
#include "binaryfile.h"
#include "util.h"

#include <QPainter>
#include <QMap>
#include <QDebug>
#include <QResizeEvent>
#include <QSettings>
#include <QGridLayout>
#include <QLabel>

#include <math.h>


HiresViewWidget::HiresViewWidget(QWidget *parent) :
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
            this, &HiresViewWidget::handleNewOffset);
    handleNewOffset(0);

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
    menu->addSeparator();
    menu->addAction(hrsw->prevPageAction());
    menu->addAction(hrsw->nextPageAction());

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

void HiresViewWidget::handleNewOffset(quint16 offset)
{
    QString text = QString("Offset: %1 (0x%2)").arg(offset).arg(uint16ToHex(offset));
    m_offsetLabel->setText(text);
}

