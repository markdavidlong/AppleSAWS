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

    m_diskLabel = nullptr;
    m_statusWidget = nullptr;
    m_currentChecked = nullptr;

    m_deferredSetup = false;

    setWindowTitle("Disk Explorer");

    QGridLayout *gridlayout = new QGridLayout(this);
   // gridlayout->setSizeConstraint(QLayout::SetMinimumSize);
    gridlayout->setHorizontalSpacing(1);
    gridlayout->setVerticalSpacing(1);

    initColors();
    makeStatusWidget();

    m_bgroup = new QButtonGroup(this);

    QLabel *tracklabel = new QLabel("Track",this);
    tracklabel->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));

    gridlayout->addWidget(tracklabel,0,0,1,m_numtracks+1,Qt::AlignHCenter);
    for (int track= 0; track < numtracks; track++)
    {
        QLabel *label = new QLabel(QString("%1").arg(track,2,10,QChar('0')));
        label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        gridlayout->addWidget(label,1,track+1);
    }
    for (int sec = 0; sec < numsectors; sec++)
    {
        QLabel *label = new QLabel(QString("Sec %1").arg(sec));
        label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        gridlayout->addWidget(label,sec+2,0);
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

            gridlayout->addWidget(tb,sec+2,track+1);
        }
    }

    gridlayout->addWidget(getStatusWidget(),18,0,1,35); // Stretchy Row
   // gridlayout->setRowStretch(18,900);

    for (auto rownum = 0; rownum < gridlayout->rowCount(); rownum++)
    {
        gridlayout->setRowStretch(rownum,1);
    }
    gridlayout->setRowStretch(gridlayout->rowCount()-1, 5000);

    for (auto colnum= 0; colnum < gridlayout->columnCount(); colnum++)
    {
        gridlayout->setColumnStretch(colnum,1);
    }

}

