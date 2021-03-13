#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

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
