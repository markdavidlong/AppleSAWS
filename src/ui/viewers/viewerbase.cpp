#include "viewerbase.h"
#include "ui_viewerbase.h"

#include <QCloseEvent>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QDebug>

#include "applesoftfileviewer.h"
#include "hexdumpviewer.h"
#include "texthexdumpviewer.h"
#include "charsetviewer.h"
#include "hiresviewwidget.h"
#include "disassemblerviewer.h"
#include "textfile.h"
#include "mazeviewer.h"

ViewerBase::ViewerBase(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerBase)
{
    setMinimumWidth(1024);
    m_stack = new QStackedWidget(this);
    ui->setupUi(this);
    setCentralWidget(m_stack);

    m_toolbar = new QToolBar(this);
    addToolBar(m_toolbar);

    QLabel *label = new QLabel("View With:  ");
    m_toolbar->addWidget(label);

    m_viewercombo = new QComboBox(m_toolbar);
    m_toolbar->addWidget(m_viewercombo);

    m_optionMenu = new QMenu("&Viewer");
    menuBar()->addMenu(m_optionMenu);
    m_optionMenu->setEnabled(false);

}

ViewerBase::~ViewerBase()
{
    delete ui;
}

void ViewerBase::setFile(GenericFile *file)
{
    m_file = file;

    QString descriptor;

    HexDumpViewer *hdv = new HexDumpViewer(0);
    hdv->setFile(file);
    descriptor = ("Hex Dump Viewer");
    addViewer(descriptor,hdv);

    if (dynamic_cast<ApplesoftFile*>(file))
    {
        ApplesoftFileViewer *afv = new ApplesoftFileViewer(0);
        afv->setFile(file);
        descriptor="Applesoft File Viewer";
        addViewer(descriptor,afv);
        showViewer(descriptor);
    }
    else if (dynamic_cast<BinaryFile*>(file))
    {
        BinaryFile *bf = dynamic_cast<BinaryFile*>(file);

        CharSetViewer *csv = new CharSetViewer();
        csv->setFile(bf);
        descriptor ="HRCG Character Set Viewer";
        addViewer(descriptor,csv);

        HiresViewWidget *hrvw = new HiresViewWidget();
        hrvw->setFile(bf);
        descriptor = "HiRes Image Viewer";
        addViewer(descriptor,hrvw);

        MazeViewer *mv = new MazeViewer();
        mv->setFile(file);
        descriptor = "MissingRing Maze Viewer";
        addViewer(descriptor,mv);

        DisassemblerViewer *dv = new DisassemblerViewer();
        dv->setFile(bf);
        descriptor = "Disassembler Viewer";
        addViewer(descriptor,dv);

        showViewer(descriptor);
    }
    else if (dynamic_cast<TextFile*>(file))
    {
        BinaryFile *bf = dynamic_cast<BinaryFile*>(file);

        TextHexDumpViewer *thdv = new TextHexDumpViewer();
        thdv->setFile(bf);
        descriptor = QString("Text/Hex Dump Viewer");
        addViewer(descriptor,thdv);

        showViewer(descriptor);
    }
    else if (dynamic_cast<RelocatableFile*>(file))
    {
        DisassemblerViewer *dv = new DisassemblerViewer();
        dv->setFile(file);
        descriptor = "Relocatable Disassembler Viewer";
        addViewer(descriptor,dv);
        showViewer(descriptor);

    }
    else
    {
        showViewer(descriptor);
    }
    connect(m_viewercombo, SIGNAL(currentIndexChanged(QString)), SLOT(showViewer(QString)));

}

void ViewerBase::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void ViewerBase::addViewer(QString descriptor, FileViewerInterface *viewer)
{
    if (!m_viewers.contains(descriptor))
    {
        m_stack->addWidget(viewer);
        m_viewers[descriptor] = viewer;
        m_viewercombo->addItem(descriptor);
    }
}

void ViewerBase::showViewer(QString descriptor)
{
    qDebug() << "showViewer";
    FileViewerInterface *fvi = m_viewers[descriptor];
    if (fvi)
    {
        m_optionMenu->clear();
        m_viewercombo->setCurrentText(descriptor);
        m_stack->setCurrentWidget(fvi);
        setWindowTitle(fvi->windowTitle());

        if (m_optionMenu)
        {
            if (!fvi->optionsMenuItems(m_optionMenu))
            {
                m_optionMenu->setEnabled(false);
            }
            else
            {
                m_optionMenu->setEnabled(true);
            }
        }
    }
    else
    {
        qDebug() << "Could not find widget for descriptor " << descriptor;
    }

}
