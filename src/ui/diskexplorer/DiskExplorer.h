#pragma once


#include "catalogwidget.h"
#include "DiskExplorerMapWidget.h"
#include "diskfile.h"
#include "hrcgcontrolsinfo.h"
#include "hexconverter.h"
#include "hexdumpviewer.h"
#include "viewerbase.h"
#include "asciiinfodialog.h"
//#include "notesdialog.h"

#include <QMainWindow>
#include <QFrame>
#include <QTimer>


class DiskExplorer : public QMainWindow
{
    Q_OBJECT
public:
    explicit DiskExplorer(QWidget *parent = 0);
    virtual ~DiskExplorer();

signals:
    void diskFileLoading(QString filename, DiskFile *file);
    void diskFileLoaded(QString filename, DiskFile *file);
    void diskFileLoadFailed(QString filename, DiskFile *file);
    void diskFileUnloading(DiskFile *file);
    void diskFileUnloaded();

public slots:
    void unloadDiskFile();
    void loadDiskFile(QString filename);
    void showLoadDialog(bool parentToThis = true);

private slots:
    void handleShowSectorData(QByteArray data, int track, int sector, QVariant metadata);
    void handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde);

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
    HexDumpViewer *m_hdv;
    QWidget *m_key;
    QGridLayout *m_gridLayout;

    QWidget *m_demwStatusWidget;

    DiskFile *m_disk;

    HRCGControlsInfo *m_hrcgDialog;
    HexConverter *m_hexConverter;
    AsciiInfoDialog *m_AsciiInfoDialog;
//    NotesDialog *m_notesDialog;

    QAction *m_action_Unload_Disk_Image;

    int m_horizSizePref;

    QList<ViewerBase*> m_viewerList;

    bool m_toolsHidden;
};

