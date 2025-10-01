#pragma once

#include "DiskExplorerMapWidget.h"
#include "DiskFile.h"
#include "HexDumpViewer.h"
#include "AsciiInfoDialog.h"

#include <QMainWindow>
#include <QFrame>
#include <QTimer>
#include <memory>

class ViewerBase;
class CatalogWidget;
class HexConverter;
class HRCGControlsInfo;

class DiskExplorer final : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiskExplorer(QWidget* parent = nullptr);
    ~DiskExplorer() override;

signals:
    void diskFileLoading(const QString& filename, DiskFile* file);
    void diskFileLoaded(const QString& filename, DiskFile* file);
    void diskFileLoadFailed(const QString& filename, DiskFile* file);
    void diskFileUnloading(DiskFile* file);
    void diskFileUnloaded();

public slots:
    void unloadDiskFile();
    void loadDiskFile(const QString& filename);
    void showLoadDialog(bool parentToThis = true);

private slots:
    void handleShowSectorData(const QByteArray& data, int track, int sector, const QVariant& metadata);
    void handleDiskItemSelectedDefaultOpen(DiskFile* disk, const FileDescriptiveEntry& fde);

    void setDiskToolsVisible(bool visible);

    void handleViewerClosing(ViewerBase* viewer);

protected:
    void initUi();

    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

protected slots:
    void doResize();
private:
    QAction* m_setDiskToolsVisibleAction{nullptr};

    CatalogWidget* m_cw{nullptr};
    DiskExplorerMapWidget* m_demw{nullptr};
    QFrame* m_frame{nullptr};
    HexDumpViewer* m_hdv{nullptr};
    QWidget* m_key{nullptr};
    QGridLayout* m_gridLayout{nullptr};

    QWidget* m_demwStatusWidget{nullptr};

    DiskFile* m_disk{nullptr};

    HRCGControlsInfo* m_hrcgDialog{nullptr};
    HexConverter* m_hexConverter{nullptr};
    AsciiInfoDialog* m_AsciiInfoDialog{nullptr};

    QAction* m_action_Unload_Disk_Image{nullptr};

    int m_horizSizePref{-1};

    QList<ViewerBase*> m_viewerList;

    bool m_toolsHidden{true};
};

