#include "centralappwindow.h"
#include "sequenceviewer.h"

#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QSplitter>
#include <QDockWidget>
#include <QGridLayout>

CentralAppWindow::CentralAppWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    initMenuBar();
    initToolBars();
    initDockWidgets();
    initStatusBar();

    setCentralWidget(new SequenceViewer());
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
    container->setLayout(new QGridLayout());
    container->setMinimumWidth(200);
    container->setFeatures(QDockWidget::DockWidgetMovable);
    m_project_area = new QWidget(container);
    m_project_area->setMinimumSize(200,200);
    m_project_area->setStyleSheet("background:black");
    m_directory_area = new QWidget(container);
    m_directory_area->setMinimumSize(200,200);
    m_directory_area->setStyleSheet("background:black");
    QSplitter *split = new QSplitter(container);
    split->setOrientation(Qt::Vertical);
    split->addWidget(m_directory_area);
    split->addWidget(m_project_area);
    container->setWidget(split);

    addDockWidget(Qt::LeftDockWidgetArea,container);


}
