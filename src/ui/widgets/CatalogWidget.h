#pragma once

#include "DiskFile.h"

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class  CatalogWidget;
}

class  CatalogWidget : public QWidget
{
    Q_OBJECT


public:
    explicit CatalogWidget(QWidget* parent = nullptr);
    ~CatalogWidget() override;

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
    Ui::CatalogWidget* ui{nullptr};
    DiskFile* m_disk{nullptr};
    QString m_diskname;
};

