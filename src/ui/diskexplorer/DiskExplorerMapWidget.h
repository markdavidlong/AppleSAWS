#ifndef DISKEXPLORERMAPWIDGET_H
#define DISKEXPLORERMAPWIDGET_H

#include <QWidget>
#include <QPair>
#include <QMap>
#include <QPushButton>
#include <QColor>
#include <QGroupBox>
#include <QLabel>


#include "diskfile.h"


class DEButton : public QPushButton
{
    Q_OBJECT
public:
    DEButton(QWidget *parent,int track = -1, int sec = -1) : QPushButton(parent)
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

    void clearBgColor() { m_backgroundColor = ""; setText(""); setStyleSheet(makeStyleSheet());}

    void setBgColor(QColor color) {
        m_fgColor = determineFgColor(color).name();
        m_backgroundColor = color.name();
        m_hlColor = color.lighter(155).name();
        setStyleSheet(makeStyleSheet());
    }

    bool highlighted() const { return m_isHighlighted; }
    void setHighlighted(bool highlighted) { m_isHighlighted = highlighted;setStyleSheet(makeStyleSheet()); }

    void reset() { setHighlighted(false); setChecked(false); makeStyleSheet(); qDebug() << "Reset";}

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
    void handleClick(bool isChecked) { emit checked(m_track,m_sector,isChecked); }

    QSize minimumSizeHint() const Q_DECL_OVERRIDE { return QSize(24,24); }
    QSize sizeHint() const Q_DECL_OVERRIDE { return QSize(24,24); }
    bool hasHeightForWidth() const Q_DECL_OVERRIDE { return true; }
    int heightForWidth(int width) const Q_DECL_OVERRIDE { return width; }

private:
    QString makeStyleSheet() const {
        return QString(" QPushButton {  font: 10px; border-width: 1px; color: %1; background-color: %2} "
                       " QPushButton:checked { font: bold italic 11px; } "
               ) .arg(m_fgColor)
                .arg(m_backgroundColor);
    }

    int m_track;
    int m_sector;
    bool m_isHighlighted;

    QString m_fgColor;
    QString m_backgroundColor;
    QString m_hlColor;
};



typedef QPair<int,int> DETSPair;

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
    void unloadDisk();

    void setAllButtonsEnabled(bool enabled);

    QGroupBox *makeKeyWidget();
    QWidget *getStatusWidget() const { return m_statusWidget; }

signals:
    void showSectorData(QByteArray data, int track, int sector, QVariant metadata);

public slots:
    void handleButtonCheck(int track, int sector, bool checked);

protected:
    void mapDiskToButtons();
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
    DiskFile *m_disk;

    bool m_deferredSetup;

    QLabel *m_diskLabel;
    QLabel *m_trackSectorLabel;
    QWidget *m_statusWidget;

    QMap< DETSPair, QString> m_sectorDescriptions;
};

#endif // DISKEXPLORERMAPWIDGET_H
