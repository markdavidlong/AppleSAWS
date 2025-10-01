#pragma once

#include "DiskFile.h"

#include <QWidget>
#include <QListWidgetItem>
#include <memory>

namespace Ui {
class  CatalogWidget;
}

class  CatalogWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget(QWidget* parent = nullptr);
    ~CatalogWidget() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    CatalogWidget(const CatalogWidget&) = delete;
    CatalogWidget& operator=(const CatalogWidget&) = delete;
    CatalogWidget(CatalogWidget&&) = delete;
    CatalogWidget& operator=(CatalogWidget&&) = delete;

public slots:
    void prepForNewDisk(const QString& filename, DiskFile* disk);
    void processNewlyLoadedDisk(const QString& filename, DiskFile* disk);
    void unloadDisk(DiskFile* disk);

signals:
    void newFileSelected(DiskFile* disk, const FileDescriptiveEntry& entry);
    void openWithDefaultViewer(DiskFile* disk, const FileDescriptiveEntry& fde);

protected:
    [[nodiscard]] QString createToolTip(FileDescriptiveEntry& fde);

private slots:
    void itemClicked(QListWidgetItem* item);
    void itemDoubleClicked(QListWidgetItem* item);

private:
    std::unique_ptr<Ui::CatalogWidget> ui;
    DiskFile* m_disk{nullptr};
    QString m_diskname;
};

