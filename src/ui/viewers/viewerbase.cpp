#include "viewerbase.h"
#include "ui_viewerbase.h"

#include <QCloseEvent>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>

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

    connect(m_viewercombo, SIGNAL(currentIndexChanged(QString)), SLOT(showViewer(QString)));
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
        m_stack->setCurrentWidget(afv);
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
    }
    else
    {
        showViewer(descriptor);
    }
}

void ViewerBase::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void ViewerBase::addViewer(QString descriptor, FileViewerInterface *viewer)
{
    if (!m_viewers.contains(descriptor))
    {
        m_viewers[descriptor] = viewer;
        m_viewercombo->addItem(descriptor);
        m_stack->addWidget(viewer);
    }
}

void ViewerBase::showViewer(QString descriptor)
{
    m_viewercombo->setCurrentText(descriptor);
    FileViewerInterface *fvi = m_viewers[descriptor];
    m_stack->setCurrentWidget(fvi);
    setWindowTitle(fvi->windowTitle());
}
