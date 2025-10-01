#include "ViewerBase.h"
#include "ui_ViewerBase.h"

#include "ApplesoftFileViewer.h"
#include "HexDumpViewer.h"
#include "TextHexDumpViewer.h"
#include "CharSetViewer.h"
#include "IntBasicFile.h"
#include "IntBasicFileViewer.h"
#include "HiresViewer.h"
#include "DisassemblerViewer.h"
#include "TextFile.h"
#include "RelocatableFile.h"
#include "MazeViewer.h"

#include <QCloseEvent>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>


ViewerBase::ViewerBase(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::ViewerBase>())
{
    setAttribute(Qt::WA_DeleteOnClose);

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
    emit viewerClosing(this);
}

void ViewerBase::setFile(GenericFile *file)
{
    m_file = file;

    QString descriptor;
    int  defaultViewerDescriptor;

    QString hexdumpviewerstring = "Hex Dump Viewer";
    QString applesoftviewerstring = "Applesoft File Viewer";
    QString intbasicfileviewerstring = "Integer Basic File Viewer";
    QString hrcgcharsetviewerstring = "HRCG Character Set Viewer";
    QString hiresviewerstring = "HiRes Image Viewer";
    QString mrmazeviewerstring = "MissingRing Maze Viewer";
    QString disassemblerviewerstring = "Diassembler Viewer";
    QString texthexviewstring = "Text/Hex Dump Viewer";
    QString relocatableviewerstring = "Relocatable Disassembler Viewer";

    HexDumpViewer *hdv = new HexDumpViewer(0);
    if (dynamic_cast<ApplesoftFile*>(file))
        hdv->setFile(file,0x801);  //TODO: Double check this offset.
    else
        hdv->setFile(file,m_file->address());
    descriptor = (hexdumpviewerstring);
    addViewer(descriptor,hdv);
    defaultViewerDescriptor = m_viewercombo->findText(descriptor);

    if (dynamic_cast<ApplesoftFile*>(file))
    {
        ApplesoftFileViewer *afv = new ApplesoftFileViewer(0);
        afv->setFile(file);
        descriptor=applesoftviewerstring;
        addViewer(descriptor,afv);
        defaultViewerDescriptor = m_viewercombo->findText(descriptor);

    }
    else if (dynamic_cast<IntBasicFile*>(file))
    {
        IntBasicFileViewer *ibf = new IntBasicFileViewer(0);
        ibf->setFile(file);
        descriptor=intbasicfileviewerstring;
        addViewer(descriptor, ibf);
        defaultViewerDescriptor = m_viewercombo->findText(descriptor);
    }
    else if (dynamic_cast<BinaryFile*>(file))
    {
        BinaryFile *bf = dynamic_cast<BinaryFile*>(file);

        CharSetViewer *csv = new CharSetViewer();
        csv->setFile(bf);
        descriptor = hrcgcharsetviewerstring;
        addViewer(descriptor,csv);

        HiresViewer *hrvw = new HiresViewer();
        hrvw->setFile(bf);
        descriptor = hiresviewerstring;
        addViewer(descriptor,hrvw);

        MazeViewer *mv = new MazeViewer();
        mv->setFile(file);
        descriptor = mrmazeviewerstring;
        addViewer(descriptor,mv);

        DisassemblerViewer *dv = new DisassemblerViewer();
        dv->setFile(bf);
        descriptor = disassemblerviewerstring;
        addViewer(descriptor,dv);
        defaultViewerDescriptor = m_viewercombo->findText(descriptor);

        if (bf->filename().toUpper().endsWith(".SET"))
        {
            defaultViewerDescriptor = m_viewercombo->findText(hrcgcharsetviewerstring);
        }
        if (bf->filename().toUpper().startsWith("MAZE"))
        {
            defaultViewerDescriptor = m_viewercombo->findText(mrmazeviewerstring);

        }
        if ((bf->address() == 0x2000 || bf->address() == 0x4000)
            && bf->length() == 0x2000)
        {
            defaultViewerDescriptor = m_viewercombo->findText(hiresviewerstring);

        }

    }
    else if (dynamic_cast<TextFile*>(file))
    {
        TextFile *bf = dynamic_cast<TextFile*>(file);

        TextHexDumpViewer *thdv = new TextHexDumpViewer();
        thdv->setFile(bf);
        descriptor = QString(texthexviewstring);
        addViewer(descriptor,thdv);

        defaultViewerDescriptor = m_viewercombo->findText(descriptor);

    }
    else if (dynamic_cast<RelocatableFile*>(file))
    {
        DisassemblerViewer *dv = new DisassemblerViewer();
        dv->setFile(file);
        descriptor = relocatableviewerstring;
        addViewer(descriptor,dv);
        defaultViewerDescriptor = m_viewercombo->findText(descriptor);
    }
    connect(m_viewercombo,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ViewerBase::showViewer);
    showViewer(defaultViewerDescriptor);
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

void ViewerBase::showViewer(int index)
{
    QString descriptor = m_viewercombo->itemText(index);
    FileViewerInterface *fvi = m_viewers[descriptor];
    if (fvi)
    {
        ui->actionExport->disconnect(SIGNAL(triggered(bool)));
        ui->actionExport->setEnabled(fvi->canExport());
        connect(ui->actionExport, &QAction::triggered,
                fvi, &FileViewerInterface::doExport);

        ui->action_Print->disconnect(SIGNAL(triggered(bool)));
        ui->action_Print->setEnabled(fvi->canPrint());
        connect(ui->action_Print, &QAction::triggered,
                fvi, &FileViewerInterface::doPrint);

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
        qDebug() << "Could not find widget for descriptor" << descriptor;
    }

}
