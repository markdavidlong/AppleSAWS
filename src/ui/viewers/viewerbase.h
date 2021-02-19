#ifndef VIEWERBASE_H
#define VIEWERBASE_H

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



#include <QMainWindow>
#include <QStackedWidget>
#include <QToolBar>
#include <QComboBox>

#include "fileviewerinterface.h"
#include "genericfile.h"

namespace Ui {
class ViewerBase;
}

class ViewerBase : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerBase(QWidget *parent = 0);
    ~ViewerBase();

    void setFile(GenericFile *file);

signals:
    void viewerClosing(ViewerBase *me);

public slots:
    void showViewer(int index);

protected:
    void closeEvent(QCloseEvent *event);
    void addViewer(QString descriptor, FileViewerInterface *viewer);

private:
    Ui::ViewerBase *ui;
    QStackedWidget *m_stack;
    QToolBar *m_toolbar;
    QComboBox *m_viewercombo;

    QMap<QString,FileViewerInterface *> m_viewers;
    GenericFile *m_file;
    QMenu *m_optionMenu;

};

#endif // VIEWERBASE_H
