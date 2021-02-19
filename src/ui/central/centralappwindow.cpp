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
#include "DiskExplorerMapWidget.h"

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

CentralAppWindow::CentralAppWindow(QWidget *parent) : QMainWindow(parent)
{
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        this->setStyleSheet(ts.readAll());
    }

    createActions();
    initMenuBar();
    initToolBars();
    initDockWidgets();
    initStatusBar();
    initCentralWidget();

}

void CentralAppWindow::createActions()
{
    m_quitAction = new QAction(QIcon(":/images/redblob.png"),tr("E&xit"));
    connect(m_quitAction, &QAction::triggered,
            qApp, &QApplication::quit);
}

void CentralAppWindow::initMenuBar()
{
    auto filemenu = menuBar()->addMenu(tr("&File"));
    filemenu->addAction(m_quitAction);

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
    m_mainToolBar->addAction(m_quitAction);
    m_mainToolBar->setFont(QFont("Ariel",14));
    addToolBar(Qt::LeftToolBarArea, m_mainToolBar);

}

void CentralAppWindow::initDockWidgets()
{
    QDockWidget *container = new QDockWidget(this);
    container->setMinimumWidth(200);
    container->setFeatures(QDockWidget::DockWidgetMovable);

    m_project_area = new QWidget(container);
    m_project_area->setMinimumSize(300,200);

    m_directory_area = new Dos33DiskTreeView(container);
    m_directory_area->setFont(QFont("Pr Number 3", 12));
    m_directory_area->setMinimumSize(300,200);

    QSplitter *split = new QSplitter(container);
    container->setWidget(split);
    split->setOrientation(Qt::Vertical);
    split->addWidget(m_directory_area);
    split->addWidget(m_project_area);
    split->setStretchFactor(0,1);
    split->setStretchFactor(1,1);
    split->setSizes(QList<int>({4000, 4000}));


    Dos33DiskImage *img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/ApplesoftToolkit.dsk");

    auto model = new Dos33ImageModel();
    model->addDiskImage(img,"ApplesoftToolkit.dsk");

    img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/dos.3.3.system.master.dsk");
    model->addDiskImage(img);

    img = new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/Print Shop Companion - Side 1.dsk");
    model->addDiskImage(img);
    m_directory_area->setModel(model);

    addDockWidget(Qt::LeftDockWidgetArea,container);



    QDockWidget *stb = new QDockWidget(this);
    stb->setMinimumWidth(200);
    stb->setFeatures(QDockWidget::DockWidgetMovable);
 //   stb->setLayout(new QGridLayout());

    m_toolbox = new SequenceToolBox(this);



    addDockWidget(Qt::RightDockWidgetArea,stb);


}

void CentralAppWindow::initCentralWidget()
{
    m_central_stack = new QStackedWidget(this);
    QScrollArea *sa = new QScrollArea(m_central_stack);
    sa->setWidgetResizable(true);
    m_central_stack->addWidget(sa);

    setCentralWidget(m_central_stack);

    Dos33DiskImage *img =  new Dos33DiskImage("c:/develop/git/AppleSAWS/disk-images/ApplesoftToolkit.dsk");
    auto dew =  new DiskExplorerMapWidget(img->tracks(),img->sectorsPerTrack());
    dew->setDisk(img);
    sa->setWidget(dew);
 //   m_central_stack->addWidget(dew);
//new SequenceViewer()
}
