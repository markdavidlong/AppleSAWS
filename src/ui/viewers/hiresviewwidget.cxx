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

