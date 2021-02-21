#ifndef CENTRALAPPWINDOW_H
#define CENTRALAPPWINDOW_H

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

#include "mainapptoolbar.h"
class QTreeView;
class QAction;
class QStatusBar;
class SequenceToolBox;
class QStackedWidget;

#include "DockAreaWidget.h"
#include "DockWidget.h"
#include "DockManager.h"
#include <QActionGroup>


class CentralAppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CentralAppWindow(QWidget *parent = nullptr);

private:
    void createActions();
    void initMenuBar();
    void initStatusBar();
    void initToolBars();
    void initDockWidgets();
    void initCentralWidget();

private:
    QAction *m_quitAction;
    QAction *m_reference_action;
    QAction *m_utils_action;
    QAction *m_image_lib_action;
    QAction *m_disk_explorer_action;
    QAction *m_projects_action;
    QActionGroup *m_tool_action_group;

    MainAppToolbar *m_mainToolBar;
    QTreeView *m_directory_area;
    QWidget *m_project_area;
    QStatusBar *m_status_bar;

    SequenceToolBox *m_toolbox;

    ads::CDockManager *DockManager;
    ads::CDockWidget *CentralDockWidget;
    ads::CDockAreaWidget *m_central_dock_area;

    QStackedWidget *m_central_stack;
};

#endif // CENTRALAPPWINDOW_H
