#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>

#include "diskfile.h"

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
    void newFileSelected(FileDescriptiveEntry *entry);

private:

    Ui::CatalogWidget *ui;

    DiskFile *m_disk;
    QString m_diskname;
};

#endif // CATALOGWIDGET_H
