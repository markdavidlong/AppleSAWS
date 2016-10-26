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

    m_epmodel = new EntryPointModel(this);


    processEntryPoints();
    processSymbols();

    ui->entryTable->setModel(m_epmodel);

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
        m_epmodel->addEntryPoint(ep);
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
    lid.setInfoLabelString("Symbol");
    lid.setWindowTitle("Add Symbol");
    if (lid.exec() == Accepted)
    {
        qDebug() << "Accepted";
    }
    else qDebug()<<"Rejected";
}

void DisassemblerMetadataDialog::handleRemoveSymbolButton()
{

}


void DisassemblerMetadataDialog::processSymbols()
{

}

void DisassemblerMetadataDialog::processEntryPoints()
{
    m_epmodel->doTestData();
}
