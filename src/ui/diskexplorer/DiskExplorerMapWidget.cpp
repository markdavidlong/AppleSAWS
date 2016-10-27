#include "DiskExplorerMapWidget.h"

//#include "vtoc.h"
//#include "catalogsector.h"

#include <QToolButton>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>

DiskExplorerMapWidget::DiskExplorerMapWidget(int numtracks, int numsectors, QWidget *parent) : QWidget(parent)
{
    m_numtracks = numtracks;
    m_numsectors = numsectors;

    setWindowTitle("Disk Explorer");
    //   m_disk = Q_NULLPTR;

    m_currentClicked = Q_NULLPTR;

    initColors();

    QGridLayout *layout = new QGridLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setHorizontalSpacing(1);
    layout->setVerticalSpacing(0);
    setLayout(layout);
    for (int track= 0; track < numtracks; track++)
    {
        QLabel *label = new QLabel(QString("%1").arg(track));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(label,0,track+1);
    }
    for (int sec = 0; sec < numsectors; sec++)
    {
        QLabel *label = new QLabel(QString("Sec %1").arg(sec));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(label,sec+1,0);
    }
    for (int track = 0; track < 35; track++)
    {
        for (int sec = 0; sec < 16; sec++)
        {
            DEButton *tb = new DEButton(this,track,sec);
            tb->setObjectName(QString("BtnT%1S%2").arg(track).arg(sec));
            tb->setBgColor(m_defaultColor);
            connect(tb,SIGNAL(clicked(int,int,bool)),SLOT(handleButtonClick(int,int)));
            m_buttons[track][sec] = tb;

            tb->setAutoFillBackground(true);

            layout->addWidget(tb,sec+1,track+1);
        }
    }
}

void DiskExplorerMapWidget::handleButtonClick(int track, int sector)
{
    if (m_currentClicked)
    {
        // Do anything needed to clean up after previous button click
    }

    DEButton *currbutton = buttonAt(track,sector);

    if (currbutton == m_currentClicked)
    {
        // Handle reclicking on the same button
    }
    else
    {
        // Handle clicking on a new button;

    }
    m_currentClicked = currbutton;
}

void DiskExplorerMapWidget::setButtonBgColor(int track, int sector, QColor color)
{
    buttonAt(track,sector)->setBgColor(color);
}

void DiskExplorerMapWidget::setDisk(DiskFile *disk)
{
    if (disk)
    {
    m_disk = disk;
    setWindowTitle(QString("Disk Explorer - %1").arg(m_disk->getDiskImageName()));
    mapDiskToButtons();
    }
}

DEButton *DiskExplorerMapWidget::buttonAt(int track, int sector)
{
    if (track >= m_numtracks || sector >= m_numsectors)
    {
        if (track >= m_numtracks) { track = 0; }
        if (sector >= m_numsectors) { sector = 0; }
        qWarning() << "Invalid track/sector for button (" << track << ","  << sector << ")";
    }
    return m_buttons[track][sector];
}

void DiskExplorerMapWidget::initColors()
{
    m_defaultColor       = QColor(Qt::lightGray);
    m_bootSectorColor    = QColor(Qt::darkGreen);
    m_dosImageColor      = QColor(Qt::green);
    m_vtocColor          = QColor("#808000");
    m_tsListColor        = QColor("#ff8000");
    m_dirEntryColor      = QColor("#ffff00");
    m_applesoftFileColor = QColor(Qt::blue);
    m_intBasicFileColor  = QColor(Qt::darkBlue);
    m_binaryFileColor    = QColor(Qt::magenta);
    m_textFileColor      = QColor("#00ffff");
    m_reloFileColor      = QColor(Qt::darkMagenta);
    m_typeAFileColor     = QColor(Qt::red);
    m_typeBFileColor     = QColor(Qt::darkRed);
    m_typeSFileColor     = QColor(Qt::darkCyan);
}

void DiskExplorerMapWidget::mapDiskToButtons()
{
    int idx = 0;
    for (int sec = 0; sec < m_numsectors; sec++)
    {
        buttonAt(0,sec)->setBgColor(m_bootSectorColor);
        buttonAt(1,sec)->setBgColor(m_dosImageColor);
        buttonAt(2,sec)->setBgColor(m_dosImageColor);
    }
    buttonAt(17,0)->setBgColor(m_vtocColor);

    foreach (CatalogSector cs, m_disk->getCatalogSectors())
    {
        Sector *sec = cs.getSector();
        buttonAt(sec->track(),sec->sector())->setBgColor(m_dirEntryColor);

        foreach(FileDescriptiveEntry fde, cs.getFDEs())
        {
            Sector *s = &(m_disk->getSector(fde.firstTSListSector));
            TrackSectorList tsl(s);

            int tsltr = fde.firstTSListSector.track;
            int tslse = fde.firstTSListSector.sector;

            while (tsltr != 0)
            {
                buttonAt(tsltr,tslse)->setBgColor(m_tsListColor);
                buttonAt(tsltr,tslse)->setText(QString("%1").arg(idx));
                qDebug() << "Button" << idx << "=" << tsltr << "," << tslse << "   " << fde.filename.printable() << "TSL";

                idx++;

                foreach(TSPair tsp, tsl.getDataTSPairs())
                {
                    int se = tsp.sector;
                    int tr = tsp.track;
                    QColor color;
                    if (fde.fileType() == "I") color = m_intBasicFileColor;
                    else if (fde.fileType() == "A") color = m_applesoftFileColor;
                    else if (fde.fileType() == "R") color = m_reloFileColor;
                    else if (fde.fileType() == "B") color = m_binaryFileColor;
                    else if (fde.fileType() == "S") color = m_typeSFileColor;
                    else if (fde.fileType() == "T") color = m_textFileColor;
                    else if (fde.fileType() == "a") color = m_typeAFileColor;
                    else if (fde.fileType() == "b") color = m_typeBFileColor;
                    else qDebug() << "Unknown file type: " << fde.fileType();
                    buttonAt(tr,se)->setBgColor(color);
                    setButtonText(tr,se,QString("%1").arg(idx));
                    qDebug() << "Button" << idx << "=" << tr << "," << se << "   " << fde.filename.printable();
                    //  fde.dump();
                    idx++;
                }
                tsltr =  tsl.getNextTSList().track;
                tslse = tsl.getNextTSList().sector;
                tsl = m_disk->getSector(tsl.getNextTSList()).promoteToTrackSectorList();
            }

        }
    }
}

