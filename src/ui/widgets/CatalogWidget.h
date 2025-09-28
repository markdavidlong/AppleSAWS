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
    explicit CatalogWidget(QWidget *parent = 0);
    ~CatalogWidget();

public slots:
    void prepForNewDisk(QString filename, DiskFile *disk);
    void processNewlyLoadedDisk(QString filename, DiskFile *disk);
    void unloadDisk(DiskFile *disk);

signals:
    void newFileSelected(DiskFile *disk, FileDescriptiveEntry entry);
    void openWithDefaultViewer(DiskFile *disk, FileDescriptiveEntry fde);

protected:
    QString createToolTip(FileDescriptiveEntry &fde);

private slots:
    void itemClicked(QListWidgetItem *item);
    void itemDoubleClicked(QListWidgetItem *item);

private:

    Ui::CatalogWidget *ui;

    DiskFile *m_disk;
    QString m_diskname;
};

