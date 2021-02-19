#ifndef DISKEXPLORERMAPWIDGET_H
#define DISKEXPLORERMAPWIDGET_H

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

#include "dos33diskimage.h"

#include <QWidget>
#include <QPair>
#include <QMap>
#include <QPushButton>
#include <QColor>
#include <QGroupBox>
#include <QLabel>
#include <QToolButton>

class DEButton : public QToolButton
{
    Q_OBJECT

public:

    DEButton(QWidget *parent,int track = -1, int sec = -1) : QToolButton(parent)
    {
        setTrack(track);
        setSector(sec);
        connect(this, &DEButton::clicked, this, &DEButton::handleClick);
        m_isHighlighted = false;
    }

    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    int track() const { return m_track; }
    int sector() const { return m_sector; }

    void clearBgColor() {
        m_backgroundColor = "";
        setText("");
        setStyleSheet(makeStyleSheet());
    }

    void setBgColor(QColor color) {
        m_fgColor = determineFgColor(color).name();
        m_backgroundColor = color.name();
        m_hlColor = color.lighter(155).name();
        setStyleSheet(makeStyleSheet());
    }

    bool highlighted() const { return m_isHighlighted; }

    void setHighlighted(bool highlighted) {
        m_isHighlighted = highlighted;
        setStyleSheet(makeStyleSheet());
    }

    void reset() {
        setHighlighted(false);
        setChecked(false);
        makeStyleSheet();
        qDebug() << "Reset";
    }

    void resetToDefault() { clearBgColor(); reset(); }

    QColor determineFgColor(QColor bgColor)
    {
        if (qGray(bgColor.rgb()) > 128)
        {
            return QColor(Qt::black);
        }
        return Qt::white;
    }

signals:
    void checked(int track, int sec,bool );

private slots:
    void handleClick(bool isChecked) {
        emit checked(m_track,m_sector,isChecked);
    }

    QSize minimumSizeHint() const Q_DECL_OVERRIDE { return QSize(12,12); }
    QSize sizeHint() const Q_DECL_OVERRIDE { return QSize(24,24); }
    bool hasHeightForWidth() const Q_DECL_OVERRIDE { return true; }
    int heightForWidth(int width) const Q_DECL_OVERRIDE { return width; }

private:

    QString makeStyleSheet() const {
        return QString("DEButton {                  "
                       "    font: 10px;             "
                       "    border-width: 1px;      "
                       "    color: %1;              "
                       "    background-color: %2    "
                       "}                           "
                       "                            "
                       "DEButton:checked {          "
                       "    font: bold italic 11px; "
                       "}"
               ).arg(m_fgColor)
                .arg(m_backgroundColor);
    }

    int m_track;
    int m_sector;

    bool m_isHighlighted;

    QString m_fgColor;
    QString m_backgroundColor;
    QString m_hlColor;

};

enum class DiskSectorRole {
    Unknown,
    Used,

    BootSector,
    DosImage,
    VTOC,
    CatalogSector,
    TSList,

    ApplesoftFile,
    IntBasicFile,
    BinaryFile,
    TextFile,
    RelocatableFile,
    TypeAFile,
    TypeBFile,
    TypeSFile
};

class DiskExplorerMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiskExplorerMapWidget(int numtracks, int numsectors, QWidget *parent = 0);

    void setButtonBgColor(int track, int sector, QColor color);
    void setButtonText(int track, int sector, QString text) {
        buttonAt(track,sector)->setText(text);
    }

    void setDisk(Dos33DiskImage *disk);
    void unloadDisk();

    void setAllButtonsEnabled(bool enabled);

    QGroupBox *makeKeyWidget();
    QWidget *getStatusWidget() const { return m_statusWidget; }

signals:
    void showSectorData(QByteArray data, int track, int sector, QVariant metadata);

public slots:
    void handleButtonCheck(int track, int sector, bool checked);

protected:
    void mapButtonsFromRoles();
    void defineRoles(TSPair vtoc = TSPair(17,0));
    bool setButtonRole(TSPair ts, DiskSectorRole role);
    void setButtonNumber(TSPair ts, int number);

    DEButton *buttonAt(int track, int sector);

    void initColors();

    QColor determineFgColor(QColor bgColor)
    {
        if (qGray(bgColor.rgb()) > 128)
        {
            return QColor(Qt::black);
        }
        return Qt::white;
    }

    QLabel *makeKeyLabel(QWidget *parent, QString name, QColor color);
    void showEvent(QShowEvent *);
    void makeStatusWidget();
    QString getSectorDescription(int track, int sector);

    void mapCatalogSectors(int &buttonNumber);
    void setDescription(TSPair ts, QString description);
    void mapFDE(FileDescriptiveEntry &fde, int fdeNum, int &buttonNumber);
    void mapTSListSector(TSPair location, FileDescriptiveEntry &fde,
                         int &buttonNumber, int &tslCount);
    DiskSectorRole getFileTypeFromID(QString id);
    void checkForUsedButUnknown(TSPair vtoc = TSPair(17,0));
private:

    QMap<int, QMap<int,DEButton*> > m_buttons;
    DEButton *m_currentChecked;

    int m_numtracks;
    int m_numsectors;

    QColor m_defaultColor;
    QColor m_bootSectorColor;
    QColor m_dosImageColor;
    QColor m_vtocColor;
    QColor m_dirEntryColor;
    QColor m_tsListColor;
    QColor m_applesoftFileColor;
    QColor m_intBasicFileColor;
    QColor m_binaryFileColor;
    QColor m_textFileColor;
    QColor m_reloFileColor;
    QColor m_typeAFileColor;
    QColor m_typeBFileColor;
    QColor m_typeSFileColor;

    QButtonGroup *m_bgroup;
    Dos33DiskImage *m_disk;

    bool m_deferredSetup;

    QLabel *m_diskLabel;
    QLabel *m_trackSectorLabel;
    QWidget *m_statusWidget;

    QMap<TSPair, QString> m_descriptions;
    QMap<TSPair, DiskSectorRole> m_roles;
    QMap<TSPair, int> m_numbers;
};


#endif // DISKEXPLORERMAPWIDGET_H
