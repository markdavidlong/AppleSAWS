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

#include "centralappwindow.h"
#include "sequenceviewer.h"

#include "dos33diskimage.h"
#include "dos33imagemodel.h"
#include "dos33disktreeview.h"
#include "sequencetoolbox.h"
#include "diskexplorermapwidget.h"

#include <QTreeView>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QSplitter>
#include <QDockWidget>
#include <QGridLayout>
#include <QStatusBar>
#include <QStackedWidget>
#include <QScrollArea>
#include <QFile>

using namespace ads;

CentralAppWindow::CentralAppWindow(QWidget *parent) : QMainWindow(parent)
{
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
    //    this->setStyleSheet(ts.readAll());
        this->setStyleSheet("font-family: \"Pr Number 3\"; font-size: 16px");
    }

    initCentralWidget();
    createActions();
    initMenuBar();
    initToolBars();
    initDockWidgets();
    initStatusBar();

}

void CentralAppWindow::createActions()
{
    m_quitAction = new QAction(QIcon(":/images/redblob.png"),tr("&Quit"));
    connect(m_quitAction, &QAction::triggered,
            qApp, &QApplication::quit);

    m_reference_action = new QAction(QIcon(":/images/redblob.png"),tr("&References"));
    m_reference_action->setCheckable(true);
//    connect(m_reference_action, &QAction::triggered,
//            qApp, &QApplication::xxxxxx);


    m_image_lib_action = new QAction(QIcon(":/images/redblob.png"),tr("&Disk Image\nLibrary"));
    m_image_lib_action->setCheckable(true);
//    connect(m_reference_action, &QAction::triggered,
//            qApp, &QApplication::xxxxxx);

    m_disk_explorer_action= new QAction(QIcon(":/images/redblob.png"),tr("Disk Image\n&Explorer"));
    m_disk_explorer_action->setCheckable(true);
//    connect(m_reference_action, &QAction::triggered,
//            qApp, &QApplication::xxxxxx);

    m_projects_action = new QAction(QIcon(":/images/redblob.png"),tr("&Projects"));
    m_projects_action->setCheckable(true);
//    connect(m_reference_action, &QAction::triggered,
//            qApp, &QApplication::xxxxxx);

    m_tool_action_group = new QActionGroup(this);
    m_tool_action_group->addAction(m_reference_action);
    m_tool_action_group->addAction(m_image_lib_action);
    m_tool_action_group->addAction(m_disk_explorer_action);
    m_tool_action_group->addAction(m_projects_action);
    m_tool_action_group->setExclusive(true);
    m_disk_explorer_action->toggle();


}


void CentralAppWindow::initMenuBar()
{
    auto filemenu = menuBar()->addMenu(tr("&File"));
    filemenu->addAction(m_quitAction);


    /*auto toolsmenu =*/ menuBar()->addMenu(tr("&Tools"));
}

void CentralAppWindow::initStatusBar()
{
    m_status_bar = new QStatusBar(this);
    setStatusBar(m_status_bar);
}

void CentralAppWindow::initToolBars()
{
    m_mainToolBar = new MainAppToolbar(this);
    m_mainToolBar->setIconSize(QSize(64,64));
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_mainToolBar->setFont(QFont("Ariel",14));

    m_mainToolBar->addAction(m_reference_action);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_image_lib_action);
    m_mainToolBar->addAction(m_disk_explorer_action);
    m_mainToolBar->addAction(m_projects_action);

    addToolBar(Qt::LeftToolBarArea, m_mainToolBar);
}

void CentralAppWindow::initDockWidgets()
{
    m_directory_area = new Dos33DiskTreeView();
    m_directory_area->setFont(QFont("Pr Number 3", 12));
    m_directory_area->setMinimumSize(300,200);

    Dos33DiskImage *img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/ApplesoftToolkit.dsk");

    auto model = new Dos33ImageModel();
    model->addDiskImage(img,"ApplesoftToolkit.dsk");

    img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/dos.3.3.system.master.dsk");
    model->addDiskImage(img);

    m_directory_area->setModel(model);

    auto DiskImageDockWidget = new CDockWidget("Disk Images");
    DiskImageDockWidget->setWidget(m_directory_area);
    DiskImageDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    DiskImageDockWidget->resize(250, 150);
    DiskImageDockWidget->setMinimumSize(200,150);
    DockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, DiskImageDockWidget);






    m_toolbox = new SequenceToolBox(this);

    auto SequenceDockWidget = new CDockWidget("Sequences");
    SequenceDockWidget->setWidget(m_toolbox);
    SequenceDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    SequenceDockWidget->resize(250, 150);
    SequenceDockWidget->setMinimumSize(200,150);
    DockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, SequenceDockWidget);

}

void CentralAppWindow::initCentralWidget()
{
     CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
     CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
     CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
     CDockManager::setConfigFlag(CDockManager::TabCloseButtonIsToolButton, true);

     DockManager = new CDockManager(this);
     DockManager->setStyleSheet("");

    m_central_stack = new QStackedWidget(this);
    QScrollArea *sa = new QScrollArea(m_central_stack);
    sa->setWidgetResizable(true);
    m_central_stack->addWidget(sa);

    CentralDockWidget = new CDockWidget("CentralWidget");
    CentralDockWidget->setWidget(m_central_stack);
    DockManager->setCentralWidget(CentralDockWidget);

    Dos33DiskImage *img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/ApplesoftToolkit.dsk");
    auto dew =  new DiskExplorerMapWidget(img->tracks(),img->sectorsPerTrack());
    dew->setDisk(img);
    sa->setWidget(dew);

    auto VWSDockWidget = new CDockWidget("Sector Contents");
    VWSDockWidget->setWidget(dew->generateViewWidgetStack());
    DockManager->addDockWidget(DockWidgetArea::BottomDockWidgetArea, VWSDockWidget);

    auto KeyDockWidget = new CDockWidget("Key");
    auto keywidget = dew->makeKeyWidget();
    keywidget->setMaximumWidth(240);
    KeyDockWidget->setWidget(keywidget);
    KeyDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);

    DockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea,
                               KeyDockWidget,
                               DockManager->centralWidget()->dockAreaWidget());

}
