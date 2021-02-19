#ifndef APPLESOFTFILEDETAILVIEWER_H
#define APPLESOFTFILEDETAILVIEWER_H

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
#include "applesoftline.h"
#include "applesofttoken.h"
#include "applesoftfile.h"
#include <QDebug>

namespace Ui {
class ApplesoftFileDetailViewer;
}

class ApplesoftFileDetailViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ApplesoftFileDetailViewer(ApplesoftFile *file, QWidget *parent = 0);
    ~ApplesoftFileDetailViewer();

    void setLineData(QVector<ApplesoftLine> lineData);
    void foo() { qDebug() << "AFDV::foo!"; }

    bool save();
    bool load();

protected:
    QString shortenName(QString name);
private:
    void  process();

    Ui::ApplesoftFileDetailViewer *ui;
    QVector<ApplesoftLine> m_lines;

    QMap<QString,QString> m_notes;

    ApplesoftFile *m_file;
};

#endif // APPLESOFTFILEDETAILVIEWER_H