void DiskExplorerMapWidget::makeStatusWidget()
{
    QWidget *statusWidget = new QWidget;
    QHBoxLayout *hbl = new QHBoxLayout();
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
    if (track == 0 && sector == 0) {
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
        if (m_descriptions.contains(TSPair(track,sector)))
        {
            desc = m_descriptions[TSPair(track,sector)];
        }
        else
        {
            if (m_roles[TSPair(track,sector)] == DiskSectorRole::Unknown)
            {
                desc = "Unused";
            }
            else
            {
                desc = "Marked as used, but use unknown";
            }
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
    }

    if (checked)
    {
        pSector sec = m_disk->getSector(track,sector);
        const QByteArray data = sec->rawData();
        emit showSectorData(data,track,sector,QVariant((int) m_roles[TSPair(track,sector)]));
        emit showSector(sec,track,sector,m_roles[TSPair(track,sector)]);

        m_trackSectorLabel->setText(
                    QString("Track: %1 Sector: %2 (%3)")
                    .arg(track)
                    .arg(sector)
                    .arg(getSectorDescription(track,sector)));
    }
    else{
        emit showSectorData(QByteArray(),-1,-1,QVariant(-1));
        emit showSector(nullptr, -1, -1, DiskSectorRole::Unknown);

        m_trackSectorLabel->setText("No Track/Sector selected");
    }

    m_currentChecked = currbutton;
}

void DiskExplorerMapWidget::setButtonBgColor(int track, int sector, QColor color)
{
    buttonAt(track,sector)->setBgColor(color);
}

void DiskExplorerMapWidget::setDisk(Dos33DiskImage *disk)
{
    if (disk)
    {
        m_descriptions.clear();

        m_disk = disk;
        setWindowTitle(QString("Disk Explorer - %1").arg(m_disk->getDiskImageName()));
        m_diskLabel->setText(disk->getDiskImageName());

        if (!isHidden())
        {
            if (m_disk)
            {
                defineRoles();
                checkForUsedButUnknown();
                mapButtonsFromRoles();
                //mapDiskToButtons();
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
        m_descriptions.clear();
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
    QGroupBox *groupbox= new QGroupBox();
    //groupbox->setTitle("Key");
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

ViewWidgetStack *DiskExplorerMapWidget::generateViewWidgetStack()
{
    ViewWidgetStack *vws = new ViewWidgetStack();
    vws->setSector(nullptr);
    connect(this, &DiskExplorerMapWidget::showSector,
            vws, &ViewWidgetStack::handleShowSectorData);
    return vws;
}

DEButton *DiskExplorerMapWidget::buttonAt(int track, int sector)
{
    //qDebug() << "Button At:" << track << "," << sector;
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
    m_reloFileColor      = QColor("#d00000");
    m_typeAFileColor     = QColor("#c040a0");
    m_typeBFileColor     = QColor("#c03030");
    m_typeSFileColor     = QColor("#20a0a0");
}

void DiskExplorerMapWidget::defineRoles(TSPair vtoc)
{
    m_descriptions.clear();
    m_numbers.clear();
    m_roles.clear();

    qDebug() << "in defineRoles with vtoc at : " << vtoc.track() << "," << vtoc.sector();

    int buttonNumber = 0;

    for (auto track = 0; track < m_numtracks; track++)
    {
        for (auto sec = 0; sec < m_numsectors; sec++)
        {
            TSPair ts(track,sec);
            if (track == 0 && sec == 0)
            {
                if (setButtonRole(ts,DiskSectorRole::BootSector))
                {
                    setButtonNumber(ts,buttonNumber++);
                    setDescription(ts,"Boot Sector");
                }
            }
            else if (track <= 2)
            {
                if (setButtonRole(ts,DiskSectorRole::DosImage))
                {
                    setButtonNumber(ts,buttonNumber++);
                    setDescription(ts,"DOS Image");
                }
            }
            else
            {
                setButtonRole(ts,DiskSectorRole::Unknown);
            }
        }
    }

    if (setButtonRole(vtoc, DiskSectorRole::VTOC))
    {
        setButtonNumber(vtoc,buttonNumber++);
        setDescription(vtoc,"VTOC");
    }

    mapCatalogSectors(buttonNumber);
}

void DiskExplorerMapWidget::mapCatalogSectors(int &buttonNumber)
{
    int catSectorCount = 0;
    foreach (auto  cs, m_disk->getCatalogSectors())
    {
        qDebug() << "Processing buttons for catalog sectors";
        TSPair ts = cs.sectorLocation();
        qDebug() << "Sector location: " << ts.track() << "," << ts.sector();
        if (setButtonRole(ts,DiskSectorRole::CatalogSector))
        {
            QString desc = QString("Catalog Sector #%1").arg(++catSectorCount);
            qDebug() << "Desc: " << desc;
            setDescription(ts,desc);
            setButtonNumber(ts,buttonNumber++);
//            qDebug() << "Setting button number " << buttonNumber-1
//                     << " at " << ts.track() << "," << ts.sector();

            int fdeNum = 0;
            foreach (FileDescriptiveEntry fde, cs.getFDEs())
            {
                mapFDE(fde, fdeNum++,  buttonNumber);
            }
        }
        else
        {
            qDebug("Not processing remap-attempted Catalog Sector");
        }

    }
}

void DiskExplorerMapWidget::mapFDE(FileDescriptiveEntry &fde, int /*fdeNum*/, int &buttonNumber)
{
    TSPair ts_list = fde.firstTSListSector();

    int tslCount = 0;

    if (fde.lengthInSectors == 0)
    {
        qDebug("FDE Reports 0 sectors used, not looking at T/S List");
        return;
    }
    mapTSListSector(ts_list,fde,buttonNumber, tslCount);
}

void DiskExplorerMapWidget::mapTSListSector(TSPair location,
                                            FileDescriptiveEntry &fde,
                                            int &buttonNumber, int &tslCount)
{
    // Don't follow deleted links.
    if (location.track() == 0xff) return;


    if (location == TSPair(0,0))
    {
        return;
    }

    if (!location.isValid())
    {
        qDebug("Invalid mapTSListSector location: %d/%d.  Not parsing.",
               location.track(),location.sector());
        return;
    }

    if (m_roles[location] != DiskSectorRole::Unknown)
    {
        qDebug("Not processing FDE for previously processed sector at %d/%d",
               location.track(), location.sector());
        return;
    }

    pSector s = m_disk->getSector(location);
    TrackSectorList tsl(s);

    if (setButtonRole(location,DiskSectorRole::TSList))
    {
        tslCount++;
        QString desc = QString("T/S List #%1 for %2").arg(tslCount)
                .arg(fde.filename.printable());
        setDescription(location, desc);
        setButtonNumber(location,buttonNumber++);

        int sectorcount = 0;
        auto pairs = tsl.getValidTSPairs();

        foreach(TSPair tsp, pairs)
        {
            if (tsp.isValid())
            {
                auto filetype = getFileTypeFromID(fde.fileTypeIdentifier());
                if (setButtonRole(tsp,filetype))
                {
                    setButtonNumber(tsp,buttonNumber++);
                    QString description = QString("Sector #%1.%2 of %3")
                            .arg(tslCount)
                            .arg(++sectorcount)
                            .arg(fde.filename.printable());
                    setDescription(tsp,description);
                }
            }
            else
            {
                qDebug("Invalid value in T/S List");
            }
        }

        TSPair next = tsl.getNextTSList();
        mapTSListSector(next, fde, buttonNumber, tslCount);
    }


}

DiskSectorRole DiskExplorerMapWidget::getFileTypeFromID(QString id)
{
    DiskSectorRole role = DiskSectorRole::Used;

    if (id == "I") role = DiskSectorRole::IntBasicFile;
    else if (id == "A") role = DiskSectorRole::ApplesoftFile;
    else if (id== "R") role = DiskSectorRole::RelocatableFile;
    else if (id == "B") role = DiskSectorRole::BinaryFile;
    else if (id== "S") role = DiskSectorRole::TypeSFile;
    else if (id == "T") role = DiskSectorRole::TextFile;
    else if (id == "a") role = DiskSectorRole::TypeAFile;
    else if (id == "b") role = DiskSectorRole::TypeBFile;

    return role;
}


bool DiskExplorerMapWidget::setButtonRole(TSPair ts, DiskSectorRole role)
{
    if (m_roles.contains(ts) && m_roles[ts] != DiskSectorRole::Unknown)
    {
        qDebug("Trying to remap button %d (Role: %d) with new role %d.",
               m_numbers[ts], (int) m_roles[ts], (int) role);
        return false;
    }

    m_roles[ts] = role;
    return true;

}

void DiskExplorerMapWidget::setButtonNumber(TSPair ts, int number)
{
    m_numbers[ts] = number;
}

void DiskExplorerMapWidget::setDescription(TSPair ts, QString description)
{
    m_descriptions[ts] = description;
}

void DiskExplorerMapWidget::mapButtonsFromRoles()
{
    for (auto track = 0; track < m_numtracks; track++)
    {
        for (auto sec = 0; sec < m_numsectors; sec++)
        {
            auto button = buttonAt(track,sec);
            TSPair ts(track,sec);

            QColor buttonColor;

            switch (m_roles[ts])
            {
            case DiskSectorRole::BootSector:
                buttonColor = m_bootSectorColor;
                break;
            case DiskSectorRole::DosImage:
                buttonColor = m_dosImageColor;
                break;
            case DiskSectorRole::VTOC:
                buttonColor = m_vtocColor;
                break;
            case DiskSectorRole::CatalogSector:
                buttonColor = m_dirEntryColor;
                break;
            case DiskSectorRole::TSList:
                buttonColor = m_tsListColor;
                break;
            case DiskSectorRole::ApplesoftFile:
                buttonColor = m_applesoftFileColor;
                break;
            case DiskSectorRole::IntBasicFile:
                buttonColor = m_intBasicFileColor;
                break;
            case DiskSectorRole::TextFile:
                buttonColor = m_textFileColor;
                break;
            case DiskSectorRole::RelocatableFile:
                buttonColor = m_reloFileColor;
                break;
            case DiskSectorRole::BinaryFile:
                buttonColor = m_binaryFileColor;
                break;
            case DiskSectorRole::TypeAFile:
                buttonColor = m_typeAFileColor;
                break;
            case DiskSectorRole::TypeBFile:
                buttonColor = m_typeBFileColor;
                break;
            case DiskSectorRole::TypeSFile:
                buttonColor = m_typeSFileColor;
                break;
            case DiskSectorRole::Used:
                buttonColor = m_defaultColor.lighter(120);
                break;
            case DiskSectorRole::Unknown:
            default:
                buttonColor = m_defaultColor;
            }

            button->setBgColor(buttonColor);
            if (m_roles.contains(ts))
            {
                if (m_roles[ts] != DiskSectorRole::Used && m_roles[ts] != DiskSectorRole::Unknown)
                {
                    setButtonText(track,sec,QString("%1").arg(m_numbers[ts]));
                }
                else
                {
                    if (m_roles[ts] == DiskSectorRole::Used)
                    {
                        setButtonText(track,sec,"?");
                    }
                }
            }
        }
    }
}

void DiskExplorerMapWidget::checkForUsedButUnknown(TSPair vtoc)
{
    auto vtocsector = m_disk->getSector(vtoc)->promoteToVTOC();

    for (auto track = 0; track < m_numtracks; track++)
    {
        for (auto sec = 0; sec < m_numsectors; sec++)
        {
            TSPair ts(track,sec);

            if (m_roles[ts] == DiskSectorRole::Unknown)
            {
                if (vtocsector.isSectorInUse(ts))
                {
                    m_roles[ts] = DiskSectorRole::Used;
                }
            }
        }
    }
}

void DiskExplorerMapWidget::showEvent(QShowEvent *)
{
    if (m_deferredSetup)
    {
        if (m_disk)
        {
            defineRoles();
            checkForUsedButUnknown();
            mapButtonsFromRoles();
            //mapDiskToButtons();
        }
        m_deferredSetup = false;
    }
}
