#ifndef DISKEXPLORER_H
#define DISKEXPLORER_H

#include <QMainWindow>

#include "catalogwidget.h"
#include "DiskExplorerMapWidget.h"
#include "diskfile.h"
#include "hrcgcontrolsinfo.h"
#include "hexconverter.h"
#include "hexdumpviewer.h"

#include <QFrame>


class DiskExplorer : public QMainWindow
{
    Q_OBJECT
public:
    explicit DiskExplorer(QWidget *parent = 0);

signals:
    void diskFileLoading(QString filename, DiskFile *file);
    void diskFileLoaded(QString filename, DiskFile *file);
    void diskFileLoadFailed(QString filename, DiskFile *file);
    void diskFileUnloading(DiskFile *file);
    void diskFileUnloaded();

public slots:
    void unloadDiskFile();
    void loadDiskFile(QString filename);
    void showLoadDialog();

private slots:
    void handleShowSectorData(QByteArray data, int track, int sector, QVariant metadata);
    void handleDiskItemSelectedDefaultOpen(DiskFile *disk, FileDescriptiveEntry fde);


protected:
    void initUi();

private:
    CatalogWidget *m_cw;
    DiskExplorerMapWidget *m_demw;
    QFrame *m_frame;
    HexDumpViewer *m_hdv;

    DiskFile *m_disk;

    HRCGControlsInfo *m_hrcgDialog;
    HexConverter *m_hexConverter;


    QAction *m_action_Unload_Disk_Image;

};

#endif // DISKEXPLORER_H
