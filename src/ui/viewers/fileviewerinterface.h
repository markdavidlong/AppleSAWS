#ifndef FILEVIEWERINTERFACE_H
#define FILEVIEWERINTERFACE_H

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

#include <QWidget>
#include <QString>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

class GenericFile;
class QMenu;

class FileViewerInterface : public QWidget
{
    Q_OBJECT

public:
    FileViewerInterface(QWidget *parent = Q_NULLPTR) : QWidget(parent) { setFont(QFont("courier")); }

    virtual bool optionsMenuItems(QMenu *) = 0;

    virtual bool canPrint() const { return false; }
    virtual bool canExport() const { return false; }

    QString title() const { return m_title; }

public slots:
    virtual void setFile(GenericFile *file) = 0;

    virtual void doPrint() { }
    virtual void doExport() { }

signals:

protected:
    QString m_title;
};

#endif // FILEVIEWERINTERFACE_H
