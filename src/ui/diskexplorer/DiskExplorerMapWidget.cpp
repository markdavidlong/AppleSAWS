#include "DiskExplorerMapWidget.h"

//#include "vtoc.h"
//#include "catalogsector.h"
#include "util.h"

#include <QPushButton>
#include <QGridLayout>
#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QGuiApplication>

DiskExplorerMapWidget::DiskExplorerMapWidget(int numtracks, int numsectors, QWidget *parent) : QWidget(parent)
{
    m_numtracks = numtracks;
    m_numsectors = numsectors;
    m_diskLabel = Q_NULLPTR;
    m_statusWidget = Q_NULLPTR;

    m_deferredSetup = false;

    setWindowTitle("Disk Explorer");

    m_currentChecked = Q_NULLPTR;

    initColors();

    QGridLayout *layout = new QGridLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setHorizontalSpacing(2);
    layout->setVerticalSpacing(1);

    m_bgroup = new QButtonGroup(this);

    setLayout(layout);
    QLabel *tracklabel = new QLabel("Track",this);
    layout->addWidget(tracklabel,0,0,1,m_numtracks+1,Qt::AlignHCenter);
    for (int track= 0; track < numtracks; track++)
    {
        QLabel *label = new QLabel(QString("%1").arg(track));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(label,1,track+1);
    }
    for (int sec = 0; sec < numsectors; sec++)
    {
        QLabel *label = new QLabel(QString("Sec %1").arg(sec));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(label,sec+2,0);
    }
    for (int track = 0; track < 35; track++)
    {
        for (int sec = 0; sec < 16; sec++)
        {
            DEButton *tb = new DEButton(this,track,sec);
            tb->setObjectName(QString("BtnT%1S%2").arg(track).arg(sec));
            tb->setBgColor(m_defaultColor);
            tb->setCheckable(true);
            m_bgroup->addButton(tb,(track * numsectors) + sec);
            connect(tb, &DEButton::checked, this, &DiskExplorerMapWidget::handleButtonCheck);
            m_buttons[track][sec] = tb;

            tb->setAutoFillBackground(true);

            layout->addWidget(tb,sec+2,track+1);
        }
    }

    makeStatusWidget();
}

void DiskExplorerMapWidget::makeStatusWidget()
{
    QWidget *statusWidget = new QWidget(this);
    QHBoxLayout *hbl = new QHBoxLayout(this);
    statusWidget->setLayout(hbl);

    m_trackSectorLabel = new QLabel(this);
    m_trackSectorLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_trackSectorLabel->setText("No Track/Sector selected");
    hbl->insertWidget(0,m_trackSectorLabel,0,Qt::AlignLeft | Qt::AlignVCenter);

    m_diskLabel = new QLabel(this);
    m_diskLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_diskLabel->setText("[No Disk]");
    hbl->insertWidget(1,m_diskLabel,0,Qt::AlignLeft | Qt::AlignVCenter);

    m_statusWidget = statusWidget;
}

QString DiskExplorerMapWidget::getSectorDescription(int track, int sector)
{
    if (track == 0) {
        return "Boot Sector";
    }
    else if (track <= 2)
    {
        return "DOS Image";
    }
    else if (track == 17 && sector == 0)
    {
        return "VTOC";
    }
    else
    {
        QString desc = "xxx";
        if (m_sectorDescriptions.contains(DETSPair(track,sector)))
        {
            desc = m_sectorDescriptions[DETSPair(track,sector)];
        }
        return desc.simplified();
    }

}

void DiskExplorerMapWidget::handleButtonCheck(int track, int sector, bool checked)
{
    DEButton *currbutton = buttonAt(track,sector);

    if (m_currentChecked)
    {
        // Do anything needed to clean up after previous button click
        //    m_currentClicked->setHighlighted(false);
    }

    if (checked)
    {
        Sector sec = m_disk->getSector(track,sector);
        QByteArray data = sec.rawData();
        emit showSectorData(data,track,sector,QVariant());
        m_trackSectorLabel->setText(
                    QString("Track: %1 Sector: %2 (%3)")
                    .arg(track)
                    .arg(sector)
                    .arg(getSectorDescription(track,sector)));
    }
    else{
        emit showSectorData(QByteArray(),-1,-1,QVariant());
        m_trackSectorLabel->setText("No Track/Sector selected");
    }

    m_currentChecked = currbutton;
}

