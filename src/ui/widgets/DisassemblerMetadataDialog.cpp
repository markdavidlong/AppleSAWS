#include "DisassemblerMetadataDialog.h"
#include "ui_DisassemblerMetadataDialog.h"
#include "util.h"
#include <QDebug>


DisassemblerMetadataDialog::DisassemblerMetadataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisassemblerMetadataDialog)
{
    ui->setupUi(this);
    setRelocatable(false);

    m_bfm = new BinaryFileMetadata("Test");
    m_bfm->load();

    m_eps = new EntryPoints(this);
    m_as = new AssemblerSymbols(this);

    processEntryPoints();
    processSymbols();

    m_epmodel = new EntryPointModel(this,m_eps);
    m_asmodel = new AssemblerSymbolModel(this,m_as);

    ui->entryTable->setModel(m_epmodel);
    ui->symbolTable->setModel(m_asmodel);

    connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(handleCancelButton()));
    connect(ui->exitButton,SIGNAL(clicked(bool)), SLOT(handleExitButton()));
    connect(ui->processButton, SIGNAL(clicked(bool)), SLOT(handleProcessButton()));

    connect(ui->addEntryPointButton, SIGNAL(clicked(bool)), SLOT(handleAddEntryPointButton()));
    connect(ui->addSymbolButton, SIGNAL(clicked(bool)), SLOT(handleAddSymbolButton()));
}

DisassemblerMetadataDialog::~DisassemblerMetadataDialog()
{
    delete m_bfm;
    delete ui;
}

void DisassemblerMetadataDialog::showEvent(QShowEvent *){
    ui->entryTable->resizeRowsToContents();
}

void DisassemblerMetadataDialog::setRelocatable(bool relocatable)
{
    ui->reloAddrLabel->setVisible(relocatable);
    ui->reloAddrText->setVisible(relocatable);
}

void DisassemblerMetadataDialog::handleCancelButton()
{
    this->close();
}

void DisassemblerMetadataDialog::handleExitButton()
{
    this->close();
}

void DisassemblerMetadataDialog::handleProcessButton()
{

}


void DisassemblerMetadataDialog::handleAddEntryPointButton()
{
    LocationInfoDialog lid(this);
    lid.setWindowTitle("Add Entry Point");
    if (lid.exec() == Accepted)
    {
        EntryPoint ep;
        ep.address = lid.getAddress();
        ep.note = lid.getInfo();
        m_eps->addPoint(ep);
        ui->entryTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveEntryPointButton()
{
//    QModelIndexList selection = ui->entryTable->selectedIndexes();
//    if (selection.count())
//    {
//        m_epmodel->removeSelection(selection);
//    }
}


void DisassemblerMetadataDialog::handleAddSymbolButton()
{
    LocationInfoDialog lid(this);
    lid.setInfoLabelString("Symbol Name");
    lid.setWindowTitle("Add Symbol");
    if (lid.exec() == Accepted)
    {
        AssemSymbol as;
        as.address = lid.getAddress();
        as.name = lid.getInfo();
        m_as->addSymbol(as);
        ui->symbolTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveSymbolButton()
{

}


void DisassemblerMetadataDialog::processSymbols()
{

}

void DisassemblerMetadataDialog::processEntryPoints()
{
    m_eps->doTestData();
}
