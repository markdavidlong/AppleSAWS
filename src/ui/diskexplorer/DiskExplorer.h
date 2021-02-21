#ifndef DISKEXPLORER_H
#define DISKEXPLORER_H

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



#include <QMainWindow>

#include "catalogwidget.h"
#include "DiskExplorerMapWidget.h"
#include "dos33diskimage.h"
#include "hrcgcontrolsinfo.h"
#include "hexconverter.h"
//#include "hexdumpviewer.h"
#include "viewerbase.h"
#include "asciiinfodialog.h"
//#include "notesdialog.h"

#include "viewwidgetstack.h"

#include <QFrame>
#include <QTimer>


class DiskExplorer : public QMainWindow
{
    Q_OBJECT
public:
    explicit DiskExplorer(QWidget *parent = 0);
    virtual ~DiskExplorer();

signals:
    void diskFileLoading(QString filename, Dos33DiskImage *file);
    void diskFileLoaded(QString filename, Dos33DiskImage *file);
    void diskFileLoadFailed(QString filename, Dos33DiskImage *file);
    void diskFileUnloading(Dos33DiskImage *file);
    void diskFileUnloaded();

public slots:
    void unloadDiskFile();
    void loadDiskFile(QString filename);
    void showLoadDialog(bool parentToThis = true);

private slots:
    void handleShowSectorData(QByteArray data, int track, int sector, QVariant metadata);
    void handleDiskItemSelectedDefaultOpen(Dos33DiskImage *disk, FileDescriptiveEntry fde);

    void setDiskToolsVisible(bool visible);

    void handleViewerClosing(ViewerBase *viewer);

protected:
    void initUi();

    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

protected slots:
    void doResize();
private:
    QAction *m_setDiskToolsVisibleAction;

    CatalogWidget *m_cw;
    DiskExplorerMapWidget *m_demw;
    QFrame *m_frame;
 //   HexDumpViewer *m_hdv;
    ViewWidgetStack *m_vws;
    QWidget *m_key;
    QGridLayout *m_gridLayout;

    QWidget *m_demwStatusWidget;

    Dos33DiskImage *m_disk;

    HRCGControlsInfo *m_hrcgDialog;
    HexConverter *m_hexConverter;
    AsciiInfoDialog *m_AsciiInfoDialog;
//    NotesDialog *m_notesDialog;

    QAction *m_action_Unload_Disk_Image;

    int m_horizSizePref;

    QList<ViewerBase*> m_viewerList;

    bool m_toolsHidden;
};

#endif // DISKEXPLORER_H
