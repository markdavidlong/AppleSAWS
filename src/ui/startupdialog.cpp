#include "startupdialog.h"
#include "ui_startupdialog.h"

#include "diskexplorer/DiskExplorer.h"

#include <QFileDialog>
#include "centralappwindow.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    m_notes = new NotesDialog(this);
    ui->setupUi(this);
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::on_exploreContentsBtn_clicked()
{
    DiskExplorer *w = new DiskExplorer(this);
    w->showLoadDialog(false);
}


void StartupDialog::on_newStuffButton_clicked()
{
    CentralAppWindow *w = new CentralAppWindow(this);
    w->showMaximized();
}

void StartupDialog::on_exploreDiskImageBtn_clicked()
{

}

void StartupDialog::on_bootSectorCompareBtn_clicked()
{

}

void StartupDialog::on_infoBtn_clicked()
{
    m_notes->show();
}
