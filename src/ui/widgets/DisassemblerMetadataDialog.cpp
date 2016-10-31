#include "DisassemblerMetadataDialog.h"
#include "ui_DisassemblerMetadataDialog.h"
#include "util.h"
#include <QDebug>


DisassemblerMetadataDialog::DisassemblerMetadataDialog(BinaryFileMetadata *bfm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisassemblerMetadataDialog)
{
    ui->setupUi(this);
    setRelocatable(false);

    m_bfm = bfm;

    m_epmodel = new EntryPointModel(this,m_bfm->entryPoints());
    m_asmodel = new AssemblerSymbolModel(this,m_bfm->assemblerSymbols());

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

void DisassemblerMetadataDialog::showEvent(QShowEvent *)
{
    ui->entryTable->resizeRowsToContents();
    ui->symbolTable->resizeRowsToContents();
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
    m_bfm->save();
    this->close();
}

void DisassemblerMetadataDialog::handleProcessButton()
{
    m_bfm->requestDisassembly();
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
        m_bfm->entryPoints()->addPoint(ep);
        ui->entryTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveEntryPointButton()
{

}

void DisassemblerMetadataDialog::handleAddSymbolButton()
{
    LocationInfoDialog lid(this);
    lid.setInfoLabelString("Symbol Name");
    lid.setWindowTitle("Add Symbol");
    if (lid.exec() == Accepted)
    {
        AssemblerSymbol as;
        as.address = lid.getAddress();
        as.name = lid.getInfo();
        m_bfm->assemblerSymbols()->addSymbol(as);
        ui->symbolTable->resizeRowsToContents();
    }
}

void DisassemblerMetadataDialog::handleRemoveSymbolButton()
{

}