void DiskExplorerMapWidget::setButtonBgColor(int track, int sector, QColor color)
{
    buttonAt(track,sector)->setBgColor(color);
}

void DiskExplorerMapWidget::setDisk(DiskFile *disk)
{
    if (disk)
    {
        m_sectorDescriptions.clear();

        m_disk = disk;
        setWindowTitle(QString("Disk Explorer - %1").arg(m_disk->getDiskImageName()));
        m_diskLabel->setText(disk->getDiskImageName());

        if (!isHidden())
        {
            if (m_disk)
            {
                mapDiskToButtons();
            }
        }
        else
        {
            m_deferredSetup =  true;
        }
    }
}

void DiskExplorerMapWidget::unloadDisk()
{
    if (m_disk)
    {
        m_sectorDescriptions.clear();
        m_bgroup->setExclusive(false);
        for (int track = 0; track < m_numtracks; track++)
        {
            for (int sec = 0; sec < m_numsectors; sec++)
            {
                DEButton *button = buttonAt(track,sec);
                button->setText("");
                button->setBgColor(m_defaultColor);
                button->setChecked(false);
            }
        }
        setAllButtonsEnabled(false);
    }
}

void DiskExplorerMapWidget::setAllButtonsEnabled(bool enabled)
{
    for (int track = 0; track < m_numtracks; track++)
    {
        for (int sec = 0; sec < m_numsectors; sec++)
        {
            DEButton *button = buttonAt(track,sec);
            button->setEnabled(enabled);
        }
    }
}

QLabel *DiskExplorerMapWidget::makeKeyLabel(QWidget *parent, QString name, QColor color)
{
    QLabel *label = new QLabel(name,parent);
    label->setStyleSheet(QString("background-color:%1; color: %2")
                         .arg(color.name())
                         .arg(determineFgColor(color).name()));
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    return label;
}

