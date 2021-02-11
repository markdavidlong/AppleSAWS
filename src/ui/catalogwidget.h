#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

#include "dos33diskimage.h"

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
    void prepForNewDisk(QString filename, Dos33DiskImage *disk);
    void processNewlyLoadedDisk(QString filename, Dos33DiskImage *disk);
    void unloadDisk(Dos33DiskImage *disk);

signals:
    void newFileSelected(Dos33DiskImage *disk, FileDescriptiveEntry entry);
    void openWithDefaultViewer(Dos33DiskImage *disk, FileDescriptiveEntry fde);

protected:
    QString createToolTip(FileDescriptiveEntry &fde);

private slots:
    void itemClicked(QListWidgetItem *item);
    void itemDoubleClicked(QListWidgetItem *item);

private:

    Ui::CatalogWidget *ui;

    Dos33DiskImage *m_disk;
    QString m_diskname;
};

#endif // CATALOGWIDGET_H
