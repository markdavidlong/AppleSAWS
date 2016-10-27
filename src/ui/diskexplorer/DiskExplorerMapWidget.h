#ifndef DISKEXPLORERMAPWIDGET_H
#define DISKEXPLORERMAPWIDGET_H

#include <QWidget>
#include <QPair>
#include <QMap>
#include <QPushButton>
#include <QColor>

#include "diskfile.h"

class DEButton : public QPushButton
{
    Q_OBJECT
public:
    DEButton(QWidget *parent,int track = -1, int sec = -1) : QPushButton(parent)
    {
        setTrack(track);
        setSector(sec);
        connect(this,SIGNAL(clicked(bool)),SLOT(handleClick(bool)));
    }
    void setTrack(int track) { m_track = track; }
    void setSector(int sector) { m_sector = sector; }

    int track() const { return m_track; }
    int sector() const { return m_sector; }

    void clearBgColor() { m_backgroundColor = ""; setStyleSheet(makeStyleSheet());}
    void setBgColor(QColor color) { m_backgroundColor = color.name(); setStyleSheet(makeStyleSheet()); }


signals:
    void clicked(int track, int sec,bool );

private slots:
    void handleClick(bool checked) { emit clicked(m_track,m_sector,checked); }

    QSize minimumSizeHint() const Q_DECL_OVERRIDE { return QSize(24,24); }
    QSize sizeHint() const Q_DECL_OVERRIDE { return QSize(24,24); }
    bool hasHeightForWidth() const Q_DECL_OVERRIDE { return true; }
    int heightForWidth(int width) const Q_DECL_OVERRIDE { return width; }

private:
    QString makeStyleSheet() const { return QString("background-color:%1;").arg(m_backgroundColor); }
    int m_track;
    int m_sector;


    QString m_backgroundColor;
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

    void setDisk(DiskFile *disk);

signals:

public slots:
    void handleButtonClick(int track, int sector);

protected:
    void mapDiskToButtons();
    DEButton *buttonAt(int track, int sector);

    void initColors();

private:

    QMap<int, QMap<int,DEButton*> > m_buttons;
    DEButton *m_currentClicked;

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

    DiskFile *m_disk;
};

#endif // DISKEXPLORERMAPWIDGET_H