QGroupBox *DiskExplorerMapWidget::makeKeyWidget()
{
    int idx = 0;
    QGroupBox *groupbox= new QGroupBox(this);
    groupbox->setTitle("Key");
    QGridLayout *layout = new QGridLayout();
    layout->setVerticalSpacing(0);
    layout->setHorizontalSpacing(0);
    groupbox->setLayout(layout);

    layout->addWidget(makeKeyLabel(groupbox,"Unused Sector",m_defaultColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Boot Sector",m_bootSectorColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"DOS Image",m_dosImageColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"VTOC",m_vtocColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Dir Entry",m_dirEntryColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"T/S List",m_tsListColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Applesoft File",m_applesoftFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Int. Basic File",m_intBasicFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Binary File",m_binaryFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Text File",m_textFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Relocatable File",m_reloFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Type-A File",m_typeAFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Type-B File",m_typeBFileColor),idx++,0);
    layout->addWidget(makeKeyLabel(groupbox,"Type-S File",m_typeSFileColor),idx++,0);

    return groupbox;
}

DEButton *DiskExplorerMapWidget::buttonAt(int track, int sector)
{
    qDebug() << "Button At:" << track << "," << sector;
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
    m_bootSectorColor    = QColor("#20b060");
    m_dosImageColor      = QColor("#30d000");
    m_vtocColor          = QColor("#efc010");
    m_tsListColor        = QColor("#ff8000");
    m_dirEntryColor      = QColor("#dfdf00");
    m_applesoftFileColor = QColor("#3030e0");
    m_intBasicFileColor  = QColor("#00d0d0");
    m_binaryFileColor    = QColor("#d060d0");
    m_textFileColor      = QColor("#F05060");
    m_reloFileColor     = QColor("#d00000");
    m_typeAFileColor      = QColor("#c040a0");
    m_typeBFileColor     = QColor("#c03030");
    m_typeSFileColor     = QColor("#20a0a0");
}

void DiskExplorerMapWidget::mapDiskToButtons()
{
    setAllButtonsEnabled(true);
    m_bgroup->setExclusive(false);

    int idx = 0;
    for (int track = 0; track < 3; track++)
    {
        for (int sec = 0; sec < m_numsectors; sec++)
        {
            if (track == 0)
                buttonAt(track,sec)->setBgColor(m_bootSectorColor);
            else
                buttonAt(track,sec)->setBgColor(m_dosImageColor);
            buttonAt(track,sec)->setText(QString("%1").arg(idx++));
        }
    }

    buttonAt(17,0)->setBgColor(m_vtocColor);
    buttonAt(17,0)->setText(QString("%1").arg(idx++));

    int catseccount = 0;
    foreach (CatalogSector cs, m_disk->getCatalogSectors())
    {
   //     qDebug() << "LOOP 1";
        Sector *sec = cs.getSector();

        QString desc = QString("Catalog Sector #%1").arg(++catseccount);
        m_sectorDescriptions.insert(DETSPair(sec->track(),sec->sector()),desc);

        buttonAt(sec->track(),sec->sector())->setBgColor(m_dirEntryColor);
        buttonAt(sec->track(),sec->sector())->setText(QString("%1").arg(idx++));

        foreach(FileDescriptiveEntry fde, cs.getFDEs())
        {
   //         qDebug() << "LOOP 2";
            Sector *s = &(m_disk->getSector(fde.firstTSListSector()));
            TrackSectorList tsl(s);

            int tsltr = fde.firstTSListSector().track();
            int tslse = fde.firstTSListSector().sector();

            if (!fde.firstTSListSector().isValid())
            {
                qDebug() << "Invalid first tse entry.  Skipping TSList.";
                break;
            }

            int tslcount = 0;
            while (tsltr != 0 /*&& tslcount < 1*/)
            {
                qDebug() << "LOOP 3";
                tslcount++;

                buttonAt(tsltr,tslse)->setBgColor(m_tsListColor);
                buttonAt(tsltr,tslse)->setText(QString("%1").arg(idx));
    //            qDebug() << "Button" << idx << "=" << tsltr << "," << tslse << "   " << fde.filename.printable() << "TSL";

                QString description = QString("T/S List #%1 for %2").arg(tslcount).arg(fde.filename.printable());
                m_sectorDescriptions.insert(DETSPair(tsltr,tslse),description);

                idx++;

                bool valid = true;

                int sectorcount = 0;
                auto pairs = tsl.getDataTSPairs();
              //  int jdx = 0;
/*if (false) */               foreach(TSPair tsp, pairs)
                {

           //         qDebug() << "LOOP 4" << jdx++ << "of" << pairs.count() << "pairs";
//QCoreApplication::processEvents(QEventLoop::AllEvents);
                    int se = tsp.sector();
                    int tr = tsp.track();

                    if (valid && tsp.isValid() && (se != 0 && tr != 0)) {
                        QString description = QString("Sector #%1.%2 of %3")
                                .arg(tslcount)
                                .arg(++sectorcount)
                                .arg(fde.filename.printable());
                        m_sectorDescriptions.insert(DETSPair(tr,se),description);

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
                          fde.dump();
                    }
                    idx++;
                }
                tsltr =  tsl.getNextTSList().track();
                tslse = tsl.getNextTSList().sector();

                valid = tsl.getNextTSList().isValid();

                tsl = m_disk->getSector(tsl.getNextTSList()).promoteToTrackSectorList();
            }
        }
    }
    m_bgroup->setExclusive(true);

}

void DiskExplorerMapWidget::showEvent(QShowEvent *)
{
    if (m_deferredSetup)
    {
        if (m_disk)
        {
            mapDiskToButtons();
        }
        m_deferredSetup = false;
    }
}
